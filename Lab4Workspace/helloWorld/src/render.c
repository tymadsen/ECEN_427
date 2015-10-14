/*
 * render.c
 *
 *  Created on: Sep 25, 2015
 *      Author: superman
 */
#include <stdio.h>
#include "render.h"
#include "bitMaps.h"
#include "globals.h"

unsigned int * foreground = (unsigned int *) FRAME_BUFFER_0_ADDR;
unsigned int * background = ((unsigned int *) FRAME_BUFFER_0_ADDR) + SCREENWIDTH*SCREENHEIGHT;
unsigned int * activeFramePointer = (unsigned int *) FRAME_BUFFER_0_ADDR;
bool alien_in = true;
void clearScreen() {
	int row, col;
	//Clear the screen by initializing every pixel to black (0)
	for(row=0; row<SCREENHEIGHT; row++) {
		 for(col=0; col<SCREENWIDTH; col++) {
			 foreground[row*SCREENWIDTH+col] = 0;
			 background[row*SCREENWIDTH+col] = 0;
		 }
	}
	return;
}

void initScreen() {
	clearScreen();
	xil_printf("Screen cleared.\r\n");
	int row;
	int col;
	//Draws the green line at the bottom of the frame
	for(row = SCREENHEIGHT-30; row < SCREENHEIGHT-28; row++) {
		for(col = 0; col < SCREENWIDTH; col++){
			foreground[row*SCREENWIDTH + col] = GREEN;
			background[row*SCREENWIDTH + col] = GREEN;
		}
	}
	point_t tempOffScreen;
	tempOffScreen.x = bullet_offscreen; tempOffScreen.y = bullet_offscreen;
	// Write the score label on top of the frame
	drawScoreLabel();
	// Draw score
	drawScore(0,0);
	// Draw the lives label
	drawLivesLabel();
	// Draw the lives
	drawLives();
	setInitialSpaceship(tempOffScreen);
	// Draw the bunkers
	drawNewBunkers();

	// Move to background
	activeFramePointer = background;
	// Write the score and label in background
	drawScoreLabel();
	drawScore(0,0);
	// Draw the lives and label in background
	drawLivesLabel();
	drawLives();
	// Draw background bunkers
	drawNewBunkers();
	// Switch back to foreground
	activeFramePointer = foreground;

	//set and draw the aliens
	setTankPositionPoint(TANKSTARTX, TANKSTARTY);
	drawTank(false, RIGHT);
	setTankBulletPositionXY(bullet_offscreen, bullet_offscreen);
	setAlienBullet0(tempOffScreen, 0, true,0);
	setAlienBullet1(tempOffScreen, 0, true,0);
	setAlienBullet2(tempOffScreen, 0, true,0);
	setAlienBullet3(tempOffScreen, 0, true,0);
	aBullet temp = getAlienBullet0();
//	xil_printf("bullet1: x:%d, y:%d, type:%d, isFree:%d\r\n", temp.pos.x, temp.pos.y, temp.type, temp.isFree);
	temp = getAlienBullet1();
//	xil_printf("bullet2: x:%d, y:%d, type:%d, isFree:%d\r\n", temp.pos.x, temp.pos.y, temp.type, temp.isFree);
	temp = getAlienBullet2();
//	xil_printf("bullet3: x:%d, y:%d, type:%d, isFree:%d\r\n", temp.pos.x, temp.pos.y, temp.type, temp.isFree);
	temp = getAlienBullet3();
//	xil_printf("bullet4: x:%d, y:%d, type:%d, isFree:%d\r\n", temp.pos.x, temp.pos.y, temp.type, temp.isFree);
	point_t aBP;
	aBP.x = ALIENBLOCKSTARTX, aBP.y = ALIENBLOCKSTARTY;
	setAlienBlockPosition(aBP);
	drawAliens(false, true);

	return;
}

