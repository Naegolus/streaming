


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tbTbdDCD_APP1_DESIGN is
end tbTbdDCD_APP1_DESIGN;

architecture Bhv of tbTbdDCD_APP1_DESIGN is

  ----------------------------------------------------------------------------------
  -- Constants
  ----------------------------------------------------------------------------------
  -- USER
  constant c_sysclk         : natural := 50E6;
  constant c_uart_baud_rate : natural := 115200;
  constant use_issi         : std_ulogic := '0';
  constant use_pll          : std_ulogic := '1';
  -- Derived
  constant c_bit_with_t     : time := 1E9 ns / c_uart_baud_rate;
  constant c_byte_pad_t     : time := 1 * c_bit_with_t;
  constant c_pkt_pad_t      : time := 8 * c_byte_pad_t;

  ----------------------------------------------------------------------------------
  -- Signals
  ----------------------------------------------------------------------------------
  signal Clk                : std_ulogic := '1';
  signal KEY                : std_ulogic_vector(1 downto 0);
  signal SW                 : std_ulogic_vector(3 downto 0);
  signal GDB_TX             : std_ulogic := '1';
  signal GDB_RX             : std_ulogic;
  signal LED                : std_ulogic_vector(7 downto 0);
  signal SPI_CS             : std_ulogic_vector(3 downto 0);
  signal SPI_MISO           : std_ulogic := '0';
  signal SPI_MOSI           : std_ulogic;
  signal SPI_CLK            : std_ulogic;
  signal SPI_EPCS_MISO      : std_ulogic;
  signal SPI_EPCS_MOSI      : std_ulogic;
  signal SPI_EPCS_CLK       : std_ulogic;
  signal ENC_CLK            : std_ulogic;
  signal DRAM_ADDR          : std_logic_vector(12 downto 0);
  signal DRAM_BA            : std_logic_vector(1 downto 0);
  signal DRAM_CKE           : std_logic;
  signal DRAM_CLK           : std_logic;
  signal DRAM_CS_N          : std_logic;
  signal DRAM_DQ            : std_logic_vector(15 downto 0);
  signal DRAM_DQM           : std_logic_vector(1 downto 0);
  signal DRAM_CAS_N         : std_logic;
  signal DRAM_RAS_N         : std_logic;
  signal DRAM_WE_N          : std_logic;
  signal dram_ctrl_str      : string(1 to 5);
  signal uart_bit_toggle    : std_ulogic := '0';
  signal ref_cnt            : natural := 0;
  signal ref_cnt_old        : natural := 0;
  signal max_ref_diff       : natural := 0;
  signal spi_flash_addr     : unsigned(23 downto 0) := (others => '0');
  signal PWM                : std_ulogic_vector(3 downto 0);

  ----------------------------------------------------------------------------------
  -- Visualization (temporary)
  ----------------------------------------------------------------------------------
 
