/*
 * globals.c
 *
 *  Created on: Sep 25, 2015
 *      Author: superman
 */

#include "globals.h"
#include "render.h"
#include "bitmaps.h"
#include "tankGlobals.h"
#include "alienGlobals.h"
#include "spaceshipGlobals.h"
#include "playSound.h"

int lives = 3;
int score = 0;
point_t oldSpaceshipLocation;
int spaceshipScore = 0;

uint32_t bunkerStates[] = { 0, 0, 0, 0 };


void resetGlobals(bool newLevel){

	if(newLevel){
		lives++;
	}
	else{
		int i;
		for(i=0; i<4;i++)
			bunkerStates[i] = 0;
		score = 0;
		lives = 3;
	}
}

uint32_t getBunkerErosion(int bunker) {
	return bunkerStates[bunker];
}

void setBunkerErosion(int bunker, int block) {
	//Creating a mask to see if the corresponding block is completely eroded
	if (((bunkerStates[bunker] & (0x7 << (3 * block))) >> (3 * block)) >= 0x4) {}
//		xil_printf("Bunker %d, Block %d is already completely eroded!!!", bunker, block);
	else {
		//If it is not completely eroded, add a 1 to the state of the block
		bunkerStates[bunker] += (0x1 << (3 * block));
		// Draw erosion
		int mask = (bunker == 3) ? bunker_3_render_mask : (bunker == 2) ? bunker_2_render_mask : (bunker == 1) ? bunker_1_render_mask : bunker_0_render_mask;
		render(false, mask, block, 0);
	}
}

void updateBullets() {
	//Update the tank bullet first
	if (!tankBulletFree) {
		// Check to see if the bullet will collide with something
		int y = tankBulletPosition.y;
		int x = tankBulletPosition.x;
		//Get the location and color of the first different colored pixel
		point_t pix = getHitPixel(x, y, TANKBULLETWIDTH, tank_bullet_pixel_adjustment, false);
		int pix_color = activeFramePointer[pix.y*SCREENWIDTH + pix.x];
		if(pix_color){
			point_t alien_pos = getAlienBlockPosition();
			// What did it hit?
			// Was it a bunker?
			if (y >= BUNKERSTARTY) {
//				xil_printf("It was below the top of the bunker (bunker or bullet)\n");
				//Determine the bunker erosion 
				point_t bunk_blk = determineBunkerErosion(pix.x, pix.y);
				if(bunk_blk.x != -1 && bunk_blk.y != -1){
					// Erode bunker bunk_blk.x = bunker, bunk_blk.y = block
					setBunkerErosion(bunk_blk.x, bunk_blk.y);
					// Erase bullet
					render(true, tank_bullet_render_mask, 0, UP);
					// Move bullet so it will be reset, and made free
					tankBulletPosition.y = -(tank_bullet_height + 1);
				} else {
					tankBulletPosition.y -= tank_bullet_pixel_adjustment;
//					xil_printf("It was offscreen (or a bullet)\n");
				}

			}// Was it an alien?! If the pix_color was not green then its not a bullet
			else if (y >= alien_pos.y && pix_color != GREEN) {
//				xil_printf("We hit an alien...Pix_color: %d\n", pix_color);
				//Get the index of the alien
				int alien_col = (x - alien_pos.x) / (2 * (ALIENWIDTH + ALIENXSPACING));
				int alien_row = (y - alien_pos.y) / (ALIENHEIGHT + ALIENYSPACING);
				short alien_index = (short) ((alien_row * ALIENSPERROW) + alien_col);
				// Kill alien at alien_index
				setAlienDeaths(alien_index, true);
				// Increase score
				incScore(alien_index, false);
				// Erase alien
				// Draw Explosion
				point_t pos;
				pos.x = alien_pos.x + (alien_col * 2 * (ALIENWIDTH + ALIENXSPACING));
				pos.y = alien_pos.y + (alien_row * (ALIENHEIGHT + ALIENYSPACING)) - 2;
				drawBitmap(alien_explosion_12x10, pos, 12, 10, true, WHITE, false);
				//Play explosion sound
				playAlienExplosionSound();
				// Erase tank bullet
				render(true, tank_bullet_render_mask, 0, UP);
				// Move bullet so it will be reset, and made free
				tankBulletPosition.y = -(tank_bullet_height + 1);
			} else if (pix_color == RED) {
//				xil_printf("You hit the spaceship!!!\n");
				// Erase tank bullet
				render(true, tank_bullet_render_mask, 0, UP);
				tankBulletPosition.y = -(tank_bullet_height + 1);
				// Erase Spaceship
				saucer *spcShp = getSpaceship();
				drawBitmap(saucer_16x7, spcShp->pos, spaceship_width, spaceship_height, true, RED, true);
				oldSpaceshipLocation = spcShp->pos;
				incScore(-1, true);
				setSpaceshipHit(true);
				setSpaceshipSoundPlaying(false);
				playSaucerExplosionSound();

				spcShp->pos.x = bullet_offscreen, spcShp->pos.y = bullet_offscreen;
				spcShp->isFree = true;
			} else {
//				xil_printf("It was above the alien block, and not green...\n");
				tankBulletPosition.y -= tank_bullet_pixel_adjustment;
			}

		} else
			// Else just increment
			tankBulletPosition.y -= tank_bullet_pixel_adjustment;
	}
	//If the tank is off the screen, set it as so
	if (tankBulletPosition.y < -tank_bullet_height) {
		tankBulletPosition.x = bullet_offscreen;
		tankBulletPosition.y = bullet_offscreen;
		tankBulletFree = true;
	}
	//Update each of the alien bullets
	//Will change the bitmap used for each of them
	updateAlienBulletCounters();
	//Will update the position of each bullet
	int i;
	aBullet* bullet;
	for(i = 0; i < 4; i++){
		bullet = getAlienBullet(i);
		if (!bullet->isFree) {
			//aBullet0.pos.y += aBullet_pixel_adjustment;
			int x = bullet->pos.x;
			int y = bullet->pos.y+2*alien_bullet_height;
			// Check if y is even at bunkers
			if(y >= BUNKERSTARTY){
				ifBulletHitBunkers(bullet, x, y);
			}
			else{
				bullet->pos.y += aBullet_pixel_adjustment;
			}
		}
	}
}