void render(bool erase, int render_objects_mask, short index, int direction) {
//	xil_printf("Objects mask: %d\r\n", render_objects_mask);
//	xil_printf("anded: %d\r\n",(render_objects_mask & alien_block_render_mask));
	//If a certain mask is set, render that object
	if((render_objects_mask & tank_render_mask) != 0)
		drawTank(erase, direction);
	if((render_objects_mask & all_bullet_render_mask) != 0) {
		drawTankBullet(erase);
		drawAlienBullet(erase, 0);
		drawAlienBullet(erase, 1);
		drawAlienBullet(erase, 2);
		drawAlienBullet(erase, 3);
	}
	if((render_objects_mask & tank_bullet_render_mask) != 0)
		drawTankBullet(erase);
	if((render_objects_mask & alien_block_render_mask) != 0){
		drawAliens(erase, alien_in);
		if(!erase) {
			alien_in = !alien_in; }
	}
	if((render_objects_mask & alien_bullet_render_mask) != 0){
//		xil_printf("We are drawing an alien bullet\r\n");
		drawAlienBullet(erase, index);
	}
	if((render_objects_mask & bunker_0_render_mask) != 0)
		drawBunkerErosion(0, index);
	if((render_objects_mask & bunker_1_render_mask) != 0)
		drawBunkerErosion(1, index);
	if((render_objects_mask & bunker_2_render_mask) != 0)
		drawBunkerErosion(2, index);
	if((render_objects_mask & bunker_3_render_mask) != 0)
		drawBunkerErosion(3, index);
	if((render_objects_mask & spaceship_render_mask) != 0)
		drawSpaceship(erase, direction);
}

void drawScoreLabel() {
	point_t score_label_pos;
	score_label_pos.x = SCORELABELX, score_label_pos.y = SCORELABELY;
	drawBitmap(word_score_30x5, score_label_pos, SCORELABELWIDTH, LABELHEIGHT, true, WHITE, false);
}

void drawScore(int index, int number) {
	point_t score_pos;
	//Put the starting position in the right place depending on the index
	score_pos.x = SCOREX + (index*NUMBERWIDTH*2) + (index*NUMBERSPACING);
	score_pos.y = SCOREY;
	//Get the bitmap for the number we are going to show
	const uint32_t* bitmap = getNumberBitmap(number);
	//Draw it in the background first.
	activeFramePointer = background;
	drawBitmap(bitmap, score_pos, NUMBERWIDTH, NUMBERHEIGHT, true, GREEN, false);
	//Now draw it in the foreground
	activeFramePointer = foreground;
	drawBitmap(bitmap, score_pos, NUMBERWIDTH, NUMBERHEIGHT, true, GREEN, false);
}

void printSpaceshipValue(int spaceshipValue, point_t position, bool erase){
//	xil_printf("We are printing the score at %d, %d\r\n", position.x, position.y);
	int index = spaceshipValue;
	const uint32_t* bitmap;
	//Draw the 100s digit if our value is over 99
	if(index > 99){
		bitmap = getNumberBitmap(index/100);
//		xil_printf("We are printing %d at %d, %d\r\n", index/100, position.x, position.y);
		drawBitmap(bitmap, position, NUMBERWIDTH, NUMBERHEIGHT, true, RED, erase);
		index = index%100;
		position.x += NUMBERWIDTH + NUMBERSPACING;
	}
	//Draw the 10s digit
	bitmap = getNumberBitmap(index/10);
//	xil_printf("We are printing %d at %d, %d\r\n", index/10, position.x, position.y);
	drawBitmap(bitmap, position, NUMBERWIDTH, NUMBERHEIGHT, true, RED, erase);
	position.x += NUMBERSPACING + NUMBERWIDTH;
	//Draw the 1s digit which will always be 0
	drawBitmap(number_0_5x5, position, NUMBERWIDTH, NUMBERHEIGHT, true, RED, erase);
	return;
}

const uint32_t* getNumberBitmap(int number){
	if(number == 0) { return number_0_5x5;	}
	else if(number == 1) {	return number_1_5x5;	}
	else if(number == 2) {	return number_2_5x5;	}
	else if(number == 3) {	return number_3_5x5;	}
	else if(number == 4) {	return number_4_5x5;	}
	else if(number == 5) {	return number_5_5x5;	}
	else if(number == 6) {	return number_6_5x5;	}
	else if(number == 7) { 	return number_7_5x5;	}
	else if(number == 8) { 	return number_8_5x5;	}
	else {	return number_9_5x5;	}
}