begin

    Clk <= not Clk after 1E9 ns / (2*c_sysclk);
    KEY <= (others => '1');
    SW <= (others => '0');
  
    -- DUT
    DUT: entity work.TbdDCD_APP1_DESIGN(rtl) -- structure, rtl
        generic map
        (
            use_pll         => use_pll
        )
        port map
        (
            CLOCK_50        => Clk,
            KEY             => KEY,
            SW              => SW,
            D_RxD           => GDB_TX,
            D_TxD           => GDB_RX,
            LED             => LED,
            SPI_CS          => SPI_CS,
            SPI_MISO        => SPI_MISO,
            SPI_MOSI        => SPI_MOSI,
            SPI_CLK         => SPI_CLK,
            SPI_EPCS_MISO   => SPI_EPCS_MISO,
            SPI_EPCS_MOSI   => SPI_EPCS_MOSI,
            SPI_EPCS_CLK    => SPI_EPCS_CLK,
            ENC_CLK         => ENC_CLK,
            DRAM_ADDR       => DRAM_ADDR,
            DRAM_BA         => DRAM_BA,
            DRAM_CKE        => DRAM_CKE,
            DRAM_CLK        => DRAM_CLK,
            DRAM_CS_N       => DRAM_CS_N,
            DRAM_DQ         => DRAM_DQ,
            DRAM_DQM        => DRAM_DQM,
            DRAM_CAS_N      => DRAM_CAS_N,
            DRAM_RAS_N      => DRAM_RAS_N,
            DRAM_WE_N       => DRAM_WE_N,
            PWM             => PWM
        );

    altera_sdram : if (use_issi = '0') generate
    eSDRAM : entity work.sdram_0_test_component(europa)
        port map
        (
            -- inputs:
            clk             => DRAM_CLK,
            zs_addr         => DRAM_ADDR,
            zs_ba           => DRAM_BA,
            zs_cas_n        => DRAM_CAS_N,
            zs_cke          => DRAM_CKE,
            zs_cs_n         => DRAM_CS_N,
            zs_dqm          => DRAM_DQM,
            zs_ras_n        => DRAM_RAS_N,
            zs_we_n         => DRAM_WE_N,

            -- outputs:
            zs_dq           => DRAM_DQ
        );
    end generate;

    issi_sdram : if (use_issi = '1') generate
    eSDRAM_issi : entity work.IS42S16160
        port map
        (
            Dq              => DRAM_DQ,
            Addr            => DRAM_ADDR,
            Ba              => DRAM_BA,
            Clk             => DRAM_CLK,
            Cke             => DRAM_CKE,
            Cs_n            => DRAM_CS_N,
            Ras_n           => DRAM_RAS_N,
            Cas_n           => DRAM_CAS_N,
            We_n            => DRAM_WE_N,
            Dqm             => DRAM_DQM
        );
    end generate;
    
    sdram_ctrl_debug: process(DRAM_CS_N, DRAM_RAS_N, DRAM_CAS_N, DRAM_WE_N)
        variable ctrl_vect : std_logic_vector(2 downto 0);
    begin
        ctrl_vect := DRAM_RAS_N & DRAM_CAS_N & DRAM_WE_N;
        
        if(DRAM_CS_N = '1')then
            dram_ctrl_str <= "DESL ";
        else
            case ctrl_vect is
                when "111" =>
                    dram_ctrl_str <= "NOP  ";
                when "101" =>
                    dram_ctrl_str <= "READ ";
                when "100" =>
                    dram_ctrl_str <= "WRITE";
                when "011" =>
                    dram_ctrl_str <= "ACT  ";
                when "010" =>
                    dram_ctrl_str <= "PALL ";
                when "001" =>
                    dram_ctrl_str <= "REF  ";
                when "000" =>
                    dram_ctrl_str <= "MRS  ";
                when others =>
                    dram_ctrl_str <= "xxx  ";
            end case;
        end if;
    end process;

    proc_reg: process(Clk, ref_cnt, ref_cnt_old)
        variable ref_diff : natural;
    begin
        if Clk'event and Clk = '1' then
            ref_cnt <= ref_cnt + 1;
            
            if(DRAM_CS_N  = '0' and
               DRAM_RAS_N = '0' and
               DRAM_CAS_N = '0' and
               DRAM_WE_N  = '1')then
                
                if(ref_cnt_old /= 0)then
                    ref_diff := ref_cnt - ref_cnt_old;
                    if(ref_diff > max_ref_diff)then
                        max_ref_diff <= ref_diff;
                    end if;
                end if;
                
                ref_cnt_old <= ref_cnt;
            end if;
        end if;
    end process;

