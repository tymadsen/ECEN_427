/*
 * pit.h
 *
 *  Created on: Nov 5, 2015
 *      Author: Ty & Hampton Madsen
 */

#ifndef PIT_H_
#define PIT_H_

#include <stdio.h>          // xil_printf and so forth.
#include "stdint.h"
#include "xil_io.h"
#include "xparameters.h"

/************************** Constant Definitions ****************************/
#define PIT_ENABLE_COUNTER_MASK 	0x00000001
#define PIT_DISABLE_COUNTER_MASK 	~(PIT_ENABLE_COUNTER_MASK)
#define PIT_ENABLE_INTERRUPT_MASK 	0x00000002
#define PIT_DISABLE_INTERRUPT_MASK 	~(PIT_ENABLE_INTERRUPT_MASK)
#define PIT_ENABLE_RELOAD_MASK 		0x00000004
#define PIT_DISABLE_RELOAD_MASK 	~(PIT_ENABLE_RELOAD_MASK)
#define PIT_CONTROL_REG_OFFSET		0x00000000
#define PIT_DELAY_VAL_REG_OFFSET	0x00000004

/***************** Macros (Inline Functions) Definitions ********************/
#define PIT_EnableControlStatus(Mask) \
	Xil_Out32((XPAR_PIT_TIMER_0_BASEADDR), (Xil_In32(XPAR_PIT_TIMER_0_BASEADDR) | Mask))
#define PIT_DisableControlStatus(Mask) \
	Xil_Out32((XPAR_PIT_TIMER_0_BASEADDR), (Xil_In32(XPAR_PIT_TIMER_0_BASEADDR) & Mask))

/************************** Function Prototypes *****************************/

/*
 * API Functions implemented in pit.c
 */
void PIT_SetDelayValue(u32 val);
void PIT_EnableCounter();
void PIT_DisableCounter();
void PIT_EnableInterrupts();
void PIT_DisableInterrupts();
void PIT_EnableReloadCounter();
void PIT_DisableReloadCounter();

#endif /* PIT_H_ */
