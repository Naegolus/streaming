
-- ******************************************************** --
--                                                          --
--                                                          --
--   ____-_-_-_-_-_-_-_-________________________            --
--                                                          --
--                                                          --
--   File:          ext_mod_servo-Rtl-ea.vhd                --
--   Description:   External servo motor module (scarts32)  --
--   Autor:         Johannes Natter                         --
--   Date:          07.03.2013                              --
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
use ieee.numeric_std.all;
use work.scarts_pkg.all;

entity em_servo is
    port
    (
        clk     : in  std_logic;
        extsel  : in  std_ulogic;
        exti    : in  module_in_type;
        exto    : out module_out_type;
        o_pwm   : out std_ulogic_vector(3 downto 0)
    );
end entity em_servo;

architecture Rtl of em_servo is

    constant CLK_PERIOD             : time := 20 ns;
    -- constant PWM_MIN_HIGH_PULSE     : time := 1 ms; 
    -- constant PWM_MAX_HIGH_PULSE     : time := 2 ms;
    constant PWM_DEFAULT_HIGH_PULSE : time := 1.5 ms; -- 1.5 ms, 2 us
    constant PWM_DEFAULT_LOW_PULSE  : time := 18 ms; -- 18 ms, 10 us -- not critical

    -- constant SERVO_MIN_TICKS        : natural := PWM_MIN_HIGH_PULSE / CLK_PERIOD; --  50.000
    -- constant SERVO_MAX_TICKS        : natural := PWM_MAX_HIGH_PULSE / CLK_PERIOD; -- 100.000
    constant SERVO_DEFAULT_TICKS    : natural := PWM_DEFAULT_HIGH_PULSE / CLK_PERIOD;   -- 75.000
    constant SERVO_TICKS_18MS       : natural := PWM_DEFAULT_LOW_PULSE / CLK_PERIOD;

    constant USR_CONFIGREG          : natural := 3;
    constant USR_CONFIG_PWM0_ENABLE : natural := 0;
    -- constant USR_CONFIG_PWM1_ENABLE : natural := 1;
    -- constant USR_CONFIG_PWM2_ENABLE : natural := 2;
    -- constant USR_CONFIG_PWM3_ENABLE : natural := 3;
    
    constant REG_PWM0_0 : natural := 4;
    constant REG_PWM0_1 : natural := 5;
    constant REG_PWM0_2 : natural := 6;
    constant REG_PWM0_3 : natural := 7;
    
    constant SERVO_PWM0 : natural := USR_CONFIG_PWM0_ENABLE;
    -- constant SERVO_PWM1 : natural := USR_CONFIG_PWM1_ENABLE;
    -- constant SERVO_PWM2 : natural := USR_CONFIG_PWM2_ENABLE;
    -- constant SERVO_PWM3 : natural := USR_CONFIG_PWM3_ENABLE;

    subtype byte is std_logic_vector(7 downto 0);
    type REGISTER_SET_TYPE is array (0 to 7) of byte;
    type STATEMACHINE_STEP_TYPE is (SM_INIT_MIDDLE_POS, SM_LOW_PULSE, SM_HIGH_PULSE);

    type REG_TYPE is record
        rset                : REGISTER_SET_TYPE;
        sm_step             : STATEMACHINE_STEP_TYPE;
        tick_cnt            : natural;
    end record;
    
    constant RSET_INIT_VAL : REG_TYPE :=
    (
        rset                => (others => (others => '0')),
        sm_step             => SM_INIT_MIDDLE_POS,
        tick_cnt            => 0
    );
    
    signal R, NxR : REG_TYPE;
    signal rstint : std_ulogic;

begin

    proc_comb: process(R, exti, extsel)
        variable v_tick_val_vec : std_logic_vector(31 downto 0) := (others => '0');
        variable v_tick_cnt : natural;
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
                        NxR.rset(REG_PWM0_0) <= exti.data( 7 downto  0);
                    end if;
                    if(exti.byte_en(1) = '1')then
                        NxR.rset(REG_PWM0_1) <= exti.data(15 downto  8);
                    end if;
                    if(exti.byte_en(2) = '1')then
                        NxR.rset(REG_PWM0_2) <= exti.data(23 downto 16);
                    end if;
                    if(exti.byte_en(3) = '1')then
                        NxR.rset(REG_PWM0_3) <= exti.data(31 downto 24);
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
                        exto.data <= R.rset(REG_PWM0_3) & R.rset(REG_PWM0_2) & R.rset(REG_PWM0_1) & R.rset(REG_PWM0_0);
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
        o_pwm <= (others => '0');
        
        case R.sm_step is
            when SM_INIT_MIDDLE_POS =>
                v_tick_val_vec := std_logic_vector(to_unsigned(SERVO_DEFAULT_TICKS, v_tick_val_vec'length));
                NxR.rset(REG_PWM0_0) <= v_tick_val_vec( 7 downto  0);
                NxR.rset(REG_PWM0_1) <= v_tick_val_vec(15 downto  8);
                NxR.rset(REG_PWM0_2) <= v_tick_val_vec(23 downto 16);
                NxR.rset(REG_PWM0_3) <= v_tick_val_vec(31 downto 24);
                
                NxR.tick_cnt <= SERVO_TICKS_18MS - 1; 
                
                NxR.sm_step <= SM_LOW_PULSE;
                
            when SM_LOW_PULSE =>
                if(R.tick_cnt /= 0)then
                    NxR.tick_cnt <= R.tick_cnt - 1;
                else
                    if(R.rset(USR_CONFIGREG)(USR_CONFIG_PWM0_ENABLE) = '1')then
                        -- PWM0 enabled
                        v_tick_val_vec := R.rset(REG_PWM0_3) & R.rset(REG_PWM0_2) & R.rset(REG_PWM0_1) & R.rset(REG_PWM0_0);
                        v_tick_cnt := to_integer(unsigned(v_tick_val_vec));
                        
                        if(v_tick_cnt /= 0)then
                            NxR.tick_cnt <= v_tick_cnt - 1;
                            
                            NxR.sm_step <= SM_HIGH_PULSE;
                        end if;
                    end if;
                end if;
                
            when SM_HIGH_PULSE =>
                o_pwm(SERVO_PWM0) <= '1';
            
                if(R.tick_cnt /= 0)then
                    NxR.tick_cnt <= R.tick_cnt - 1;
                else
                    NxR.tick_cnt <= SERVO_TICKS_18MS - 1;
                    
                    NxR.sm_step <= SM_LOW_PULSE;
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
