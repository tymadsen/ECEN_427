/*
 * globals.c
 *
 *  Created on: Sep 25, 2015
 *      Author: superman
 */

#include "globals.h"
#include "render.h"
#include "bitmaps.h"

point_t tankPosition;
point_t tankBulletPosition;
bool tankBulletFree = true;
point_t alienBlockPosition;
saucer spaceship;
aBullet aBullet0;
aBullet aBullet1;
aBullet aBullet2;
aBullet aBullet3;
//short aBullet0Free;
//short aBullet1Free;
//short aBullet2Free;
//short aBullet3Free;
//short aBulletType0;
//short aBulletType1;
//short aBulletType2;
//short aBulletType3;
bool alienRight = true;
bool alienDown = false;
bool alienOnLeftScreen = false;
int alien_block_width = 4 * 10 + 11 * alien_width * 2;
int alienSpacing = alien_width * 2 + alien_x_spacing*2;
int lives = 3;
int score = 0;
int spaceshipScore = 0;
point_t oldSpaceshipLocation;
bool spaceshipHit = false;

//srand((unsigned)time(NULL));

//int t = rand();

uint32_t bunkerStates[] = { 0, 0, 0, 0 };
//uint32_t bunker1State = 0;
//uint32_t bunker2State = 0;
//uint32_t bunker3State = 0;
int liveAliens = 55;
bool alienDeaths[55] = { false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false };

void setSpaceship(int direction) {
	if (spaceship.isFree) {
		if (direction == LEFT) {
			//If the direction of the saucer is left, we will start on the right side
			spaceship.pos.x = spaceship_right_x;
		} else {
			spaceship.pos.x = spaceship_left_x;
		}
		spaceship.pos.y = spaceship_y;
		spaceship.isFree = false;
		spaceship.direction = direction;
	}
}

void setInitialSpaceship(point_t pos) {
	spaceship.pos = pos;
	spaceship.isFree = true;
	//	drawBitmap(saucer_16x7, pos,spaceship_width, spaceship_height, true, BLACK, true);
	return;
}

saucer getSpaceship() {
	return spaceship;
}

void updateSpaceship() {
	if (!spaceship.isFree) {
		bool offscreen = false;
		if (spaceship.direction == LEFT) {
			//			xil_printf("The direction is left \r\n");
			//make the saucer go left by pixel_adjustment
			spaceship.pos.x -= pixel_adjustment;
			//Check to see if the saucer is off the screen yet
			if (spaceship.pos.x <= (0)) {
				offscreen = true;
			}
		}
		else {
			//			xil_printf("The direction is right\r\n");
			spaceship.pos.x += pixel_adjustment;
			if (spaceship.pos.x >= screen_width - spaceship_width * 2) {
				offscreen = true;
			}
		}
		if (offscreen) {
			//Erasing the last of the spaceship, the direction doesn't matter
			drawBitmap(saucer_16x7, spaceship.pos, spaceship_width, spaceship_height, true, RED, true);
			spaceship.pos.x = bullet_offscreen;
			spaceship.pos.y = bullet_offscreen;
			spaceship.isFree = true;
		}
	}
}

point_t getTankPosition() {
	return tankPosition;
}

bool getAlienDown() {
	return alienDown;
}

bool getAlienRight() {
	return alienRight;
}

void setTankPosition(signed short pixels) {
	//Will move the tank left or right
	tankPosition.x += pixels;
	//These if statements will keep the tank on the screen
	if (tankPosition.x < 0) {
		tankPosition.x = 0;
	}
	if (tankPosition.x + (tank_width * 2) > 640) {
		tankPosition.x = 640 - (tank_width * 2);
	}
	return;
}

void setTankPositionPoint(int x, int y) {
	tankPosition.x = x;
	tankPosition.y = y;
	return;
}

point_t getTankBulletPosition() {
	return tankBulletPosition;
}

void setTankBulletPositionXY(int x, int y) {
	tankBulletPosition.x = x;
	tankBulletPosition.y = y;
	return;
}

