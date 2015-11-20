

entity rx is
	Generic (CLK_RATE : NATURAL := 50_000_000;
				BAUD_RATE : NATURAL := 19_200);
   Port ( clk : in  STD_LOGIC;
          rst : in  STD_LOGIC;
          data_in : in  STD_LOGIC;
          data_out : out  STD_LOGIC_VECTOR (7 downto 0);
          LEDs : out  STD_LOGIC_VECTOR (7 downto 0);
          data_strobe : out  STD_LOGIC;
          rx_busy : out  STD_LOGIC
	);
end rx;

architecture archie of rx is

	--Functions
	function log2c(n:integer) return integer is
		variable m,p:integer;
	begin
		m := 0;
		p := 1;
		while p < n loop
			m := m+1;
			p := p*2;
		end loop;
		return m;
	end log2c;
	
	--Enum type
	type rx_state_type is
		(RESET, IDLE, STRT, BA, BB, BSEL, BSTRT, BUP, BDOWN, BLEFT, BRIGHT, DONE);
	type tx_state_type is
		(RST, IDL, LATCH, PULSE);
	
	--Constants
	constant BIT_COUNTER_MAX_VAL : Natural := CLK_RATE / BAUD_RATE - 1;
	constant BIT_COUNTER_BITS : Natural := log2c(BIT_COUNTER_MAX_VAL);
	
	--Signals
	signal count : STD_LOGIC_VECTOR(19 downto 0);
	signal bit_timer, bit_timer_next : unsigned(BIT_COUNTER_BITS-1 downto 0);
	signal clrTimer, data_bit, data_bit_half, data_done, pulse_out : STD_LOGIC;
	signal button : STD_LOGIC_VECTOR(7 downto 0);
	signal rx_state_reg, tx_state_reg, rx_state_next, tx_state_next : rx_state_type;

