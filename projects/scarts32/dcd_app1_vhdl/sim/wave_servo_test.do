onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /tbtbddcd_app1_design/Clk
add wave -noupdate /tbtbddcd_app1_design/DUT/CLOCK_25
add wave -noupdate /tbtbddcd_app1_design/DUT/scarts_reset
add wave -noupdate -expand -group SPI /tbtbddcd_app1_design/SPI_CS(0)
add wave -noupdate -expand -group SPI /tbtbddcd_app1_design/SPI_CS(1)
add wave -noupdate -expand -group SPI /tbtbddcd_app1_design/SPI_CS(2)
add wave -noupdate -expand -group SPI /tbtbddcd_app1_design/SPI_MISO
add wave -noupdate -expand -group SPI /tbtbddcd_app1_design/SPI_MOSI
add wave -noupdate -expand -group SPI /tbtbddcd_app1_design/SPI_CLK
add wave -noupdate /tbtbddcd_app1_design/DUT/eSCARTS32/regf_unit/ram1/ram
add wave -noupdate -group brom_unit /tbtbddcd_app1_design/DUT/eSCARTS32/brom_unit/clk
add wave -noupdate -group brom_unit /tbtbddcd_app1_design/DUT/eSCARTS32/brom_unit/hold
add wave -noupdate -group brom_unit -radix hexadecimal /tbtbddcd_app1_design/DUT/eSCARTS32/brom_unit/addr
add wave -noupdate -group brom_unit -radix hexadecimal /tbtbddcd_app1_design/DUT/eSCARTS32/brom_unit/data
add wave -noupdate -group iram_unit -radix hexadecimal /tbtbddcd_app1_design/DUT/eSCARTS32/use_prog_gen/iram_unit/wdata
add wave -noupdate -group iram_unit -radix hexadecimal /tbtbddcd_app1_design/DUT/eSCARTS32/use_prog_gen/iram_unit/waddr
add wave -noupdate -group iram_unit /tbtbddcd_app1_design/DUT/eSCARTS32/use_prog_gen/iram_unit/wen
add wave -noupdate -group iram_unit -radix hexadecimal /tbtbddcd_app1_design/DUT/eSCARTS32/use_prog_gen/iram_unit/raddr
add wave -noupdate -group iram_unit -radix hexadecimal /tbtbddcd_app1_design/DUT/eSCARTS32/use_prog_gen/iram_unit/rdata
add wave -noupdate -group sdram /tbtbddcd_app1_design/DUT/eSCARTS32/use_prog_gen/iram_unit/R
add wave -noupdate -group sdram /tbtbddcd_app1_design/DRAM_BA
add wave -noupdate -group sdram -radix hexadecimal /tbtbddcd_app1_design/DRAM_ADDR
add wave -noupdate -group sdram -radix hexadecimal /tbtbddcd_app1_design/DRAM_DQ
add wave -noupdate -group sdram /tbtbddcd_app1_design/DRAM_CLK
add wave -noupdate /tbtbddcd_app1_design/dram_ctrl_str
add wave -noupdate -divider UART
add wave -noupdate /tbtbddcd_app1_design/GDB_RX
add wave -noupdate /tbtbddcd_app1_design/GDB_TX
add wave -noupdate /tbtbddcd_app1_design/uart_bit_toggle
add wave -noupdate -expand /tbtbddcd_app1_design/DUT/eSCARTS32/ext_miniUART_unit/exto
add wave -noupdate -divider {Servo module}
add wave -noupdate -expand -subitemconfig {/tbtbddcd_app1_design/DUT/eSERVO/R.rset {-height 15 -childformat {{/tbtbddcd_app1_design/DUT/eSERVO/R.rset(0) -radix hexadecimal} {/tbtbddcd_app1_design/DUT/eSERVO/R.rset(1) -radix hexadecimal} {/tbtbddcd_app1_design/DUT/eSERVO/R.rset(2) -radix hexadecimal} {/tbtbddcd_app1_design/DUT/eSERVO/R.rset(3) -radix hexadecimal} {/tbtbddcd_app1_design/DUT/eSERVO/R.rset(4) -radix hexadecimal} {/tbtbddcd_app1_design/DUT/eSERVO/R.rset(5) -radix hexadecimal} {/tbtbddcd_app1_design/DUT/eSERVO/R.rset(6) -radix hexadecimal} {/tbtbddcd_app1_design/DUT/eSERVO/R.rset(7) -radix hexadecimal}} -expand} /tbtbddcd_app1_design/DUT/eSERVO/R.rset(0) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/DUT/eSERVO/R.rset(1) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/DUT/eSERVO/R.rset(2) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/DUT/eSERVO/R.rset(3) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/DUT/eSERVO/R.rset(4) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/DUT/eSERVO/R.rset(5) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/DUT/eSERVO/R.rset(6) {-height 15 -radix hexadecimal} /tbtbddcd_app1_design/DUT/eSERVO/R.rset(7) {-height 15 -radix hexadecimal}} /tbtbddcd_app1_design/DUT/eSERVO/R
add wave -noupdate /tbtbddcd_app1_design/DUT/eSERVO/o_pwm(0)
add wave -noupdate /tbtbddcd_app1_design/DUT/eSERVO/o_pwm(1)
add wave -noupdate /tbtbddcd_app1_design/DUT/eSERVO/o_pwm(2)
add wave -noupdate /tbtbddcd_app1_design/DUT/eSERVO/o_pwm(3)
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {1466865672 ps} 0} {{Cursor 3} {2868537313 ps} 0}
configure wave -namecolwidth 388
configure wave -valuecolwidth 148
configure wave -justifyvalue left
configure wave -signalnamewidth 1
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
WaveRestoreZoom {0 ps} {325158 ps}
