/*
 * globals.h
 *
 *  Created on: Sep 25, 2015
 *      Author: superman
 */

#ifndef TANKGLOBALS_H_
#define TANKGLOBALS_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

point_t getAlienBlockPosition();
void setAlienBlockPosition(point_t point);

aBullet getAlienBullet0();
void setAlienBullet0(point_t point, unsigned short type, bool isFree, short counter);
aBullet getAlienBullet1();
void setAlienBullet1(point_t point, unsigned short type, bool isFree, short counter);
aBullet getAlienBullet2();
void setAlienBullet2(point_t point, unsigned short type, bool isFree, short counter);
aBullet getAlienBullet3();
void setAlienBullet3(point_t point, unsigned short type, bool isFree, short counter);

bool getAlienRight();
bool getAlienDown();
int getAlienUpdateTime();
void eraseBullet(point_t pos, unsigned short type);

bool* getAlienDeaths();
void setAlienDeaths(short alien, bool dead);
void updateAlienBlock();
void fireAlienBullet();
void updateAlienBulletCounters();

#endif /* TANKGLOBALS_H_ */
