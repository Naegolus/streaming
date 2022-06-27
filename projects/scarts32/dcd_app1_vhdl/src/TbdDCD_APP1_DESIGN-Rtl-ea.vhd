
-- ******************************************************** --
--                                                          --
--                                                          --
--   ____-_-_-_-_-_-_-_-________________________            --
--                                                          --
--                                                          --
--   File:          TbdDCD_APP1_DESIGN-Rtl-ea.vhd           --
--   Description:   Testbed of DCD                          --
--   Autor:         Johannes Natter                         --
--   Date:          04.10.2012                              --
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

entity TbdDCD_APP1_DESIGN is
    generic
    (
        use_pll         : std_ulogic := '1'
    );
    port
    (
        CLOCK_50        : in    std_ulogic;                       -- Main Clock 50MHz
        KEY             : in    std_ulogic_vector(1 downto 0);    -- User keys
        SW              : in    std_ulogic_vector(3 downto 0);    -- DIP Switches
        D_RxD           : in    std_ulogic;                       -- Debug interface receive line
        D_TxD           : out   std_ulogic;                       -- Debug interface transmit line
        LED             : out   std_ulogic_vector(7 downto 0);    -- Debug LED
        SPI_CS          : out   std_ulogic_vector(3 downto 0);    -- Chip select's
        SPI_MISO        : in    std_ulogic;
        SPI_MOSI        : out   std_ulogic;
        SPI_CLK         : out   std_ulogic;
        SPI_EPCS_MISO   : in    std_ulogic;
        SPI_EPCS_MOSI   : out   std_ulogic;
        SPI_EPCS_CLK    : out   std_ulogic;
        ENC_CLK         : out   std_ulogic;
        DRAM_ADDR       : out   std_logic_vector(12 downto 0);
        DRAM_BA         : out   std_logic_vector(1 downto 0);
        DRAM_CKE        : out   std_logic;
        DRAM_CLK        : out   std_logic;
        DRAM_CS_N       : out   std_logic;
        DRAM_DQ         : inout std_logic_vector(15 downto 0);
        DRAM_DQM        : out   std_logic_vector(1 downto 0);
        DRAM_CAS_N      : out   std_logic;
        DRAM_RAS_N      : out   std_logic;
        DRAM_WE_N       : out   std_logic;
        PWM             : out   std_ulogic_vector(3 downto 0)
    );
end TbdDCD_APP1_DESIGN;

architecture Rtl of TbdDCD_APP1_DESIGN is

    component scarts_pll
    port
    (
        areset          : in  std_logic := '0';
        inclk0          : in  std_logic := '0';
        c0              : out std_logic;
        locked          : out std_logic
    );
    end component;

    constant c_scarts_conf : scarts_conf_type :=
    (
        tech                    => ALTERA,
        word_size               => 32,
        boot_rom_size           => 7,
        instr_ram_size          => 24,
        data_ram_size           => 15,
        use_iram                => true,
        use_amba                => false,
        amba_shm_size           => 8,
        amba_word_size          => 32,
        gdb_mode                => 1,
        bootrom_base_address    => 29
    );

    signal scarts_in    : scarts_in_type;
    signal scarts_out   : scarts_out_type;
    signal debug_in     : debug_if_in_type;
    signal debug_out    : debug_if_out_type;
    signal exti         : module_in_type;

    signal gpio_sel     : std_ulogic;
    signal gpio_exto    : module_out_type;
    signal gpi          : std_ulogic_vector(15 downto 0);
    signal gpo          : std_ulogic_vector(15 downto 0);

    signal spi_sel      : std_ulogic;
    signal spi_exto     : module_out_type;
    signal em_spi_miso  : std_ulogic;
    signal em_spi_mosi  : std_ulogic;
    signal em_spi_clk   : std_ulogic;
    
    signal altremote_sel  : std_ulogic;
    signal altremote_exto : module_out_type;
    
    signal time_sel     : std_ulogic;
    signal time_exto    : module_out_type;
    signal strobe250us  : std_ulogic;
    
    signal servo_sel    : std_ulogic;
    signal servo_exto   : module_out_type;
    
--    signal enc_clock    : std_ulogic;
    signal led_inv      : std_ulogic;

    signal reset_done   : std_ulogic := '0';
    signal scarts_resest_done : std_ulogic := '0';
    signal nReset       : std_ulogic;
    signal pll_reset    : std_ulogic;
    signal CLOCK_25     : std_ulogic;
    signal pll_locked   : std_ulogic;
    signal scarts_reset : std_ulogic;
    
begin

    -- Reset
    proc_reset: process(CLOCK_50)
    begin
        if CLOCK_50'event and CLOCK_50 = '1' then
            reset_done <= '1';
        end if;
    end process;
    nReset <= reset_done and KEY(1);

    -- ENC28J60