void drawLivesLabel() { 
	point_t lives_label_pos;
	lives_label_pos.x = LIVESLABELX, lives_label_pos.y = LIVESLABELY;
	drawBitmap(word_lives_24x5, lives_label_pos, LIVESLABELWIDTH, LABELHEIGHT, true, WHITE, false);
}

void drawLives() {
	point_t lives_pos;
	lives_pos.x = LIFESTARTX, lives_pos.y = LIFESTARTY;
	drawBitmapRepeat(tank_15x8, lives_pos, TANKWIDTH, TANKHEIGHT, true, GREEN, false, LIFEXSPACING, LIVESLEFT);
}

void eraseLife(int lives){
	//Update the position to be at the top left of the life to be erased
	point_t lifePos;
	lifePos.x = LIFESTARTX, lifePos.y = LIFESTARTY;
	lifePos.x += ((lives-1)*LIFEXSPACING) + (2*(lives-1)*TANKWIDTH);
	//erase the bitmap of the life.
	drawBitmap(tank_15x8, lifePos, TANKWIDTH, TANKHEIGHT, true, BLACK, true);
	return;
}

void drawNewBunkers() {
	point_t bunker_pos;
	bunker_pos.y = BUNKERSTARTY;
	bunker_pos.x = BUNKERSTARTX;
	// TODO: this will need to change to draw each bunker individually
	drawBitmapRepeat(bunker_24x18, bunker_pos, BUNKERWIDTH, BUNKERHEIGHT, true, GREEN, false, BUNKERXSPACING, 4);
}

void drawBunkerErosion(int bunker, int block){
	point_t block_pos;
	/* Set x position to the start of the bunkers, and then add an offset of 
	 * twice the bunker number (since we are doubling the bitmap to pixel ratio) times the width of
	 * the bunker plus the gap between bunkers. Also adds the width of the block
	*/
	short row = block/4;
	short col = block - (row*4);
	//comment this out if we want to say the bottom right block is block 11 instead of block 9
	if(block == 9)
		col = 3;
	block_pos.x = BUNKERSTARTX + (2*(bunker*(BUNKERWIDTH+BUNKERXSPACING) + (BLOCKWIDTH*col)));
	block_pos.y = BUNKERSTARTY + (2*BLOCKHEIGHT*(row));
	// Get erosion state to know which bitmap to use
//	xil_printf("Position of bunker x: %d, y: %d\r\n", block_pos.x, block_pos.y);
//	xil_printf("Bunker: %d\r\n", bunker);
//	xil_printf("Block: %d\r\n", block);
	uint32_t erosionState = 0;
	switch(bunker){
		case 0: erosionState = getBunkerErosion(0);
		break;
		case 1: erosionState = getBunkerErosion(1);
		break;
		case 2: erosionState = getBunkerErosion(2);
		break;
		case 3: erosionState = getBunkerErosion(3);
		break;
		default: break;
	}
//	xil_printf("erosion state: 0x%08x\r\n", erosionState);
//	short erosion_block = 1;
	uint32_t erosion_block = ((erosionState & (0x7 << (3*block))) >> (3*block));
//	xil_printf("erosion_block: 0x%03x \r\n", erosion_block);
	// Draw bunker erosion using (erase = true) flag
	if(erosion_block == 0x0){
		//do nothing
	}
	else if(erosion_block == 0x1){
		drawBitmap(bunkerDamage0_6x6, block_pos, BLOCKWIDTH, BLOCKHEIGHT, true, BLACK, false);
	}
	else if(erosion_block == 0x2){
		drawBitmap(bunkerDamage1_6x6, block_pos, BLOCKWIDTH, BLOCKHEIGHT, true, BLACK, false);
	}
	else if(erosion_block == 0x3){
		drawBitmap(bunkerDamage2_6x6, block_pos, BLOCKWIDTH, BLOCKHEIGHT, true, BLACK, false);
	}
	else {
		drawBitmap(bunkerDamage3_6x6, block_pos, BLOCKWIDTH, BLOCKHEIGHT, true, BLACK, false);
	}
	activeFramePointer = background;
	if(erosion_block == 0x0){
		//do nothing
	}
	else if(erosion_block == 0x1){
		drawBitmap(bunkerDamage0_6x6, block_pos, BLOCKWIDTH, BLOCKHEIGHT, true, BLACK, false);
	}
	else if(erosion_block == 0x2){
		drawBitmap(bunkerDamage1_6x6, block_pos, BLOCKWIDTH, BLOCKHEIGHT, true, BLACK, false);
	}
	else if(erosion_block == 0x3){
		drawBitmap(bunkerDamage2_6x6, block_pos, BLOCKWIDTH, BLOCKHEIGHT, true, BLACK, false);
	}
	else {
		drawBitmap(bunkerDamage3_6x6, block_pos, BLOCKWIDTH, BLOCKHEIGHT, true, BLACK, false);
	}
	activeFramePointer = foreground;

}

