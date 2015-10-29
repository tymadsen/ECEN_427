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
#include "globals.h"

point_t getTankPosition();
void setTankPosition(signed short pixels);
void setTankPositionPoint(int x, int y);
bool isTankFree();
bool isTankHit();
bool isTankBulletFree();
void setIsTankHit(bool hit);
void setIsTankFree(bool free);
void killTankGlobals();
point_t getTankBulletPosition();
void setTankBulletPosition(point_t point);
void setTankBulletPositionXY(int x, int y);

void fireTankBullet();

extern point_t tankPosition;
extern point_t tankBulletPosition;
extern bool tankBulletFree;
extern bool tankFree;
extern bool tankHit;

#endif /* TANKGLOBALS_H_ */