void setTankBulletPosition(point_t point) {
	tankBulletPosition = point;
	return;
}

void fireTankBullet() {
	point_t temp;
	//Determine where the tank bullet should appear
	//Will appear in the center top of the tank
	if (tankBulletFree) {
		temp.y = tankPosition.y - tank_bullet_height * 2;
		temp.x = tankPosition.x + tank_width - 1;
		tankBulletFree = false;
		setTankBulletPosition(temp);
	}
	return;
}

point_t getAlienBlockPosition() {
	return alienBlockPosition;
}

void setAlienBlockPosition(point_t point) {
	alienBlockPosition = point;
}

aBullet getAlienBullet0() {
	return aBullet0;
}
void setAlienBullet0(point_t point, unsigned short type, bool isFree, short counter) {
	aBullet0.pos = point;
	aBullet0.type = type;
	aBullet0.isFree = isFree;
	aBullet0.counter = counter;
}
aBullet getAlienBullet1() {
	return aBullet1;
}
void setAlienBullet1(point_t point, unsigned short type, bool isFree, short counter) {
	aBullet1.pos = point;
	aBullet1.type = type;
	aBullet1.isFree = isFree;
	aBullet1.counter = counter;
}
aBullet getAlienBullet2() {
	return aBullet2;
}
void setAlienBullet2(point_t point, unsigned short type, bool isFree, short counter) {
	aBullet2.pos = point;
	aBullet2.type = type;
	aBullet2.isFree = isFree;
	aBullet2.counter = counter;
}
aBullet getAlienBullet3() {
	return aBullet3;
}
void setAlienBullet3(point_t point, unsigned short type, bool isFree, short counter) {
	aBullet3.pos = point;
	aBullet3.type = type;
	aBullet3.isFree = isFree;
	aBullet3.counter = counter;
}

void fireAlienBullet() {
	point_t temp;
	int idx;
	//Pick which column we are firing from
	int col = rand() % 11;
	bool emptyCol = true;
	//Determine which alien it is coming from
	while (emptyCol) {
		idx = col + 44;
		if (!alienDeaths[idx] || !alienDeaths[idx -= 11] || !alienDeaths[idx-= 11] || !alienDeaths[idx -= 11] || !alienDeaths[idx -= 11]) {
			emptyCol = false;
		}
		else {
			col = rand() % 11;
		}
	}
	//Set the right coordinates
	int row = 0;
	//Determine which row the alien is in
	if ((idx >= 0) && (idx < 11)) {
		row = 0;
	} else if ((idx >= 11) && (idx < 22)) {
		row = 1;
	} else if ((idx >= 22) && (idx < 33)) {
		row = 2;
	} else if ((idx >= 33) && (idx < 44)) {
		row = 3;
	} else {
		row = 4;
	}
	//Calculate the coordinates based on the row and col
	temp.x = alienBlockPosition.x + (col * (2 * alien_width)) + (col * 2 * alien_x_spacing) + (alien_width - alien_x_spacing - 1);
	temp.y = alienBlockPosition.y + (row * (2 * (alien_height))) + ((row)*alien_y_actual_spacing) + (alien_height * 2);
	//Choose what type the bullet will be. 1 = squiggly, 0 = cross
	//	xil_printf("row: %d, idx: %d, col: %d\r\n", row, idx, col);
	//	xil_printf("These are the coordinates of the bullets: x- %d, y- %d\r\n", temp.x, temp.y);
	unsigned short bulletType = rand() % 2;
	//Choose which bullet to place
	if (aBullet0.isFree) {
		setAlienBullet0(temp, bulletType, false, 0);
		drawAlienBullet(false, 0);
	} else if (aBullet1.isFree) {
		setAlienBullet1(temp, bulletType, false, 0);
		drawAlienBullet(false, 1);
	} else if (aBullet2.isFree) {
		setAlienBullet2(temp, bulletType, false, 0);
		drawAlienBullet(false, 2);
	} else if (aBullet3.isFree) {
		setAlienBullet3(temp, bulletType, false, 0);
		drawAlienBullet(false, 3);
	} else {
		//Do nothing because the maximum number of bullets are on the screen
	}
	return;
}

