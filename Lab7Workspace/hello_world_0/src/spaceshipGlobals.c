

#include "globals.h"
#include "render.h"
#include "bitmaps.h"
#include "spaceshipGlobals.h"
#include "playSound.h"

saucer spaceship;

bool spaceshipHit = false;

void setSpaceship(int direction) {
	if (spaceship.isFree) {
		if (direction == LEFT) {
			//If the direction of the saucer is left, we will start on the right side
			spaceship.pos.x = spaceship_right_x;
		} else {
			spaceship.pos.x = spaceship_left_x;
		}
		//Set the y coordinates and the direction
		spaceship.pos.y = spaceship_y;
		spaceship.isFree = false;
		spaceship.direction = direction;
	}
}

void setInitialSpaceship(point_t pos) {
	//Set the initial position to pos during initialization
	spaceship.pos = pos;
	spaceship.isFree = true;
	//	drawBitmap(saucer_16x7, pos,spaceship_width, spaceship_height, true, BLACK, true);
	return;
}

saucer* getSpaceship() {
	return &spaceship;
}

void updateSpaceship() {
	if (!spaceship.isFree) {
		bool offscreen = false;
		if (spaceship.direction == LEFT) {
			//			xil_printf("The direction is left \r\n");
			//make the saucer go left by pixel_adjustment
			spaceship.pos.x -= spaceship_pixel_adjustment;
			//Check to see if the saucer is off the screen yet
			if (spaceship.pos.x <= (0)) {
				offscreen = true;
			}
		}
		else {
			//			xil_printf("The direction is right\r\n");
			//Make the spaceship go right by pixel_adjustment
			spaceship.pos.x += spaceship_pixel_adjustment;
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
			setSpaceshipSoundPlaying(false);
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


