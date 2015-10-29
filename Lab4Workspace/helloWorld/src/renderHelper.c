/*
 * renderHelper.c
 *
 *  Created on: Oct 8, 2015
 *      Author: superman
 */

#include "globals.h"
#include "render.h"
#include "tankGlobals.h"
#include "alienGlobals.h"
#include "spaceshipGlobals.h"
#include "playSound.h"


//Erase directions
#define DOWN 0
#define UP 1
#define LEFT 2
#define RIGHT 3

void updateAliens(){
	//Erase the aliens, update the block, and redraw
	render(true, alien_block_render_mask, 0, DOWN);
	updateAlienBlock();
	playAlienSound();
	render(false,alien_block_render_mask, 0, DOWN);
	return;
}

void fireAlienBulletHelper(){
	//Fire the alien bullet and draw the bullet
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

void killTankHelper(bool death1, bool reset){
	if(!reset){
		killTank(true,death1);
		killTank(false, death1);
	}
	//If we do want to reset it, draw the tank and reset it back to the starting position
	else {
		//Erase the tank before we move it to the beginning
		killTank(true,death1);
//		xil_printf("Printing the tank at the beginning\r\n");
		//Set the tank free so everything on the screen can move again
		setIsTankFree(true);
		//Reset the tank back to its starting point
		setTankPositionPoint(TANKSTARTX, TANKSTARTY);
		//Draw the tank again
		drawTank(false, RIGHT);
	}
	return;
}

void shootTankBullet(){
	//Fire the tank bullet and then draw it
	fireTankBullet();
	render(false, tank_bullet_render_mask, 0, UP);
	return;
}

void updateAllBullets(){
	//Erase all bullets, update their positions, and redraw them
	render(true, all_bullet_render_mask, 0, UP);
	updateBullets();
	render(false, all_bullet_render_mask, 0, UP);
	return;
}

void flySpaceship(){
	//Determine the direction, left or right
	int direction = rand()%2;
	if(getSpaceship()->isFree){
//		xil_printf("we are adding a saucer\r\n");
		//Set the spaceship and draw it
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
	//If the spaceship is not free, move it across the screen
	saucer *ship = getSpaceship();
	if(!ship->isFree){
		render(true, spaceship_render_mask, 0,ship->direction);
		updateSpaceship();
		render(false, spaceship_render_mask, 0, ship->direction);
		playSpaceshipSound();
	}
	setSpaceshipSoundPlaying(ship->isFree);
}

void eraseSpaceshipScore(bool erase){
	//Erase or draw the spaceship score
	printSpaceshipValue(getSpaceshipValue(), getOldSpaceshipLoc(), erase);
	return;
}

bool isSpaceshipHitHelper(){
	return isSpaceshipHit();
}

void setSpaceshipHitHelper(bool hit){
	//Used to start the timer for displaying the flashing spaceship score
	setSpaceshipHit(hit);
	return;
}
