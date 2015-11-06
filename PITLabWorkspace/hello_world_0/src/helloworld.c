#include "xgpio.h"          // Provides access to PB GPIO driver.
#include <stdio.h>          // xil_printf and so forth.
#include <stdlib.h>
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include "pit.h"			// Provides handy macros for the PIT registers
#define DOWN 0x4			// Down button mask
#define LEFT 0x8			// Left (Hour) button mask
#define RIGHT 0x2			// Right (Second) button mask
#define CENTER 0x1			// Center (Minute) button mask
#define UP 0x10				// Up button mask
#define TRUE 1
#define FALSE 0
#define BUFFERLENGTH 10

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.
int currentButtonState;		// Value the button interrupt handler saves button values to
int debouncedButtonState;	// Saves the debounced button states
int fitCounter = 0;			// Counter for fit interrupts, goes up to 100 and resets
int printCounter = 0;		// Counter for updating the time, goes up to 20 and resets
int btnCounter = 0;			// Counter for debouncing buttons, goes up to 7 and resets
int autoIncCounter = 0;		// Counter for auto incrementing clock after more than 1 second hold, goes up to 50 and resets
int seconds = 0;
int minutes = 0;
int hours = 0;
int clockDisabled = 0;


char buffer[BUFFERLENGTH+1] = {' '};
int delayVal = 1000000;
int buffIndex = 0;

// This masks the currentButtonState to determine if buttons are debounced
void debounce_buttons(){
	debouncedButtonState = currentButtonState & 0x0000001F;
}
// Increment or decrement hours
void inc_dec_hours(int inc){
	if(inc) {//increment
		hours++;
		if(hours == 24) //roll hours over
			hours = 0;
	}
	else {//decrement
		hours--;
		if(hours == -1) //roll hours over
			hours = 23;
	}
}
// Increment or decrement minutes
void inc_dec_mins(int inc){
	if(inc) {//increment
		minutes++;
		if(minutes == 60){//roll minutes over
			minutes = 0;
			//if no buttons are pushed, then cascade to hours
			if(debouncedButtonState == 0)
				inc_dec_hours(TRUE);
		}
	}
	else {//decrement
		minutes--;
		if(minutes == -1) //roll minutes over
			minutes = 59;
	}
}
// Increment or decrement seconds
void inc_dec_secs(int inc){
	if(inc) {// increment
		seconds++;
		if(seconds == 60){// roll seconds over
			seconds = 0;
			// If no buttons are pushed, then cascade to minutes
			if(debouncedButtonState == 0)
				inc_dec_mins(TRUE);
		}
	}
	else {//decrement
		seconds--;
		if(seconds == -1) //roll seconds over
			seconds = 59;
	}
}
// Function increments or decrements the appropriate values corresponding to buttons
void inc_clock(){
	// If down is pushed
	if(debouncedButtonState & DOWN) {
		// If left (hours) pushed then decrement hours
		if(debouncedButtonState & LEFT)
			inc_dec_hours(FALSE);
		// If middle (mins) pushed then decrement minutes
		if(debouncedButtonState & CENTER)
			inc_dec_mins(FALSE);
		// If right (secs) pushed then decrement seconds
		if(debouncedButtonState & RIGHT)
			inc_dec_secs(FALSE);
	}
	// If up is pushed
	if(debouncedButtonState & UP) {
		// If left (hours) pushed then increment the hours
		if(debouncedButtonState & LEFT)
			inc_dec_hours(TRUE);
		// If middle (mins) pushed then increment the minutes
		if(debouncedButtonState & CENTER)
			inc_dec_mins(TRUE);
		// If right (secs) pushed then increment the seconds
		if(debouncedButtonState & RIGHT)
			inc_dec_secs(TRUE);
	}
}

