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
#include "xgpio.h"          // Provides access to PB GPIO driver.
#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xio.h"
#include "time.h"
#include "unistd.h"
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include "render.h"
#include "renderHelper.h"
#define DEBUG
#define LEFTBTN 0x8			// Left (Hour) button mask
#define RIGHTBTN 0x2			// Right (Second) button mask
#define CENTERBTN 0x1			// Center (Minute) button mask
#define UPBTN 0x10				//Up btn
#define TRUE 1
#define FALSE 0
void print(char *str);

#define FRAME_BUFFER_0_ADDR 0xC1000000  // Starting location in DDR where we will store the images that we display.
#define MAX_SILLY_TIMER 10000000;
#define PIXEL_ADJUSTMENT 4

#define ALIEN_HEIGHT 16
#define WORD_WIDTH 32
#define WHITE 	0x00FFFFFF
#define RED 	0x00FF0000
#define GREEN 	0x0000FF00
#define BLUE 	0x000000FF
#define BLACK 	0x00000000

int fitCounter = 0;			// Counter for fit interrupts, goes up to 100 and resets
int screenUpdateCounter = 0;		// Counter for updating the time, goes up to 20 and resets
int updateSpaceshipCounter = 0;
int currentButtonState;		// Value the button interrupt handler saves button values to
int alienBulletCounter = 0;		//Contains the count until the random time an alien bullet will be fired
int spaceshipCounter = 0;		//Counts to the next time a spaceship will appear
int randBulletTime = 0;
int randSpaceshipTime = 0;
int alienUpdateTime = 60;		//The time that will increase as more and more aliens are killed
bool started = false;
int ssValueDisplayCounter = 0;
int tankKilledCounter = 0;

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.


void updateScreenElements(){
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
	return;
}

void timer_interrupt_handler() {
	fitCounter++;
	screenUpdateCounter++;
	alienBulletCounter++;
	spaceshipCounter++;
	updateSpaceshipCounter++;
	ssValueDisplayCounter++;
	tankKilledCounter++;
//	xil_printf("ssCounter: %d\r\n", ssValueDisplayCounter);
	// The FIT counter that will update the aliens every half second
	if(fitCounter >= alienUpdateTime) {
		if(started && isTankFree()){
			updateAliens();
		}
		alienUpdateTime = getAlienUpdateTime();
		fitCounter = 0;
	}
	// The screen will update every 5ms
	if(screenUpdateCounter >= 6) {
		//Call function to update the screen
		updateScreenElements();
		screenUpdateCounter = 0;
	}
	//An alienBullet will fire at a random time between (1*25 - 10*25)
	if(alienBulletCounter >= randBulletTime){
//		xil_printf("Fire Alien Bullet\r\n");
		if(started && isTankFree())
			fireAlienBulletHelper();
		randBulletTime = (rand()%10)*25 + 50;
		alienBulletCounter = 0;
	}
	//The spaceship will go across the screen at a random time between 1-20 seconds;
	if(spaceshipCounter >= randSpaceshipTime){
//		xil_printf("Send out the saucer\r\n");
		if(started && isTankFree()){
			flySpaceship();
//			xil_printf("We are sending out the saucer\r\n");
		}
		randSpaceshipTime = (rand()%25)*100 + 2000;
		spaceshipCounter = 0;
	}
	if(isSpaceshipHitHelper()){
//		xil_printf("We are reseting the value counter\r\n");
		ssValueDisplayCounter = 0;
		setSpaceshipHitHelper(false);
	}
	if(ssValueDisplayCounter == 50 || ssValueDisplayCounter == 150 || ssValueDisplayCounter == 250){
		//If the score has been on for 1/2 second, erase it
//		xil_printf("We are going to erase the spaceship value\r\n");
		if(getSpaceship().isFree){
			eraseSpaceshipScore(true);
		}
	}
	if(ssValueDisplayCounter == 100 || ssValueDisplayCounter == 200){
//		xil_printf("We are going to draw the spaceship value\r\n");
		if(getSpaceship().isFree){
			eraseSpaceshipScore(false);
		}
	}
	//Will move the spaceship across the screen
	if(updateSpaceshipCounter >= 5){
//		xil_printf("We are updating the saucer position\r\n");
		if(started && isTankFree())
			updateSpaceshipHelper();
		updateSpaceshipCounter = 0;
	}
	//If the tank is not free, it has been hit
	if(isTankHit()){
		if(started){
			tankKilledCounter = 0;
			setIsTankHit(false);
		}
	}
	if(tankKilledCounter == 25 || tankKilledCounter == 75){
//		xil_printf("Printing death1\r\n");
		killTankHelper(true, false);
	}
	if(tankKilledCounter == 50 || tankKilledCounter == 100){
//		xil_printf("Printing death2\r\n");
		killTankHelper(false, false);
	}
	if(tankKilledCounter == 125){
//		xil_printf("reseting\r\n");
		killTankHelper(false, true);
	}
}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
void pb_interrupt_handler() {
  // Clear the GPIO interrupt.
  XGpio_InterruptGlobalDisable(&gpPB);                // Turn off all PB interrupts for now.
  currentButtonState = XGpio_DiscreteRead(&gpPB, 1);  // Get the current state of the buttons.

  XGpio_InterruptClear(&gpPB, 0xFFFFFFFF);            // Ack the PB interrupt.
  XGpio_InterruptGlobalEnable(&gpPB);                 // Re-enable PB interrupts.
}

void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timer_interrupt_handler();
	}
	// Check the push buttons.
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
		pb_interrupt_handler();
	}
}

int main()
{
	init_platform();                   // Necessary for all programs.
	int Status;                        // Keep track of success/failure of system function calls.
	XAxiVdma videoDMAController;
    // Initialize the GPIO peripherals.
    int success;
    success = XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
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
	 myFrameBuffer.FrameStoreStartAddr[1] = FRAME_BUFFER_0_ADDR + 4*640*480;
    // Set the push button peripheral to be inputs.
    XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);
    // Enable the global GPIO interrupt for push buttons.
    XGpio_InterruptGlobalEnable(&gpPB);
    // Enable all interrupts in the push button peripheral.
    XGpio_InterruptEnable(&gpPB, 0xFFFFFFFF);

    microblaze_register_handler(interrupt_handler_dispatcher, NULL);
    XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
	    		(XPAR_FIT_TIMER_0_INTERRUPT_MASK | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK));
    XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
    microblaze_enable_interrupts();

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
	int frameIndex = 0;
	// We have two frames, let's park on frame 0. Use frameIndex to index them.
	// Note that you have to start the DMA process before parking on a frame.
	if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
		xil_printf("vdma parking failed\n\r");
	}
	initScreen();
	randBulletTime = (rand()%10)*50 + 100;
	randSpaceshipTime = (rand()%25)*100 + 1000;
	//setvbuf(stdin, NULL, _IONBF, 0)
	while(!isGameOver()) {
		started = true;
		// Mput = getchar();
	}
	cleanup_platform();

	return 0;
}
