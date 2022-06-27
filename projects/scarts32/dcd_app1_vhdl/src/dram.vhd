-----------------------------------------------------------------------
-- This file is part of SCARTS.
-- 
-- SCARTS is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
-- 
-- SCARTS is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
-- 
-- You should have received a copy of the GNU General Public License
-- along with SCARTS.  If not, see <http://www.gnu.org/licenses/>.
-----------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.scarts_core_pkg.all;
use work.scarts_pkg.all;

entity scarts_dram is
  generic (
    CONF : scarts_conf_type);
  port (
    clk     : in  std_ulogic;
    hold    : in  std_ulogic;
    dramsel : in  std_ulogic;

    write_en  : in  std_ulogic;
    byte_en   : in  std_logic_vector(3 downto 0);
    data_in   : in  std_logic_vector(31 downto 0);
    addr      : in  std_logic_vector(CONF.data_ram_size-1 downto 2);

    data_out  : out std_logic_vector(31 downto 0));
end scarts_dram;


architecture behaviour of scarts_dram is

  signal enable, ram_wen : std_ulogic;
  
begin

    enable <= not hold;
    ram_wen <= write_en and dramsel;

    scarts_bootram_inst : scarts_bootram
        generic map
        (
            CONF    => CONF
        )
        port map
        (
            address => addr,
            byteena => byte_en,
            clken   => enable,
            clock   => clk,
            data    => data_in,
            wren    => ram_wen,
            q       => data_out
        );

    -- DEBUG Purposes
    --watch0_proc: process(addr)
    --begin
    --    if(addr = X"E0A")then
    --        watch0_addr_rd_wr <= '1';
    --    else
    --        watch0_addr_rd_wr <= '0';
    --    end if;
    --end process;

end behaviour;
