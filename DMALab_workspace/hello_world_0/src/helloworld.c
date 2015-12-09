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

	if (intc_status & XPAR_DMA_CONTROLLER_0_DMA_TX_COMPLETE_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_DMA_CONTROLLER_0_DMA_TX_COMPLETE_INTERRUPT_MASK);
//		xil_printf("pit\n\r");
		dma_interrupt_handler();
	}

}


int main()
{
	int source_word1 = 0xDEAEBEEF;
	int source_word2 = 0xDEAABEEF;
	int source_word = 0xDEADBEEF;
	int destination_word = 0x0;
	int source_word3 = 0xDEABBEEF;
	int source_word4 = 0xDEACBEEF;

	init_platform();                   // Necessary for all programs.

	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	// Enable interrupts for pit, Push Buttons, and FIFO on the AC97
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,(XPAR_FIT_TIMER_0_INTERRUPT_MASK | XPAR_DMA_CONTROLLER_0_DMA_TX_COMPLETE_INTERRUPT_MASK));
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);

	// Print a sanity message if you get this far.
	xil_printf("Woohoo! I made it through initialization.\n\r");

	microblaze_enable_interrupts();

	print("Hello World\n\r");
	cleanup_platform();
	printf("Printing value before DMA transfer.\n\r");
	xil_printf("%x\r\n", destination_word);

	DMA_CONTROLLER_TranseferInitialize(XPAR_DMA_CONTROLLER_0_BASEADDR, (Xuint32) &source_word, (Xuint32) &destination_word, (Xuint32) 0x10);
	DMA_CONTROLLER_TransferGoGoGOOOOO(XPAR_DMA_CONTROLLER_0_BASEADDR);

	printf("Printing value after DMA burst transfer.\n\r");
	xil_printf("%x\r\n", destination_word);

//	while(1) {
//
//	}

	cleanup_platform();

	return 0;
}