begin

	--Bit Timer
	process(clk, clrTimer, rst)
		begin
			if(rst='1') then
				button = (others <= '0');
				bit_timer <= (others=>'0');
				rx_state_reg <= RESET;
				tx_state_reg <= RST;
			elsif(clk'event and clk='1') then
				rx_state_reg <= rx_state_next;
				tx_state_reg <= tx_state_next;
				if(clrTimer='1') then
					bit_timer <= (others=>'0');
				else
					bit_timer <= bit_timer_next;
				end if;
			end if;
		end process;
		
		--Next state logic
		bit_timer_next <= bit_timer+1 when bit_timer < BIT_COUNTER_MAX_VAL else
								(others => '0');
		--Output logic
		data_bit <= '1' when bit_timer = BIT_COUNTER_MAX_VAL else
					'0';
		data_bit_half <= '1' when bit_timer = BIT_COUNTER_MAX_VAL/2 else
							'0';

		
		--Next state logic
		process(rx_state_reg, tx_state_reg, data_bit, data_bit_half, data_in, count)
		begin
			--Defaults
			load <= '0';
			rx_state_next <= rx_state_reg;
			data_done <= '0';
			case rx_state_reg is
				when RESET => 
					if(data_in='0') then
						button <= (others => '0');
						rx_state_next <= IDLE;
					end if;
				when IDLE => 
					if(tx_state_reg=LATCH) then
						rx_state_next <= BA;
					else
						clrTimer <= '1'; -- restart the data_bit timer
					end if;
				when BA => 
					if(tx_state_reg=PULSE) then
						button(0) <= not data_in; -- save A Button state
						if(data_bit_half='1') then -- wait half pulse and then start again
							clrTimer <= '1'; -- restart the data_bit timer
							rx_state_next <= BB; -- set next state to BB
						end if;
					end if;
				when BB => 
					if(data_bit='1') then
						rx_state_next <= BSEL;
					end if;
					if(data_bit_half='1') then
						button(1) <= not data_in; -- save B Button state
					end if;
				when BSEL => 
					if(data_bit='1') then
						rx_state_next <= BSTRT;
					end if;
					if(data_bit_half='1') then
						button(2) <= not data_in; -- save Select Button state
					end if;
				when BSTRT => 
					if(data_bit='1') then
						rx_state_next <= BUP;
					end if;
					if(data_bit_half='1') then
						button(3) <= not data_in; -- save Start Button state
					end if;
				when BUP => 
					if(data_bit='1') then
						rx_state_next <= BDOWN;
					end if;
					if(data_bit_half='1') then
						button(4) <= not data_in; -- save Up Button state
					end if;
				when BDOWN => 
					if(data_bit='1') then
						rx_state_next <= BLEFT;
					end if;
					if(data_bit_half='1') then
						button(5) <= not data_in; -- save Down Button state
					end if;
				when BLEFT => 
					if(data_bit='1') then
						rx_state_next <= BRIGHT;
					end if;
					if(data_bit_half='1') then
						button(6) <= not data_in; -- save Left Button state
					end if;
				when BRIGHT => 
					if(data_bit='1') then
						rx_state_next <= DONE;
					end if;
					if(data_bit_half='1') then
						button(7) <= not data_in; -- save Right Button state
					end if;
				when DONE =>
					if(data_bit_half='1') then
						if(data_in='0') then
							data_done <= '1';
						end if;
					end if;
					rx_state_next <= IDLE;
			end case;

			tx_state_next <= tx_state_reg;
			case tx_state_reg is
				when RST => 
					if(data_bit='0') then
						state_next <= LATCH;
						clrTimer <= '1'; -- reset the timer
					end if;
				when IDL => 
					if(count=COUNTER_LIMIT) then
						state_next <= LATCH;
						clrTimer <= '1'; -- reset the timer
					end if;
				when LATCH => 
					if(data_bit='1') then
						state_next <= PULSE;
					end if;
				when PULSE => 
					if(data_bit='1') then
						pulse_out <= '1';
					end if;
					if(data_bit_half='1') then
						pulse_out <= '0';
					end if;
					if(data_done='1') then
						pulse_out <= '0';
						state_next <= IDL;
					end if;
			end case;
		end process;
		
		--Output logic
		rx_busy <= '0' when (state_reg=IDLE) else
						'1';
		data_strobe <= '1' when data_done='1' else
							'0';
		data_out <= shift_reg when data_done='1' else
						"00000000";
		clrTimer <= '1' when state_reg=IDLE or state_reg=POWER_UP else
						'0';
end archie;

entity tx is
	Generic (
			CLK_RATE : NATURAL := 50_000_000;
			BAUD_RATE : NATURAL := 19_200			
	);
	Port (
		clk : in  STD_LOGIC;
      rst : in  STD_LOGIC;
      data_in : in  STD_LOGIC_VECTOR (7 downto 0);
      send_character : in  STD_LOGIC;
      tx_out : out  STD_LOGIC;
      tx_busy : out  STD_LOGIC
	);
end tx;

architecture Behavioral of tx is
	
	--Functions
	function log2c(n:integer) return integer is
		variable m,p:integer;
	begin
		m := 0;
		p := 1;
		while p < n loop
			m := m+1;
			p := p*2;
		end loop;
		return m;
	end log2c;
	
	--Enum type
	type uart_state_type is
		(IDLE, STRT, B0, B1, B2, B3, B4, B5, B6, B7, STP, RET);
	
	--Constants
	constant BIT_COUNTER_MAX_VAL : Natural := CLK_RATE / BAUD_RATE - 1;
	constant BIT_COUNTER_BITS : Natural := log2c(BIT_COUNTER_MAX_VAL);
	
	--Signals
	signal bit_timer,bit_timer_next : unsigned(BIT_COUNTER_BITS-1 downto 0);
	signal clrTimer, tx_bit, load, shift, shift_out, start, 
				stop, tx_o, tx_out_next : STD_LOGIC;
	signal shift_reg, shift_reg_next : STD_LOGIC_VECTOR(7 downto 0);
	signal state_reg, state_next : uart_state_type;
	
begin

	--Bit Timer
		process(clk, clrTimer, rst)
		begin
			if(rst='1') then
				bit_timer <= (others=>'0');
			elsif(clk'event and clk='1') then
				if(clrTimer='1') then
					bit_timer <= (others=>'0');
				else
					bit_timer <= bit_timer_next;
				end if;
			end if;
		end process;
		
		--Next state logic
		bit_timer_next <= bit_timer+1 when bit_timer < BIT_COUNTER_MAX_VAL else
								(others => '0');
		--Output logic
		tx_bit <= '1' when bit_timer = BIT_COUNTER_MAX_VAL else
					'0';
				
	--Shift Register
		process(clk, rst)
		begin
			if(rst='1') then
				shift_reg <= (others=>'0');
			elsif(clk'event and clk='1') then
				shift_reg <= shift_reg_next;
			end if;
		end process;
		--Next state logic
		shift_reg_next <= data_in when load='1' else
								'0' & shift_reg(7 downto 1) when shift='1' else
								shift_reg;
		--Output logic
		shift_out <= shift_reg(0);
			
	--Transmit Out
		process(clk,rst)
		begin
			if(rst='1') then
				tx_o <= '1';
			elsif(clk'event and clk='1') then
				tx_o <= tx_out_next;
			end if;
		end process;
		--Next state logic
		tx_out_next <= '0' when start='1' else
							'1' when stop='1' else
							shift_out;
		--Output logic
		tx_out <= tx_o;
	--FSM
		process(state_reg, clk, rst)
		begin
			if(rst='1') then
				state_reg <= IDLE;
			elsif(clk'event and clk='1') then
				state_reg <= state_next;
			end if;
		end process;
		
		
		--Output logic
		tx_busy <= '0' when (state_reg=IDLE) else
						'1';
		stop <= '1' when state_reg=STP or state_reg=RET or state_reg=IDLE else
					'0';
		start <= '1' when state_reg=STRT else
					'0';
		clrTimer <= '1' when state_reg=IDLE else
						'0';

end Behavioral;