void updateAlienBulletCounters() {
	//Increment each bullet counter, If the counter is equal to three (Maximum bitmaps), reset to 0
	aBullet0.counter += 1;
	if (aBullet0.counter >= 3) {
		aBullet0.counter = 0;
	}
	aBullet1.counter += 1;
	if (aBullet1.counter >= 3) {
		aBullet1.counter = 0;
	}
	aBullet2.counter += 1;
	if (aBullet2.counter >= 3) {
		aBullet2.counter = 0;
	}
	aBullet3.counter += 1;
	if (aBullet3.counter >= 3) {
		aBullet3.counter = 0;
	}
	return;
}

uint32_t getBunkerErosion(int bunker) {
	return bunkerStates[bunker];
}
void setBunkerErosion(int bunker, int block) {
	//Creating a mask to see if the corresponding block is completely eroded
	if (((bunkerStates[bunker] & (0x7 << (3 * block))) >> (3 * block)) >= 0x4)
		xil_printf("Bunker %d, Block %d is already completely eroded!!!", bunker, block);
	else {
		//If it is not completely eroded, add a 1 to the state of the block
		bunkerStates[bunker] += (0x1 << (3 * block));
	}
}
//uint32_t getBunkerErosion0() {
//	return bunker0State;
//}
//void setBunkerErosion0(short block) {
//	//Creating a mask to see if the corresponding block is completely eroded
//	if(((bunker0State & (0x7 << (3*block))) >> (3*block)) >= 0x4)
//		xil_printf("Block %d selected to erode is already completely eroded!!!", block);
//	else {
//		//If it is not completely eroded, add a 1 to the state of the block
//		bunker0State += (0x1 << (3*block));
//	}
//}
//uint32_t getBunkerErosion1() {
//	return bunker1State;
//}
//void setBunkerErosion1(short block) {
//	//Creating a mask to see if the corresponding block is completely eroded
//	if(((bunker1State & (0x7 << (3*block))) >> (3*block)) >= 0x4)
//		xil_printf("Block %d selected to erode is already completely eroded!!!", block);
//	else
//		bunker1State += (0x1 << (3*block));
//}
//uint32_t getBunkerErosion2() {
//	return bunker2State;
//}
//void setBunkerErosion2(short block) {
//	//Creating a mask to see if the corresponding block is completely eroded
//	if(((bunker2State & (0x7 << (3*block))) >> (3*block)) >= 0x4)
//		xil_printf("Block %d selected to erode is already completely eroded!!!", block);
//	else
//		bunker2State += (0x1 << (3*block));
//}
//uint32_t getBunkerErosion3() {
//	return bunker3State;
//}
//void setBunkerErosion3(short block) {
//	//Creating a mask to see if the corresponding block is completely eroded
//	if(((bunker3State & (0x7 << (3*block))) >> (3*block)) >= 0x4)
//		xil_printf("Block %d selected to erode is already completely eroded!!!", block);
//	else
//		bunker3State += (0x1 << (3*block));
//}
bool* getAlienDeaths() {
	return alienDeaths;
}

void setAlienDeaths(short alien, bool dead) {
	//If there is a valid alien in the block, set the corresponding value in the array to true (passed in)
	if (alien >= 0 && alien < 55) {
		alienDeaths[alien] = dead;
		liveAliens --;
	}
}

