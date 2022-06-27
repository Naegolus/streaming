
-- ******************************************************** --
--                                                          --
--                                                          --
--   ____-_-_-_-_-_-_-_-________________________            --
--                                                          --
--                                                          --
--   File:          ext_mod_gpio-Rtl-ea.vhd                 --
--   Description:   External GPIO module for scarts32       --
--   Autor:         Johannes Natter                         --
--   Date:          08.06.2012                              --
--                                                          --
--   Notes:                                                 --
--                                                          --
--   Updates:                                               --
--       Date: dd.mm.yy   Autor:                            --
--       Changes:                                           --
--                                                          --
-- ******************************************************** --

library ieee;
use ieee.std_logic_1164.all;
use work.scarts_pkg.all;

entity em_gpio is
    port
    (
        clk     : in  std_logic;
        extsel  : in  std_ulogic;
        exti    : in  module_in_type;
        exto    : out module_out_type;
        i_gpi   : in  std_ulogic_vector(15 downto 0);
        o_gpo   : out std_ulogic_vector(15 downto 0)
    );
end entity em_gpio;

architecture Rtl of em_gpio is

    constant REG_GPO0 : natural := 4;
    constant REG_GPO1 : natural := 5;
    constant REG_GPI0 : natural := 6;
    constant REG_GPI1 : natural := 7;

    subtype byte is std_logic_vector(7 downto 0);
    type REGISTER_SET_TYPE is array (0 to 7) of byte;

    type REG_TYPE is record
        rset    : REGISTER_SET_TYPE;
    end record;
    
    constant RSET_INIT_VAL : REG_TYPE :=
    (
        rset => (others => (others => '0'))
    );
    
    signal R, NxR : REG_TYPE;
    signal rstint : std_ulogic;

begin

    proc_comb: process(R, exti, extsel, i_gpi)
    begin
    
        NxR <= R;
        
        -- write memory mapped addresses
        if((extsel = '1') and (exti.write_en = '1'))then
            case exti.addr(4 downto 2) is
                when "000" =>
                    if((exti.byte_en(0) = '1') or (exti.byte_en(1) = '1'))then
                        NxR.rset(STATUSREG)(STA_INT) <= '1';
                        NxR.rset(CONFIGREG)(CONF_INTA) <= '0';
                    else
                        if(exti.byte_en(2) = '1')then
                            NxR.rset(2) <= exti.data(23 downto 16);
                        end if;
                        if(exti.byte_en(3) = '1')then
                            NxR.rset(3) <= exti.data(31 downto 24);
                        end if;
                    end if;

                when "001" =>
                    if(exti.byte_en(0) = '1')then
                        NxR.rset(REG_GPO0) <= exti.data(7 downto 0);
                    end if;
                    if(exti.byte_en(1) = '1')then
                        NxR.rset(REG_GPO1) <= exti.data(15 downto 8);
                    end if;

                when others =>
                    null;
            end case;
        end if;
        
        -- read memory mapped addresses
        exto.data <= (others => '0');
        if((extsel = '1') and (exti.write_en = '0'))then
            case exti.addr(4 downto 2) is
                when "000" =>
                    exto.data <= R.rset(3) & R.rset(2) & R.rset(1) & R.rset(0);
                when "001" =>
                    if(R.rset(CONFIGREG)(CONF_ID) = '1')then
                        exto.data <= MODULE_VER & MODULE_ID;
                    else
                        exto.data <= R.rset(REG_GPI1) & R.rset(REG_GPI0) & R.rset(REG_GPO1) & R.rset(REG_GPO0);
                    end if;
                when others =>
                    null;
            end case;
        end if;
        
        -- compute status flags
        NxR.rset(STATUSREG)(STA_LOOR) <= R.rset(CONFIGREG)(CONF_LOOW);
        NxR.rset(STATUSREG)(STA_FSS) <= '0';
        NxR.rset(STATUSREG)(STA_RESH) <= '0';
        NxR.rset(STATUSREG)(STA_RESL) <= '0';
        NxR.rset(STATUSREG)(STA_BUSY) <= '0';
        NxR.rset(STATUSREG)(STA_ERR) <= '0';
        NxR.rset(STATUSREG)(STA_RDY) <= '1';
        
        -- set output enabled (default)
        NxR.rset(CONFIGREG)(CONF_OUTD) <= '1';
        
        -- module specific part
        o_gpo <= std_ulogic_vector(R.rset(REG_GPO1)) & std_ulogic_vector(R.rset(REG_GPO0));
        NxR.rset(REG_GPI0) <= std_logic_vector(i_gpi(7 downto 0));
        NxR.rset(REG_GPI1) <= std_logic_vector(i_gpi(15 downto 8));
        
        -- combine soft- and hard-reset
        rstint <= not RST_ACT;
        if((exti.reset = RST_ACT) or (R.rset(CONFIGREG)(CONF_SRES) = '1'))then
            rstint <= RST_ACT;
        end if;
        
        -- reset interrupt
        if((R.rset(STATUSREG)(STA_INT) = '1') and (R.rset(CONFIGREG)(CONF_INTA) = '0'))then
            NxR.rset(STATUSREG)(STA_INT) <= '0';
        end if;
        exto.intreq <= R.rset(STATUSREG)(STA_INT);
    
    end process;
    
    proc_reg: process(clk)
    begin
        if((clk'event) and (clk = '1'))then
            if(rstint = RST_ACT)then
                R <= RSET_INIT_VAL;
            else
                R <= NxR;
            end if;
        end if;
    end process;
    
end architecture Rtl;
