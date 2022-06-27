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

entity scarts_iram is
  generic (
    CONF : scarts_conf_type);    
  port
  (
        nReset          : in    std_ulogic;
        clk             : in    std_ulogic;
        strobe250us     : in    std_ulogic;

        wdata           : in    INSTR;
        waddr           : in    std_logic_vector(CONF.instr_ram_size-1 downto 0);
        wen             : in    std_ulogic;
        raddr           : in    std_logic_vector(CONF.instr_ram_size-1 downto 0);
        rdata           : out   INSTR;
        hold            : out   std_ulogic;

        DRAM_CLK        : out   std_logic;
        DRAM_CKE        : out   std_logic;
        DRAM_DQM        : out   std_logic_vector(1 downto 0);
        
        DRAM_CS_N       : out   std_logic;
        DRAM_RAS_N      : out   std_logic;
        DRAM_CAS_N      : out   std_logic;
        DRAM_WE_N       : out   std_logic;
        
        DRAM_BA         : out   std_logic_vector(1 downto 0);
        DRAM_ADDR       : out   std_logic_vector(12 downto 0);
        DRAM_DQ         : inout std_logic_vector(15 downto 0)
    );
end scarts_iram;

architecture behaviour of scarts_iram is

    type STATEMACHINE_STEP_TYPE is
    (
        SM_WAIT_SDRAM_INIT_TIME,
        SM_INIT_PRECH,
        SM_INIT_REF,
        SM_INIT_MRS,
        SM_INIT_DESELECT,
        SM_PRECHARGE_ROW,
        SM_ACTIVATE_ROW,
        SM_WAIT_CAS,
        SM_WRITE,
        SM_MAIN,
        SM_REFRESH,
        SM_NOP
    );
    
    type SDRAM_CMD_TYPE is record
        cs_n                : std_logic;
        ras_n               : std_logic;
        cas_n               : std_logic;
        we_n                : std_logic;
    end record;
    
    --                                               cs_n   ras_n   cas_n   we_n
    constant SDRAM_DESELECTED   : SDRAM_CMD_TYPE := ('1',   '1',    '1',    '1');
    constant SDRAM_CMD_NOP      : SDRAM_CMD_TYPE := ('0',   '1',    '1',    '1');
    constant SDRAM_CMD_REF      : SDRAM_CMD_TYPE := ('0',   '0',    '0',    '1');
    constant SDRAM_CMD_ACT      : SDRAM_CMD_TYPE := ('0',   '0',    '1',    '1');
    constant SDRAM_CMD_READ     : SDRAM_CMD_TYPE := ('0',   '1',    '0',    '1');
    constant SDRAM_CMD_WRITE    : SDRAM_CMD_TYPE := ('0',   '1',    '0',    '0');
    constant SDRAM_CMD_PRE      : SDRAM_CMD_TYPE := ('0',   '0',    '1',    '0');
    constant SDRAM_CMD_MRS      : SDRAM_CMD_TYPE := ('0',   '0',    '0',    '0');
    
    -- SDRAM:           IS42S16160D-7TLI
    -- Speed grade:     -7
    -- CAS Latency:     2
    -- Operation speed: 25 MHz (40 ns period)
    
    constant SDRAM_NUM_INIT_REF : natural := 8 - 1; -- (-1 implementation) - 
    constant SDRAM_NUM_REF_NOPS : natural := 3 - 1; -- t_RC  = 67.5 ns => 3 nop, Command Period (REF to REF / ACT to ACT) 
    constant SDRAM_NUM_PRE_NOPS : natural := 2 - 1; -- t_RP  = 21   ns => 2 nop, Command Period (PRE to ACT) -> 21 ns according issi sim. model
    constant SDRAM_NUM_MRS_NOPS : natural := 1 - 1; -- t_MRD = 15   ns => 1 nop, Mode Register Program Time
    constant SDRAM_NUM_ACT_NOPS : natural := 3 - 1; -- t_RCD = 20   ns => 1 nop, Active Command To Read / Write Command Delay Time
                                                -- but t_RAS = 45   ns => 3 nop, Command Period (ACT to PRE)
    constant SDRAM_NUM_WRITE2PRE_NOPS : natural := 1 - 1; -- t_DPL = 14 ns => 1 nop, Input Data To Precharge Command Delay time
    
    -- Mode Register
    constant SDRAM_MODE_REGISTER : std_logic_vector(9 downto 0) := "0000100000";

    type REG_TYPE is record
        sm_step             : STATEMACHINE_STEP_TYPE;
        sm_step_ret         : STATEMACHINE_STEP_TYPE;
        sm_next_step        : STATEMACHINE_STEP_TYPE;
        cmd_idx             : natural;
        nop_idx             : natural;
        curr_ba             : std_logic_vector(1 downto 0);
        curr_row            : std_logic_vector(12 downto 0);
        n_refresh_strobe_hold : std_ulogic;
        refresh_req         : std_ulogic;
        give_virtual_data   : std_ulogic;
        
        raddr_latch         : std_logic_vector(CONF.instr_ram_size-1 downto 0);
        rdata_latch         : INSTR;
        waddr_latch         : std_logic_vector(CONF.instr_ram_size-1 downto 0);
        wdata_latch         : INSTR;
        wen_latch           : std_ulogic;
        
        -- Ext. comm.
        hold                : std_ulogic;
        sdram_cmd           : SDRAM_CMD_TYPE;
        ba                  : std_logic_vector(1 downto 0);
        addr                : std_logic_vector(12 downto 0);
        dq                  : std_logic_vector(15 downto 0);
    end record;
    
    constant RSET_INIT_VAL : REG_TYPE :=
    (
        sm_step             => SM_WAIT_SDRAM_INIT_TIME,
        sm_step_ret         => SM_WAIT_SDRAM_INIT_TIME,
        sm_next_step        => SM_WAIT_SDRAM_INIT_TIME,
        cmd_idx             => 0,
        nop_idx             => 0,
        curr_ba             => "11",
        curr_row            => (others => '1'),
        n_refresh_strobe_hold => '0',
        refresh_req         => '0',
        give_virtual_data   => '0',
        
        raddr_latch         => (others => '1'),
        rdata_latch         => (others => '1'),
        waddr_latch         => (others => '1'),
        wdata_latch         => (others => '1'),
        wen_latch           => '0',
        
        hold                => '1',
        sdram_cmd           => SDRAM_DESELECTED,
        ba                  => "00",
        addr                => (others => '0'), 
        dq                  => (others => 'Z')
    );
    
    
    signal R, NxR : REG_TYPE;
    signal clk_ctrl, pll_locked : std_ulogic;
    signal pll_reset : std_ulogic;
    signal refresh_srobe : std_ulogic;