void updateBullets() {
	//Update the tank bullet first
	if (!tankBulletFree) {
		// Check to see if the bullet will collide with something
		int y = tankBulletPosition.y;
		int x = tankBulletPosition.x;

		int pix_color = getPixelColor(x,y,2,true);
		if(pix_color){
			point_t alien_pos = getAlienBlockPosition();
			// What did it hit?
			// Was it a bunker?
			if (y >= BUNKERSTARTY) {
				xil_printf("It was below the top of the bunker (bunker or bullet)\n");
				if(determineBunkerErosion(x,y)){//new function name)
					// Erase bullet
					render(true, tank_bullet_render_mask, 0, UP);
					// Move bullet so it will be reset, and made free
					tankBulletPosition.y = -(tank_bullet_height + 1);
				} else {
					tankBulletPosition.y -= tank_bullet_pixel_adjustment;
					xil_printf("It was offscreen (or a bullet)\n");
				}

			}// Was it an alien?! If the pix_color was not green then its not a bullet
			else if (y >= alien_pos.y && pix_color != GREEN) {
				xil_printf("We hit an alien...Pix_color: %d\n", pix_color);

				//#define ALIENHEIGHT 8
				//#define ALIENWIDTH 12
				//#define ALIENXSPACING 2
				//#define ALIENYSPACING 20
				//#define ALIENBLOCKSTARTX 167
				//#define ALIENBLOCKSTARTY 75
				//#define ALIENSPERROW 11
				//				left_check_pos = alien_pos.x;
				//				xil_printf("Params: x:%d\ny:%d\n,alien_blk_x:%d\n,alien_blk_y:%d\nAlienWidth:%d\nAlienHeight:%d\nXspc:%d\nYspc:%d\n",x,y,alien_pos.x,alien_pos.y,ALIENWIDTH,ALIENHEIGHT,ALIENXSPACING,ALIENYSPACING);
				int alien_col = (x - alien_pos.x) / (2 * (ALIENWIDTH + ALIENXSPACING));
				int alien_row = (y - alien_pos.y) / (ALIENHEIGHT + ALIENYSPACING);
				short alien_index = (short) ((alien_row * ALIENSPERROW) + alien_col);
				//				xil_printf("alien col: %d\n", alien_col);
				//				xil_printf("alien row: %d\n", alien_row);
				//				xil_printf("Killing alien: %d\n", alien_index);
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
				// Erase tank bullet
				render(true, tank_bullet_render_mask, 0, UP);
				// Move bullet so it will be reset, and made free
				tankBulletPosition.y = -(tank_bullet_height + 1);
			} else if (pix_color == RED) {
				xil_printf("You hit the spaceship!!!\n");
				// Erase tank bullet
				render(true, tank_bullet_render_mask, 0, UP);
				tankBulletPosition.y = -(tank_bullet_height + 1);
				// Erase Spaceship
				drawBitmap(saucer_16x7, getSpaceship().pos, spaceship_width, spaceship_height, true, RED, true);
				oldSpaceshipLocation = spaceship.pos;
				incScore(-1, true);
				setSpaceshipHit(true);
				//TODO: Show score with spaceship.pos
				spaceship.pos.x = bullet_offscreen, spaceship.pos.y = bullet_offscreen;
				spaceship.isFree = true;
			} else {
				xil_printf("It was above the alien block, and not green...\n");
				tankBulletPosition.y -= tank_bullet_pixel_adjustment;
			}

		} else
			// Else just increment
			tankBulletPosition.y -= tank_bullet_pixel_adjustment;
	}
	if (tankBulletPosition.y < -tank_bullet_height) {
		tankBulletPosition.x = bullet_offscreen;
		tankBulletPosition.y = bullet_offscreen;
		tankBulletFree = true;
	}
	//Update each of the alien bullets
	//Will change the bitmap used for each of them
	updateAlienBulletCounters();
	//Will update the position of each bullet
	if (!aBullet0.isFree) {
		aBullet0.pos.y += pixel_adjustment;
		int x = aBullet0.pos.x;
		int y = aBullet0.pos.y;
		// Check if y is even at bunkers
		if(y >= BUNKERSTARTY){
			// If bullet is at the ground or hit & eroded a bunker
			if ((aBullet0.pos.y > green_line_y - (2 * alien_bullet_height))
				|| (getPixelColor(x,y,3,false) && determineBunkerErosion(x,y))) {
				// Remove the bullet
				eraseBullet(aBullet0.pos, aBullet0.type);
				aBullet0.pos.x = bullet_offscreen;
				aBullet0.pos.y = bullet_offscreen;
				aBullet0.isFree = true;
			}
		}
	}
	if (!aBullet1.isFree) {
		aBullet1.pos.y += pixel_adjustment;
		int x = aBullet1.pos.x;
		int y = aBullet1.pos.y;
		// Check if y is even at bunkers
		if(y >= BUNKERSTARTY){
			// If bullet is at the ground or hit & eroded a bunker
			if ((aBullet1.pos.y > green_line_y - (2 * alien_bullet_height))
				|| (getPixelColor(x,y,3,false) && determineBunkerErosion(x,y))) {
				// Remove the bullet
				eraseBullet(aBullet1.pos, aBullet1.type);
				aBullet1.pos.x = bullet_offscreen;
				aBullet1.pos.y = bullet_offscreen;
				aBullet1.isFree = true;
			}
		}
	}
	if (!aBullet2.isFree) {
		aBullet2.pos.y += pixel_adjustment;
		int x = aBullet2.pos.x;
		int y = aBullet2.pos.y;
		// Check if y is even at bunkers
		if(y >= BUNKERSTARTY){
			// If bullet is at the ground or hit & eroded a bunker
			if ((aBullet2.pos.y > green_line_y - (2 * alien_bullet_height))
				|| (getPixelColor(x,y,3,false) && determineBunkerErosion(x,y))) {
				// Remove the bullet
				eraseBullet(aBullet2.pos, aBullet2.type);
				aBullet2.pos.x = bullet_offscreen;
				aBullet2.pos.y = bullet_offscreen;
				aBullet2.isFree = true;
			}
		}
	}
	if (!aBullet3.isFree) {
		aBullet3.pos.y += pixel_adjustment;
		int x = aBullet3.pos.x;
		int y = aBullet3.pos.y;
		// Check if y is even at bunkers
		if(y >= BUNKERSTARTY){
			// If bullet is at the ground or hit & eroded a bunker
			if ((aBullet3.pos.y > green_line_y - (2 * alien_bullet_height))
				|| (getPixelColor(x,y,3,false) && determineBunkerErosion(x,y))) {
				// Remove the bullet
				eraseBullet(aBullet3.pos, aBullet3.type);
				aBullet3.pos.x = bullet_offscreen;
				aBullet3.pos.y = bullet_offscreen;
				aBullet3.isFree = true;
			}
		}
	}
}

