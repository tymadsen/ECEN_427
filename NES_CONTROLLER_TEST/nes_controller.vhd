----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    16:32:18 11/20/2015 
-- Design Name: 
-- Module Name:    nes_controller - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity nes_controller is

  port
  (
    -- ADD USER PORTS BELOW THIS LINE ------------------
    --USER ports added here
		LEDs													 : out std_logic_vector (7 downto 0);
		C1_PULSE											 : out std_logic;
		C1_LATCH											 : out std_logic;
		C1_DATA												 : in std_logic;
		C2_PULSE											 : out std_logic;
		C2_LATCH											 : out std_logic;
		C2_DATA												 : in std_logic;
    -- ADD USER PORTS ABOVE THIS LINE ------------------
		clk 													 : in std_logic;
		rst 													 : in std_logic
  );

end nes_controller;

architecture Behavioral of nes_controller is
	--Enum type
  type rx_state_type is
    (IDLE, WA, BA, WB, BB, WSE, BSE, WST, BST, WU, BU, WD, BD, WL, BL, WR, BR, DONE);
  --type tx_state_type is (RST, IDL, LATCH, PULSE);
  
  --Constants
  constant READ_TIMER_LIMIT    	        : natural   := 1_000_000;
  constant PULSE_TIMER_LIMIT            : natural   := 1000;
  
  --USER signal declarations added here, as needed for user logic
  
  --signal pulse_timer, pulse_timer_next      : unsigned(9 downto 0);
  signal clrTimer, pulse_on, 
		data_done, latch, readBtn						: STD_LOGIC;
  signal c1_buttons, c1_buttons_next,
		c2_buttons, c2_buttons_next	        : STD_LOGIC_VECTOR(7 downto 0);
  signal rx_state_reg, rx_state_next		: rx_state_type;
	--signal tx_state_reg, tx_state_next    : tx_state_type;
  
  signal read_timer                     : std_logic_vector(19 downto 0); -- Timer to read buttons every 100 Hz
  signal pulse_timer                    : std_logic_vector(9 downto 0); -- Timer to sapce out pulses 

begin
	
	-- Create counter for read timer and pulse timer
	counter: process (clk)
	begin
		if (rst = '1') then
			read_timer <= (others => '0');
			pulse_timer <= (others => '0');
      rx_state_reg <= IDLE;
      --tx_state_reg <= RST;
			c1_buttons <= (others => '0');
			c2_buttons <= (others => '0');
			--c1_buttons_next <= (others => '0');
			--c2_buttons_next <= (others => '0');
		elsif (clk'event and clk = '1') then
			if read_timer = READ_TIMER_LIMIT then
        read_timer <= (others => '0'); -- Rollover read controller timer
				pulse_timer <= (others => '0'); -- Reset pulse timer
      else
				read_timer <= read_timer + 1;				
      end if;
			
			if(data_done = '1') then
				LEDs <= c1_buttons; -- Attach button values to LEDs
			end if;
			c1_buttons <= c1_buttons_next;
			c2_buttons <= c2_buttons_next;
			
			rx_state_reg <= rx_state_next;
			--tx_state_reg <= tx_state_next;
			
			if(clrTimer='1' or pulse_timer = PULSE_TIMER_LIMIT) then
				pulse_timer <= (others => '0');
			else
				pulse_timer <= pulse_timer + 1;
			end if;
		end if;
	end process counter;

  --Control logic
  pulse_on <= '1' when pulse_timer >= PULSE_TIMER_LIMIT/2 else '0';
	
	latch <= '1' when read_timer > 0 and read_timer < PULSE_TIMER_LIMIT else '0';
	
	clrTimer <= '1' when rx_state_reg = IDLE or rx_state_reg = WA else '0';
	
	c1_buttons_next <= (others => '0') when latch = '1' else 
											c1_buttons(6 downto 0) & not C1_DATA when readBtn = '1' else
											c1_buttons;
	
	c2_buttons_next <= (others => '0') when latch = '1' else 
											c2_buttons(6 downto 0) & not C2_DATA when readBtn = '1' else
											c2_buttons;
      
  --Next state logic
  process(rx_state_reg, pulse_on, latch)--, C1_DATA, C2_DATA, c1_buttons, c2_buttons)
  begin
    --Defaults
    rx_state_next <= rx_state_reg;
    data_done <= '0';
		readBtn <= '0';
    case rx_state_reg is
      when IDLE => 
        if(latch = '1') then
          rx_state_next <= WA;
        end if;
			when WA => 
				if(latch = '0') then
					rx_state_next <= BA; -- set next state to BA
					readBtn <= '1';
				end if;
      when BA => 
				if(pulse_on = '1') then -- wait for pulse to go high to change state
          rx_state_next <= WB; -- set next state to WB
        end if;
      when WB => 
				if(pulse_on = '0') then -- wait for pulse to go low to change state
					rx_state_next <= BB;
					readBtn <= '1';
        end if;
			when BB => 
				if(pulse_on = '1') then -- wait for pulse to go high to change state
					rx_state_next <= WSE;
        end if;
      when WSE => 
        if(pulse_on = '0') then -- wait for pulse to go low to change state
          rx_state_next <= BSE;
					readBtn <= '1';
        end if;
			when BSE => 
        if(pulse_on = '1') then -- wait for pulse to go high to change state
          rx_state_next <= WST;
        end if;
			when WST => 
        if(pulse_on = '0') then -- wait for pulse to go low to change state
          rx_state_next <= BST;
					readBtn <= '1';
        end if;
			when BST => 
        if(pulse_on = '1') then -- wait for pulse to go high to change state
          rx_state_next <= WU;
        end if;
			when WU => 
        if(pulse_on = '0') then -- wait for pulse to go low to change state
          rx_state_next <= BU;
					readBtn <= '1';
        end if;
			when BU => 
        if(pulse_on = '1') then -- wait for pulse to go high to change state
          rx_state_next <= WD;
        end if;
			when WD => 
        if(pulse_on = '0') then -- wait for pulse to go low to change state
          rx_state_next <= BD;
					readBtn <= '1';
        end if;
			when BD => 
        if(pulse_on = '1') then -- wait for pulse to go high to change state
          rx_state_next <= WL;
        end if;
			when WL => 
        if(pulse_on = '0') then -- wait for pulse to go low to change state
          rx_state_next <= BL;
					readBtn <= '1';
        end if;
			when BL => 
        if(pulse_on = '1') then -- wait for pulse to go high to change state
          rx_state_next <= WR;
        end if;
			when WR => 
        if(pulse_on = '0') then -- wait for pulse to go low to change state
          rx_state_next <= BR;
					readBtn <= '1';
        end if;
			when BR => 
        if(pulse_on = '1') then -- wait for pulse to go high to change state
          rx_state_next <= DONE;
        end if;
      when DONE =>
        if(pulse_on = '0') then
					rx_state_next <= IDLE;
          data_done <= '1';
        end if;
			when others =>
				if(latch = '1') then
          rx_state_next <= WA;
				end if;
    end case;
  end process;
    
  --Output logic
	-- write buttons to slv register
	C1_PULSE <= pulse_on; -- assert pulse
	C1_LATCH <= latch; -- assert latch
	C2_PULSE <= pulse_on; -- assert pulse
	C2_LATCH <= latch; -- assert latch

end Behavioral;