begin

    pll_reset <= not(nReset);
    eSDRAM_PLL : sdram_pll
    port map
    (
        areset              => pll_reset,
        inclk0              => clk,
        c0                  => DRAM_CLK,
        locked              => pll_locked
    );
    
    clk_ctrl <= clk;
    
    DRAM_CKE <= '1';
    DRAM_DQM <= "00"; -- not(output enable)
    
    DRAM_BA <= R.ba;
    DRAM_CS_N <= R.sdram_cmd.cs_n;
    DRAM_RAS_N <= R.sdram_cmd.ras_n;
    DRAM_CAS_N <= R.sdram_cmd.cas_n;
    DRAM_WE_N <= R.sdram_cmd.we_n;
    DRAM_DQ <= R.dq;
    
    hold <= R.hold or not(pll_locked);
    
    proc_data: process(R, DRAM_DQ)
    begin
        -- little-endianness used for storing instructions in memory
        -- => swap bytes
        if(R.give_virtual_data = '0')then
            rdata(15 downto 8) <= DRAM_DQ(7 downto 0);
            rdata(7 downto 0) <= DRAM_DQ(15 downto 8);
        else
            rdata(15 downto 8) <= R.rdata_latch(7 downto 0);
            rdata(7 downto 0) <= R.rdata_latch(15 downto 8);
        end if;
    end process;

    proc_comb: process(R, refresh_srobe, wen, raddr, waddr, wdata, strobe250us, DRAM_DQ)
    begin
    
        NxR <= R;
    
        -- Standard values
        NxR.hold <= '1';
        NxR.ba <= "00";
        NxR.addr <= (others => '0');
        NxR.sdram_cmd <= SDRAM_DESELECTED;
        NxR.dq <= (others => 'Z');
        NxR.give_virtual_data <= '0';
        
        DRAM_ADDR <= R.addr; -- MUST be here!!

        if(refresh_srobe = '1')then
            NxR.refresh_req <= '1';
        end if;

        case R.sm_step is
        