bool isSpaceshipHit(){
	return spaceshipHit;
}

void setSpaceshipHit(bool hit){
	spaceshipHit = hit;
	return;
}

point_t getOldSpaceshipLoc(){
	return oldSpaceshipLocation;
}

void eraseBullet(point_t pos, unsigned short type) {
	drawBitmap(alien_bullet_11_3x7, pos, alien_bullet_width, alien_bullet_height, true, GREEN, true);
}

bool determineBunkerErosion(int x, int y){

	int left_check_pos = BUNKERSTARTX;
	int bunker = 0;
	bool off_screen = false;
	while ((x < left_check_pos || x > left_check_pos + 2 * BUNKERWIDTH) && !off_screen) {
		left_check_pos += 2 * (BUNKERXSPACING + BUNKERWIDTH);
		if (left_check_pos > SCREENWIDTH)
			off_screen = true;
		else
			bunker++;
	}
	// Now find which block it was
	if (!off_screen) {
		xil_printf("It is not offscreen, Bunker: %d\n", bunker);
		int block_col = (x - left_check_pos) / (2 * BLOCKWIDTH);
		int block_row = (y - BUNKERSTARTY) / (2 * BLOCKHEIGHT);
		int block = (block_row * 4) + block_col;
		// Set to block 9 if it was greater than 9, since we
		// don't use 10 or 11
		block = (block > 9) ? 9 : block;
		xil_printf("Block: %d\n", block);
		setBunkerErosion(bunker, block);
		// Draw erosion
		int mask = (bunker == 3) ? bunker_3_render_mask : (bunker == 2) ? bunker_2_render_mask : (bunker == 1) ? bunker_1_render_mask : bunker_0_render_mask;
		render(false, mask, block, 0);
	}
	return !off_screen;
}

