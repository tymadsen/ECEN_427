/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 */
#include <xgpio.h>          	// Provides access to PB GPIO driver.
#include <stdlib.h>
#include <xaxivdma.h>
#include <xio.h>
#include <unistd.h>
#include "mb_interface.h"   	// provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        	// Provides handy macros for the interrupt controller.
//#include "pit.h"				// Provides handy macros for the PIT registers
#include "xac97_l.h"			// xac97 sound
#include "dma_controller.h"     // driver for DMA controller
#include "platform.h"
#include "xparameters.h"
#include "time.h"
#include "render.h"
#include "tankGlobals.h"
#include "spaceshipGlobals.h"
#include "playSound.h"
#include "renderHelper.h"

#define SW7_MASK		0x80
#define SW6_MASK		0x40
#define SW5_MASK		0x20
#define LEFTBTN 		0x8					// Left (Hour) button mask
#define RIGHTBTN 		0x2					// Right (Second) button mask
#define CENTERBTN 	0x1					// Center (Minute) button mask
#define UPBTN 			0x10				// Up btn
#define DOWNBTN 		0x4					// Down btn
#define FIFOSIZE 		512					// Size of AC97 Fifo
#define BUFFERLENGTH 10

// startLevel and getInt function prototype
void startLevel(bool first);

bool gameIsOver = false;					// Game over flag
bool started = false;							// Game started flag

XAxiVdma videoDMAController;
int frameIndex;								// VDMA frameIndex (which frame we are viewing

unsigned pitCounter = 0;								// Counter for pit interrupts, goes up to 100 and resets
unsigned screenUpdateCounter = 0;			// Counter for updating the time, goes up to 20 and resets
unsigned updateSpaceshipCounter = 0;		// Counter for updating the spaceship
unsigned currentButtonState;						// Value the button interrupt handler saves button values to
unsigned prevSwitchState;						// Old value from switches
unsigned currentSwitchState;						// Current Value of switches
unsigned alienBulletCounter = 0;				// Contains the count until the random time an alien bullet will be fired
unsigned spaceshipCounter = 0;					// Counts to the next time a spaceship will appear
unsigned randBulletTime = 0;						// This will be set to a random number for spacing alien bullets
unsigned randSpaceshipTime = 0;				// Random time set to determine when the Spaceship will come out
unsigned alienUpdateTime = 60;					// The time that will decrease as more and more aliens are killed
unsigned ssValueDisplayCounter = 0;		// Counter for the spaceship score flashing animation
unsigned tankKilledCounter = 0;				// Counter for tank death animation
unsigned volumeUpdateCounter = 0;			// For debouncing the volume buttons
int currentVolume = AC97_VOL_MID;	// Initialize volume to mid point
bool gamePaused = false;

char buffer[BUFFERLENGTH+1];
int buffIndex = 0;

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.
XGpio gpSW;   // This is a handle for the switches GPIO block.

void fifo_interrupt_handler(){
	// Add only 128(512/4) samples at a time
	// And make sure we are no overfilling the FIFO
	int i = 0;
	while(i < (FIFOSIZE/4) && !XAC97_isInFIFOFull(XPAR_AXI_AC97_0_BASEADDR)){
		// Call getCurrentSample from playSound.h
		uint32_t sample = getCurrentSample();
		XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, sample);
		i++;
	}
}

void updateScreenElements(){
	// Update tank, and bullets on the screen
	if(started && isTankFree()){
		updateAllBullets();
		if(currentButtonState & LEFTBTN){
			moveTankLeft();
		}
		else if(currentButtonState & RIGHTBTN) {
			moveTankRight();
		}
		if(currentButtonState & CENTERBTN){
			shootTankBullet();
		}
	}

}

void updateKillTankAnimation() {
	// Start switching the animations, switching every 1/4 second
	if(tankKilledCounter == 25 || tankKilledCounter == 75){
		killTankHelper(true, false);
	}
	if(tankKilledCounter == 50 || tankKilledCounter == 100){
		killTankHelper(false, false);
	}
	if(tankKilledCounter == 125){
		killTankHelper(false, true);
	}
}

