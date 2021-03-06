set_property PACKAGE_PIN M19 [get_ports {RED_O[0]}]
set_property PACKAGE_PIN L20 [get_ports {RED_O[1]}]
set_property PACKAGE_PIN J20 [get_ports {RED_O[2]}]
set_property PACKAGE_PIN G20 [get_ports {RED_O[3]}]
set_property PACKAGE_PIN F19 [get_ports {RED_O[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {RED_O[*]}]
set_property SLEW FAST [get_ports {RED_O[*]}]

set_property PACKAGE_PIN H18 [get_ports {GREEN_O[0]}]
set_property PACKAGE_PIN N20 [get_ports {GREEN_O[1]}]
set_property PACKAGE_PIN L19 [get_ports {GREEN_O[2]}]
set_property PACKAGE_PIN J19 [get_ports {GREEN_O[3]}]
set_property PACKAGE_PIN H20 [get_ports {GREEN_O[4]}]
set_property PACKAGE_PIN F20 [get_ports {GREEN_O[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {GREEN_O[*]}]
set_property SLEW FAST [get_ports {GREEN_O[*]}]

set_property PACKAGE_PIN P20 [get_ports {BLUE_O[0]}]
set_property PACKAGE_PIN M20 [get_ports {BLUE_O[1]}]
set_property PACKAGE_PIN K19 [get_ports {BLUE_O[2]}]
set_property PACKAGE_PIN J18 [get_ports {BLUE_O[3]}]
set_property PACKAGE_PIN G19 [get_ports {BLUE_O[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {BLUE_O[*]}]
set_property SLEW FAST [get_ports {BLUE_O[*]}]

set_property PACKAGE_PIN R19 [get_ports VSYNC_O]
set_property PACKAGE_PIN P19 [get_ports HSYNC_O]
set_property IOSTANDARD LVCMOS33 [get_ports VSYNC_O]
set_property IOSTANDARD LVCMOS33 [get_ports HSYNC_O]
set_property SLEW FAST [get_ports VSYNC_O]
set_property SLEW FAST [get_ports HSYNC_O]

create_generated_clock -name vga_pxlclk -source [get_pins {system_i/processing_system7_0/inst/PS7_i/FCLKCLK[0]}] -multiply_by 1 [get_pins system_i/axi_dispctrl_0/inst/DONT_USE_BUFR_DIV5.BUFG_inst/O]
set_false_path -from [get_clocks vga_pxlclk] -to [get_clocks clk_fpga_0]
set_false_path -from [get_clocks clk_fpga_0] -to [get_clocks vga_pxlclk]
