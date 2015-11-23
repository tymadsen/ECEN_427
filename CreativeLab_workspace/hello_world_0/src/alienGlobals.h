/*
 * globals.h
 *
 *  Created on: Sep 25, 2015
 *      Author: superman
 */

#ifndef ALIENGLOBALS_H_
#define ALIENGLOBALS_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

void resetAliens();
point_t getAlienBlockPosition();
void setAlienBlockPosition(point_t point);

void setAlienBullet0(point_t point, unsigned short type, bool isFree, short counter);
void setAlienBullet1(point_t point, unsigned short type, bool isFree, short counter);
void setAlienBullet2(point_t point, unsigned short type, bool isFree, short counter);
void setAlienBullet3(point_t point, unsigned short type, bool isFree, short counter);

aBullet* getAlienBullet(int bullet);

bool getAlienRight();
bool getAlienDown();
int getAlienUpdateTime();
void eraseBullet(point_t pos, unsigned short type);

bool* getAlienDeaths();
void setAlienDeaths(short alien, bool dead);
void updateAlienBlock(int direction);
void fireAlienBullet(unsigned short bullet);
void updateAlienBulletCounters();

int getLiveAliens();

#endif /* TANKGLOBALS_H_ */
