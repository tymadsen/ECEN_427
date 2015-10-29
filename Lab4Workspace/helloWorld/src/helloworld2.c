/*
 * playSound.c
 *
 *  Created on: Oct 28, 2015
 *      Author: superman
 */
#include "xgpio.h"          // Provides access to PB GPIO driver.
#include <stdio.h>
#include <stdint.h>
#include "platform.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xio.h"
#include "time.h"
#include "unistd.h"
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include "xac97_l.h"		// xac97 sound
//#include "Sounds/explosion.c"

XGpio gpAC97;

extern int explosion_numberOfSamples;
extern int explosion_soundData[];
extern int explosion_sampleRate;

extern int ufo_lowpitch_soundData[];
extern int ufo_lowpitch_numberOfSamples;
extern int ufo_lowpitch_sampleRate;

extern int ufo_highpitch_soundData[];
extern int ufo_highpitch_numberOfSamples;
extern int ufo_highpitch_sampleRate;

extern int shoot_soundData[];
extern int shoot_numberOfSamples;
extern int shoot_sampleRate;

//extern int tankFireSoundRate;
//extern int tankFireSoundFrames;
//extern int tankFireSound[];

extern int invaderkilled_soundData[];
extern int invaderkilled_numberOfSamples;
extern int invaderkilled_sampleRate;

extern int fastinvader1_soundData[];
extern int fastinvader1_numberOfSamples;
extern int fastinvader1_sampleRate;

extern int fastinvader2_soundData[];
extern int fastinvader2_numberOfSamples;
extern int fastinvader2_sampleRate;

extern int fastinvader3_soundData[];
extern int fastinvader3_numberOfSamples;
extern int fastinvader3_sampleRate;

extern int fastinvader4_soundData[];
extern int fastinvader4_numberOfSamples;
extern int fastinvader4_sampleRate;

int fifoCount = 0;
int currentSound = 0;
int maxSamples = 0;

static uint32_t currentSampleNum = 0;
uint32_t getCurrentSample();

void timer_interrupt_handler() {
//	xil_printf("We got a timer interrupt!");
}


uint32_t getCurrentSample() {

	if (currentSampleNum == ufo_highpitch_numberOfSamples) {
		currentSampleNum = 0;
		return 0;
	}

	uint32_t sample = ufo_highpitch_soundData[currentSampleNum]<<16|ufo_highpitch_soundData[currentSampleNum];

	currentSampleNum++;

	return sample;
}


void fifo_interrupt_handler(){
	// Clear the GPIO interrupt.
//	XGpio_InterruptGlobalDisable(&gpPB);                // Turn off all PB interrupts for now.
//	currentButtonState = XGpio_DiscreteRead(&gpPB, 1);  // Get the current state of the buttons.
//
//	XGpio_InterruptClear(&gpPB, 0xFFFFFFFF);            // Ack the PB interrupt.
//	XGpio_InterruptGlobalEnable(&gpPB);                 // Re-enable PB interrupts.
//
//	xil_printf("We got a fifo interrupt!");
	int i = 0;
//	while(i < 100) {
//		if((fifoCount + i) > explosion_numberOfSamples){
//			fifoCount = 100;
//		}
//		XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, explosion_soundData[fifoCount + i]<<16|explosion_soundData[fifoCount + i]);
//		i++;
//	}
//	fifoCount += 100;



	while (i < 100 && !XAC97_isInFIFOFull(XPAR_AXI_AC97_0_BASEADDR)) {
		uint32_t sample = getCurrentSample();
		XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, sample);
	}


//	XAC97_PlayAudio(XPAR_AXI_AC97_0_BASEADDR, explosion_soundData, &(explosion_soundData[explosion_numberOfSamples-1]));
}

void interrupt_handler_dispatcher(void* ptr) {
//	xil_printf("We got an interrupt!");
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timer_interrupt_handler();
	}
//		xil_printf("we got a not timer interrupt: %p\n", ptr);
	// Check the AC97 interrupt first.
	if (intc_status & XPAR_AXI_AC97_0_INTERRUPT_MASK){

		fifo_interrupt_handler();

		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_AXI_AC97_0_INTERRUPT_MASK);
	}

}

int main(){

	init_platform();                   // Necessary for all programs.

	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,(XPAR_FIT_TIMER_0_INTERRUPT_MASK | XPAR_AXI_AC97_0_INTERRUPT_MASK));
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();

	//Provide a hard reset for the X97 core
	XAC97_HardReset(XPAR_AXI_AC97_0_BASEADDR);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_ExtendedAudioStat, 1);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, AC97_PCM_RATE_11025_HZ);
	XAC97_mSetControl(XPAR_AXI_AC97_0_BASEADDR, AC97_ENABLE_IN_FIFO_INTERRUPT);
//	XAC97_PlayAudio(XPAR_AXI_AC97_0_BASEADDR, explosion_soundData, &(explosion_soundData[explosion_numberOfSamples-1]));

	xil_printf("Carrots\n");

	while(1){
		//		XAC97_PlayAudio(XPAR_AXI_AC97_0_BASEADDR, ufo_lowpitch_soundData, (int*)(&ufo_lowpitch_soundData[ufo_lowpitch_numberOfSamples-1]));
//		XAC97_PlayAudio(XPAR_AXI_AC97_0_BASEADDR, ufo_highpitch_soundData, (int*)(&ufo_highpitch_soundData[ufo_highpitch_numberOfSamples-1]));
//		XAC97_PlayAudio(XPAR_AXI_AC97_0_BASEADDR, shoot_soundData, (int*)(&shoot_soundData[shoot_numberOfSamples-1]));
//		XAC97_PlayAudio(XPAR_AXI_AC97_0_BASEADDR, invaderkilled_soundData, (int*)(&invaderkilled_soundData[invaderkilled_numberOfSamples-1]));
//		XAC97_PlayAudio(XPAR_AXI_AC97_0_BASEADDR, fastinvader1_soundData, (int*)(&fastinvader1_soundData[fastinvader1_numberOfSamples-1]));
		//spin forever
	}

	cleanup_platform();

	return 0;
}
