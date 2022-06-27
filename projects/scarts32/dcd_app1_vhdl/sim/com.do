#----------------------------------*-tcl-*- 
set Root ../SRC

vlib work
vmap work work

# Packages
vcom ${Root}/Global-p.vhd
vcom ${Root}/scarts_amba_pkg.vhd
vcom ${Root}/scarts_pkg.vhd
vcom ${Root}/scarts_core_pkg.vhd

# Global
vcom ${Root}/FreqDivider-Rtl-ea.vhd
vcom ${Root}/StrobeGen-Rtl-ea.vhd
vcom ${Root}/scarts_pll.vhd

# scarts32
vcom ${Root}/wpt.vhd
vcom ${Root}/bpt.vhd
vcom ${Root}/rs232.vhd
vcom ${Root}/prog.vhd
vcom ${Root}/sysc.vhd
vcom ${Root}/vectab.vhd
vcom ${Root}/boot_rom.vhd
vcom ${Root}/brom.vhd
vcom ${Root}/scarts_bootram.vhd
vcom ${Root}/dram.vhd
vcom ${Root}/regfram.vhd
vcom ${Root}/regf.vhd
vcom ${Root}/sdram_pll.vhd
vcom ${Root}/iram.vhd
vcom ${Root}/core.vhd
vcom ${Root}/scarts.vhd

# External modules
vcom ${Root}/ext_mod_gpio-Rtl-ea.vhd
vcom ${Root}/ext_mod_spi-Rtl-ea.vhd
vcom ${Root}/altremote.vhd
vcom ${Root}/ext_mod_altremote-Rtl-ea.vhd
vcom ${Root}/ext_mod_time-Rtl-ea.vhd
vcom ${Root}/ext_mod_servo-Rtl-ea.vhd

# Testbed
vcom ${Root}/sdram_0_test_component.vhd
vlog ${Root}/42s16160.v
vcom ${Root}/TbdDCD_APP1_DESIGN-Rtl-ea.vhd
# vcom ${Root}/../Quartus/simulation/questasim/TbdDCD_APP1_DESIGN.vho

# Testbenches
vcom ${Root}/tbTbdDCD_APP1_DESIGN-Bhv-ea.vhd
