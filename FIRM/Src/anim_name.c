/*
 * anim_name.c
 *
 *  Created on: Jul 22, 2023
 *      Author: Lam
 */

#include "screenBuffer.h"
#include "ascii_letter.h"
#include <string.h>

void anim_name_word(char* name)
{
	static uint8_t index = 0;
	screen_show_letter( name[index] );
	index++;
	if(index >= strlen(name))
		index = 0;

	HAL_Delay(1);
}


