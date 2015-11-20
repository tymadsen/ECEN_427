------------------------------------------------------------------------------
-- user_logic.vhd - entity/architecture pair
------------------------------------------------------------------------------
--
-- ***************************************************************************
-- ** Copyright (c) 1995-2011 Xilinx, Inc.  All rights reserved.            **
-- **                                                                       **
-- ** Xilinx, Inc.                                                          **
-- ** XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"         **
-- ** AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND       **
-- ** SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,        **
-- ** OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,        **
-- ** APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION           **
-- ** THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,     **
-- ** AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE      **
-- ** FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY              **
-- ** WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE               **
-- ** IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR        **
-- ** REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF       **
-- ** INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       **
-- ** FOR A PARTICULAR PURPOSE.                                             **
-- **                                                                       **
-- ***************************************************************************
--
------------------------------------------------------------------------------
-- Filename:          user_logic.vhd
-- Version:           1.00.a
-- Description:       User logic.
-- Date:              Thu Nov 19 13:48:35 2015 (by Create and Import Peripheral Wizard)
-- VHDL Standard:     VHDL'93
------------------------------------------------------------------------------
-- Naming Conventions:
--   active low signals:                    "*_n"
--   clock signals:                         "clk", "clk_div#", "clk_#x"
--   reset signals:                         "rst", "rst_n"
--   generics:                              "C_*"
--   user defined types:                    "*_TYPE"
--   state machine next state:              "*_ns"
--   state machine current state:           "*_cs"
--   combinatorial signals:                 "*_com"
--   pipelined or register delay signals:   "*_d#"
--   counter signals:                       "*cnt*"
--   clock enable signals:                  "*_ce"
--   internal version of output port:       "*_i"
--   device pins:                           "*_pin"
--   ports:                                 "- Names begin with Uppercase"
--   processes:                             "*_PROCESS"
--   component instantiations:              "<ENTITY_>I_<#|FUNC>"
------------------------------------------------------------------------------

-- DO NOT EDIT BELOW THIS LINE --------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

library proc_common_v3_00_a;
use proc_common_v3_00_a.proc_common_pkg.all;

-- DO NOT EDIT ABOVE THIS LINE --------------------

--USER libraries added here

------------------------------------------------------------------------------
-- Entity section
------------------------------------------------------------------------------
-- Definition of Generics:
--   C_NUM_REG                    -- Number of software accessible registers
--   C_SLV_DWIDTH                 -- Slave interface data bus width
--
-- Definition of Ports:
--   Bus2IP_Clk                   -- Bus to IP clock
--   Bus2IP_Resetn                -- Bus to IP reset
--   Bus2IP_Data                  -- Bus to IP data bus
--   Bus2IP_BE                    -- Bus to IP byte enables
--   Bus2IP_RdCE                  -- Bus to IP read chip enable
--   Bus2IP_WrCE                  -- Bus to IP write chip enable
--   IP2Bus_Data                  -- IP to Bus data bus
--   IP2Bus_RdAck                 -- IP to Bus read transfer acknowledgement
--   IP2Bus_WrAck                 -- IP to Bus write transfer acknowledgement
--   IP2Bus_Error                 -- IP to Bus error response
------------------------------------------------------------------------------

entity user_logic is
  generic
  (
    -- ADD USER GENERICS BELOW THIS LINE ---------------
    --USER generics added here
    -- ADD USER GENERICS ABOVE THIS LINE ---------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol parameters, do not add to or delete
    C_NUM_REG                      : integer              := 1;
    C_SLV_DWIDTH                   : integer              := 32
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );
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

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol ports, do not add to or delete
    Bus2IP_Clk                     : in  std_logic;
    Bus2IP_Resetn                  : in  std_logic;
    Bus2IP_Data                    : in  std_logic_vector(C_SLV_DWIDTH-1 downto 0);
    Bus2IP_BE                      : in  std_logic_vector(C_SLV_DWIDTH/8-1 downto 0);
    Bus2IP_RdCE                    : in  std_logic_vector(C_NUM_REG-1 downto 0);
    Bus2IP_WrCE                    : in  std_logic_vector(C_NUM_REG-1 downto 0);
    IP2Bus_Data                    : out std_logic_vector(C_SLV_DWIDTH-1 downto 0);
    IP2Bus_RdAck                   : out std_logic;
    IP2Bus_WrAck                   : out std_logic;
    IP2Bus_Error                   : out std_logic
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );

  attribute MAX_FANOUT : string;
  attribute SIGIS : string;

  attribute SIGIS of Bus2IP_Clk    : signal is "CLK";
  attribute SIGIS of Bus2IP_Resetn : signal is "RST";

