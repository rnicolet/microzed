#!/usr/bin/tclsh

# set top_folder "/home/rnicolet/workspace/microzed"
set top_folder "."

create_project zynqPL $top_folder/zynqPL -part xc7z010clg400-1

set_property board_part em.avnet.com:microzed_7010:part0:1.1 [current_project]
create_bd_design "design_zynq"

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:processing_system7:5.5 processing_system7_0
endgroup
startgroup
set_property -dict [list CONFIG.PCW_USE_M_AXI_GP0 {0} CONFIG.PCW_QSPI_PERIPHERAL_ENABLE {1} CONFIG.PCW_QSPI_GRP_FBCLK_ENABLE {1} CONFIG.PCW_ENET0_PERIPHERAL_ENABLE {1} CONFIG.PCW_ENET0_GRP_MDIO_ENABLE {1} CONFIG.PCW_ENET0_RESET_ENABLE {1} CONFIG.PCW_ENET0_RESET_IO {MIO 50} CONFIG.PCW_UART1_PERIPHERAL_ENABLE {1} CONFIG.PCW_USB0_PERIPHERAL_ENABLE {1} CONFIG.PCW_USB0_RESET_ENABLE {1} CONFIG.PCW_USB0_RESET_IO {MIO 7} CONFIG.PCW_GPIO_MIO_GPIO_ENABLE {1} CONFIG.PCW_GPIO_MIO_GPIO_IO {MIO}] [get_bd_cells processing_system7_0]
endgroup
startgroup
apply_bd_automation -rule xilinx.com:bd_rule:processing_system7 -config {make_external "FIXED_IO, DDR" apply_board_preset "1" Master "Disable" Slave "Disable" }  [get_bd_cells processing_system7_0]
endgroup

make_wrapper -files [get_files $top_folder/zynqPL/zynqPL.srcs/sources_1/bd/design_zynq/design_zynq.bd] -top

add_files -norecurse $top_folder/zynqPL/zynqPL.srcs/sources_1/bd/design_zynq/hdl/design_zynq_wrapper.v
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1
launch_runs impl_1 -to_step write_bitstream -jobs 8


file mkdir $top_folder/zynqPL/zynqPL.sdk
file copy -force $top_folder/zynqPL/zynqPL.runs/impl_1/design_zynq_wrapper.sysdef /home/rnicolet/workspace/microzed/zynqPL/zynqPL.sdk/design_zynq_wrapper.hdf
