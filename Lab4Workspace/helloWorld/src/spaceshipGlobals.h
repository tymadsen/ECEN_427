/*
 * globals.h
 *
 *  Created on: Sep 25, 2015
 *      Author: superman
 */

#ifndef SPACESHIPGLOBALS_H_
#define SPACESHIPGLOBALS_H_

#include "globals.h"

void setSpaceship(int direction);
void setInitialSpaceship(point_t pos);
saucer* getSpaceship();
void updateSpaceship();
bool isSpaceshipHit();
void setSpaceshipHit(bool hit);

#endif /* SPACESHIPGLOBALS_H_ */