end entity user_logic;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of user_logic is
  
  --Enum type
  type rx_state_type is
    (RESET, IDLE, STRT, BA, BB, BSEL, BSTRT, BUP, BDOWN, BLEFT, BRIGHT, DONE);
  type tx_state_type is
    (RST, IDL, LATCH, PULSE);
  
  --Constants
  constant READ_COUNTER_LIMIT             : natural   := 1_000_000;
  constant PULSE_COUNTER_LIMIT            : natural   := 1000;
  
  --USER signal declarations added here, as needed for user logic
  
  --signal pulse_timer, pulse_timer_next      : unsigned(9 downto 0);
  signal clrTimer, pulse_sig, pulse_on,
   pulse_sig_half, data_done, pulse_out : STD_LOGIC;
  signal button                         : STD_LOGIC_VECTOR(7 downto 0);
  signal rx_state_reg, rx_state_next		: rx_state_type
	signal tx_state_reg, tx_state_next    : tx_state_type;
  
  signal read_timer                     : std_logic_vector(19 downto 0); -- Timer to read buttons every 100 Hz
  signal pulse_timer                    : std_logic_vector(9 downto 0); -- Timer to sapce out pulses 
  ------------------------------------------
  -- Signals for user logic slave model s/w accessible register example
  ------------------------------------------
  signal slv_reg0                       : std_logic_vector(C_SLV_DWIDTH-1 downto 0);
  signal slv_reg_write_sel              : std_logic_vector(0 to 0);
  signal slv_reg_read_sel               : std_logic_vector(0 to 0);
  signal slv_ip2bus_data                : std_logic_vector(C_SLV_DWIDTH-1 downto 0);
  signal slv_read_ack                   : std_logic;
  signal slv_write_ack                  : std_logic;