-- INIT
            when SM_WAIT_SDRAM_INIT_TIME =>
                if(strobe250us = '1')then
                    NxR.n_refresh_strobe_hold <= '1';
                    NxR.sm_step <= SM_INIT_PRECH;
                end if;
                
            when SM_INIT_PRECH =>
                NxR.addr <= (others => '1'); -- Precharge ALL banks (A10 = H -> PALL)
                NxR.sdram_cmd <= SDRAM_CMD_PRE;
                
                NxR.cmd_idx <= SDRAM_NUM_INIT_REF;
                
                NxR.nop_idx <= SDRAM_NUM_PRE_NOPS;
                NxR.sm_step <= SM_NOP;
                NxR.sm_step_ret <= SM_INIT_REF;
                
            when SM_INIT_REF =>
                NxR.sdram_cmd <= SDRAM_CMD_REF;
                
                NxR.nop_idx <= SDRAM_NUM_REF_NOPS;
                NxR.sm_step <= SM_NOP;
                if(R.cmd_idx = 0)then
                    NxR.sm_step_ret <= SM_INIT_MRS;
                else
                    NxR.cmd_idx <= R.cmd_idx - 1;
                    NxR.sm_step_ret <= SM_INIT_REF;
                end if;
                
            when SM_INIT_MRS =>
                NxR.addr(9 downto 0) <= SDRAM_MODE_REGISTER;
                NxR.sdram_cmd <= SDRAM_CMD_MRS;
                
                NxR.nop_idx <= SDRAM_NUM_MRS_NOPS;
                NxR.sm_step <= SM_NOP;
                NxR.sm_step_ret <= SM_INIT_DESELECT;
                
            when SM_INIT_DESELECT =>
                NxR.sm_step <= SM_MAIN;

