#include <stdio.h>
#include "dma_controller.h"
#include <xgpio.h>          	// Provides access to PB GPIO driver.
#include <stdlib.h>
#include <xaxivdma.h>
#include <xio.h>
#include <unistd.h>
#include "mb_interface.h"   	// provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        	// Provides handy macros for the interrupt controller.
#include "platform.h"
#include "xparameters.h"
#include "time.h"

void timer_interrupt_handler() {
//	xil_printf("timer interrupt\r\n");
}

void dma_interrupt_handler() {
	xil_printf("DMA interrupt\r\n");
}

void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the pit interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
//		xil_printf("pit\n\r");
		timer_interrupt_handler();
	}
	xil_printf("intrpt\r\n");
	if (intc_status & XPAR_DMA_CONTROLLER_0_DMA_TX_COMPLETE_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_DMA_CONTROLLER_0_DMA_TX_COMPLETE_INTERRUPT_MASK);
//		xil_printf("pit\n\r");
		dma_interrupt_handler();
	}

}


int main()
{
	int source_arr[] = {0x12345678, 0xDEADBEEF, 0x09876543, 0xBEEFB00B, 0xBEEF1234};
	int dest_arr[] = {0x0, 0x0, 0x0, 0x0, 0x0};

	init_platform();                   // Necessary for all programs.

	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	// Enable interrupts for pit, Push Buttons, and FIFO on the AC97 XPAR_FIT_TIMER_0_INTERRUPT_MASK |
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,( XPAR_DMA_CONTROLLER_0_DMA_TX_COMPLETE_INTERRUPT_MASK));
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);

	// Print a sanity message if you get this far.
	xil_printf("Woohoo! I made it through initialization.\n\r");

	microblaze_enable_interrupts();

	print("Hello World\n\r");
	cleanup_platform();
	printf("Printing value before DMA transfer.\n\r");
	int i;
	int length = 6; // in words
	for(i = 0; i < length; i++){
		xil_printf("%x\r\n", dest_arr[i]);
	}

//	DMA_CONTROLLER_MasterRecvWord(XPAR_DMA_CONTROLLER_0_BASEADDR,  (Xuint32) source_arr, (Xuint32) dest_arr);
	DMA_CONTROLLER_TranseferInitialize(XPAR_DMA_CONTROLLER_0_BASEADDR, (Xuint32) source_arr, (Xuint32) dest_arr, (Xuint32) (length-1)*4);
	DMA_CONTROLLER_TransferGoGoGOOOOO(XPAR_DMA_CONTROLLER_0_BASEADDR);

	printf("Printing value after DMA burst transfer.\n\r");
	for(i = 0; i < length; i++){
		xil_printf("%x\r\n", dest_arr[i]);
	}
//	DMA_CONTROLLER_TransferStopSTOPSTOOOOOOOOP(XPAR_DMA_CONTROLLER_0_BASEADDR);

//	while(1) {
//
//	}

	cleanup_platform();
	xil_printf("DONE!!!");
	return 0;
}