begin

  --USER logic implementation added here

  ------------------------------------------
  -- Example code to read/write user logic slave model s/w accessible registers
  -- 
  -- Note:
  -- The example code presented here is to show you one way of reading/writing
  -- software accessible registers implemented in the user logic slave model.
  -- Each bit of the Bus2IP_WrCE/Bus2IP_RdCE signals is configured to correspond
  -- to one software accessible register by the top level template. For example,
  -- if you have four 32 bit software accessible registers in the user logic,
  -- you are basically operating on the following memory mapped registers:
  -- 
  --    Bus2IP_WrCE/Bus2IP_RdCE   Memory Mapped Register
  --                     "1000"   C_BASEADDR + 0x0
  --                     "0100"   C_BASEADDR + 0x4
  --                     "0010"   C_BASEADDR + 0x8
  --                     "0001"   C_BASEADDR + 0xC
  -- 
  ------------------------------------------
  slv_reg_write_sel <= Bus2IP_WrCE(0 downto 0);
  slv_reg_read_sel  <= Bus2IP_RdCE(0 downto 0);
  slv_write_ack     <= Bus2IP_WrCE(0);
  slv_read_ack      <= Bus2IP_RdCE(0);

  -- implement slave model software accessible register(s)
  SLAVE_REG_WRITE_PROC : process( Bus2IP_Clk ) is
  begin

    if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
      if Bus2IP_Resetn = '0' then
        slv_reg0 <= (others => '0');
      else
        case slv_reg_write_sel is
          when "1" =>
            for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
              if ( Bus2IP_BE(byte_index) = '1' ) then
                slv_reg0(byte_index*8+7 downto byte_index*8) <= Bus2IP_Data(byte_index*8+7 downto byte_index*8);
              end if;
            end loop;
          when others => null;
        end case;
      end if;
    end if;

  end process SLAVE_REG_WRITE_PROC;

  -- implement slave model software accessible register(s) read mux
  SLAVE_REG_READ_PROC : process( slv_reg_read_sel, slv_reg0 ) is
  begin

    case slv_reg_read_sel is
      when "1" => slv_ip2bus_data <= slv_reg0;
      when others => slv_ip2bus_data <= (others => '0');
    end case;

  end process SLAVE_REG_READ_PROC;

  ------------------------------------------
  -- Example code to drive IP to Bus signals
  ------------------------------------------
  IP2Bus_Data  <= slv_ip2bus_data when slv_read_ack = '1' else
                  (others => '0');

  IP2Bus_WrAck <= slv_write_ack;
  IP2Bus_RdAck <= slv_read_ack;
  IP2Bus_Error <= '0';
	
	-- Create counter for read timer and pulse timer
	counter: process (Bus2IP_Clk)
	begin
		if (Bus2IP_Resetn = '0') then
			read_counter <= (others => '0');
			pulse_counter <= (others => '0');
      rx_state_reg <= RESET;
      tx_state_reg <= RST;
			--pulse_on <= '0';
		elsif (Bus2IP_Clk 'event and Bus2IP_Clk = '1') then
			if read_counter = READ_COUNTER_LIMIT then
        read_counter <= (others => '0'); -- Rollover read controller timer
				--pulse_on <= '1';
				--button = (others => '0'); -- Reset button values
				pulse_timer <= (others => '0'); -- Reset pulse timer
      else
				read_counter <= read_counter + 1;
				--pulse_on <= '0';				
      end if;
			
			rx_state_reg <= rx_state_next;
			tx_state_reg <= tx_state_next;
			
			if(clrTimer='1' or pulse_timer = PULSE_COUNTER_LIMIT) then
				pulse_timer <= (others => '0');
			else
				pulse_timer <= pulse_timer + 1;
			end if;
		end if;
	end process counter;

  --Control logic
  pulse_sig <= '1' when pulse_timer = PULSE_COUNTER_LIMIT else '0';
  
  pulse_sig_half <= '1' when pulse_timer = PULSE_COUNTER_LIMIT/2 else '0';
    
  --Next state logic
  process(rx_state_reg, tx_state_reg, pulse_sig, pulse_sig_half, data_in, read_counter)
  begin
    --Defaults
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
          clrTimer <= '1'; -- restart the pulse_sig timer
        end if;
      when BA => 
        if(tx_state_reg=PULSE) then
          button(0) <= not data_in; -- save A Button state
          if(pulse_sig_half='1') then -- wait half pulse and then start again
            clrTimer <= '1'; -- restart the pulse_sig timer
            rx_state_next <= BB; -- set next state to BB
          end if;
        end if;
      when BB => 
        if(pulse_sig='1') then
          rx_state_next <= BSEL;
        end if;
        if(pulse_sig_half='1') then
          button(1) <= not data_in; -- save B Button state
        end if;
      when BSEL => 
        if(pulse_sig='1') then
          rx_state_next <= BSTRT;
        end if;
        if(pulse_sig_half='1') then
          button(2) <= not data_in; -- save Select Button state
        end if;
      when BSTRT => 
        if(pulse_sig='1') then
          rx_state_next <= BUP;
        end if;
        if(pulse_sig_half='1') then
          button(3) <= not data_in; -- save Start Button state
        end if;
      when BUP => 
        if(pulse_sig='1') then
          rx_state_next <= BDOWN;
        end if;
        if(pulse_sig_half='1') then
          button(4) <= not data_in; -- save Up Button state
        end if;
      when BDOWN => 
        if(pulse_sig='1') then
          rx_state_next <= BLEFT;
        end if;
        if(pulse_sig_half='1') then
          button(5) <= not data_in; -- save Down Button state
        end if;
      when BLEFT => 
        if(pulse_sig='1') then
          rx_state_next <= BRIGHT;
        end if;
        if(pulse_sig_half='1') then
          button(6) <= not data_in; -- save Left Button state
        end if;
      when BRIGHT => 
        if(pulse_sig='1') then
          rx_state_next <= DONE;
        end if;
        if(pulse_sig_half='1') then
          button(7) <= not data_in; -- save Right Button state
        end if;
      when DONE =>
        if(pulse_sig_half='1') then
          if(data_in='0') then
            data_done <= '1';
          end if;
        end if;
        rx_state_next <= IDLE;
    end case;

    tx_state_next <= tx_state_reg;
    case tx_state_reg is
      when RST => 
        if(pulse_sig='0') then
          tx_state_next <= LATCH;
          clrTimer <= '1'; -- reset the timer
        end if;
      when IDL => 
        if(read_counter=READ_COUNTER_LIMIT) then
          state_next <= LATCH;
          clrTimer <= '1'; -- reset the timer
        end if;
      when LATCH => 
        if(pulse_sig='1') then
          state_next <= PULSE;
        end if;
      when PULSE => 
        if(pulse_sig='1') then
          pulse_out <= '1';
        end if;
        if(pulse_sig_half='1') then
          pulse_out <= '0';
        end if;
        if(data_done='1') then
          pulse_out <= '0';
          state_next <= IDL;
        end if;
    end case;
  end process;
    
  --Output logic
	-- write buttons to slv register
	-- assert pulse
	-- assert latch
	-- Attach button values to LEDs
	LEDs <= button when data_done='1' else (others => '0');
	
  rx_busy <= '0' when (state_reg=IDLE) else
          '1';
  data_strobe <= '1' when data_done='1' else
            '0';
  data_out <= shift_reg when data_done='1' else
          "00000000";
  clrTimer <= '1' when state_reg=IDLE or state_reg=POWER_UP else
          '0';

end IMP;
