/*
 * playSound.c
 *
 *  Created on: Oct 28, 2015
 *      Author: superman
 */

#include "playSound.h"
#include "tankGlobals.h"

#define explosion_sound 0
#define ufo_low_sound 1
#define ufo_high_sound 2
#define shoot_sound 3
#define invader_death_sound 4
#define invader1_sound 5
#define invader2_sound 6
#define invader3_sound 7
#define invader4_sound 8

bool spaceshipSoundPlaying = false;

static uint32_t currentSampleNum = 0;
int* soundData[] = {explosion_soundData, ufo_lowpitch_soundData, ufo_highpitch_soundData, shoot_soundData,
					invaderkilled_soundData,fastinvader1_soundData,fastinvader2_soundData, fastinvader3_soundData,fastinvader4_soundData};
int* numSamples[] = {&explosion_numberOfSamples, &ufo_lowpitch_numberOfSamples, &ufo_highpitch_numberOfSamples, &shoot_numberOfSamples,
					&invaderkilled_numberOfSamples, &fastinvader1_numberOfSamples, &fastinvader2_numberOfSamples,
					&fastinvader3_numberOfSamples, &fastinvader4_numberOfSamples};
int soundIndex = 0;

short alienCounter = 0;
short spaceshipSoundCounter = 0;

uint32_t getCurrentSample(){
	if (currentSampleNum >= *numSamples[soundIndex]) {
		return 0;
	}

	uint32_t sample = (soundData[soundIndex][currentSampleNum]<<16)|soundData[soundIndex][currentSampleNum];
	currentSampleNum++;
	return sample;
}

void playAlienSound(){
	//if(spaceshipSoundPlaying == false){
		switch(alienCounter){
		case 0: soundIndex = invader1_sound;
		case 1: soundIndex = invader2_sound;
		case 2: soundIndex = invader3_sound;
		case 3: soundIndex = invader4_sound;
		}
		currentSampleNum = 0;
		alienCounter++;
		if(alienCounter > 3){
			alienCounter = 0;
		}
	//}
	return;
}

void playTankBullet(){
	soundIndex = shoot_sound;
	currentSampleNum = 0;
	return;
}

void playExplosionSound(){
	spaceshipSoundPlaying = false;
	soundIndex = explosion_sound;
	currentSampleNum = 0;
	return;
}

//Called when the spaceship goes off the screen
void setSpaceshipSoundPlaying(bool isFree){
	spaceshipSoundPlaying = !isFree;
	return;
}

void playSpaceshipSound(){
	if(spaceshipSoundCounter == 0){
		soundIndex = ufo_low_sound;
	} else {
		soundIndex = ufo_high_sound;
	}
	spaceshipSoundPlaying = true;
	spaceshipSoundCounter++;
	currentSampleNum = 0;
	if(spaceshipSoundCounter > 1){
		spaceshipSoundCounter = 0;
	}
	return;
}