--    proc_enc_clk: process(CLOCK_50, nReset)
--    begin
--        if nReset = '0' then
--            enc_clock <= '0';
--        elsif CLOCK_50'event and CLOCK_50 = '1' then
--            enc_clock <= not(enc_clock);
--        end if;
--    end process;
    ENC_CLK <= CLOCK_25;

    eCLOCK_PLL : if(use_pll = '1')generate
    pll_reset <= not(nReset);
    eSCARTS_PLL : scarts_pll
    port map
    (
        areset              => pll_reset,
        inclk0              => CLOCK_50,
        c0                  => CLOCK_25,
        locked              => pll_locked
    );
    
    -- TODO: Warum CLOCK_25?
    proc_reset: process(CLOCK_25, pll_locked)
    begin
        if(pll_locked = '0')then
            scarts_resest_done <= '0';
        elsif CLOCK_25'event and CLOCK_25 = '1' then
            scarts_resest_done <= '1';
        end if;
    end process;
    end generate;
    
    -- TODO: Modus entfernen
    eCLOCK_DIRECT : if(use_pll = '0')generate
    pll_reset <= '0';
    pll_locked <= '0';
    scarts_resest_done <= nReset;
    CLOCK_25 <= CLOCK_50;
    end generate;
    
    scarts_reset <= scarts_resest_done;

    -- SCARTS_32
    scarts_in.hold <= '0'; -- High active. Usable for external RAM?
    scarts_in.interruptin <= (others => '0');
    scarts_in.data <= gpio_exto.data or spi_exto.data or altremote_exto.data or
                      time_exto.data or servo_exto.data;
    
    exti.data <= scarts_out.data;
    exti.addr <= scarts_out.addr;
    exti.byte_en <= scarts_out.byte_en;
    exti.write_en <= scarts_out.write_en;
    exti.reset <= scarts_out.reset;
    
    proc_ext_mod_sel: process(scarts_out)
    begin
    
        -- standard outputs !!!! IMPORTANT! DON'T FORGET!!!!
        gpio_sel <= '0';
        spi_sel <= '0';
        altremote_sel <= '0';
        time_sel <= '0';
        servo_sel <= '0';

        if(scarts_out.extsel = '1')then
            case scarts_out.addr(14 downto 5) is
                -- GPIO
                when "1111111001" =>    -- (-224) / 0xFFFFFF20
                    gpio_sel <= '1';

                -- Main SPI (Ethernet, Flash, Config)
                when "1111111000" =>    -- (-256) / 0xFFFFFF00
                    spi_sel <= '1';
                    
                -- GDB SPI
                when "1111110111" =>    -- (-288) / 0xFFFFFEE0
                    null; -- Reserved for spi_gdb
                    
                -- ALTREMOTE Update
                when "1111110110" =>    -- (-320) / 0xFFFFFEC0
                    altremote_sel <= '1';

                -- TIME
                when "1111110101" =>    -- (-352) / 0xFFFFFEA0
                    time_sel <= '1';

                -- SERVO
                when "1111110100" =>    -- (-384) / 0xFFFFFE80
                    servo_sel <= '1';

                when others =>
                    null;
            end case;
        end if;
    end process;
    
    debug_in.D_RxD <= D_RxD;
    D_TxD <= debug_out.D_TxD;
    
    eSCARTS32: entity work.scarts(behaviour)
        generic map
        (
            CONF        => c_scarts_conf
        )
        port map
        (
            clk         => CLOCK_50,
            extrst      => scarts_reset,
            sysrst      => open,
            -- Extension Module Interface
            scarts_i    => scarts_in,
            scarts_o    => scarts_out,
            strobe250us => strobe250us,
            -- Debug Interface
            debugi_if   => debug_in,
            debugo_if   => debug_out,
            -- SDRAM
            DRAM_ADDR   => DRAM_ADDR,
            DRAM_BA     => DRAM_BA,
            DRAM_CKE    => DRAM_CKE,
            DRAM_CLK    => DRAM_CLK,
            DRAM_CS_N   => DRAM_CS_N,
            DRAM_DQ     => DRAM_DQ,
            DRAM_DQM    => DRAM_DQM,
            DRAM_CAS_N  => DRAM_CAS_N,
            DRAM_RAS_N  => DRAM_RAS_N,
            DRAM_WE_N   => DRAM_WE_N
        );
    
    -- Extensions
    eGPIO: entity work.em_gpio(Rtl)
        port map
        (
            clk         => CLOCK_50,
            extsel      => gpio_sel,
            exti        => exti,
            exto        => gpio_exto,
            i_gpi       => gpi,
            o_gpo       => gpo
        );
    gpi <= "000000" & KEY(1) & KEY(0) & "0000" & SW;  
    LED(7 downto 1) <= gpo(15 downto 9);
    SPI_CS <= gpo(3 downto 0);

    eSPI: entity work.em_spi(Rtl)
        port map
        (
            clk         => CLOCK_50,
            extsel      => spi_sel,
            exti        => exti,
            exto        => spi_exto,
            i_miso      => em_spi_miso,
            o_spi_clk   => em_spi_clk,
            o_mosi      => em_spi_mosi
        );
    SPI_CLK <= em_spi_clk;
    SPI_EPCS_CLK <= em_spi_clk;
    SPI_MOSI <= em_spi_mosi;
    SPI_EPCS_MOSI <= em_spi_mosi;
    em_spi_miso <= SPI_MISO when gpo(2) = '1' else -- EPCS16 not selected
                   SPI_EPCS_MISO;
    
    eALTREMOTE: entity work.em_altremote(Rtl)
        port map
        (
            clk         => CLOCK_50,
            extsel      => altremote_sel,
            exti        => exti,
            exto        => altremote_exto
        );

    eTIME: entity work.em_time(Rtl)
        port map
        (
            clk         => CLOCK_50,
            strobe250us => strobe250us,
            extsel      => time_sel,
            exti        => exti,
            exto        => time_exto
        );

    eSERVO: entity work.em_servo(Rtl)
        port map
        (
            clk         => CLOCK_50,
            extsel      => servo_sel,
            exti        => exti,
            exto        => servo_exto,
            o_pwm       => PWM
        );

    -- IO
    eLED: entity work.FreqDivider(Rtl)
        generic map
        (
            gDivideBy    => 25E7
        )
        port map
        (
            iClk         => CLOCK_25,
            inResetAsync => scarts_reset,
            oDivdClk     => led_inv
        );
    LED(0) <= not(led_inv);

end architecture Rtl;
