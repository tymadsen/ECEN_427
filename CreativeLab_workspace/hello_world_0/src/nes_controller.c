/*****************************************************************************
* Filename:          C:\Madsen\SpaceInvadersCreativeHW/drivers/nes_controller_v1_00_a/src/nes_controller.c
* Version:           1.00.a
* Description:       nes_controller Driver Source File
* Date:              Thu Nov 19 13:48:36 2015 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "nes_controller.h"

unsigned controller_btns;

/************************** Function Definitions ***************************/

void NES_CONTROLLER_read(){
	xil_printf("NES Controller register value: %d\r\n", Xil_In32(XPAR_NES_CONTROLLER_0_BASEADDR));
}

unsigned NES_CONTROLLER_1_read(){
	return Xil_In32(XPAR_NES_CONTROLLER_0_BASEADDR) & NES_CONTROLLER_1_BUTTONS_MASK;
}

unsigned NES_CONTROLLER_2_read(){
	return (Xil_In32(XPAR_NES_CONTROLLER_0_BASEADDR) & NES_CONTROLLER_2_BUTTONS_MASK) >> NES_CONTROLLER_2_RSHIFT_OFFSET;
}
