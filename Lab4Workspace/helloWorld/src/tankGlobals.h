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
void setIsTankHit(bool hit);
void setIsTankFree(bool free);
void killTankGlobals();
point_t getTankBulletPosition();
void setTankBulletPosition(point_t point);
void setTankBulletPositionXY(int x, int y);

void fireTankBullet();

#endif /* TANKGLOBALS_H_ */
