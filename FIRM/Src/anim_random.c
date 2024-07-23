/*
 * anim_random.c
 *
 *  Created on: Jul 22, 2023
 *      Author: Lam
 */

#include "screenBuffer.h"

void anim_random_run()
{
	for(int i=0; i<8; i++)
	{
		for(int j=0; j<8; j++)
		{
			uint8_t state = rand()%2;
			screen_set_bit(i, j, state);
		}
	}
	HAL_Delay(25);
	screen_clear();
}
