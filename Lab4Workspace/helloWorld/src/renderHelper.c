/*
 * renderHelper.c
 *
 *  Created on: Oct 8, 2015
 *      Author: superman
 */

#include "globals.h"
#include "render.h"


//Erase directions
#define DOWN 0
#define UP 1
#define LEFT 2
#define RIGHT 3

void updateAliens(){
	render(true, alien_block_render_mask, 0, DOWN);
	updateAlienBlock();
	render(false,alien_block_render_mask, 0, DOWN);
	return;
}

void fireAlienBulletHelper(){
	fireAlienBullet();
	render(false, alien_bullet_render_mask,0, DOWN);
	return;
}

void moveTankLeft(){
	render(true, tank_render_mask, 0, LEFT);
	//change position
	setTankPosition(-pixel_adjustment);
	//render(draw)
	render(false, tank_render_mask, 0, LEFT);
	return;
}

void moveTankRight(){
	render(true, tank_render_mask, 0, RIGHT);
	//change position
	setTankPosition(pixel_adjustment);
	//render(draw)
	render(false, tank_render_mask, 0, RIGHT);
	return;
}

void shootTankBullet(){
	fireTankBullet();
	render(false, tank_bullet_render_mask, 0, UP);
	return;
}

void updateAllBullets(){
	render(true, all_bullet_render_mask, 0, UP);
	updateBullets();
	render(false, all_bullet_render_mask, 0, UP);
	return;
}

void flySpaceship(){
	int direction = rand()%2;
	if(getSpaceship().isFree){
//		xil_printf("we are adding a saucer\r\n");
		if(direction == 0){
			setSpaceship(LEFT);
			render(false, spaceship_render_mask, 0, LEFT);
		}
		else{
			setSpaceship(RIGHT);
			render(false, spaceship_render_mask, 0, RIGHT);
		}
	}
}

void updateSpaceshipHelper(){
	if(!getSpaceship().isFree){
		render(true, spaceship_render_mask, 0, getSpaceship().direction);
		updateSpaceship();
		render(false, spaceship_render_mask, 0, getSpaceship().direction);
	}
}

void eraseSpaceshipScore(bool erase){
	printSpaceshipValue(getSpaceshipValue(), getOldSpaceshipLoc(), erase);
	return;
}

bool isSpaceshipHitHelper(){
	return isSpaceshipHit();
}

void setSpaceshipHitHelper(bool hit){
	setSpaceshipHit(hit);
	return;
}