void updateSpaceshipScore(){
	if(ssValueDisplayCounter == 50 || ssValueDisplayCounter == 150){
		// If the score has been on for 1/2 second, erase it
		if(getSpaceship()->isFree){
			eraseSpaceshipScore(true);
		}
	}
	if(ssValueDisplayCounter == 100){
		if(getSpaceship()->isFree){
			eraseSpaceshipScore(false);
		}
	}
}

void timer_interrupt_handler() {
	// Increment counters..
	screenUpdateCounter++;
	if(!gamePaused){
		pitCounter++;
		alienBulletCounter++;
		spaceshipCounter++;
		updateSpaceshipCounter++;
		ssValueDisplayCounter++;
		tankKilledCounter++;
		volumeUpdateCounter++;
	}

	if(!gameIsOver){
		// The pit counter that will update the aliens every half second
		if(pitCounter >= alienUpdateTime) {
			if(started && isTankFree()){
				updateAliens();
			}
			alienUpdateTime = getAlienUpdateTime();
			pitCounter = 0;
		}
		// The screen will update every 5ms
		if(screenUpdateCounter >= 6) {

			// read switches and update switch values
			prevSwitchState = currentSwitchState;
			currentSwitchState = XGpio_DiscreteRead(&gpSW, 1);  // Get the current state of the switches
			if( (prevSwitchState & (SW7_MASK | SW6_MASK | SW5_MASK)) != (currentSwitchState & (SW7_MASK | SW6_MASK | SW5_MASK))){
				// one of SW5-7 have changed
				if((prevSwitchState & SW7_MASK) != (currentSwitchState & SW7_MASK) && ((currentSwitchState & SW7_MASK) == SW7_MASK)){
					//SW7 has been set high
					xil_printf("SW7 moved up.\r\n");
					gamePaused = true;
					DMA_CONTROLLER_TransferGoGoGOOOOO(XPAR_DMA_CONTROLLER_0_BASEADDR);
				}
				else if((prevSwitchState & SW6_MASK) != (currentSwitchState & SW6_MASK) && ((currentSwitchState & SW6_MASK) == SW6_MASK)){
					//SW6 has been set high
					xil_printf("SW6 moved up.\r\n");
					gamePaused = true;
					saveScreen(FRAME_BUFFER_0_ADDR + 8*640*480);
					gamePaused = false;
				}
				if((prevSwitchState & SW5_MASK) != (currentSwitchState & SW5_MASK)){
					// SW5 has changed, so change the display frame
					frameIndex = (currentSwitchState & SW5_MASK) >> 5;
					if(frameIndex == 1){
						gamePaused = true;
					}
					else {
						gamePaused = false;
					}
					xil_printf("SW5 moved. New Val: %d\r\n", frameIndex);
					if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
						xil_printf("vdma parking failed\n\r");
					}
				}
			}

			// Call function to update the screen
			if(started && !gamePaused){
				updateScreenElements();
			}
			screenUpdateCounter = 0;
		}
		// An alienBullet will fire at a random time between (1*25 - 10*25)
//		xil_printf("alien bullet counter: %d randBulletTime: %d\r\n",  alienBulletCounter, randBulletTime);

		if(alienBulletCounter >= randBulletTime){
//			xil_printf("Fire an alien bullet\n\r");
			if(started && isTankFree())
				fireAlienBulletHelper();
			randBulletTime = (rand()%10)*25 + 50;
			alienBulletCounter = 0;
		}
		// The spaceship will go across the screen at a random time between 1-20 seconds;
		if(spaceshipCounter >= randSpaceshipTime){
			if(started && isTankFree()){
				flySpaceship();
			}
			randSpaceshipTime = (rand()%25)*100 + 2000;
			spaceshipCounter = 0;
		}
		if(isSpaceshipHitHelper()){
			ssValueDisplayCounter = 0;
			setSpaceshipHitHelper(false);
		}
		// Will move the spaceship across the screen
		if(updateSpaceshipCounter >= 5){
			if(started && isTankFree())
				updateSpaceshipHelper();
			updateSpaceshipCounter = 0;
		}
	}
	if(started){
		updateKillTankAnimation();
		updateSpaceshipScore();
	}
	// If the tank is not free, it has been hit
	if(isTankHit()){
		if(started){
			tankKilledCounter = 0;
			setIsTankHit(false);
		}
	}
	// Use the pit to "debounce" the button
	if(volumeUpdateCounter >= 20){
		// Up button is pushed
		if(currentButtonState & UPBTN){
			// Increase volume
			currentVolume -= AC97_VOL_ATTN_1_5_DB;
			// If volume is at max, make max the limit
			if(currentVolume < AC97_VOL_MAX)
				currentVolume = AC97_VOL_MAX;
		// Else if down is pushed
		}else if(currentButtonState & DOWNBTN){
			// Decrease volume
			currentVolume += AC97_VOL_ATTN_1_5_DB;
			// If volume is at min, make min the limit
			if(currentVolume > AC97_VOL_MIN)
				currentVolume = AC97_VOL_MIN;
		}
		// Write to volume registers to update the new volume
		XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVol, currentVolume);
		XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_AuxOutVol, currentVolume);
		volumeUpdateCounter = 0;
	}
}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
void pb_interrupt_handler() {
	XGpio_InterruptGlobalDisable(&gpPB);                // Turn off all PB interrupts for now.
	currentButtonState = XGpio_DiscreteRead(&gpPB, 1);  // Get the current state of the buttons.

	XGpio_InterruptClear(&gpPB, 0xFFFFFFFF);            // Ack the PB interrupt.
	XGpio_InterruptGlobalEnable(&gpPB);                 // Re-enable PB interrupts.
}