void drawSpaceship(bool erase, int direction){
	if(erase){
		eraseBitmap(getSpaceship().pos, spaceship_width, spaceship_height, true, RED, direction);
	}
	else {
		drawBitmap(saucer_16x7, getSpaceship().pos, spaceship_width, spaceship_height, true, RED, false);
	}
	return;
}

void drawTank(bool erase, int direction) {
	point_t tank_pos = getTankPosition();
	if(erase == true){
		eraseBitmap(tank_pos, TANKWIDTH, TANKHEIGHT, true, GREEN, direction);
	}
	else {
		drawBitmap(tank_15x8, tank_pos, TANKWIDTH, TANKHEIGHT, true, GREEN, false);
	}
	return;
}

void drawTankBullet(bool erase) {
	point_t tankBulletPosition = getTankBulletPosition();
	drawBitmap(tank_bullet_1x3, tankBulletPosition, TANKBULLETWIDTH, TANKBULLETHEIGHT, true, GREEN, erase);
	return;
}

void drawAliens(bool erase, bool in_pose) {
	//Use row and col to traverse the bit map of each alien
	//xil_printf("We are in the drawAliens Function %d\r\n", in_pose);
	int  alienRow, color;
	point_t pos = getAlienBlockPosition();
	bool* deaths = getAlienDeaths();
//	const uint32_t* bitmap;
	for(alienRow = 0; alienRow < 5; alienRow++){
		color = WHITE;
		//Will print the right color if we are not erasing
		int col;
		point_t new_pos = pos;
		//offset is the distance between the x position of each new alien
		int offset = ALIENWIDTH+ALIENXSPACING;
		//Go through each alien in the row and drawing them. Will draw if its not dead
		for(col = 0; col < ALIENSPERROW; col++){
			//Will only apply the offset to every alien but the first one
			if(col > 0){
				new_pos.x += 2*offset;
			}
			// drawBitmap(bitmap, pos, width, height, double_size, color, erase);
			bool eraseDead = deaths[col+(ALIENSPERROW*alienRow)];
			if(erase){
				if(getAlienRight() == true){
					eraseBitmap(new_pos, ALIENWIDTH,ALIENHEIGHT, true, color, RIGHT); }
				else {
					eraseBitmap(new_pos, ALIENWIDTH, ALIENHEIGHT, true, color, LEFT); }
				if(getAlienDown() == true) {
					eraseBitmap(new_pos, ALIENWIDTH,ALIENHEIGHT, true, color, DOWN); }
			}
			else{
				if(alienRow == 0){
					if(in_pose)
						drawBitmap(alien_top_in_12x8, new_pos, ALIENWIDTH, ALIENHEIGHT, true, color, eraseDead);
					else
						drawBitmap(alien_top_out_12x8, new_pos, ALIENWIDTH, ALIENHEIGHT, true, color, eraseDead);
				}
				else if(alienRow == 1 || alienRow == 2){
					if(in_pose)
						drawBitmap(alien_middle_in_12x8, new_pos, ALIENWIDTH, ALIENHEIGHT, true, color, eraseDead);
					else
						drawBitmap(alien_middle_out_12x8, new_pos, ALIENWIDTH, ALIENHEIGHT, true, color, eraseDead);
				}
				else{
					if(in_pose)
						drawBitmap(alien_bottom_in_12x8, new_pos, ALIENWIDTH, ALIENHEIGHT, true, color, eraseDead);
					else
						drawBitmap(alien_bottom_out_12x8, new_pos, ALIENWIDTH, ALIENHEIGHT, true, color, eraseDead);
				}
			}
		}
			// drawBitmapRepeat(bitmap, pos, ALIENWIDTH, ALIENHEIGHT, true, color, erase, ALIENXSPACING, ALIENSPERROW, true);
			// drawBitmapRepeat(bitmap, pos, ALIENWIDTH, ALIENHEIGHT, true, color, erase, ALIENXSPACING, ALIENSPERROW, true);
		//Update the position by putting x back at the starting point and moving y down to the next row
		new_pos.x = pos.x;
		pos.y += (ALIENHEIGHT+ALIENYSPACING);
	}
	return;
}

