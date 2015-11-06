
#include "tankGlobals.h"

point_t tankPosition;
point_t tankBulletPosition;
bool tankBulletFree = true;
bool tankFree = true;
bool tankHit = true;

bool isTankBulletFree(){
	return tankBulletFree;
}

bool isTankFree(){
	return tankFree;
}

void setIsTankFree(bool free){
	tankFree = free;
}

bool isTankHit(){
	return tankHit;
}

void setIsTankHit(bool hit){
	tankHit = hit;
}

void killTankGlobals(){
	// draw the killed tank with death 2 first
	activeFramePointer = background;
	drawBitmap(tank_15x8, getTankPosition(), TANKWIDTH, TANKHEIGHT, true, BLACK, false);
	activeFramePointer = foreground;
	drawBitmap(tank_15x8, getTankPosition(), TANKWIDTH, TANKHEIGHT, true, BLACK, false);
	// Erase all of the bullets and spaceship
	int i;
	aBullet* bullet;
	for(i = 0; i < 4; i++){
		bullet = getAlienBullet(i);
		eraseBullet(bullet->pos, bullet->type);
	}
	drawBitmap(saucer_16x7, getSpaceship()->pos, spaceship_width, spaceship_height, true, GREEN, true);
	drawTankBullet(true);
	// Take all of the bullets off the screen as well as the spaceship
	point_t tempOffScreen;
	tempOffScreen.x = bullet_offscreen; tempOffScreen.y = bullet_offscreen;
	setTankBulletPositionXY(bullet_offscreen, bullet_offscreen);
	setAlienBullet0(tempOffScreen, 0, true,0);
	setAlienBullet1(tempOffScreen, 0, true,0);
	setAlienBullet2(tempOffScreen, 0, true,0);
	setAlienBullet3(tempOffScreen, 0, true,0);
	setInitialSpaceship(tempOffScreen);
	// Decrement the lives
	killTank(false, false);
	// Set everything so we can kill the tank
	tankFree = false;
	tankHit = true;
	tankBulletFree = true;
	setLives(false);
	
}

point_t getTankPosition() {
	return tankPosition;
}

void setTankPosition(signed short pixels) {
	// Will move the tank left or right
	tankPosition.x += pixels;
	// These if statements will keep the tank on the screen
	if (tankPosition.x < 0) {
		tankPosition.x = 0;
	}
	if (tankPosition.x + (tank_width * 2) > 640) {
		tankPosition.x = 640 - (tank_width * 2);
	}
	
}

void setTankPositionPoint(int x, int y) {
	tankPosition.x = x;
	tankPosition.y = y;
	
}

point_t getTankBulletPosition() {
	return tankBulletPosition;
}

void setTankBulletPositionXY(int x, int y) {
	tankBulletPosition.x = x;
	tankBulletPosition.y = y;
	
}

void setTankBulletPosition(point_t point) {
	tankBulletPosition = point;
	
}

void fireTankBullet() {
	point_t temp;
	// Determine where the tank bullet should appear
	// Will appear in the center top of the tank
	if (tankBulletFree) {
		temp.y = tankPosition.y - tank_bullet_height * 2;
		temp.x = tankPosition.x + tank_width - 1;
		tankBulletFree = false;
		setTankBulletPosition(temp);
		playTankBullet();
	}
	
}
