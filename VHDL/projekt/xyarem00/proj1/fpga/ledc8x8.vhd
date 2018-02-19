library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port ( 
	LED, ROW: out std_logic_vector (0 to 7);
	RESET, SMCLK: in std_logic
);
end ledc8x8;

architecture main of ledc8x8 is
	signal pix, lines, ncounter: std_logic_vector (7 downto 0);
	signal tfcounter: std_logic;
    -- Sem doplnte definice vnitrnich signalu.

begin

frek_down: process (SMCLK, RESET)
	begin
		if RESET = '1' then
			ncounter <= "00000000";
		elsif SMCLK'event and SMCLK = '1' then
			ncounter <= 1+ncounter;
		end if;
end process frek_down;

tfcounter <= '1' when ncounter = X"FF" else '0';


rotation: process (RESET, SMCLK, tfcounter)
	begin
		if RESET = '1' then
			lines <= "10000000";
		elsif tfcounter = '1' and SMCLK = '1' and SMCLK'event then
			lines <= lines (0) & lines( 7 downto 1);
		end if;
		end process rotation;
		
		print: process (lines)
		begin
		case lines is
			when "10000000" => pix <= "00011111";
			when "01000000" => pix <= "01010101";
			when "00100000" => pix <= "01010101";
			when "00010000" => pix <= "00110101";
			when "00001000" => pix <= "01011011";
			when "00000100" => pix <= "01101011";
			when "00000010" => pix <= "11111011";
			when "00000001" => pix <= "11111111";
			when others 	 => pix <= "11111111";
		end case;
	end process print;
    -- Nezapomente take doplnit mapovani signalu rozhrani na piny FPGA
    -- v souboru ledc8x8.ucf.
ROW <=lines;
LED <= pix;
end main;
