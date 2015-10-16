#include "globals.h"
#include "render.h"
#include "bitmaps.h"

point_t alienBlockPosition;
aBullet aBullet0;
aBullet aBullet1;
aBullet aBullet2;
aBullet aBullet3;
bool alienRight = true;
bool alienDown = false;
bool alienOnLeftScreen = false;
int alien_block_width = 4 * 10 + 11 * alien_width * 2;
int alienSpacing = alien_width * 2 + alien_x_spacing*2;
int liveAliens = 55;
bool alienDeaths[55] = { false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false };


bool getAlienDown() {
	return alienDown;
}

bool getAlienRight() {
	return alienRight;
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
		alienBlockPosition.y += (alien_height*3)/2;
		//Make the aliens go left instead of right
		alienRight = false;
	}
	//Will move the alien block down a row
	else if (alienOnLeftScreen) {
		alienBlockPosition.x = -leftOffset;
		alienBlockPosition.y += (alien_height*3)/2;
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

void eraseBullet(point_t pos, unsigned short type) {
	drawBitmap(alien_bullet_11_3x7, pos, alien_bullet_width, alien_bullet_height, true, GREEN, true);
}