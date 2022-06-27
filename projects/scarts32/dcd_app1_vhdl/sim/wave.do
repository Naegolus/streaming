onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /tbtbddcd_app1_design/clk
add wave -noupdate /tbtbddcd_app1_design/dut/clock_25
add wave -noupdate /tbtbddcd_app1_design/dut/scarts_reset
add wave -noupdate -expand -group SPI /tbtbddcd_app1_design/spi_cs(0)
add wave -noupdate -expand -group SPI /tbtbddcd_app1_design/spi_cs(1)
add wave -noupdate -expand -group SPI /tbtbddcd_app1_design/spi_cs(2)
add wave -noupdate -expand -group SPI /tbtbddcd_app1_design/spi_miso
add wave -noupdate -expand -group SPI /tbtbddcd_app1_design/spi_mosi
add wave -noupdate -expand -group SPI /tbtbddcd_app1_design/spi_clk
add wave -noupdate -subitemconfig {/tbtbddcd_app1_design/dut/escarts32/regf_unit/ram1/ram(0) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/regf_unit/ram1/ram(1) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/regf_unit/ram1/ram(2) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/regf_unit/ram1/ram(3) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/regf_unit/ram1/ram(4) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/regf_unit/ram1/ram(5) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/regf_unit/ram1/ram(6) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/regf_unit/ram1/ram(7) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/regf_unit/ram1/ram(8) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/regf_unit/ram1/ram(9) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/regf_unit/ram1/ram(10) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/regf_unit/ram1/ram(11) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/regf_unit/ram1/ram(12) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/regf_unit/ram1/ram(13) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/regf_unit/ram1/ram(14) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/regf_unit/ram1/ram(15) {-height 15 -radix hexadecimal}} /tbtbddcd_app1_design/dut/escarts32/regf_unit/ram1/ram
add wave -noupdate -group brom_unit /tbtbddcd_app1_design/dut/escarts32/brom_unit/clk
add wave -noupdate -group brom_unit /tbtbddcd_app1_design/dut/escarts32/brom_unit/hold
add wave -noupdate -group brom_unit -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/brom_unit/addr
add wave -noupdate -group brom_unit -radix hexadecimal -subitemconfig {/tbtbddcd_app1_design/dut/escarts32/brom_unit/data(15) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/brom_unit/data(14) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/brom_unit/data(13) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/brom_unit/data(12) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/brom_unit/data(11) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/brom_unit/data(10) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/brom_unit/data(9) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/brom_unit/data(8) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/brom_unit/data(7) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/brom_unit/data(6) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/brom_unit/data(5) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/brom_unit/data(4) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/brom_unit/data(3) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/brom_unit/data(2) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/brom_unit/data(1) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/brom_unit/data(0) {-radix hexadecimal}} /tbtbddcd_app1_design/dut/escarts32/brom_unit/data
add wave -noupdate -group iram_unit -radix hexadecimal -subitemconfig {/tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wdata(15) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wdata(14) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wdata(13) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wdata(12) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wdata(11) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wdata(10) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wdata(9) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wdata(8) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wdata(7) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wdata(6) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wdata(5) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wdata(4) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wdata(3) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wdata(2) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wdata(1) {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wdata(0) {-radix hexadecimal}} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wdata
add wave -noupdate -group iram_unit -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/waddr
add wave -noupdate -group iram_unit /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/wen
add wave -noupdate -group iram_unit -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/raddr
add wave -noupdate -group iram_unit -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/rdata
add wave -noupdate -expand -group sdram -subitemconfig {/tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/r.curr_row {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/r.raddr_latch {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/r.waddr_latch {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/r.wdata_latch {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/r.addr {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/r.dq {-radix hexadecimal}} /tbtbddcd_app1_design/dut/escarts32/use_prog_gen/iram_unit/r
add wave -noupdate -expand -group sdram /tbtbddcd_app1_design/dram_ba
add wave -noupdate -expand -group sdram -radix hexadecimal -subitemconfig {/tbtbddcd_app1_design/dram_addr(12) {-radix hexadecimal} /tbtbddcd_app1_design/dram_addr(11) {-radix hexadecimal} /tbtbddcd_app1_design/dram_addr(10) {-radix hexadecimal} /tbtbddcd_app1_design/dram_addr(9) {-radix hexadecimal} /tbtbddcd_app1_design/dram_addr(8) {-radix hexadecimal} /tbtbddcd_app1_design/dram_addr(7) {-radix hexadecimal} /tbtbddcd_app1_design/dram_addr(6) {-radix hexadecimal} /tbtbddcd_app1_design/dram_addr(5) {-radix hexadecimal} /tbtbddcd_app1_design/dram_addr(4) {-radix hexadecimal} /tbtbddcd_app1_design/dram_addr(3) {-radix hexadecimal} /tbtbddcd_app1_design/dram_addr(2) {-radix hexadecimal} /tbtbddcd_app1_design/dram_addr(1) {-radix hexadecimal} /tbtbddcd_app1_design/dram_addr(0) {-radix hexadecimal}} /tbtbddcd_app1_design/dram_addr
add wave -noupdate -expand -group sdram -radix hexadecimal -subitemconfig {/tbtbddcd_app1_design/dram_dq(15) {-radix hexadecimal} /tbtbddcd_app1_design/dram_dq(14) {-radix hexadecimal} /tbtbddcd_app1_design/dram_dq(13) {-radix hexadecimal} /tbtbddcd_app1_design/dram_dq(12) {-radix hexadecimal} /tbtbddcd_app1_design/dram_dq(11) {-radix hexadecimal} /tbtbddcd_app1_design/dram_dq(10) {-radix hexadecimal} /tbtbddcd_app1_design/dram_dq(9) {-radix hexadecimal} /tbtbddcd_app1_design/dram_dq(8) {-radix hexadecimal} /tbtbddcd_app1_design/dram_dq(7) {-radix hexadecimal} /tbtbddcd_app1_design/dram_dq(6) {-radix hexadecimal} /tbtbddcd_app1_design/dram_dq(5) {-radix hexadecimal} /tbtbddcd_app1_design/dram_dq(4) {-radix hexadecimal} /tbtbddcd_app1_design/dram_dq(3) {-radix hexadecimal} /tbtbddcd_app1_design/dram_dq(2) {-radix hexadecimal} /tbtbddcd_app1_design/dram_dq(1) {-radix hexadecimal} /tbtbddcd_app1_design/dram_dq(0) {-radix hexadecimal}} /tbtbddcd_app1_design/dram_dq
add wave -noupdate -expand -group sdram /tbtbddcd_app1_design/dram_clk
add wave -noupdate /tbtbddcd_app1_design/dram_ctrl_str
add wave -noupdate -divider UART
add wave -noupdate /tbtbddcd_app1_design/gdb_rx
add wave -noupdate /tbtbddcd_app1_design/gdb_tx
add wave -noupdate /tbtbddcd_app1_design/uart_bit_toggle
add wave -noupdate -subitemconfig {/tbtbddcd_app1_design/dut/escarts32/ext_miniuart_unit/exto.data {-height 15 -radix hexadecimal}} -expand -subitemconfig {/tbtbddcd_app1_design/dut/escarts32/ext_miniuart_unit/exto.data {-height 15 -radix hexadecimal}} /tbtbddcd_app1_design/dut/escarts32/ext_miniuart_unit/exto
add wave -noupdate -divider CORE
add wave -noupdate -subitemconfig {/tbtbddcd_app1_design/dut/escarts32/core_unit/r.f -expand /tbtbddcd_app1_design/dut/escarts32/core_unit/r.f.pcnt {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/core_unit/r.f.jmpdest {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/core_unit/r.d -expand /tbtbddcd_app1_design/dut/escarts32/core_unit/r.d.pcnt {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/core_unit/r.d.vectabaddr {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/core_unit/r.d.regfaddr1 {-radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/core_unit/r.d.regfaddr2 {-radix hexadecimal}} -expand /tbtbddcd_app1_design/dut/escarts32/core_unit/r
add wave -noupdate -divider SYSC
add wave -noupdate -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/core_unit/prog_cnt
add wave -noupdate /tbtbddcd_app1_design/dut/escarts32/sysc_unit/hold
add wave -noupdate /tbtbddcd_app1_design/dut/escarts32/sysc_unit/extsel
add wave -noupdate -subitemconfig {/tbtbddcd_app1_design/dut/escarts32/sysc_unit/exti.data {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/dut/escarts32/sysc_unit/exti.addr {-height 15 -radix hexadecimal}} /tbtbddcd_app1_design/dut/escarts32/sysc_unit/exti
add wave -noupdate -subitemconfig {/tbtbddcd_app1_design/dut/escarts32/sysc_unit/exto.data {-height 15 -radix hexadecimal}} /tbtbddcd_app1_design/dut/escarts32/sysc_unit/exto
add wave -noupdate /tbtbddcd_app1_design/dut/escarts32/sysc_unit/staen
add wave -noupdate /tbtbddcd_app1_design/dut/escarts32/sysc_unit/stactrl
add wave -noupdate -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/sysc_unit/interruptin
add wave -noupdate /tbtbddcd_app1_design/dut/escarts32/sysc_unit/interruptnr
add wave -noupdate /tbtbddcd_app1_design/dut/escarts32/sysc_unit/intcmd
add wave -noupdate -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/sysc_unit/r.ifacereg(3)
add wave -noupdate -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/sysc_unit/r.ifacereg(4)
add wave -noupdate -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/sysc_unit/r.ifacereg(5)
add wave -noupdate -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/sysc_unit/r.ifacereg(6)
add wave -noupdate -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/sysc_unit/r.ifacereg(7)
add wave -noupdate -subitemconfig {/tbtbddcd_app1_design/dut/escarts32/sysc_unit/r.tmp_int_prot {-height 15 -radix hexadecimal}} /tbtbddcd_app1_design/dut/escarts32/sysc_unit/r
add wave -noupdate /tbtbddcd_app1_design/dut/escarts32/sysc_unit/rstint
add wave -noupdate /tbtbddcd_app1_design/dut/escarts32/sysc_unit/int_hold
add wave -noupdate /tbtbddcd_app1_design/dut/escarts32/sysc_unit/staflag
add wave -noupdate /tbtbddcd_app1_design/dut/escarts32/sysc_unit/condflag
add wave -noupdate /tbtbddcd_app1_design/dut/escarts32/sysc_unit/carryflag
add wave -noupdate -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/sysc_unit/fptrwnew
add wave -noupdate -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/sysc_unit/fptrxnew
add wave -noupdate -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/sysc_unit/fptrynew
add wave -noupdate -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/sysc_unit/fptrznew
add wave -noupdate -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/sysc_unit/fptrw
add wave -noupdate -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/sysc_unit/fptrx
add wave -noupdate -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/sysc_unit/fptry
add wave -noupdate -radix hexadecimal /tbtbddcd_app1_design/dut/escarts32/sysc_unit/fptrz
add wave -noupdate -divider {Time module}
add wave -noupdate /tbtbddcd_app1_design/dut/etime/strobe250us
add wave -noupdate -subitemconfig {/tbtbddcd_app1_design/dut/etime/r.rset -expand /tbtbddcd_app1_design/dut/etime/r.rset(0) {-radix hexadecimal} /tbtbddcd_app1_design/dut/etime/r.rset(1) {-radix hexadecimal} /tbtbddcd_app1_design/dut/etime/r.rset(2) {-radix hexadecimal} /tbtbddcd_app1_design/dut/etime/r.rset(3) {-radix hexadecimal} /tbtbddcd_app1_design/dut/etime/r.rset(4) {-radix hexadecimal} /tbtbddcd_app1_design/dut/etime/r.rset(5) {-radix hexadecimal} /tbtbddcd_app1_design/dut/etime/r.rset(6) {-radix hexadecimal} /tbtbddcd_app1_design/dut/etime/r.rset(7) {-radix hexadecimal}} -expand /tbtbddcd_app1_design/dut/etime/r
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {3619940000 ps} 0} {{Cursor 2} {3524460000 ps} 0}
configure wave -namecolwidth 388
configure wave -valuecolwidth 148
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ms
update
WaveRestoreZoom {249912982 ps} {268543523 ps}
