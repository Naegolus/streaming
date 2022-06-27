
-- ******************************************************** --
--                                                          --
--                                                          --
--   ____-_-_-_-_-_-_-_-________________________            --
--                                                          --
--                                                          --
--   File:          ext_mod_spi-Rtl-ea.vhd                  --
--   Description:   External SPI Master module for scarts32 --
--   Autor:         Johannes Natter                         --
--   Date:          12.06.2012                              --
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

entity em_spi is
    port
    (
        clk         : in  std_logic; -- !!!MUST be 50 MHz!!!
        extsel      : in  std_ulogic;
        exti        : in  module_in_type;
        exto        : out module_out_type;
        i_miso      : in  std_ulogic;
        o_spi_clk   : out std_ulogic;
        o_mosi      : out std_ulogic
    );
end entity em_spi;

architecture Rtl of em_spi is

    constant USR_STATUSREG      : natural := 1;
    constant USR_STAT_SPI_BUSY  : natural := 0;
    
    constant USR_CONFIGREG      : natural := 3;
    constant USR_CONFIG_SPI_LSB : natural := 0;
    constant USR_CONFIG_SPI_2X  : natural := 1;
    
    constant WDATAREG           : natural := 4;
    constant RDATAREG           : natural := 5;

    subtype byte is std_logic_vector(7 downto 0);
    type REGISTER_SET_TYPE is array (0 to 7) of byte;
    type STATEMACHINE_STEP_TYPE is (SM_WAIT_SPI_DATA_WRITTEN, SM_SEND_SPI_DATA);

    type REG_TYPE is record
        rset                : REGISTER_SET_TYPE;
        sm_step             : STATEMACHINE_STEP_TYPE;
        spi_data_written    : std_ulogic;
        bit_idx             : natural;
        scale_bit           : std_ulogic;
        spi_clk             : std_ulogic;
        miso_sync_1         : std_ulogic;
        miso_sync           : std_ulogic;
        lsb_latch           : std_ulogic;
        clock_2x_latch      : std_ulogic;
    end record;
    
    constant RSET_INIT_VAL : REG_TYPE :=
    (
        rset                => (others => (others => '0')),
        sm_step             => SM_WAIT_SPI_DATA_WRITTEN,
        spi_data_written    => '0',
        bit_idx             => 0,
        scale_bit           => '0',
        spi_clk             => '0',
        miso_sync_1         => '0',
        miso_sync           => '0',
        lsb_latch           => '0',
        clock_2x_latch      => '0'
    );
    
    signal R, NxR : REG_TYPE;
    signal rstint : std_ulogic;

begin

    proc_comb: process(R, exti, extsel, i_miso)
        variable v_miso : std_ulogic := '0';
    begin
    
        NxR <= R;
        
        -- standard values
        NxR.spi_data_written <= '0';
        
        -- write memory mapped addresses
        if((extsel = '1') and (exti.write_en = '1'))then
            case exti.addr(4 downto 2) is
                when "000" =>
                    if((exti.byte_en(0) = '1') or (exti.byte_en(1) = '1'))then
                        NxR.rset(STATUSREG)(STA_INT) <= '1';
                        NxR.rset(CONFIGREG)(CONF_INTA) <= '0';
                    else
                        if(exti.byte_en(2) = '1')then
                            NxR.rset(CONFIGREG) <= exti.data(23 downto 16);
                        end if;
                        if(exti.byte_en(3) = '1')then
                            NxR.rset(USR_CONFIGREG) <= exti.data(31 downto 24);
                        end if;
                    end if;

                when "001" =>
                    if(exti.byte_en(0) = '1')then
                        NxR.rset(WDATAREG) <= exti.data(7 downto 0);
                        
                        NxR.spi_data_written <= '1';
                        NxR.rset(USR_STATUSREG)(USR_STAT_SPI_BUSY) <= '1';
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
                        exto.data <= R.rset(7) & R.rset(6) & R.rset(RDATAREG) & R.rset(WDATAREG);
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
        NxR.miso_sync_1 <= i_miso;
        NxR.miso_sync <= R.miso_sync_1;
        
        o_spi_clk <= R.spi_clk;
        o_mosi <= '0';
        
        case R.sm_step is
            when SM_WAIT_SPI_DATA_WRITTEN =>
                NxR.rset(USR_STATUSREG)(USR_STAT_SPI_BUSY) <= '0';
                NxR.spi_clk <= '0';
                
                if(R.spi_data_written = '1')then
                    NxR.rset(USR_STATUSREG)(USR_STAT_SPI_BUSY) <= '1';
                    NxR.lsb_latch <= R.rset(USR_CONFIGREG)(USR_CONFIG_SPI_LSB);
                    NxR.clock_2x_latch <= R.rset(USR_CONFIGREG)(USR_CONFIG_SPI_2X);
                    NxR.scale_bit <= '1';
                    NxR.spi_clk <= '0';
                    NxR.bit_idx <= 7;

                    NxR.sm_step <= SM_SEND_SPI_DATA;
                end if;

            when SM_SEND_SPI_DATA =>
                NxR.rset(USR_STATUSREG)(USR_STAT_SPI_BUSY) <= '1';
                
                if(R.lsb_latch = '0')then
                    o_mosi <= R.rset(WDATAREG)(R.bit_idx);
                else
                    o_mosi <= R.rset(WDATAREG)(7 - R.bit_idx);
                end if;
                
                NxR.scale_bit <= not(R.scale_bit);

                if((R.scale_bit = '0') or (R.clock_2x_latch = '1'))then
                    NxR.spi_clk <= not(R.spi_clk);
                    
                    if(R.spi_clk = '1')then
                        if(R.clock_2x_latch = '1')then
                            v_miso := R.miso_sync_1;
                        else
                            v_miso := R.miso_sync;
                        end if;
                    
                        if(R.lsb_latch = '0')then
                            NxR.rset(RDATAREG)(R.bit_idx) <= v_miso;
                        else
                            NxR.rset(RDATAREG)(7 - R.bit_idx) <= v_miso;
                        end if;
                        
                        if(R.bit_idx = 0)then
                            NxR.sm_step <= SM_WAIT_SPI_DATA_WRITTEN;
                        else
                            NxR.bit_idx <= R.bit_idx - 1;
                        end if;
                    end if;
                end if;

            when others =>
                null;
        end case;
        
        
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
