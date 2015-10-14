/*
 * render.h
 *
 *  Created on: Sep 25, 2015
 *      Author: superman
 */

#ifndef RENDER_H_
#define RENDER_H_
#include "stdbool.h"
#include "stdint.h"
#include "globals.h"
#define FRAME_BUFFER_0_ADDR 0xC1000000
#define SCREENWIDTH 640
#define SCREENHEIGHT 480
#define WORD_WIDTH 32
// Life dimension/spacing
#define LIFESTARTX 410
#define LIFESTARTY 10
#define LIFEXSPACING 5
#define LIVESLABELX 350
#define LIVESLABELY 15
#define LIVESLEFT 3
#define LIVESLABELWIDTH 24
#define LABELHEIGHT 5
// Score dimension/spacing
#define SCORELABELX 20
#define SCORELABELY 15
#define SCOREX 90
#define SCOREY 15
#define SCORELABELWIDTH 30
#define NUMBERWIDTH 5
#define NUMBERHEIGHT 5
#define NUMBERSPACING 3
// Tank dimenstion/spacing
#define TANKSTARTY 400
#define TANKSTARTX 98
#define TANKHEIGHT 8
#define TANKWIDTH 15
// Bunker/damage block dimension/spacing
#define BUNKERWIDTH 24
#define BUNKERHEIGHT 18
#define BUNKERXSPACING 45
#define BUNKERSTARTY 335
#define BUNKERSTARTX 89
#define BLOCKWIDTH 6
#define BLOCKHEIGHT 6
// Alien/alien block dimension/spacing
#define ALIENHEIGHT 8
#define ALIENWIDTH 12
#define ALIENXSPACING 2
#define ALIENYSPACING 20
#define ALIENBLOCKSTARTX 167
#define ALIENBLOCKSTARTY 75
#define ALIENSPERROW 11
// Bullet dimension/spacing
#define BULLETHEIGHT 7
#define BULLETWIDTH 3
#define TANKBULLETWIDTH 1
#define TANKBULLETHEIGHT 3
//Erase directions
#define DOWN 0
#define UP 1
#define LEFT 2
#define RIGHT 3

extern unsigned int * foreground;// = (unsigned int *) FRAME_BUFFER_0_ADDR;
extern unsigned int * background;// = ((unsigned int *) FRAME_BUFFER_0_ADDR) + SCREENWIDTH*SCREENHEIGHT;
extern unsigned int * activeFramePointer;// = (unsigned int *) FRAME_BUFFER_0_ADDR;

void clearScreen();
void initScreen();
void render(bool erase, int render_objects_mask, short index, int direction);
void drawScoreLabel();
void drawScore(int index, int number, int prevNum);
const uint32_t* getNumberBitmap(int number);
void drawLivesLabel();
void drawLives();
void eraseLife(int lives);
void drawNewBunkers();
void drawBunkerErosion(int bunker, int block);
void drawTank(bool erase, int direction);
void drawTankBullet(bool erase);
void drawSpaceship(bool erase, int direction);
void printSpaceshipValue(int spaceshipValue, point_t position, bool erase);
void drawAliens(bool erase, bool in_pose);
const uint32_t* determineAlienBulletBitmap(short bulletType, short counter);
void drawAlienBullet(bool erase, short bullet_number);
void drawBitmap(const uint32_t* bitmap, point_t pos, int width, int height, bool double_size, int color, bool erase);
void drawBitmapRepeat(const uint32_t* bitmap, point_t pos, int width, int height, bool double_size, int color, bool erase, int x_space, int times);
void eraseBitmapRepeat(point_t pos, int width, int height, bool double_size, int color, int direction, int x_space, int times);
void eraseBitmap(point_t pos, int width, int height, bool double_size, int color, int direction);

#endif /* RENDER_H_ */
