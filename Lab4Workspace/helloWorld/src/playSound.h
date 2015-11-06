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

extern int alien_explosion_numberOfSamples;
extern int alien_explosion_soundData[];
extern int alien_explosion_sampleRate;

extern int alien1_soundData[];
extern int alien1_numberOfSamples;
extern int alien1_sampleRate;

extern int alien2_soundData[];
extern int alien2_numberOfSamples;
extern int alien2_sampleRate;

extern int alien3_soundData[];
extern int alien3_numberOfSamples;
extern int alien3_sampleRate;

extern int alien4_soundData[];
extern int alien4_numberOfSamples;
extern int alien4_sampleRate;

extern int saucer_explosion_soundData[];
extern int saucer_explosion_numberOfSamples;
extern int saucer_explosion_sampleRate;

extern int saucer_soundData[];
extern int saucer_numberOfSamples;
extern int saucer_sampleRate;

extern int tank_explosion_soundData[];
extern int tank_explosion_numberOfSamples;
extern int tank_explosion_sampleRate;

extern int tank_fire_soundData[];
extern int tank_fire_numberOfSamples;
extern int tank_fire_sampleRate;

uint32_t getCurrentSample();
void playAlienSound();
void playTankBullet();
void playSaucerExplosionSound();
void playAlienExplosionSound();
void playTankExplosionSound();
void setSpaceshipSoundPlaying(bool isFree);
void playSpaceshipSound();

#endif /* PLAYSOUND_H_ */