// This is invoked in response to a timer interrupt.
// It does 2 things: 1) debounce switches, and 2) advances the time.
void timer_interrupt_handler() {
	fitCounter++;
	printCounter++;
	autoIncCounter++;
	// The FIT counter that will increment the clock every second
	if(fitCounter >= 100) {
		// The clock will only increment when there are no buttons pressed
		if(debouncedButtonState == 0)
			inc_dec_secs(TRUE);
		// Reset the FIT counter every second
		fitCounter = 0;
	}
	// The clock on the terminal will update every 20ms
	if(printCounter >= 20) {
		xil_printf("\r%02d:%02d:%02d", hours, minutes, seconds);
		printCounter = 0;
	}
	// This will wait 7ms after the last button interrupt before reading the stable button value
	if(btnCounter == 7){
		// Save the debounced button values
		debounce_buttons();
		// If a button is pressed, call the inc_clock function and decide whether or not to change the time
		if(debouncedButtonState > 0)
			inc_clock();
	}
	// If buttons are pushed down more than 1 second
	if(btnCounter >= 100) {
		// Increment the clock at 2 times per second
		if(autoIncCounter >= 50) {
			inc_clock();
			autoIncCounter = 0;
		}
	}
	btnCounter++;
}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
void pb_interrupt_handler() {
  // Clear the GPIO interrupt.
  XGpio_InterruptGlobalDisable(&gpPB);                // Turn off all PB interrupts for now.
  currentButtonState = XGpio_DiscreteRead(&gpPB, 1);  // Get the current state of the buttons.

  // Reset button counter
  btnCounter = 0;

  XGpio_InterruptClear(&gpPB, 0xFFFFFFFF);            // Ack the PB interrupt.
  XGpio_InterruptGlobalEnable(&gpPB);                 // Re-enable PB interrupts.
}

// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
// This routine acks the interrupt at the controller level but the peripheral
// interrupt must be ack'd by the dispatched interrupt handler.
void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if ((intc_status & XPAR_BLINK_0_USER_PIT_INTERRUPT_MASK)){
//		xil_printf("pit intr\r\n");
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_BLINK_0_USER_PIT_INTERRUPT_MASK);
		if(clockDisabled == 0){
			timer_interrupt_handler();
		}
	}
	// Check the push buttons.
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK){
//		xil_printf("btn intr\r\n");
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
		pb_interrupt_handler();
	}
}

int main (void) {
    init_platform();
    // Initialize the GPIO peripherals.
    int success;
    success = XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
    // Set the push button peripheral to be inputs.
    XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);
    // Enable the global GPIO interrupt for push buttons.
    XGpio_InterruptGlobalEnable(&gpPB);
    // Enable all interrupts in the push button peripheral.
    XGpio_InterruptEnable(&gpPB, 0xFFFFFFFF);

    microblaze_register_handler(interrupt_handler_dispatcher, NULL);
    XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
    		(XPAR_BLINK_0_USER_PIT_INTERRUPT_MASK | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK));
    XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
    microblaze_enable_interrupts();

    PIT_SetDelayValue(delayVal);
    PIT_EnableCounter();
    PIT_EnableInterrupts();
    PIT_EnableReloadCounter();
    // Program never ends.
    char nextChar = getchar();
    while(1){

    	if(nextChar == 's'){
    		clockDisabled = 1;
    		xil_printf("\r\nEnter the number:");
    		nextChar = getchar();
    		while(nextChar != 'e' && buffIndex < 10){
    			xil_printf("%c", nextChar);
    			buffer[buffIndex] = nextChar;
    			nextChar = getchar();
    			buffIndex++;
    		}
    		//convert to number
    		//write to delay reg
    		xil_printf("\n\r");
    		int val = atoi(buffer);
    		xil_printf("value is: %d\r\n", val);
    		PIT_SetDelayValue(val);
    		buffIndex= 0;
    		clockDisabled = 0;
    		nextChar = getchar();
    	}
//
    	// last character was a carriage return
//    	if(buffer[buffIndex] == 0xd){
//    		buffer[buffIndex+1] = 0x0;
//    		PIT_SetDelayValue(atoi(buffer));
//    		buffIndex = 0;
//    	}else{
//    		buffer[buffIndex] = getchar();
//			buffIndex++;
//			if(buffIndex == BUFFERLENGTH)
//				buffIndex = 0;
//    	}


    }

    cleanup_platform();

    return 0;
}
