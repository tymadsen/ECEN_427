/*
 * globals.h
 *
 *  Created on: Sep 25, 2015
 *      Author: superman
 */

#ifndef ALIENGLOBALS_H_
#define ALIENGLOBALS_H_

#include "globals.h"

void resetAliens();
bool getAlienDown();
bool getAlienRight();
int getLiveAliens();
point_t getAlienBlockPosition();
void setAlienBlockPosition(point_t point);
void setAlienBullet0(point_t point, unsigned short type, bool isFree, short counter);
void setAlienBullet1(point_t point, unsigned short type, bool isFree, short counter);
void setAlienBullet2(point_t point, unsigned short type, bool isFree, short counter);
void setAlienBullet3(point_t point, unsigned short type, bool isFree, short counter);
aBullet* getAlienBullet(int bullet);
void fireAlienBullet();
void updateAlienBulletCounters();
bool* getAlienDeaths();
void setAlienDeaths(short alien, bool dead);
void updateAlienBlock();
void eraseBullet(point_t pos, unsigned short type);

#endif /* TANKGLOBALS_H_ */
