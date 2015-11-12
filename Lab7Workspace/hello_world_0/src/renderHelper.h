/*
 * renderHelper.h
 *
 *  Created on: Oct 8, 2015
 *      Author: superman
 */

#ifndef RENDERHELPER_H_
#define RENDERHELPER_H_


void updateAliens();
void moveTankLeft();
void moveTankRight();
void killTankHelper(bool death1, bool reset);
void shootTankBullet();
void updateAllBullets();
void fireAlienBulletHelper();
void flySpaceship();
void updateSpaceshipHelper();
void eraseSpaceshipScore(bool erase);
bool isSpaceshipHitHelper();
void setSpaceshipHitHelper(bool hit);

#endif /* RENDERHELPER_H_ */
