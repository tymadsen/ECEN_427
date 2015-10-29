/*
 * playSound.c
 *
 *  Created on: Oct 28, 2015
 *      Author: superman
 */

#include "playSound.h"

static uint32_t currentSampleNum = 0;

uint32_t getCurrentSample(){
	if (currentSampleNum == ufo_highpitch_numberOfSamples) {
		currentSampleNum = 0;
		return 0;
	}

	uint32_t sample = ufo_highpitch_soundData[currentSampleNum]<<16|ufo_highpitch_soundData[currentSampleNum];

	currentSampleNum++;

	return sample;
}