void dma_interrupt_handler() {
	xil_printf("DMA interrupt\r\n");
	gamePaused = false;
}

void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the pit interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
//		xil_printf("pit\n\r");
		timer_interrupt_handler();
	}
	// Check the push buttons.
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
//		xil_printf("buttons\n\r");
		pb_interrupt_handler();
	}
	// Check the sound interrupt
	if(intc_status & XPAR_AXI_AC97_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_AXI_AC97_0_INTERRUPT_MASK);
//		xil_printf("sound\n\r");
		fifo_interrupt_handler();
	}
	if (intc_status & XPAR_DMA_CONTROLLER_0_DMA_TX_COMPLETE_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_DMA_CONTROLLER_0_DMA_TX_COMPLETE_INTERRUPT_MASK);
//		xil_printf("pit\n\r");
		dma_interrupt_handler();
	}
}

// This will clear the screen for a new game if the flad is true,
// otherwise it will clear the screen for a new level
void startLevel(bool first){
	gameIsOver = false;
	setIsTankFree(true);
	initScreen(!first);
	randBulletTime = (rand()%10)*50 + 100;
	randSpaceshipTime = (rand()%25)*100 + 1000;
}

int main()
{
	init_platform();                   // Necessary for all programs.
	int Status;                        // Keep track of success/failure of system function calls.
	// Initialize the GPIO peripherals.
	if(XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID)){
		xil_printf("Push Buttons GPIO initialization failed.\r\n");
	}

	if(XGpio_Initialize(&gpSW, XPAR_SWITCHES_8BITS_DEVICE_ID)){
		xil_printf("Switches GPIO initialization failed.\r\n");
	}

	// There are 3 steps to initializing the vdma driver and IP.
	// Step 1: lookup the memory structure that is used to access the vdma driver.
	XAxiVdma_Config * VideoDMAConfig = XAxiVdma_LookupConfig(XPAR_AXI_VDMA_0_DEVICE_ID);
	// Step 2: Initialize the memory structure and the hardware.
	if(XST_FAILURE == XAxiVdma_CfgInitialize(&videoDMAController, VideoDMAConfig,	XPAR_AXI_VDMA_0_BASEADDR)) {
		xil_printf("VideoDMA Did not initialize.\r\n");
	}
	// Step 3: (optional) set the frame store number.
	if(XST_FAILURE ==  XAxiVdma_SetFrmStore(&videoDMAController, 2, XAXIVDMA_READ)) {
		xil_printf("Set Frame Store Failed.");
	}
	// Initialization is complete at this point.

	// Setup the frame counter. We want two read frames. We don't need any write frames but the
	// function generates an error if you set the write frame count to 0. We set it to 2
	// but ignore it because we don't need a write channel at all.
	XAxiVdma_FrameCounter myFrameConfig;
	myFrameConfig.ReadFrameCount = 2;
	myFrameConfig.ReadDelayTimerCount = 10;
	myFrameConfig.WriteFrameCount =2;
	myFrameConfig.WriteDelayTimerCount = 10;
	Status = XAxiVdma_SetFrameCounter(&videoDMAController, &myFrameConfig);
	if (Status != XST_SUCCESS) {
		xil_printf("Set frame counter failed %d\r\n", Status);
		if(Status == XST_VDMA_MISMATCH_ERROR)
			xil_printf("DMA Mismatch Error\r\n");
	}
	// Now we tell the driver about the geometry of our frame buffer and a few other things.
	// Our image is 480 x 640.
	XAxiVdma_DmaSetup myFrameBuffer;
	myFrameBuffer.VertSizeInput = 480;    // 480 vertical pixels.
	myFrameBuffer.HoriSizeInput = 640*4;  // 640 horizontal (32-bit pixels).
	myFrameBuffer.Stride = 640*4;         // Dont' worry about the rest of the values.
	myFrameBuffer.FrameDelay = 0;
	myFrameBuffer.EnableCircularBuf=1;
	myFrameBuffer.EnableSync = 0;
	myFrameBuffer.PointNum = 0;
	myFrameBuffer.EnableFrameCounter = 0;
	myFrameBuffer.FixedFrameStoreAddr = 0;
	if(XST_FAILURE == XAxiVdma_DmaConfig(&videoDMAController, XAXIVDMA_READ, &myFrameBuffer)) {
		xil_printf("DMA Config Failed\r\n");
	 }
	// We need to give the frame buffer pointers to the memory that it will use. This memory
	// is where you will write your video data. The vdma IP/driver then streams it to the HDMI
	// IP.
	myFrameBuffer.FrameStoreStartAddr[0] = FRAME_BUFFER_0_ADDR;
	myFrameBuffer.FrameStoreStartAddr[1] = FRAME_BUFFER_0_ADDR + 8*640*480;
	// Initialize DMA Controller
	// set saved screen data to another frame
	int length = 640*480;
	int destAddr = FRAME_BUFFER_0_ADDR + 8*length;
	DMA_CONTROLLER_TranseferInitialize(XPAR_DMA_CONTROLLER_0_BASEADDR, (Xuint32) FRAME_BUFFER_0_ADDR, (Xuint32) destAddr, (Xuint32) ((4*length)-4));
