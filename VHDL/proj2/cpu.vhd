-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2017 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Roman Yaremchuk LOGIN: XYAREM00
--            

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni z pameti (DATA_RDWR='0') / zapis do pameti (DATA_RDWR='1')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WE musi byt '0'
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- zde dopiste potrebne deklarace signalu
type fsm_state is (
IDLE, --
DECODER, --
DATA_INCREMEN, DATA_INCREMEN_DO, DATA_INCREMEN_FINISH,-- --
DATA_DECRIMEN, DATA_DECRIMEN_DO, DATA_DECRIMEN_FINISH,-- --
PTR_INCREMEN, PTR_DECRIMEN,-- --
DECINCEND, --
PRINT, DO_PRINT, -- --
GETSS, DO_GETSS, -- --
DOWHILE, DOWHILE2, DOWHILE3, -- -- --
STOPWHILE, STOPWHILE2, STOPWHILE3, STOPWHILE4,-- -- -- --
BREAK, 
MAKE_VHDL_GREAT_AGAIN, OTHERSS, HALTS
   );

signal nstate : fsm_state;
signal pstate : fsm_state;


signal reg_pc : std_logic_vector(11 downto 0);
signal pc_incr, pc_decr : std_logic;

signal reg_ptr: std_logic_vector (9 downto 0);
signal ptr_incr, ptr_decr: std_logic;

signal reg_cnt: std_logic_vector (7 downto 0);
signal cnt_incr, cnt_decr: std_logic;

