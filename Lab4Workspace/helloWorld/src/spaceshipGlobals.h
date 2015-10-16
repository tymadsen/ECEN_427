/*
 * globals.h
 *
 *  Created on: Sep 25, 2015
 *      Author: superman
 */

#ifndef SPACESHIPGLOBALS_H_
#define SPACESHIPGLOBALS_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

void setSpaceship(int direction);
void setInitialSpaceship(point_t pos);
saucer getSpaceship();
void updateSpaceship();
int getSpaceshipValue();
point_t getOldSpaceshipLoc();
bool isSpaceshipHit();
void setSpaceshipHit(bool hit);

#endif /* GLOBALS_H_ */
