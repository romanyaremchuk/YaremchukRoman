-- fsm.vhd: Finite State Machine
-- Author(s): 
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
	CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is ( PRINT_MESSAGE, PRINT_MESSAGE_OK, FINISH, KEY1_OR_2_1, KEY1_OR_2_2, KEY1_3, KEY1_4, KEY1_5, KEY1_6, KEY1_7, KEY1_8, KEY1_9, KEY1_10, KEY2_3, KEY2_4, KEY2_5, KEY2_6, KEY2_7, KEY2_8, KEY2_9, KEY2_10, KEY_FULL, WRONG_KEY);
   signal present_state, next_state: t_state;
begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= KEY1_OR_2_1 ;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when KEY1_OR_2_1 =>
      next_state <= KEY1_OR_2_1;
		if (KEY(1) = '1') then
         next_state <= KEY1_OR_2_2;
		else if (KEY(14 downto 2) /= "0000000000000") OR (KEY(0) /= '0') then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      end if;
      end if;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -	
   when KEY1_OR_2_2 =>
      next_state <= KEY1_OR_2_2;
		if (KEY(1) = '1') then
         next_state <= KEY1_3;
		else if (KEY(7) = '1') then
			next_state <= KEY2_3;
		else if (KEY(14 downto 8) /= "0000000") OR (KEY(6 downto 2) /="00000") OR (KEY(0) /= '0') then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE;  
      end if;
      end if;
      end if;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -	
   when KEY1_3 =>
      next_state <= KEY1_3;
		if (KEY(5) = '1') then
         next_state <= KEY1_4;
		else if (KEY(14 downto 6) /= "000000000") OR (KEY(4 downto 0) /= "00000") then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      end if;
      end if;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -	
   when KEY1_4 =>
      next_state <= KEY1_4;
		if (KEY(0) = '1') then
         next_state <= KEY1_5;
		else if (KEY(14 downto 1) /= "00000000000000") then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      end if;
      end if;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -	
   when KEY1_5 =>
      next_state <= KEY1_5;
		if (KEY(2) = '1') then
         next_state <= KEY1_6;
		else if (KEY(14 downto 3) /= "000000000000") OR (KEY(1 downto 0) /="00") then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      end if;
      end if;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -	
   when KEY1_6 =>
      next_state <= KEY1_6;
		if (KEY(4) = '1') then
         next_state <= KEY1_7;
		else if (KEY(14 downto 5) /= "0000000000") OR (KEY(3 downto 0) /="0000") then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      end if;
      end if;
      end if;
		-- - - - - - - - - - - - - - - - - - - - - - -
	when KEY1_7 =>
      next_state <= KEY1_7;
		if (KEY(7) = '1') then
         next_state <= KEY1_8;
		else if (KEY(14 downto 8) /= "0000000") OR (KEY(6 downto 0) /= "0000000")then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      end if;
      end if;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when KEY1_8 =>
      next_state <= KEY1_8;
		if (KEY(1) = '1') then
         next_state <= KEY1_9;
		else if (KEY(14 downto 2) /= "0000000000000") OR (KEY(0) /= '0')then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      end if;
      end if;
      end if;
   
   -- - - - - - - - - - - - - - - - - - - - - - -	
   when KEY1_9 =>
      next_state <= KEY1_9;
		if (KEY(5) = '1') then
         next_state <= KEY1_10;
		else if (KEY(14 downto 6) /= "000000000") OR (KEY(4 downto 0) /= "00000")then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      end if;
      end if;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -	
   when KEY1_10 =>
      next_state <= KEY1_10;
		if (KEY(0) = '1') then
         next_state <= KEY_FULL;
		else if (KEY(14 downto 1) /= "00000000000000") then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      end if;
      end if;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -	
   when KEY2_3 =>
      next_state <= KEY2_3;
		if (KEY(6) = '1') then
         next_state <= KEY2_4;
		else if (KEY(14 downto 7) /= "00000000") OR (KEY(5 downto 0) /= "000000") then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      end if;
      end if;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -	
   when KEY2_4 =>
      next_state <= KEY2_4;
			if (KEY(0) = '1') then
         next_state <= KEY2_5;
		else if (KEY(14 downto 1) /= "00000000000000") then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      end if;
      end if;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when KEY2_5 =>
      next_state <= KEY2_5;
		if (KEY(5) = '1') then
         next_state <= KEY2_6;
		else if (KEY(14 downto 6) /= "000000000") OR (KEY(4 downto 0) /= "00000") then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      end if;
      end if;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -	
   when KEY2_6 =>
      next_state <= KEY2_6;
		if (KEY(9) = '1') then
         next_state <= KEY2_7;
		else if (KEY(14 downto 10) /= "00000") OR (KEY(8 downto 0) /= "000000000") then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      end if;
      end if;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -	
   when KEY2_7 =>
      next_state <= KEY2_7;
		if (KEY(3) = '1') then
         next_state <= KEY2_8;
		else if (KEY(14 downto 4) /= "00000000000") OR (KEY(2 downto 0) /= "000") then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      end if;
      end if;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -	
   when KEY2_8 =>
      next_state <= KEY2_8;
		if (KEY(1) = '1') then
         next_state <= KEY2_9;
		else if (KEY(14 downto 2) /= "0000000000000") OR (KEY(0) /= '0') then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      end if;
      end if;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -	
   when KEY2_9 =>
      next_state <= KEY2_9;
		if (KEY(6) = '1') then
         next_state <= KEY2_10;
		else if (KEY(14 downto 7) /= "00000000") OR (KEY(5 downto 0) /= "000000") then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      end if;
      end if;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -	
   when KEY2_10 =>
      next_state <= KEY2_10;
		if (KEY(1) = '1') then
         next_state <= KEY_FULL;
		else if (KEY(14 downto 2) /= "0000000000000") OR (KEY(0) /= '0') then
         next_state <= WRONG_KEY;
		else if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE; 
      end if;
      end if;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -	
   when KEY_FULL =>
      next_state <= KEY_FULL;
		if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_OK;
		else if (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG_KEY; 
      end if;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when WRONG_KEY =>
      next_state <= WRONG_KEY;
		if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE =>
      next_state <= PRINT_MESSAGE;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when PRINT_MESSAGE_OK =>
		next_state <= PRINT_MESSAGE_OK;
		if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= KEY1_OR_2_1; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= WRONG_KEY;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_CLR    <= '0';
   FSM_LCD_WR     <= '0';
	
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY1_OR_2_1 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY1_OR_2_2 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY1_3 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY1_4 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY1_5 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY1_6 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY1_7 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY1_8 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY1_9 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY1_10 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY2_3 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY2_4 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY2_5 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY2_6 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY2_7 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY2_8 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY2_9 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY2_10 =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when WRONG_KEY =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when KEY_FULL =>
	if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		end if;
		if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
	when PRINT_MESSAGE_OK =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
		FSM_MX_MEM		<= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -		
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
when others =>
   end case;
end process output_logic;

end architecture behavioral;