signal multiplexor: std_logic_vector(1 downto 0);

   
begin

   reg_pc_process: process(CLK, RESET)
   begin
      if (RESET = '1') then
         reg_pc <= "000000000000";
      elsif ((CLK'event)and(CLK = '1')) then
         if (pc_incr = '1') then 
            reg_pc <= reg_pc + 1;
         elsif (pc_decr = '1') then
            reg_pc <= reg_pc - 1;
         end if;
         end if;
   end process;

   reg_ptr_process: process(CLK, RESET)
   begin
      if (RESET = '1') then
         reg_ptr <= "0000000000";
         elsif ((CLK'event)and(CLK = '1')) then 
            if (ptr_incr = '1') then
               reg_ptr <= reg_ptr +1;
            elsif (ptr_decr = '1') then 
               reg_ptr <= reg_ptr -1;
         end if;
         end if;
   end process;

DATA_ADDR <= reg_ptr;
CODE_ADDR <= reg_pc;

reg_cnt_process:process (CLK, RESET)
begin
   if (RESET = '1') then
      reg_cnt <= "00000000";
    elsif ((CLK'event)and(CLK = '1')) then 
    if (cnt_incr = '1') then
      reg_cnt <= reg_cnt +1;
   elsif (cnt_decr = '1') then
      reg_cnt <= reg_cnt -1;
   end if;
   end if;
end process;

MUX: process (CLK, MULTIPLEXOR, DATA_RDATA, IN_DATA)
begin
      case MULTIPLEXOR is
         when "00" => DATA_WDATA <= IN_DATA;
         when "01" => DATA_WDATA <= DATA_RDATA + 1;
         when "10" => DATA_WDATA <= DATA_RDATA - 1;
         when "11" => OUT_DATA <= DATA_RDATA; 
         when others =>
         end case;
         end process;

fsm_pstate: process(CLK, RESET)
  begin
    if (RESET = '1') then
      pstate <= IDLE;
    elsif (CLK'event and CLK = '1') then
      if(EN = '1') then
        pstate <= nstate;
      end if;
    end if;
  end process;

fsm_nstate: process (pstate, CLK, RESET, EN, CODE_DATA, DATA_RDATA, IN_DATA, IN_VLD, OUT_BUSY)
  begin                                                                                                                               
    nstate <= IDLE;--
    CODE_EN <= '1';
    ptr_incr <= '0';
    ptr_decr <= '0';
    pc_incr <= '0'; --
    pc_decr <= '0'; --
    cnt_incr <= '0';
    cnt_decr <= '0';
    DATA_EN <= '0';
    OUT_WE <= '0';
    IN_REQ <= '0';


    case pstate is
      when IDLE =>
        CODE_EN <= '1';
        nstate <= DECODER;
    
       when DECODER =>         
      if (CODE_DATA = X"3E") then 
         nstate <=PTR_INCREMEN;
      elsif (CODE_DATA = X"3C") then 
         nstate <= PTR_DECRIMEN;
      elsif (CODE_DATA = X"2B") then 
         nstate <= DATA_INCREMEN;
      elsif (CODE_DATA = X"2D") then 
         nstate <= DATA_DECRIMEN;
      elsif (CODE_DATA = X"5B") then   
         nstate <= DOWHILE;
      elsif (CODE_DATA = X"5D") then 
         nstate <= STOPWHILE;
      elsif (CODE_DATA = X"2E") then   
         nstate <= PRINT;
      elsif (CODE_DATA = X"2C") then   
         nstate <= GETSS;
      elsif (CODE_DATA = X"7E") then
         nstate <= BREAK;
      elsif (CODE_DATA = X"00") then   
         nstate <= HALTS;
      else nstate <= OTHERSS;
      end if;
        
      when PTR_INCREMEN =>
        ptr_incr <= '1';
        nstate <= MAKE_VHDL_GREAT_AGAIN;
      
      when PTR_DECRIMEN =>
        ptr_decr <= '1';
        nstate <= MAKE_VHDL_GREAT_AGAIN;
    
      when DATA_INCREMEN =>
        DATA_RDWR <= '0';
        DATA_EN   <= '1';
        nstate    <= DATA_INCREMEN_DO;
    
      when DATA_INCREMEN_DO =>
        MULTIPLEXOR         <= "01";
        DATA_RDWR <= '1';
        DATA_EN   <= '1';
        nstate      <= DECINCEND;
      
      when DATA_DECRIMEN =>
        DATA_RDWR <= '0';
        DATA_EN   <= '1';
        nstate      <= DATA_DECRIMEN_DO;
      
      when DATA_DECRIMEN_DO =>
        MULTIPLEXOR         <= "10";
        DATA_RDWR <= '1';
        DATA_EN   <= '1';
        nstate      <= DECINCEND; 
        
      when DECINCEND =>
        pc_incr      <= '1';
        nstate      <= IDLE;
    
      when PRINT =>
        if(OUT_BUSY = '1') then
          nstate <= PRINT;
        else
          nstate <= DO_PRINT;
         end if;
      
      when DO_PRINT =>
          DATA_RDWR <= '0';
          MULTIPLEXOR         <= "11";
          OUT_WE <= '1';
          nstate <= MAKE_VHDL_GREAT_AGAIN;
       
  
      when GETSS =>
        IN_REQ <= '1';
        if (IN_VLD = '0') then
          nstate <= GETSS;         
        else
          nstate <= DO_GETSS;
        end if;
        
       when DO_GETSS =>--!PEREDELAL NO NE PROVERIL
          IN_REQ <= '1';
          MULTIPLEXOR         <= "00";
          DATA_RDWR <= '1';
          DATA_EN <= '1';
          nstate <= MAKE_VHDL_GREAT_AGAIN;  
          
         
      when DOWHILE =>
        pc_incr <= '1';
        DATA_RDWR <= '0';
        DATA_EN <= '1';
        if (DATA_RDATA = "00000000") then
          nstate <= DOWHILE2;
        else
          nstate <= IDLE;  
        end if;
        
      when DOWHILE2 =>
        if (reg_cnt = "00000000") then
          cnt_incr <= '1';
          nstate <= IDLE;
        else
          CODE_EN <= '1';
          nstate <= DOWHILE3;
        end if;
        
      when DOWHILE3 =>
      DATA_EN <= '1';
      case CODE_DATA is
      when  X"5B" => cnt_incr <= '1';pc_incr <= '1'; nstate <= DOWHILE2;
      when  X"5D" => cnt_decr <= '1';pc_incr <= '1'; nstate <= DOWHILE2;
      when others => pc_incr <= '1'; nstate <= DOWHILE2; --mb tut next state
      end case;
      
    
      when STOPWHILE =>
        DATA_RDWR <= '0';
        DATA_EN <= '1';
        if (DATA_RDATA = "00000000") then
          nstate <= MAKE_VHDL_GREAT_AGAIN;
        else
          pc_decr <= '1';
          cnt_incr <= '1';      
          nstate <= STOPWHILE2;
        end if;
        
      when STOPWHILE2 =>
        if (reg_cnt = "00000000") then
          nstate <= IDLE;
        else
          CODE_EN <= '1';
          nstate <= STOPWHILE3;
        end if;
      
      when STOPWHILE3 =>  
        DATA_EN <= '1';
      case CODE_DATA is
      when  X"5B" => cnt_decr <= '1'; nstate <= STOPWHILE4;
      when  X"5D" => cnt_incr <= '1'; nstate <= STOPWHILE4;
      when others => nstate <= STOPWHILE4; --mb tut next state
      end case;


      when STOPWHILE4 =>
        if (reg_cnt = "00000000") then
          pc_incr <= '1';
        else
          pc_decr <= '1';
        end if;
        nstate <= STOPWHILE2;
      
      when BREAK =>
         pc_incr <= '1';
         if (CODE_DATA = X"5D") then
            pc_incr <= '1';
            nstate  <=IDLE;
         else
            nstate  <= BREAK;
         end if;
     
      when OTHERSS =>
        pc_incr <= '1';
        nstate <= IDLE;
      
      when MAKE_VHDL_GREAT_AGAIN =>
        pc_incr  <= '1';
        nstate  <=IDLE;
        
      when HALTS =>
        nstate <= HALTS;
    
      when others => 

    end case;
  end process;

 -- zde dopiste vlastni VHDL kod

end behavioral;
 