-- CYCLIC
            when SM_PRECHARGE_ROW =>
                NxR.addr <= (others => '1'); -- Precharge ALL banks (A10 = H -> PALL)
                NxR.sdram_cmd <= SDRAM_CMD_PRE;
                
                NxR.rdata_latch <= DRAM_DQ; -- Latch virtual data
                
                NxR.nop_idx <= SDRAM_NUM_PRE_NOPS;
                NxR.sm_step <= SM_NOP; 
                NxR.sm_step_ret <= R.sm_next_step;

            when SM_ACTIVATE_ROW =>
                NxR.ba <= R.curr_ba;
                NxR.addr <= R.curr_row;
                NxR.sdram_cmd <= SDRAM_CMD_ACT;
                
                NxR.nop_idx <= SDRAM_NUM_ACT_NOPS;
                NxR.sm_step <= SM_NOP;
                
                if(R.wen_latch = '0')then
                    NxR.sm_step_ret <= SM_WAIT_CAS;
                else
                    NxR.sm_step_ret <= SM_WRITE;
                end if;
                
            when SM_WAIT_CAS =>
                NxR.sdram_cmd <= SDRAM_CMD_READ;
                
                NxR.ba <= R.curr_ba;
                NxR.addr <= "0000" & R.raddr_latch(8 downto 0);

                if(R.sdram_cmd = SDRAM_CMD_READ)then
                    NxR.hold <= '0'; -- must be here because of refresh cycle
                    
                    NxR.give_virtual_data <= '1';
                
                    NxR.sm_step <= SM_MAIN;
                end if;

            when SM_WRITE =>
                NxR.sdram_cmd <= SDRAM_CMD_WRITE;
                NxR.ba <= R.curr_ba;
                NxR.addr <= "0000" & R.waddr_latch(8 downto 0);
                NxR.dq <= R.wdata_latch;
            
                NxR.wen_latch <= '0';
                NxR.curr_ba <= R.raddr_latch(23 downto 22);
                NxR.curr_row <= R.raddr_latch(21 downto 9);
                
                NxR.nop_idx <= SDRAM_NUM_WRITE2PRE_NOPS;
                NxR.sm_step <= SM_NOP;
                NxR.sm_step_ret <= SM_PRECHARGE_ROW;
                NxR.sm_next_step <= SM_ACTIVATE_ROW;

            when SM_MAIN =>

                -- 0. LOWEST - Prior.: Standard read cylce
                NxR.hold <= '0';
                NxR.sdram_cmd <= SDRAM_CMD_READ;
                DRAM_ADDR <= "0000" & raddr(8 downto 0);
                
                -- LATCHING
                NxR.raddr_latch <= raddr;
                NxR.waddr_latch <= waddr;
                NxR.wdata_latch <= wdata;
                NxR.wen_latch <= wen;

                -- 1. LOW - Prior.: Different ROW read
                if(raddr(23 downto 22) /= R.curr_ba or
                   raddr(21 downto  9) /= R.curr_row)then
                    NxR.hold <= '1';
                    NxR.sdram_cmd <= SDRAM_DESELECTED;
                    
                    NxR.curr_ba <= raddr(23 downto 22);
                    NxR.curr_row <= raddr(21 downto  9);
                    
                    NxR.sm_step <= SM_PRECHARGE_ROW;
                    NxR.sm_next_step <= SM_ACTIVATE_ROW;
                end if;

                -- 2. MIDDLE - Prior.: Write cycle
                if(wen = '1')then
                    NxR.hold <= '1';
                    NxR.sdram_cmd <= SDRAM_DESELECTED;
                    
                    NxR.curr_ba <= waddr(23 downto 22);
                    NxR.curr_row <= waddr(21 downto  9);
                    
                    NxR.sm_step <= SM_PRECHARGE_ROW;
                    NxR.sm_next_step <= SM_ACTIVATE_ROW;
                end if;

                -- 3. HIGH - Prior.: Refresh cycle
                if(R.refresh_req = '1')then
                    NxR.hold <= '1';
                    NxR.sdram_cmd <= SDRAM_DESELECTED;
                    
                    NxR.sm_step <= SM_PRECHARGE_ROW;
                    NxR.sm_next_step <= SM_REFRESH;
                end if;

            when SM_REFRESH =>
                NxR.refresh_req <= '0';
                NxR.sdram_cmd <= SDRAM_CMD_REF;
                
                NxR.nop_idx <= SDRAM_NUM_REF_NOPS;
                NxR.sm_step <= SM_NOP;
                NxR.sm_step_ret <= SM_ACTIVATE_ROW;

            when SM_NOP =>
                NxR.sdram_cmd <= SDRAM_CMD_NOP;
                if(R.nop_idx = 0)then
                    NxR.sm_step <= R.sm_step_ret;
                else
                    NxR.nop_idx <= R.nop_idx - 1;
                end if;

            when others =>
                null;

        end case;
        
    end process;
    
    proc_reg: process(clk_ctrl, pll_locked, nReset)
    begin
        if(nReset = '0')then
            R <= RSET_INIT_VAL;
        elsif clk_ctrl'event and clk_ctrl = '1' and pll_locked = '1' then
            R <= NxR;
        end if;
    end process;

    eSTROBE_refresh : StrobeGen
        generic map
        (
            gClkFrequency       => 50E6,
            gStrobeCycleTime    => 6 us -- 8192 refresh cycles every 64 ms -> 7.8125 us - time to process refresh_strobe (x)
            -- (x) .. A write cycle directly before refresh request can delay the refresh cycle, although the refresh cycle
            --        has a higher priority. -> refresh_request is proceeded in SM_MAIN...
        )
        port map
        (
            iClk                => clk_ctrl,
            inResetAsync        => R.n_refresh_strobe_hold,
            oStrobe             => refresh_srobe
        );

end behaviour;
