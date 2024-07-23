/*
 * anim_vertical.c
 *
 *  Created on: Aug 8, 2023
 *      Author: Ang3l
 */

#include "screenBuffer.h"


// Generated using https://xantorohara.github.io/led-matrix-editor
// To use your own animation, replace the array with yours :)
// You can change REVERSE_END to decide whether your animation repeats or reverses at the end.
const uint64_t IMAGES[] = {
  0x0000030300000000,
  0x0000000606000000,
  0x000000000c0c0000,
  0x0000000000181800,
  0x0000000000003030,
  0x0000000000606000,
  0x00000000c0c00000,
  0x0000006060000000,
  0x0000303000000000,
  0x0018180000000000,
  0x00000c0c00000000,
  0x0000000606000000,
  0x0000000003030000,
  0x0000000000060600,
  0x0000000000000c0c,
  0x0000000000181800,
  0x0000000030300000,
  0x0000006060000000,
  0x0000c0c000000000,
  0x0060600000000000,
  0x0000303000000000,
  0x0000001818000000,
  0x000000000c0c0000,
  0x0000000000060600,
  0x0000000000000303
};
const int IMAGES_LEN = sizeof(IMAGES)/8;
const uint8_t REVERSE_END = 1;


void anim_ball_run()
{
	static int currState = 0;
	static uint8_t dirrection = 0;

		screen_display_image(IMAGES[currState]);

	if (REVERSE_END)
	{
		if (dirrection) currState--;
		else currState++;
		if(currState == IMAGES_LEN)
		{
			dirrection = 1;
			currState = IMAGES_LEN - 2;
		}
		if(currState == 0)
			dirrection = 0;
	}
	else
	{
		currState++;
		if(currState == IMAGES_LEN)
			currState = 0;
	}



	HAL_Delay(100);
	screen_clear();

}