void drawAlienBullet(bool erase, short bullet_number) {
//	xil_printf("We are printing bullet #%d\r\n", bullet_number);
	aBullet tempBullet;
	const uint32_t* bitmap;
	//Determine which bullet to use
	if(bullet_number == 0) {
		tempBullet = getAlienBullet0();
	}
	else if(bullet_number == 1){
		tempBullet = getAlienBullet1();
	}
	else if(bullet_number == 2) {
		tempBullet = getAlienBullet2();
	}
	else if(bullet_number == 3) {
		tempBullet = getAlienBullet3();
	}
	else //Invalide bullet number
	{}
	//Assign the bitmap
	//Draw the bullet
	if(!erase && (!tempBullet.isFree)){
		bitmap = determineAlienBulletBitmap(tempBullet.type, tempBullet.counter);
		drawBitmap(bitmap, tempBullet.pos, BULLETWIDTH, BULLETHEIGHT, true, GREEN, erase);
	}
	else if(erase){
		eraseBitmap(tempBullet.pos, BULLETWIDTH, BULLETHEIGHT, true, GREEN, DOWN);
	}
	return;
}

const uint32_t* determineAlienBulletBitmap(short bulletType, short counter)
{
	//Return the correct bitmap for the bulletType
	if(bulletType == 1){
		if(counter == 0){
			return alien_bullet_21_3x7;
		}
		else if(counter == 1){
			return alien_bullet_22_3x7;
		}
		else if(counter == 2){
			return alien_bullet_23_3x7;
		}
		else {}
	}
	else if(bulletType == 0) {
		if(counter == 0){
			return alien_bullet_11_3x7;
		}
		else if(counter == 1){
			return alien_bullet_12_3x7;
		}
		else if(counter == 2){
			return alien_bullet_13_3x7;
		}
		else {}
	}
	else {}
	return 0;
}

void drawBitmap(const uint32_t* bitmap, point_t pos, int width, int height, bool double_size, int color, bool erase) {
	int sRow, sCol, row, col;

	for(row = 0, sRow=0; row < height; row++, sRow+=2) {
		for(col = 0, sCol=0; col < width; col++, sCol+=2) {
			//Will print the right color if we are not erasing
			if ((bitmap[row] & (1<<(width-1-col))) && !erase) {
				if(!double_size)//Only print the bitmap to size
					activeFramePointer[(pos.y*SCREENWIDTH + pos.x)] = color;
				else{
					//Expand each pixel to 4 pixels
					activeFramePointer[(sRow+pos.y)*SCREENWIDTH + (sCol+pos.x)] = color;
					activeFramePointer[(sRow+pos.y)*SCREENWIDTH + (sCol+pos.x+1)] = color;
					activeFramePointer[(sRow+pos.y+1)*SCREENWIDTH + (sCol+pos.x)] = color;
					activeFramePointer[(sRow+pos.y+1)*SCREENWIDTH + (sCol+pos.x+1)] = color;
				}
			}
			else if(activeFramePointer != background){//ignore the color
				int index = (pos.y*SCREENWIDTH + pos.x);
				if(!double_size)
					activeFramePointer[index] = background[index];
				else{
					index = (sRow+pos.y)*SCREENWIDTH + (sCol+pos.x);
					activeFramePointer[index] = background[index];
					activeFramePointer[index+1] = background[index+1];
					index += SCREENWIDTH;
					activeFramePointer[index] = background[index];
					activeFramePointer[index+1] = background[index+1];
				}
			}
		}
	}
}

