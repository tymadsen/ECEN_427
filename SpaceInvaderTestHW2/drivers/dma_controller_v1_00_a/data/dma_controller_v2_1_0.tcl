##############################################################################
## Filename:          C:\Madsen\SpaceInvaderTestHW2/drivers/dma_controller_v1_00_a/data/dma_controller_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Mon Dec 07 16:26:13 2015 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "dma_controller" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
