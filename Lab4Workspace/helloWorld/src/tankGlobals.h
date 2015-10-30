/*
 * globals.h
 *
 *  Created on: Sep 25, 2015
 *      Author: superman
 */

#ifndef TANKGLOBALS_H_
#define TANKGLOBALS_H_

#include "globals.h"

bool isTankBulletFree();
bool isTankFree();
void setIsTankFree(bool free);
bool isTankHit();
void setIsTankHit(bool hit);
void killTankGlobals();
point_t getTankPosition();
void setTankPosition(signed short pixels);
void setTankPositionPoint(int x, int y);
point_t getTankBulletPosition();
void setTankBulletPositionXY(int x, int y);
void setTankBulletPosition(point_t point);
void fireTankBullet();

extern point_t tankPosition;
extern point_t tankBulletPosition;
extern bool tankBulletFree;
extern bool tankFree;
extern bool tankHit;

#endif /* TANKGLOBALS_H_ */