void ifBulletHitBunkers(aBullet* bullet, int x, int y){
	point_t pix = getHitPixel(x, y, 2*BULLETWIDTH, aBullet_pixel_adjustment, true);
	point_t bunk_blk = determineBunkerErosion(pix.x,pix.y);
	// If bullet is at the ground or hit & eroded a bunker
	if (bullet->pos.y > green_line_y - (2 * alien_bullet_height)
		|| (bunk_blk.x != -1 && bunk_blk.y != -1)
		|| bulletHitTank(pix.x, pix.y)) {
		setBunkerErosion(bunk_blk.x, bunk_blk.y);
		// Remove the bullet
		eraseBullet(bullet->pos, bullet->type);
		bullet->pos.x = bullet_offscreen;
		bullet->pos.y = bullet_offscreen;
		bullet->isFree = true;
	}
	else{
		//Just increase the bullet position
		bullet->pos.y += aBullet_pixel_adjustment;
	}
	return;
}

 point_t getOldSpaceshipLoc(){
 	return oldSpaceshipLocation;
 }

bool bulletHitTank(int x, int y){
	point_t tank_pos = getTankPosition();
	if(x < tank_pos.x || y < TANKSTARTY || y > (2*TANKHEIGHT+TANKSTARTY))
		return false;
	else{
//		xil_printf("dead tank!\n");
		//The tank has been hit, call killTank Globals to kill the tank and start animations
		killTankGlobals();
		setSpaceshipSoundPlaying(false);
		// Play explosion sound
		playTankExplosionSound();
		return true;
	}
}

