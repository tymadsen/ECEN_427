/*
 * pit.c
 *
 *  Created on: Nov 5, 2015
 *      Author: Ty & Hampton Madsen
 */

#include "pit.h"


void PIT_SetDelayValue(u32 val){
	Xil_Out32((PIT_BASE_ADDR) + (PIT_DELAY_VAL_REG_OFFSET), val);
}

void PIT_EnableCounter(){
	PIT_EnableControlStatus(PIT_ENABLE_COUNTER_MASK);
}

void PIT_DisableCounter(){
	PIT_DisableControlStatus(PIT_DISABLE_COUNTER_MASK);
}

void PIT_EnableInterrupts(){
	PIT_EnableControlStatus(PIT_ENABLE_INTERRUPT_MASK);
}

void PIT_DisableInterrupts(){
	PIT_DisableControlStatus(PIT_DISABLE_INTERRUPT_MASK);
}

void PIT_EnableReloadCounter(){
	PIT_EnableControlStatus(PIT_ENABLE_RELOAD_MASK);
}

void PIT_DisableReloadCounter(){
	PIT_DisableControlStatus(PIT_DISABLE_RELOAD_MASK);
}