----------------------------------------------------------------------------------------------------------------------------
-- Testing process
Stimu : process

        -- UART
        procedure uart_send_byte(dat : in std_ulogic_vector(7 downto 0)) is
        begin
            GDB_TX <= '0'; -- Start bit
            wait for c_bit_with_t;
            for i in 0 to 7 loop
                uart_bit_toggle <= not(uart_bit_toggle);
                
                GDB_TX <= dat(i); -- Data
                wait for c_bit_with_t;
            end loop;
            GDB_TX <= '1'; -- Stop bit
            wait for c_bit_with_t;
            GDB_TX <= '1'; -- Idle
            wait for c_byte_pad_t;
        end procedure;
            
        procedure uart_rcv_byte(data : out std_ulogic_vector(7 downto 0)) is
            variable d : std_ulogic_vector(7 downto 0) := X"00";
        begin
            wait until GDB_RX = '0';
            wait for c_bit_with_t / 2;
            -- Start Bit
            for i in 0 to 7 loop
                wait for c_bit_with_t;
                d(i) := GDB_RX;
            end loop;
            -- Stop Bit
            data := d;
        end procedure;
        
        procedure uart_rcv_last_3_bytes is
            variable d : std_ulogic_vector(7 downto 0) := X"00";
        begin
            uart_rcv_byte(d);
            uart_rcv_byte(d);
            uart_rcv_byte(d);
        end procedure;
        
        -- GDB
        procedure gdb_read_regs is
            type REG_DATA_TYPE is array (7 downto 0) of std_ulogic_vector(7 downto 0);
            type REG_FILE_TYPE is array (0 to 16) of REG_DATA_TYPE;
            variable data : REG_FILE_TYPE;
        begin
            uart_send_byte(X"24"); -- $
            uart_send_byte(X"67"); -- g
            uart_send_byte(X"23"); -- #
            uart_send_byte(X"36"); -- 6
            uart_send_byte(X"37"); -- 7
            
            uart_rcv_byte(data(0)(0)); -- Receive ack +
            
            report "Response: " & character'val(to_integer(unsigned(data(0)(0))));
            
            if(data(0)(0) = X"2B")then -- +
            
                uart_rcv_byte(data(0)(0)); -- Receive framestart $
                
                if(data(0)(0) = X"24")then -- $
                    
                    -- Receive register file
                    for r in 0 to 16 loop
                        for b in 7 downto 0 loop
                            uart_rcv_byte(data(r)(b)); -- Receive byte b of register r
                        end loop;
                    end loop;
                    
                    report  "Response data" & LF &
                            "Register file:" & LF &
                            "r0 : "
                               & character'val(to_integer(unsigned(data(0)(1))))
                               & character'val(to_integer(unsigned(data(0)(0))))
                               & character'val(to_integer(unsigned(data(0)(3))))
                               & character'val(to_integer(unsigned(data(0)(2))))
                               & character'val(to_integer(unsigned(data(0)(5))))
                               & character'val(to_integer(unsigned(data(0)(4))))
                               & character'val(to_integer(unsigned(data(0)(7))))
                               & character'val(to_integer(unsigned(data(0)(6))))
                               & LF &
                            "r1 : "
                               & character'val(to_integer(unsigned(data(1)(1))))
                               & character'val(to_integer(unsigned(data(1)(0))))
                               & character'val(to_integer(unsigned(data(1)(3))))
                               & character'val(to_integer(unsigned(data(1)(2))))
                               & character'val(to_integer(unsigned(data(1)(5))))
                               & character'val(to_integer(unsigned(data(1)(4))))
                               & character'val(to_integer(unsigned(data(1)(7))))
                               & character'val(to_integer(unsigned(data(1)(6))))
                               & LF &
                            "r2 : "
                               & character'val(to_integer(unsigned(data(2)(1))))
                               & character'val(to_integer(unsigned(data(2)(0))))
                               & character'val(to_integer(unsigned(data(2)(3))))
                               & character'val(to_integer(unsigned(data(2)(2))))
                               & character'val(to_integer(unsigned(data(2)(5))))
                               & character'val(to_integer(unsigned(data(2)(4))))
                               & character'val(to_integer(unsigned(data(2)(7))))
                               & character'val(to_integer(unsigned(data(2)(6))))
                               & LF &
                            "r3 : "
                               & character'val(to_integer(unsigned(data(3)(1))))
                               & character'val(to_integer(unsigned(data(3)(0))))
                               & character'val(to_integer(unsigned(data(3)(3))))
                               & character'val(to_integer(unsigned(data(3)(2))))
                               & character'val(to_integer(unsigned(data(3)(5))))
                               & character'val(to_integer(unsigned(data(3)(4))))
                               & character'val(to_integer(unsigned(data(3)(7))))
                               & character'val(to_integer(unsigned(data(3)(6))))
                               & LF &
                            "r4 : "
                               & character'val(to_integer(unsigned(data(4)(1))))
                               & character'val(to_integer(unsigned(data(4)(0))))
                               & character'val(to_integer(unsigned(data(4)(3))))
                               & character'val(to_integer(unsigned(data(4)(2))))
                               & character'val(to_integer(unsigned(data(4)(5))))
                               & character'val(to_integer(unsigned(data(4)(4))))
                               & character'val(to_integer(unsigned(data(4)(7))))
                               & character'val(to_integer(unsigned(data(4)(6))))
                               & LF &
                            "r5 : "
                               & character'val(to_integer(unsigned(data(5)(1))))
                               & character'val(to_integer(unsigned(data(5)(0))))
                               & character'val(to_integer(unsigned(data(5)(3))))
                               & character'val(to_integer(unsigned(data(5)(2))))
                               & character'val(to_integer(unsigned(data(5)(5))))
                               & character'val(to_integer(unsigned(data(5)(4))))
                               & character'val(to_integer(unsigned(data(5)(7))))
                               & character'val(to_integer(unsigned(data(5)(6))))
                               & LF &
                            "r6 : "
                               & character'val(to_integer(unsigned(data(6)(1))))
                               & character'val(to_integer(unsigned(data(6)(0))))
                               & character'val(to_integer(unsigned(data(6)(3))))
                               & character'val(to_integer(unsigned(data(6)(2))))
                               & character'val(to_integer(unsigned(data(6)(5))))
                               & character'val(to_integer(unsigned(data(6)(4))))
                               & character'val(to_integer(unsigned(data(6)(7))))
                               & character'val(to_integer(unsigned(data(6)(6))))
                               & LF &
                            "r7 : "
                               & character'val(to_integer(unsigned(data(7)(1))))
                               & character'val(to_integer(unsigned(data(7)(0))))
                               & character'val(to_integer(unsigned(data(7)(3))))
                               & character'val(to_integer(unsigned(data(7)(2))))
                               & character'val(to_integer(unsigned(data(7)(5))))
                               & character'val(to_integer(unsigned(data(7)(4))))
                               & character'val(to_integer(unsigned(data(7)(7))))
                               & character'val(to_integer(unsigned(data(7)(6))))
                               & LF &
                            "r8 : "
                               & character'val(to_integer(unsigned(data(8)(1))))
                               & character'val(to_integer(unsigned(data(8)(0))))
                               & character'val(to_integer(unsigned(data(8)(3))))
                               & character'val(to_integer(unsigned(data(8)(2))))
                               & character'val(to_integer(unsigned(data(8)(5))))
                               & character'val(to_integer(unsigned(data(8)(4))))
                               & character'val(to_integer(unsigned(data(8)(7))))
                               & character'val(to_integer(unsigned(data(8)(6))))
                               & LF &
                            "r9 : "
                               & character'val(to_integer(unsigned(data(9)(1))))
                               & character'val(to_integer(unsigned(data(9)(0))))
                               & character'val(to_integer(unsigned(data(9)(3))))
                               & character'val(to_integer(unsigned(data(9)(2))))
                               & character'val(to_integer(unsigned(data(9)(5))))
                               & character'val(to_integer(unsigned(data(9)(4))))
                               & character'val(to_integer(unsigned(data(9)(7))))
                               & character'val(to_integer(unsigned(data(9)(6))))
                               & LF &
                            "r10: "
                               & character'val(to_integer(unsigned(data(10)(1))))
                               & character'val(to_integer(unsigned(data(10)(0))))
                               & character'val(to_integer(unsigned(data(10)(3))))
                               & character'val(to_integer(unsigned(data(10)(2))))
                               & character'val(to_integer(unsigned(data(10)(5))))
                               & character'val(to_integer(unsigned(data(10)(4))))
                               & character'val(to_integer(unsigned(data(10)(7))))
                               & character'val(to_integer(unsigned(data(10)(6))))
                               & LF &
                            "r11: "
                               & character'val(to_integer(unsigned(data(11)(1))))
                               & character'val(to_integer(unsigned(data(11)(0))))
                               & character'val(to_integer(unsigned(data(11)(3))))
                               & character'val(to_integer(unsigned(data(11)(2))))
                               & character'val(to_integer(unsigned(data(11)(5))))
                               & character'val(to_integer(unsigned(data(11)(4))))
                               & character'val(to_integer(unsigned(data(11)(7))))
                               & character'val(to_integer(unsigned(data(11)(6))))
                               & LF &
                            "r12: "
                               & character'val(to_integer(unsigned(data(12)(1))))
                               & character'val(to_integer(unsigned(data(12)(0))))
                               & character'val(to_integer(unsigned(data(12)(3))))
                               & character'val(to_integer(unsigned(data(12)(2))))
                               & character'val(to_integer(unsigned(data(12)(5))))
                               & character'val(to_integer(unsigned(data(12)(4))))
                               & character'val(to_integer(unsigned(data(12)(7))))
                               & character'val(to_integer(unsigned(data(12)(6))))
                               & LF &
                            "r13: "
                               & character'val(to_integer(unsigned(data(13)(1))))
                               & character'val(to_integer(unsigned(data(13)(0))))
                               & character'val(to_integer(unsigned(data(13)(3))))
                               & character'val(to_integer(unsigned(data(13)(2))))
                               & character'val(to_integer(unsigned(data(13)(5))))
                               & character'val(to_integer(unsigned(data(13)(4))))
                               & character'val(to_integer(unsigned(data(13)(7))))
                               & character'val(to_integer(unsigned(data(13)(6))))
                               & LF &
                            "r14: "
                               & character'val(to_integer(unsigned(data(14)(1))))
                               & character'val(to_integer(unsigned(data(14)(0))))
                               & character'val(to_integer(unsigned(data(14)(3))))
                               & character'val(to_integer(unsigned(data(14)(2))))
                               & character'val(to_integer(unsigned(data(14)(5))))
                               & character'val(to_integer(unsigned(data(14)(4))))
                               & character'val(to_integer(unsigned(data(14)(7))))
                               & character'val(to_integer(unsigned(data(14)(6))))
                               & LF &
                            "r15: "
                               & character'val(to_integer(unsigned(data(15)(1))))
                               & character'val(to_integer(unsigned(data(15)(0))))
                               & character'val(to_integer(unsigned(data(15)(3))))
                               & character'val(to_integer(unsigned(data(15)(2))))
                               & character'val(to_integer(unsigned(data(15)(5))))
                               & character'val(to_integer(unsigned(data(15)(4))))
                               & character'val(to_integer(unsigned(data(15)(7))))
                               & character'val(to_integer(unsigned(data(15)(6))))
                               & LF &
                            "pc:  "
                               & character'val(to_integer(unsigned(data(16)(1))))
                               & character'val(to_integer(unsigned(data(16)(0))))
                               & character'val(to_integer(unsigned(data(16)(3))))
                               & character'val(to_integer(unsigned(data(16)(2))))
                               & character'val(to_integer(unsigned(data(16)(5))))
                               & character'val(to_integer(unsigned(data(16)(4))))
                               & character'val(to_integer(unsigned(data(16)(7))))
                               & character'val(to_integer(unsigned(data(16)(6)))) severity note;
                               
                    uart_rcv_last_3_bytes;
                else
                    report "Error receiving framestart '$' at command gdb_read_regs" severity failure;
                end if;
            else
                report "Error receiving acknowledge '+' at command gdb_read_regs" severity failure;
            end if;
            
            wait for c_pkt_pad_t; -- For debugging
        end procedure;

        procedure gdb_step is
            variable data0 : std_ulogic_vector(7 downto 0);
            variable data1 : std_ulogic_vector(7 downto 0);
        begin
            uart_send_byte(X"24"); -- $
            uart_send_byte(X"73"); -- s
            uart_send_byte(X"23"); -- #
            uart_send_byte(X"37"); -- 7
            uart_send_byte(X"33"); -- 7
            
            uart_rcv_byte(data0); -- Receive ack +
            
            report "Response: " & character'val(to_integer(unsigned(data0)));
            
            if(data0 = X"2B")then -- +
                
                uart_rcv_byte(data0); -- Receive framestart $
                
                if(data0 = X"24")then -- $
                
                    uart_rcv_byte(data0); -- Receive S
                    
                    if(data0 = X"53")then -- S
                    
                        uart_rcv_byte(data0);
                        uart_rcv_byte(data1);
                        
                        report "Data: S" & character'val(to_integer(unsigned(data0))) & character'val(to_integer(unsigned(data1)));
                        
                        uart_rcv_last_3_bytes;
                    else
                        report "Error receiving 'S' at command at command gdb_step" severity failure;
                    end if;
                
                else
                    report "Error receiving framestart '$' at command gdb_step" severity failure;
                end if;
            else
                report "Error receiving acknowledge '+' at command gdb_step" severity failure;
            end if;
        end procedure;
        
        -- SPI
        procedure spi_rcv_byte(data : out std_ulogic_vector(7 downto 0)) is
            variable d : std_ulogic_vector(7 downto 0);
        begin
            for i in 7 downto 0 loop
                wait until SPI_CLK = '1';
                d(i) := SPI_MOSI;
                wait until SPI_CLK = '0';
            end loop;
            data := d;
        end procedure;
        
        procedure spi_send_byte(dat : in std_ulogic_vector(7 downto 0)) is
        begin
            for i in 7 downto 0 loop
                SPI_MISO <= dat(i);
                wait until SPI_CLK = '1';
                wait until SPI_CLK = '0';
            end loop;
            SPI_MISO <= '0';
        end procedure;
        
        -- BSL_MST
        procedure bsl_get_version is
            type BYTE_ARRAY_TYPE is array(natural range <>) of std_ulogic_vector(7 downto 0);
            constant BYTE_CNT : natural := 61;
            
            variable data : BYTE_ARRAY_TYPE(0 to BYTE_CNT - 1) :=
            (
                X"00", X"01",   -- Dummy + ENC_REG_ADR_EPKTCNT
                X"01", X"00",   -- eth.c: 785, Select bank nr. 0
                X"02", X"00",   -- eth.c: 786
                X"03", X"00",   -- ENC_REG_ADR_ERDPTL
                X"04", X"00",   -- ENC_REG_ADR_ERDPTH
                X"05",          -- rd RCV_STATUS_TYPE
                X"00", X"00", X"00", X"00", X"80", X"00",    -- p_rcv
                X"06",          -- rd ETH_HEADER_TYPE
                X"FF", X"FF", X"FF", X"FF", X"FF", X"FF",    -- dst_mac_addr
                X"00", X"1A", X"6B", X"35", X"54", X"00",    -- src_mac_addr
                X"00", X"1D",   -- type_len
                X"07",          -- rd BSL_CMD_TYPE
                X"42", X"53", X"4C", X"5F", X"50", X"72", X"6F", X"74", X"6F", X"63", X"6F", X"6C", -- bsl_key
                X"00", X"00",   -- mem_id
                X"00", X"00",   -- mem_idx
                X"01", X"00",   -- data_len
                X"00", X"00",   -- dummy0
                X"00", X"00",   -- data_addr
                X"00", X"00",   -- dummy1
                X"10", X"01",   -- cmd_id
                X"00",          -- cmd
                X"5B"           -- chk_sum
            );
        begin
            for i in 0 to BYTE_CNT - 1 loop
                spi_send_byte(data(i));
            end loop;
        end procedure;

        procedure bsl_wr_flash is
            type BYTE_ARRAY_TYPE is array(natural range <>) of std_ulogic_vector(7 downto 0);
            constant BYTE_CNT : natural := 118;
            
            variable data : BYTE_ARRAY_TYPE(0 to BYTE_CNT - 1) :=
            (
                X"FF", X"01",   -- Dummy + ENC_REG_ADR_EPKTCNT
                X"FF", X"00",   -- eth.c: 785, Select bank nr. 0
                X"FF", X"00",   -- eth.c: 786
                X"FF", X"00",   -- ENC_REG_ADR_ERDPTL
                X"FF", X"00",   -- ENC_REG_ADR_ERDPTH
                X"FF",          -- rd RCV_STATUS_TYPE
                X"00", X"00", X"00", X"00", X"80", X"00",    -- p_rcv
                X"FF",          -- rd ETH_HEADER_TYPE
                X"FF", X"FF", X"FF", X"FF", X"FF", X"FF",    -- dst_mac_addr
                X"00", X"1A", X"6B", X"35", X"54", X"00",    -- src_mac_addr
                X"00", X"1D",   -- type_len
                X"FF",          -- rd BSL_CMD_TYPE
                X"42", X"53", X"4C", X"5F", X"50", X"72", X"6F", X"74", X"6F", X"63", X"6F", X"6C", -- bsl_key
                X"00", X"00",   -- mem_id
                X"00", X"00",   -- mem_idx
                X"01", X"00",   -- data_len
                X"00", X"00",   -- dummy0
                X"00", X"00",   -- data_addr
                X"00", X"00",   -- dummy1
                X"10", X"01",   -- cmd_id
                X"03",          -- cmd
                X"58",          -- chk_sum
                X"FF", X"00",
                X"FF", X"00",
                X"FF",          -- MACPutHeader
                X"00", X"00", X"00", X"00", X"00", X"00",
                X"FF",
                X"00", X"00", X"00", X"00", X"00", X"00",
                X"FF", X"00",
                X"FF", X"00",
                X"FF", X"00",
                X"FF", X"00",
                X"FF",          -- wr BSL_CMD_TYPE
                X"42", X"53", X"4C", X"5F", X"50", X"72", X"6F", X"74", X"6F", X"63", X"6F", X"6C", -- bsl_key
                X"00", X"00",   -- mem_id
                X"00", X"00",   -- mem_idx
                X"01", X"00",   -- data_len
                X"00", X"00",   -- dummy0
                X"00", X"00",   -- data_addr
                X"00", X"00",   -- dummy1
                X"10", X"01",   -- cmd_id
                X"03",          -- cmd
                X"58",          -- chk_sum
                X"FF",          -- rd DATA
                X"77"
            );
        begin
            for i in 0 to BYTE_CNT - 1 loop
                spi_send_byte(data(i));
            end loop;
        end procedure;
        
        -- FLASH
        procedure flash_send_data is
            type char_file is file of character;
            file p_file : char_file;
            
            variable cdata : character;
            variable vdata : std_ulogic_vector(7 downto 0);
            variable addr : std_ulogic_vector(23 downto 0);
            variable uaddr : unsigned(23 downto 0);
        begin
        
            wait until SPI_CS(1) = '0'; -- Wait until flash communication
        
            spi_rcv_byte(addr(23 downto 16));
            spi_rcv_byte(addr(23 downto 16));
            spi_rcv_byte(addr(15 downto  8));
            spi_rcv_byte(addr( 7 downto  0));
            
            uaddr := unsigned(addr);
            
            if(unsigned(addr) < X"20000")then
                -- ro_data - Section
                file_open(p_file, "../../DCD_APP1/SYS_DE0N/objs/dcd_app1_data.bin", read_mode);
                for i in 0 to 86 - 1 loop -- __bss_start [dez]
                    read(p_file, cdata);
                    vdata := std_ulogic_vector(to_unsigned(character'pos(cdata), 8));
                    spi_send_byte(vdata);
                    uaddr := uaddr + 1;
                    spi_flash_addr <= uaddr;
                end loop;
                file_close(p_file);
            else
                if(use_issi = '1')then
                    -- text - Section
                    -- 
                    -- bsl
                    -- 
                    file_open(p_file, "../../DCD_BSL_SLV_IP/SYS_DE0N/objs/udp_ip_bsl.bin", read_mode);
                    for i in 0 to 65536 - 1 loop -- 65536 (0x10000)
                        read(p_file, cdata);
                        vdata := std_ulogic_vector(to_unsigned(character'pos(cdata), 8));
                        spi_send_byte(vdata);
                        uaddr := uaddr + 1;
                        spi_flash_addr <= uaddr;
                    end loop;
                    file_close(p_file);
                    -- app1
                    -- 
                    file_open(p_file, "../../DCD_APP1/SYS_DE0N/objs/dcd_app1.bin", read_mode);
                    for i in 0 to 524288 - 1 loop -- 524288 (0x80000)
                        read(p_file, cdata);
                        vdata := std_ulogic_vector(to_unsigned(character'pos(cdata), 8));
                        spi_send_byte(vdata);
                        uaddr := uaddr + 1;
                        spi_flash_addr <= uaddr;
                    end loop;
                    file_close(p_file);
                else
                    file_open(p_file, "../../DCD_APP1/SYS_DE0N/objs/dcd_app1.bin", read_mode);
                    for i in 0 to 4 - 1 loop -- 524288 (0x80000)
                        read(p_file, cdata);
                        vdata := std_ulogic_vector(to_unsigned(character'pos(cdata), 8));
                        spi_send_byte(vdata);
                        uaddr := uaddr + 1;
                        spi_flash_addr <= uaddr;
                    end loop;
                    file_close(p_file);
                end if;
            end if;
            
            wait until SPI_CS(1) = '1'; -- Wait until flash communication ended
            
        end procedure;

  begin

    wait until SPI_CS(0) = '1'; -- Wait until program is initialized

-- ########################################################################################################
-----------------------------------------------------------------------------------------------------------
-- Testing Code

    flash_send_data; -- Send program
    flash_send_data; -- Send ro_data
    
    spi_send_byte(X"00"); -- ENC28 reset cmd
    spi_send_byte(X"00"); -- ENC28 read cmd
    spi_send_byte(X"01"); -- ENC28 phy clock ready
    wait for 2 ms; -- Wait until ethernet is initialized
    
    gdb_read_regs;
    --gdb_step;
    --gdb_step;
    
    bsl_get_version;
    --bsl_wr_flash;
    -- send_program;

    -- TODO: SCARTS32-IRQ Test

-----------------------------------------------------------------------------------------------------------
-- ########################################################################################################

    assert false report "SIMULATION ENDED SUCCESSFULLY" severity note;
    assert false report "max_ref_diff: " & integer'image(max_ref_diff * 20) & " ns" severity note;
    wait;
    
end process;

----------------------------------------------------------------------------------------------------------------------------
end Bhv;