point_t determineBunkerErosion(int x, int y){
	point_t bunker_block = {-1,-1};
	if(x < BUNKERSTARTX || y < BUNKERSTARTY || y > (BUNKERSTARTY+2*BUNKERHEIGHT))
		return bunker_block;

	int left_check_pos = BUNKERSTARTX;
	int bunker = 0;
	bool off_screen = false;
	//Check to see if the bullet is in the bunker
	while ((x < left_check_pos || x > left_check_pos + 2 * BUNKERWIDTH) && !off_screen) {
		left_check_pos += 2 * (BUNKERXSPACING + BUNKERWIDTH);
		if (left_check_pos > SCREENWIDTH)
			off_screen = true;
		else
			bunker++;
	}
	// Now find which block it was
	if (!off_screen) {
//		xil_printf("It is not offscreen, Bunker: %d\n", bunker);
		int block_col = (x - left_check_pos) / (2 * BLOCKWIDTH);
		int block_row = (y - BUNKERSTARTY) / (2 * BLOCKHEIGHT);
		int block = (block_row * 4) + block_col;
		// Set to block 9 if it was greater than 9, since we
		// don't use 10 or 11
		block = (block > 9) ? 9 : block;
//		xil_printf("Block: %d\n", block);
		bunker_block.x = bunker;
		bunker_block.y = block;
	}
	return bunker_block;
}

/***********************************************************
 * Check every pixel in the rectangle bounded
 * by the top left (x,y) pair and width and height args
 * return the pixel that was not black or 0,0
 ***********************************************************
 */
point_t getHitPixel(int x, int y, int w, int h, bool alienBullet){

	int i;
	int j;
	point_t hit_coord = {0,0};
	if(alienBullet){//Alien bullets
		for(i=y;i<y+h;i++){
			for(j=x;j<x+w;j++){
				// If not black pixel found
				if(activeFramePointer[i*SCREENWIDTH + j]){
					// Set coordinates of the pixel
					hit_coord.x = j;
					hit_coord.y = i;
					//break out of loop
					j = (x+w);
					i = (y+h);
				}
			}
		}
	}
	else{// Tank bullets
		for(i=y;i>y-h;i--){
			for(j=x;j<x+w;j++){
				// If not black pixel found
				if(activeFramePointer[i*SCREENWIDTH + j]){
					//we found a pixel that is not black
					hit_coord.x = j;
					hit_coord.y = i;
					j = (x+w);
					i = (y-h);
				}
			}
		}
	}
	return hit_coord;
}

void setLives(bool increment) {
	//Increment or decrement the lives
	if (increment) {
		lives++;
	} else {
		lives--;
	}
	//Redraw the number of lives by erasing the last life
	eraseLife(lives);
}

int getLives() {
	return lives;
}

bool isGameOver() {
	return (getLiveAliens() != 0);
}

bool isLevelOver() {
	int i = 54;
	int row = 0;
	bool gameOver = false;
	//If the player has no lives left, the game is over
	if (lives == 0) {
		playTankExplosionSound();
		return true;
	}
	//If all of the aliens are dead, the game is over
	if(getLiveAliens() == 0){
		return true;
	}
	//If the aliens have reached the bottom of the bunker, the game is over
	bool* aDeaths = getAlienDeaths();
	while(gameOver == false) {
		//Keep going until we find the bottom alien
		if (aDeaths[i] == false) {
			gameOver = true;
		}
		if(i == 0){
			break;
		}
		else {
			i--;
		}
	}
	if ((i >= 44)) {
		row = 5;
	} else if ((i < 44) && (i >= 33)) {
		row = 4;
	} else if ((i < 33) && (i >= 22)) {
		row = 3;
	} else if ((i < 22) && (i >= 11)) {
		row = 2;
	} else {
		row = 1;
	}
	int spacing = alien_y_spacing - alien_height;
	//	xil_printf("This is the row of the last live alien: %d\r\n", row);
	//If the last live alien reaches the bottom of the bunkers, the game is over
	if ((getAlienBlockPosition().y + (row * alien_height * 2) + (row - 1) * (spacing)) >= (BUNKERSTARTY + BUNKERHEIGHT * 2)) {
//		xil_printf("Position: %d, bunker: %d\r\n",(alienBlockPosition.y + (row * alien_height * 2) + (row - 1) * (spacing)),(BUNKERSTARTY + BUNKERHEIGHT * 2));
//		xil_printf("The row is: %d\r\n", row);
		return true;
	}
	return false;
}

