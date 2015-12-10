/*
 * playSound.c
 *
 *  Created on: Oct 28, 2015
 *      Author: superman
 */

#include "playSound.h"
#include "tankGlobals.h"

#define alien_explosion_sound 0
#define saucer_explosion_sound 1
#define saucer_sound 2
#define tank_explosion_sound 3
#define tank_fire_sound 4
#define alien1_sound 5
#define alien2_sound 6
#define alien3_sound 7
#define alien4_sound 8

bool spaceshipSoundPlaying = false;

static uint32_t currentSampleNum = 0;
int* soundData[] = {alien_explosion_soundData, saucer_explosion_soundData, saucer_soundData, tank_explosion_soundData,
					tank_fire_soundData,alien1_soundData,alien2_soundData, alien3_soundData,alien4_soundData};
int* numSamples[] = {&alien_explosion_numberOfSamples, &saucer_explosion_numberOfSamples, &saucer_numberOfSamples, &tank_explosion_numberOfSamples,
					&tank_fire_numberOfSamples, &alien1_numberOfSamples, &alien2_numberOfSamples,
					&alien3_numberOfSamples, &alien4_numberOfSamples };
int soundIndex = 0;

short alienCounter = 0;
short spaceshipSoundCounter = 0;

uint32_t getCurrentSample(){
	if (currentSampleNum == *numSamples[soundIndex]) {
		if(spaceshipSoundPlaying == true){
			currentSampleNum = 0;
		}
		return 0;
	}

	uint32_t sample = (soundData[soundIndex][currentSampleNum]<<16)|soundData[soundIndex][currentSampleNum];
	currentSampleNum++;
	return sample;
}

void playAlienSound(){
	if(spaceshipSoundPlaying == false){
		switch(alienCounter){
		case 0: soundIndex = alien1_sound; break;
		case 1: soundIndex = alien2_sound; break;
		case 2: soundIndex = alien3_sound; break;
		case 3: soundIndex = alien4_sound; break;
		default : soundIndex = tank_explosion_sound;
		}
		currentSampleNum = 0;
		alienCounter++;
		if(alienCounter > 3){
			alienCounter = 0;
		}
	}
	return;
}

void playTankBullet(){
	if(spaceshipSoundPlaying == false){
		soundIndex = tank_fire_sound;
		currentSampleNum = 0;
	}
	return;
}

void playSaucerExplosionSound(){
	spaceshipSoundPlaying = false;
	soundIndex = saucer_explosion_sound;
	currentSampleNum = 0;
	return;
}

void playAlienExplosionSound(){
	if(spaceshipSoundPlaying == false){
		soundIndex = alien_explosion_sound;
		currentSampleNum = 0;
	}
	return;
}

void playTankExplosionSound(){
	if(spaceshipSoundPlaying == false){
		soundIndex = tank_explosion_sound;
		currentSampleNum = 0;
	}
	return;
}

//Called when the spaceship goes off the screen
void setSpaceshipSoundPlaying(bool isFree){
	spaceshipSoundPlaying = isFree;
	if(spaceshipSoundPlaying == false)
		currentSampleNum = 0;
	return;
}

void playSpaceshipSound(){

	soundIndex = saucer_sound;
	spaceshipSoundPlaying = true;
//	spaceshipSoundCounter++;
//	if(spaceshipSoundCounter > 1){
//		spaceshipSoundCounter = 0;
//	}
	return;
}