int getPixelColor(int x, int y, int w, bool above){
	// Get 2 pixels (above the top/below the bottom) of the current x,y position and right of those (4 pixels total)
	// Set color to the color of the first non-black thing it will go over, (if none, then it will be black)
	int pix_color = BLACK;
	int op = above ? -1 : 1;
//	xil_printf("op: %d\n",op);
	// did it hit something?
	if ((pix_color = activeFramePointer[(y + op*1) * SCREENWIDTH + x])
			|| (pix_color = activeFramePointer[(y + op*1) * SCREENWIDTH + x + op*w])
			|| (pix_color = activeFramePointer[(y + op*2) * SCREENWIDTH + x])
			|| (pix_color = activeFramePointer[(y + op*2) * SCREENWIDTH + x + op*w])) {
//		xil_printf("We hit something.. Pix_color: %x\n",pix_color);
	}
	return pix_color;
}

void updateAlienBlock() {
	int rightOffset = 0;
	int leftOffset = 0;
	int row, col;
	int colWithLiveAlien = 0;
	bool alienAlive = false;
	//Determine how far the alien block can go left or right depending on which aliens are dead
	//Go through each column to check if the alien is dead
	for (col = 0; col < 11; col++) {
		for (row = 0; row < 5; row++) {
			if (alienDeaths[(row * 11) + col] == false) {
//				xil_printf("index of live alien: %d\r\n", ((row*11)+col));
				//If the alien is alive, set a flag
				alienAlive = true;
			}
		}
		//If there was a live alien, mark with a one in that bit
		if (alienAlive == true) {
			colWithLiveAlien = colWithLiveAlien | (1 << (10 - col));
		}
		//Reset for the next column
		alienAlive = false;
	}
	// xil_printf("colWithLiveAlien: %x\n\r", colWithLiveAlien);
	//Determine the right side first
	//If the number anded with 1 is 0, there is no live alien in the column
	if ((colWithLiveAlien & col11_mask) == 0) {
		leftOffset += alienSpacing;
		if ((colWithLiveAlien & col10_mask) == 0) {
			leftOffset += alienSpacing;
			if ((colWithLiveAlien & col9_mask) == 0) {
				leftOffset += alienSpacing;
				if ((colWithLiveAlien & col8_mask) == 0) {
					leftOffset += alienSpacing;
					if ((colWithLiveAlien & col7_mask) == 0) {
						leftOffset += alienSpacing;
						if ((colWithLiveAlien & col6_mask) == 0) {
							leftOffset += alienSpacing;
							if ((colWithLiveAlien & col5_mask) == 0) {
								leftOffset += alienSpacing;
								if ((colWithLiveAlien & col4_mask) == 0) {
									leftOffset += alienSpacing;
									if ((colWithLiveAlien & col3_mask) == 0) {
										leftOffset += alienSpacing;
										if ((colWithLiveAlien & col2_mask) == 0) {
											leftOffset += alienSpacing;
											if ((colWithLiveAlien & col1_mask) == 0) {
												leftOffset += alienSpacing;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	//Now determine the right side
	if ((colWithLiveAlien & col1_mask) == 0) {
		rightOffset += alienSpacing;
		if ((colWithLiveAlien & col2_mask) == 0) {
			rightOffset += alienSpacing;
			if ((colWithLiveAlien & col3_mask) == 0) {
				rightOffset += alienSpacing;
				if ((colWithLiveAlien & col4_mask) == 0) {
					rightOffset += alienSpacing;
					if ((colWithLiveAlien & col5_mask) == 0) {
						rightOffset += alienSpacing;
						if ((colWithLiveAlien & col6_mask) == 0) {
							rightOffset += alienSpacing;
							if ((colWithLiveAlien & col7_mask) == 0) {
								rightOffset += alienSpacing;
								if ((colWithLiveAlien & col8_mask) == 0) {
									rightOffset += alienSpacing;
									if ((colWithLiveAlien & col9_mask) == 0) {
										rightOffset += alienSpacing;
										if ((colWithLiveAlien & col10_mask) == 0) {
											rightOffset += alienSpacing;
											if ((colWithLiveAlien & col11_mask) == 0) {
												rightOffset += alienSpacing;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	rightOffset -= alien_x_spacing*2;
	//If the alien is moving right, add pixels
	if (alienRight == true) {
		alienDown = false;
		alienBlockPosition.x += pixel_adjustment;
	}
	//If the block is moving left, detract the pixels
	else {
		alienDown = false;
		alienBlockPosition.x -= pixel_adjustment;
	}
	//If the block has hit the right side of the screen, set them equal to the screen and move them down
	if ((alienBlockPosition.x + alien_block_width - rightOffset) > 640) {
		alienDown = true;
		//call the render function
		render(true, alien_block_render_mask, 0, DOWN);
		alienBlockPosition.x = 640 - alien_block_width + rightOffset;
		alienBlockPosition.y += alien_height;
		//Make the aliens go left instead of right
		alienRight = false;
	}
	//Will move the alien block down a row
	else if (alienOnLeftScreen) {
		alienBlockPosition.x = -leftOffset;
		alienBlockPosition.y += alien_height;
		alienOnLeftScreen = false;
		alienDown = true;
	}
	//If the block hits the left side of the string, set x equal to 0 and move the aliens down
	else if (alienBlockPosition.x + leftOffset <= 0) {
		alienBlockPosition.x = -leftOffset;
		alienOnLeftScreen = true;
		//Make the aliens go right instead of left
		alienRight = true;
		alienDown = true;
	}
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
	int i;
	int row = 0;
	bool gameOver = false;
	//If the player has no lives left, the game is over
	if (lives == 0) {
		return true;
	}
	//If all of the aliens are dead, the game is over
	for (i = 55; i < 0; i--) {
		if (alienDeaths[i - 1] == false) {
			gameOver = false;
		}
	}
	i--;
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
	//	xil_printf("This is the row of the last live alien: %d\r\n", row);
	//If the last live alien reaches the bottom of the bunkers, the game is over
	if ((alienBlockPosition.y + (row * alien_height * 2) + (row - 1) * (alienSpacing)) >= (BUNKERSTARTY + BUNKERHEIGHT * 2)) {
		gameOver = true;
	}
	return gameOver;
}

void incScore(int alienNum, bool isSpaceshipHit) {
	//alienNum of -1 means just the spaceship is hit
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
	}
	int tempScore = score;
	int index = 0;
	//Update the screen to reflect the new score
	//Update the first number?
	if ((score > 999)) {
//		xil_printf("The first number is %d \r\n",tempScore/1000);
		if ((oldScore / 1000 != tempScore / 1000)) {
			drawScore(index, tempScore / 1000);
		}
		index++;
	}
	oldScore = oldScore % 1000;
	tempScore = tempScore % 1000;
	if ((score > 99)) {
//		xil_printf("The second number is %d \r\n",tempScore/100);
		if (oldScore / 100 != tempScore / 100) {
			drawScore(index, tempScore / 100);
		}
		index++;
		//update drawScore(index, number)
	} //Update the second number?
	oldScore = oldScore % 100;
	tempScore = tempScore % 100;
	if ((score > 9)) {
//		xil_printf("The third number is %d \r\n",tempScore/10);
		if ((oldScore / 10 != tempScore / 10)) {
			drawScore(index, tempScore / 10);
		}
		index++;
	}
	oldScore = oldScore % 10;
	tempScore = tempScore % 10;
	//update the third number?
//	xil_printf("The last number is %d \r\n",tempScore);
	drawScore(index, tempScore);
	//Draw the last number of the score

}

int getSpaceshipValue(){
	return spaceshipScore;
}

int getScore() {
	return score;
}

int getAlienUpdateTime() {

	if(liveAliens/11 >= 4) {	return 60;	}
	else if(liveAliens/11 == 3)	{ 	return 55;	}
	else if(liveAliens/11 == 2)	{	return 50;	}
	else if(liveAliens/11 == 1)	{	return 40;	}
	else if(liveAliens/11 == 0)	{	return 30;	}
	else return 0;
}
