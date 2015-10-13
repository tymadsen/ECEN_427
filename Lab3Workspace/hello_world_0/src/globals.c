/*
 * globals.c
 *
 *  Created on: Sep 25, 2015
 *      Author: superman
 */

#include "globals.h"
#include "render.h"

point_t tankPosition;
point_t tankBulletPosition;
bool tankBulletFree = true;
point_t alienBlockPosition;
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
bool alienOnLeftScreen = false;
int alien_block_width = 4*10 + 11*12*2;
int alienSpacing = 12*2+4;

//srand((unsigned)time(NULL));

//int t = rand();

uint32_t bunker0State = 0;
uint32_t bunker1State = 0;
uint32_t bunker2State = 0;
uint32_t bunker3State = 0;
bool alienDeaths[55] = {false, false, false, false, false, false, false, false, false, false, false,
						false, false, false, false, false, false, false, false, false, false, false,
						false, false, false, false, false, false, false, false, false, false, false,
						false, false, false, false, false, false, false, false, false, false, false,
						false, false, false, false, false, false, false, false, false, false, false};


point_t getTankPosition() {
	return tankPosition;
}

void setTankPosition(signed short pixels) {
	//Will move the tank left or right
	tankPosition.x += pixels;
	//These if statements will keep the tank on the screen
	if(tankPosition.x < 0){
		tankPosition.x = 0;
	}
	if(tankPosition.x + (tank_width*2) > 640){
		tankPosition.x = 640 - (tank_width*2);
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

void setTankBulletPositionXY(int x, int y){
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
	if(tankBulletFree) {
		temp.y = tankPosition.y - tank_bullet_height*2;
		temp.x = tankPosition.x + tank_width-1;
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
	int col = rand()%11;
	bool emptyCol = true;
	//Determine which alien it is coming from
	while(emptyCol) {
		idx = col + 44;
		if(!alienDeaths[idx] || !alienDeaths[idx-=11] || !alienDeaths[idx-=11] || !alienDeaths[idx-=11] || !alienDeaths[idx-=11]) {
			emptyCol = false;
		}
		else {
			col = rand()%11;
		}
	}
	//Set the right coordinates
	int row = 0;
	//Determine which row the alien is in
	if((idx >= 0) && (idx <11)) { row = 0;}
	else if((idx >= 11) && (idx < 22)) { row = 1; }
	else if((idx >= 22) && (idx < 33)) { row = 2; }
	else if((idx >= 33) && (idx < 44)) { row = 3; }
	else { row = 4; }
	//Calculate the coordinates based on the row and col
	temp.x = alienBlockPosition.x + (col*(2*alien_width)) + (col*2*alien_x_spacing) + (alien_width-alien_x_spacing-1);
	temp.y = alienBlockPosition.y + (row*(2*(alien_height))) + ((row)*alien_y_actual_spacing) + (alien_height*2);
	//Choose what type the bullet will be. 1 = squiggly, 0 = cross
	xil_printf("row: %d, idx: %d, col: %d\r\n", row, idx, col);
	xil_printf("These are the coordinates of the bullets: x- %d, y- %d\r\n", temp.x, temp.y);
	unsigned short bulletType = rand()%2;
	//Choose which bullet to place
	if(aBullet0.isFree) {
		setAlienBullet0(temp, bulletType, false, 0);
		drawAlienBullet(false, 0);
	}
	else if(aBullet1.isFree) {
		setAlienBullet1(temp, bulletType, false, 0);
		drawAlienBullet(false, 1);
	}
	else if(aBullet2.isFree) {
		setAlienBullet2(temp, bulletType, false, 0);
		drawAlienBullet(false, 2);
	}
	else if(aBullet3.isFree) {
		setAlienBullet3(temp, bulletType, false, 0);
		drawAlienBullet(false, 3);
	}
	else {
		//Do nothing because the maximum number of bullets are on the screen
	}
	return;
}

void updateAlienBulletCounters()
{
	//Increment each bullet counter, If the counter is equal to three (Maximum bitmaps), reset to 0
	aBullet0.counter += 1;
	if(aBullet0.counter >= 3) { aBullet0.counter = 0; }
	aBullet1.counter += 1;
	if(aBullet1.counter >= 3) { aBullet1.counter = 0; }
	aBullet2.counter += 1;
	if(aBullet2.counter >= 3) { aBullet2.counter = 0; }
	aBullet3.counter += 1;
	if(aBullet3.counter >= 3) { aBullet3.counter = 0; }
	return;
}

uint32_t getBunkerErosion0() {
	return bunker0State;
}
void setBunkerErosion0(short block) {
	//Creating a mask to see if the corresponding block is completely eroded
	if(((bunker0State & (0x7 << (3*block))) >> (3*block)) >= 0x4)
		xil_printf("Block %d selected to erode is already completely eroded!!!", block);
	else {
		//If it is not completely eroded, add a 1 to the state of the block
		bunker0State += (0x1 << (3*block));
	}
}
uint32_t getBunkerErosion1() {
	return bunker1State;
}
void setBunkerErosion1(short block) {
	//Creating a mask to see if the corresponding block is completely eroded
	if(((bunker1State & (0x7 << (3*block))) >> (3*block)) >= 0x4)
		xil_printf("Block %d selected to erode is already completely eroded!!!", block);
	else
		bunker1State += (0x1 << (3*block));
}
uint32_t getBunkerErosion2() {
	return bunker2State;
}
void setBunkerErosion2(short block) {
	//Creating a mask to see if the corresponding block is completely eroded
	if(((bunker2State & (0x7 << (3*block))) >> (3*block)) >= 0x4)
		xil_printf("Block %d selected to erode is already completely eroded!!!", block);
	else
		bunker2State += (0x1 << (3*block));
}
uint32_t getBunkerErosion3() {
	return bunker3State;
}
void setBunkerErosion3(short block) {
	//Creating a mask to see if the corresponding block is completely eroded
	if(((bunker3State & (0x7 << (3*block))) >> (3*block)) >= 0x4)
		xil_printf("Block %d selected to erode is already completely eroded!!!", block);
	else
		bunker3State += (0x1 << (3*block));
}

bool* getAlienDeaths() {
	return alienDeaths;
}

void setAlienDeaths(short alien, bool dead) {
	xil_printf("alien value: %d\r\n", alien);
	//If there is a valid alien in the block, set the corresponding value in the array to true (passed in)
	if(alien >= 0 && alien < 55) {
		alienDeaths[alien] = dead;
	}
}

void updateBullets(){
	//Update the tank bullet first
	if(!tankBulletFree) {
		tankBulletPosition.y -= pixel_adjustment;
	}
	if(tankBulletPosition.y < -tank_bullet_height) {
		tankBulletPosition.x = bullet_offscreen;
		tankBulletPosition.y = bullet_offscreen;
		tankBulletFree= true;
	}
	//Update each of the alien bullets
	//Will change the bitmap used for each of them
	updateAlienBulletCounters();
	//Will update the position of each bullet
	aBullet0.pos.y += pixel_adjustment;
	if(aBullet0.pos.y > green_line_y - (2*alien_bullet_height)) {
		aBullet0.pos.x = bullet_offscreen;
		aBullet0.pos.y = bullet_offscreen;
		aBullet0.isFree = true;
	}
	aBullet1.pos.y += pixel_adjustment;
	if(aBullet1.pos.y > green_line_y - (2*alien_bullet_height)) {
		aBullet1.pos.x = bullet_offscreen;
		aBullet1.pos.y = bullet_offscreen;
		aBullet1.isFree = true;
	}
	aBullet2.pos.y += pixel_adjustment;
	if(aBullet2.pos.y > green_line_y-(2*alien_bullet_height)) {
		aBullet2.pos.x = bullet_offscreen;
		aBullet2.pos.y = bullet_offscreen;
		aBullet2.isFree = true;
	}
	aBullet3.pos.y += pixel_adjustment;
	if(aBullet3.pos.y > green_line_y - (2*alien_bullet_height)) {
		aBullet3.pos.x = bullet_offscreen;
		aBullet3.pos.y = bullet_offscreen;
		aBullet3.isFree = true;
	}
}

void updateAlienBlock(){
	int rightOffset = 0;
	int leftOffset = 0;
	int row, col;
	int colWithLiveAlien = 0;
	bool alienAlive= false;
	//Determine how far the alien block can go left or right depending on which aliens are dead
	//Go through each column to check if the alien is dead
	for(col = 0; col < 11; col++){
		for(row = 0; row < 5; row++) {
			if(alienDeaths[(row*11)+col] == false){
				//If the alien is alive, set a flag
				alienAlive = true;
			}
		}
		//If there was a live alien, mark with a one in that bit
		if (alienAlive == true) {
			colWithLiveAlien = colWithLiveAlien | (1<<(10-col));
		}
		//Reset for the next column
		alienAlive = false;
	}
	//Determine the right side first
	//If the number anded with 1 is 0, there is no live alien in the column
	if((colWithLiveAlien & col11_mask) == 0){
		leftOffset += alienSpacing;
		if((colWithLiveAlien & col10_mask) == 0){
			leftOffset += alienSpacing;
			if((colWithLiveAlien & col9_mask) == 0) {
				leftOffset += alienSpacing;
				if((colWithLiveAlien & col8_mask) == 0) {
					leftOffset += alienSpacing;
					if((colWithLiveAlien & col7_mask) == 0) {
						leftOffset += alienSpacing;
						if((colWithLiveAlien & col6_mask) == 0) {
							leftOffset += alienSpacing;
							if((colWithLiveAlien & col5_mask) == 0) {
								leftOffset += alienSpacing;
								if((colWithLiveAlien & col4_mask) == 0) {
									leftOffset += alienSpacing;
									if((colWithLiveAlien & col3_mask) == 0) {
										leftOffset += alienSpacing;
										if((colWithLiveAlien & col2_mask) == 0) {
											leftOffset += alienSpacing;
											if((colWithLiveAlien & col1_mask) == 0) {
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
	//Now determine the left side
	if((colWithLiveAlien & col1_mask) == 0){
		rightOffset += alienSpacing;
			if((colWithLiveAlien & col2_mask) == 0){
				rightOffset += alienSpacing;
				if((colWithLiveAlien & col3_mask) == 0) {
					rightOffset += alienSpacing;
					if((colWithLiveAlien & col4_mask) == 0) {
						rightOffset += alienSpacing;
						if((colWithLiveAlien & col5_mask) == 0) {
							rightOffset += alienSpacing;
							if((colWithLiveAlien & col6_mask) == 0) {
								rightOffset += alienSpacing;
								if((colWithLiveAlien & col7_mask) == 0) {
									rightOffset += alienSpacing;
									if((colWithLiveAlien & col8_mask) == 0) {
										rightOffset += alienSpacing;
										if((colWithLiveAlien & col9_mask) == 0) {
											rightOffset += alienSpacing;
											if((colWithLiveAlien & col10_mask) == 0) {
												rightOffset += alienSpacing;
												if((colWithLiveAlien & col11_mask) == 0) {
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
	//If the alien is moving right, add pixels
	if(alienRight == true) {
		alienBlockPosition.x += pixel_adjustment;}
	//If the block is moving left, detract the pixels
	else {
		alienBlockPosition.x -= pixel_adjustment;}
	//If the block has hit the right side of the screen, set them equal to the screen and move them down
	if((alienBlockPosition.x + alien_block_width-rightOffset*2) > 640) {
		alienBlockPosition.x = 640-alien_block_width+rightOffset*2;
		alienBlockPosition.y += alien_height;
		//Make the aliens go left instead of right
		alienRight = false;
	}
	//Will move the alien block down a row
	else if(alienOnLeftScreen) {
		alienBlockPosition.y += alien_height;
		alienBlockPosition.x = -leftOffset*2;
		alienOnLeftScreen = false;
	}
	//If the block hits the left side of the string, set x equal to 0 and move the aliens down
	else if(alienBlockPosition.x+leftOffset*2 <= 0) {
		alienBlockPosition.x = -leftOffset*2;
		alienOnLeftScreen = true;
		//Make the aliens go right instead of left
		alienRight = true;
	}
	return;
}
