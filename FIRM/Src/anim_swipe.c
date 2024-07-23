/*
 * anim_vertical.c
 *
 *  Created on: Jul 22, 2023
 *      Author: Lam
 */

#include "screenBuffer.h"

void anim_vertical_run()
{
	static uint8_t currCol = 0;
	screen_draw_line(0, currCol, 7, currCol);

	currCol++;
	if(currCol == 8)
		currCol = 0;

	HAL_Delay(25);
	screen_clear();
}

void anim_rotate_run()
{
	static uint8_t currState = 0;

	if(currState < 7)
	{
		screen_draw_line(0, currState, 7, 7-currState);
	}
	else
	{
		screen_draw_line(currState-7, 7,14-currState,0);
	}


	currState++;
	if(currState == 14)
		currState = 0;

	HAL_Delay(25);
	screen_clear();

}

void anim_horizontal_run()
{
	static uint8_t currRow = 0;
	screen_draw_line(currRow, 0 ,currRow, 7);

	currRow++;
	if(currRow == 8)
		currRow = 0;

	HAL_Delay(25);
	screen_clear();
}


void anim_diagonal_run()
{
	static uint8_t shifting = 0;
	static uint8_t currIndex = 0;
	for(int i=0; i<8; i++)
	{
		screen_set_bit(currIndex+shifting, i, 1);

		currIndex++;
		if(currIndex >= 8)
			currIndex = 0;
	}

	shifting++;
	if(shifting >= 8)
		shifting = 0;

	HAL_Delay(25);
	screen_clear();
}

#define SWIPEALL_ANIM1 16
#define SWIPEALL_ANIM2 32
#define SWIPEALL_ANIM3 48
#define SWIPEALL_ANIM4 64
void anim_swipeAll_run()
{
	static uint8_t counter = 0;
	if(counter < SWIPEALL_ANIM1)
	{
		anim_vertical_run();
	}
	else if(counter >= SWIPEALL_ANIM1 && counter < SWIPEALL_ANIM2)
	{
		anim_horizontal_run();
	}
	else if(counter >= SWIPEALL_ANIM2 && counter < SWIPEALL_ANIM3)
	{
		anim_diagonal_run();
	}
	else
	{
		anim_rotate_run();
	}

	counter++;
	if(counter > SWIPEALL_ANIM4)
	{
		counter=0;
	}

}