//	DMA_CONTROLLER_TransferGoGoGOOOOO(XPAR_DMA_CONTROLLER_0_BASEADDR);
	// Set the push button peripheral to be inputs.
	XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);
	// Enable the global GPIO interrupt for push buttons.
	XGpio_InterruptGlobalEnable(&gpPB);
	// Enable all interrupts in the push button peripheral.
	XGpio_InterruptEnable(&gpPB, 0xFFFFFFFF);

//	xil_printf("Starting the fit stuff\n\r");
//	PIT_SetDelayValue(1000000);
//	PIT_EnableReloadCounter();
//	PIT_EnableInterrupts();
//	PIT_EnableCounter();
//	xil_printf("Made it past the fit stuff\r\n");


	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	// Enable interrupts for pit, Push Buttons, and FIFO on the AC97
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,(XPAR_FIT_TIMER_0_INTERRUPT_MASK | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK
										| XPAR_AXI_AC97_0_INTERRUPT_MASK | XPAR_DMA_CONTROLLER_0_DMA_TX_COMPLETE_INTERRUPT_MASK));
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);

	// Initialize AC97
	XAC97_HardReset(XPAR_AXI_AC97_0_BASEADDR);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_ExtendedAudioStat, 1);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, AC97_PCM_RATE_11025_HZ);
	XAC97_mSetControl(XPAR_AXI_AC97_0_BASEADDR, AC97_ENABLE_IN_FIFO_INTERRUPT);


	if(XST_FAILURE == XAxiVdma_DmaSetBufferAddr(&videoDMAController, XAXIVDMA_READ,myFrameBuffer.FrameStoreStartAddr)) {
		xil_printf("DMA Set Address Failed Failed\r\n");
	}
	// Print a sanity message if you get this far.
	xil_printf("Woohoo! I made it through initialization.\n\r");
	// Now, let's get ready to start displaying some stuff on the screen.
	// The variables framePointer and framePointer1 are just pointers to the base address
	// of frame 0 and frame 1.
	// Just paint some large red, green, blue, and white squares in different
	// positions of the image for each frame in the buffer (framePointer0 and framePointer1).

	// Let's print out the alien as ASCII characters on the screen.
	// Each line of the alien is a 32-bit integer. We just need to strip the bits out and send
	// them to stdout.
	// MSB is the left-most pixel for the alien, so start from the MSB as we print from left to right.


	// This tells the HDMI controller the resolution of your display (there must be a better way to do this).
	XIo_Out32(XPAR_AXI_HDMI_0_BASEADDR, 640*480);

	// Start the DMA for the read channel only.
	if(XST_FAILURE == XAxiVdma_DmaStart(&videoDMAController, XAXIVDMA_READ)){
		xil_printf("DMA START FAILED\r\n");
	}
	frameIndex = 0;
	// We have two frames, let's park on frame 0. Use frameIndex to index them.
	// Note that you have to start the DMA process before parking on a frame.
	if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
		xil_printf("vdma parking failed\n\r");
	}

	// Start the game
	startLevel(true);
	microblaze_enable_interrupts();
	setvbuf(stdin, NULL, _IONBF, 0);
	while(1) {

		// Check if the game or level is over
		// if so, set some flags to be handled in the interrupt handlers
		if(!gameIsOver && isGameOver()){
			xil_printf("game over...\n");
			// Draw GameOver
			drawGameOver();
			gameIsOver = true;
		}


		started = true;
		// blocking call: wait until a character is present
//		input = getchar();
////
//
//		if(input >= '0' && input <= '9'){
////			xil_printf("\r\ngOT A ONE \r\n\r\n");
//			// pause pit so we can get a new value
//			PIT_DisableCounter();
////			xil_printf("\r\nValue to load: ");
////			input = getchar();
////			input = getchar();
//			while(input != '\r' && input != '\n' && buffIndex < 10){
//				if(input >= '0' && input <= '9'){
//					buffer[buffIndex] = input;
//					xil_printf("%c", input);
//					buffIndex++;
//				}
//				input = getchar();
//			}
//			buffer[buffIndex] = '\0';
//			xil_printf("buffer: %s\r\n",buffer);
////			//convert to number
////			//write to delay reg
//			loadValue = atoi(buffer);
////			xil_printf("\n\r");
////			xil_printf("value is: %d\r\n", atoi(buffer));
//			if(loadValue > 0 && loadValue <= 1000000000){
//				PIT_SetDelayValue(loadValue);
//				xil_printf("Timer has been updated!\r\n");
//			}
//			else {
//				xil_printf("Timer has NOT been updated!\r\n");
//			}
//			PIT_EnableCounter();
//			buffIndex = 0;
//		}

//		xil_printf("got character: %c\r\n", input);
	}

	cleanup_platform();

	return 0;
}