void incScore(int alienNum, bool isSpaceshipHit) {
	int oldScore = score;
	if(alienNum != -1){
		//Increment the score to reflect the value of the alien just killed
		//If the alien is in the bottom two rows, it is worth 10 pts
		if ((alienNum >= 0) && (alienNum < 11)) {
//			xil_printf("We are adding %d pts\r\n", top_row_pts);
			score += top_row_pts;
		} //If the alien is in the middle two rows, it is worth 20 pts
		else if ((alienNum >= 11) && (alienNum < 33)) {
//			xil_printf("We are adding %d pts\r\n", middle_row_pts);
			score += middle_row_pts;
		} //If the alien is in the top row, it is worth 40 pts
		else if ((alienNum >= 33)) {
//			xil_printf("We are adding %d pts\r\n", bottom_row_pts);
			score += bottom_row_pts;
		}
	}
	//add the value of the spaceship
	if (isSpaceshipHit) {
		spaceshipScore = (rand() % 6 + 1) * spaceship_multiple;
		//Print the value of the spaceship underneath the ship
		printSpaceshipValue(spaceshipScore, getOldSpaceshipLoc(), false);
//		xil_printf("The spaceship score is: %d\r\n", spaceScore);
		score += spaceshipScore;
//		xil_printf("Score: %d, spaceshipScore: %d\r\n", score, spaceshipScore);
	}
	int tempScore = score;
	int index = 0;
	//See if the score is going from 2 digits to 3 digits or 3 to 4 digits
	if((oldScore < 100 && tempScore > 99) || (oldScore < 1000 && tempScore > 999)){
		oldScore = oldScore * 10;
	}
//	xil_printf("Score: %d, oldScore: %d\r\n", score, oldScore);
	//Update the screen to reflect the new score
	//Update the first number?
	if ((score > 999)) {
//		xil_printf("The first number is %d \r\n",tempScore/1000);
		if ((oldScore / 1000 != tempScore / 1000)) {
			drawScore(index, tempScore / 1000, oldScore/1000);
		}
		index++;
	}
	oldScore = oldScore % 1000;
	tempScore = tempScore % 1000;
	if ((score > 99)) {
//		xil_printf("The second number is %d \r\n",tempScore/100);
		if (oldScore / 100 != tempScore / 100) {
			drawScore(index, tempScore / 100, oldScore/100);
		}
		index++;
		//update drawScore(index, number)
	} //Update the second number?
	oldScore = oldScore % 100;
	tempScore = tempScore % 100;
	if ((score > 9)) {
//		xil_printf("The third number is %d \r\n",tempScore/10);
		if ((oldScore / 10 != tempScore / 10)) {
			drawScore(index, tempScore / 10, oldScore/10);
		}
		index++;
	}
	oldScore = oldScore % 10;
	tempScore = tempScore % 10;
	//update the third number?
//	xil_printf("The last number is %d \r\n",tempScore);
	drawScore(index, tempScore, oldScore);
	//Draw the last number of the score

}

int getSpaceshipValue(){
 	return spaceshipScore;
 }

int getScore() {
	return score;
}

int getAlienUpdateTime() {
	//Return the alien update time based on how many aliens are still alive
	int liveAliens = getLiveAliens();
	if(liveAliens/11 >= 4) {	return 60;	}
	else if(liveAliens/11 == 3)	{ 	return 50;	}
	else if(liveAliens/11 == 2)	{	return 40;	}
	else if(liveAliens/11 == 1)	{	return 35;	}
	else if(liveAliens/11 == 0)	{	return 25;	}
	else return 0;
}