void eraseBitmap(point_t pos, int width, int height, bool double_size, int color, int direction){
	//Very similar to the draw function.
	//It will set pixel of color(param) to background 4 pixels behind the direction it is going
//	xil_printf("We are in the erase bitmap function\r\n");
	int sRow, sCol, row, col, rowLimit, colLimit, newHeight, newWidth;
	point_t startingPoint;
	int pixAdjust = pixel_adjustment;
	if(double_size){
		newHeight = 2* height;
		newWidth = width*2;
	}
	else {
		newHeight = height;
		newWidth = width;
	}
	//Figure out the starting position and row and col limits.
	if(direction == RIGHT){
//		xil_printf("We are going right\r\n");
		startingPoint.x = pos.x;
		startingPoint.y = pos.y;
		rowLimit = newHeight;
		colLimit = pixAdjust;
	}
	else if(direction == LEFT){
//		xil_printf("We are going left\r\n");
		startingPoint.y = pos.y;
		startingPoint.x = pos.x+newWidth-pixAdjust;
		rowLimit = newHeight;
		colLimit = pixAdjust;
	}
	else if(direction == DOWN){
//		xil_printf("We are going down\r\n");
		startingPoint.x = pos.x;
		startingPoint.y = pos.y;
		rowLimit = pixAdjust;
		colLimit = newWidth;
	}
	else {//If direction is up
//		xil_printf("We are going up\r\n");
		startingPoint.y = pos.y+newHeight-pixAdjust;
		startingPoint.x = pos.x;
		rowLimit = pixAdjust;
		colLimit = newWidth;
	}
//	xil_printf("Starting point x: %d, y: %d, rowLimit: %d, colLimit: %d\r\n", startingPoint.x, startingPoint.y, rowLimit, colLimit);
	//Now go through and turn all of the ones that are color to black
	for(row = 0, sRow=0; row < rowLimit; row++, sRow+=2) {
		for(col = 0, sCol=0; col < colLimit; col++, sCol+=2) {
			if(activeFramePointer[(startingPoint.y+sRow)*SCREENWIDTH + sCol +startingPoint.x] == color){
//				xil_printf("We are going to set something to black\r\n");
				if(!double_size){
					activeFramePointer[(startingPoint.y*SCREENWIDTH + startingPoint.x)] = background[(startingPoint.y*SCREENWIDTH + startingPoint.x)];
				}
				else{
//					xil_printf("We should be setting them to black\r\n");
					activeFramePointer[(sRow+startingPoint.y)*SCREENWIDTH + (sCol+startingPoint.x)] = background[(sRow+startingPoint.y)*SCREENWIDTH + (sCol+startingPoint.x)];
					activeFramePointer[(sRow+startingPoint.y)*SCREENWIDTH + (sCol+startingPoint.x+1)] = background[(sRow+startingPoint.y)*SCREENWIDTH + (sCol+startingPoint.x+1)];
					activeFramePointer[(sRow+startingPoint.y+1)*SCREENWIDTH + (sCol+startingPoint.x)] = background[(sRow+startingPoint.y+1)*SCREENWIDTH + (sCol+startingPoint.x)];
					activeFramePointer[(sRow+startingPoint.y+1)*SCREENWIDTH + (sCol+startingPoint.x+1)] = background[(sRow+startingPoint.y+1)*SCREENWIDTH + (sCol+startingPoint.x+1)];
				}
			}
		}
	}
	return;
}

void drawBitmapRepeat(const uint32_t* bitmap, point_t pos, int width, int height, bool double_size, int color, bool erase, int x_space, int times) {
	int element_num;
	int offset = width+x_space;
	for(element_num = 0; element_num < times; element_num++){
		if(element_num >0){
			pos.x += offset;
			if(double_size)
				pos.x += offset;
		}
		drawBitmap(bitmap, pos, width, height, double_size, color, erase);
	}
}

void eraseBitmapRepeat(point_t pos, int width, int height, bool double_size, int color, int direction, int x_space, int times){
	int element_num;
	int offset = width+x_space;
	for(element_num = 0; element_num < times; element_num++){
		if(element_num > 0){
			pos.x+= offset;
			if(double_size)
				pos.x += offset;
		}
		eraseBitmap(pos, width, height, double_size, color, direction);
	}
	return;
}
