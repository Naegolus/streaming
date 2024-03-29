#************************************************************
# THIS IS A WIZARD-GENERATED FILE.                           
#
# Version 10.1 Build 153 11/29/2010 SJ Web Edition
#
#************************************************************

# Copyright (C) 1991-2010 Altera Corporation
# Your use of Altera Corporation's design tools, logic functions 
# and other software and tools, and its AMPP partner logic 
# functions, and any output files from any of the foregoing 
# (including device programming or simulation files), and any 
# associated documentation or information are expressly subject 
# to the terms and conditions of the Altera Program License 
# Subscription Agreement, Altera MegaCore Function License 
# Agreement, or other applicable license agreement, including, 
# without limitation, that your use is for the sole purpose of 
# programming logic devices manufactured by Altera and sold by 
# Altera or its authorized distributors.  Please refer to the 
# applicable agreement for further details.



# Clock constraints

create_clock -name "ClkMain50" -period 20.000ns [get_ports {CLOCK_50}] -waveform {0.000 10.000}
create_clock -name "Clk25" -period 10.000ns [get_ports {em_altremote:eALTREMOTE|clk_25}] -waveform {0.000 5.000}


# Automatically constrain PLL and other generated clocks
derive_pll_clocks -create_base_clocks

# Automatically calculate clock uncertainty to jitter and other effects.
derive_clock_uncertainty

# tsu/th constraints

# tco constraints

# tpd constraints

