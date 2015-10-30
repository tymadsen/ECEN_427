/*
 * playSound.c
 *
 *  Created on: Oct 28, 2015
 *      Author: superman
 */

#include "playSound.h"
#include "tankGlobals.h"

#define alien_explosion_sound 	0
#define saucer_explosion_sound 	1
#define saucer_sound 			2
#define tank_explosion_sound 	3
#define tank_fire_sound 		4
#define alien1_sound 			5
#define alien2_sound 			6
#define alien3_sound 			7
#define alien4_sound 			8

bool spaceshipSoundPlaying = false;

static uint32_t currentSampleNum = 0;
//Array of all the sound data
int* soundData[] = {alien_explosion_soundData, saucer_explosion_soundData, saucer_soundData, tank_explosion_soundData,
					tank_fire_soundData,alien1_soundData,alien2_soundData, alien3_soundData,alien4_soundData};
//Array of the number of samples for each sound data
int* numSamples[] = {&alien_explosion_numberOfSamples, &saucer_explosion_numberOfSamples, &saucer_numberOfSamples, &tank_explosion_numberOfSamples,
					&tank_fire_numberOfSamples, &alien1_numberOfSamples, &alien2_numberOfSamples,
					&alien3_numberOfSamples, &alien4_numberOfSamples };
//The index of the current sound being played
int soundIndex = 0;
//Kept track of which alien marching sound to play
short alienCounter = 0;

//Function called by the interrupt routine that returned the sample to load into the FIFO
uint32_t getCurrentSample(){
	//If the current sample is greater than the number of samples, return 0 to play nothing
	if (currentSampleNum == *numSamples[soundIndex]) {
		//If the spaceship sound is playing, reset the sample num to 0 so we can loop through the sound
		if(spaceshipSoundPlaying == true){
			currentSampleNum = 0;
		}
		return 0;
	}
	//To create the sample, shift the sound data up and or it together to send the sound to the left and right channels
	uint32_t sample = (soundData[soundIndex][currentSampleNum]<<16)|soundData[soundIndex][currentSampleNum];
	currentSampleNum++;
	return sample;
}

void playAlienSound(){
	if(spaceshipSoundPlaying == false){
		//Pick the alien sound to play based on the alienCounter
		switch(alienCounter){
		case 0: soundIndex = alien1_sound; break;
		case 1: soundIndex = alien2_sound; break;
		case 2: soundIndex = alien3_sound; break;
		case 3: soundIndex = alien4_sound; break;
		default : soundIndex = tank_explosion_sound;
		}
		//Reset the sampleNum to 0
		currentSampleNum = 0;
		//Increase the alien counter, if it goes above 3, reset it to 0
		alienCounter++;
		if(alienCounter > 3){
			alienCounter = 0;
		}
	}
	return;
}

void playTankBullet(){
	//If the spaceship sound is not playing, play the tank bullet sound
	if(spaceshipSoundPlaying == false){
		soundIndex = tank_fire_sound;
		//Reset the sample num to play from the beginning
		currentSampleNum = 0;
	}
	return;
}

void playSaucerExplosionSound(){
	//Stop playing the saucer sound
	spaceshipSoundPlaying = false;
	//change the current sound and reset the sample num
	soundIndex = saucer_explosion_sound;
	currentSampleNum = 0;
	return;
}

void playAlienExplosionSound(){
	//If the saucer sound is not playing, play the alien explosion sound
	if(spaceshipSoundPlaying == false){
		soundIndex = alien_explosion_sound;
		currentSampleNum = 0;
	}
	return;
}

void playTankExplosionSound(){
	//If the saucer sound is not playing, play the tank explosion sound
	if(spaceshipSoundPlaying == false){
		soundIndex = tank_explosion_sound;
		currentSampleNum = 0;
	}
	return;
}

//Called when the spaceship goes off the screen to stop the saucer sound
void setSpaceshipSoundPlaying(bool isFree){
	//Sets the saucer playing flag to isFree
	spaceshipSoundPlaying = isFree;
	if(spaceshipSoundPlaying == false)
		currentSampleNum = 0;
	return;
}

void playSpaceshipSound(){
	//Changes the current sound to the saucer sound and raises the flag
	soundIndex = saucer_sound;
	spaceshipSoundPlaying = true;
	return;
}
