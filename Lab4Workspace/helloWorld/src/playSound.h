/*
 * playSound.h
 *
 *  Created on: Oct 28, 2015
 *      Author: superman
 */

#ifndef PLAYSOUND_H_
#define PLAYSOUND_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

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



uint32_t getCurrentSample();
void playAlienSound();
void playTankBullet();
void playExplosionSound();
void playSpaceshipSound();
void setSpaceshipSoundPlaying(bool isFree);

#endif /* PLAYSOUND_H_ */
