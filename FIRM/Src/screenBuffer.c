#include "screenBuffer.h"
#include "ledHandler.h"
#include "ascii_letter.h"

#define COL_SIZE 8
#define ROW_SIZE 8

uint8_t screenBuffer[ROW_SIZE][COL_SIZE] = {
	 {0, 0, 0, 0, 	0, 0, 0, 0},
	 {0, 0, 0, 0, 	0, 0, 0, 0},
	 {0, 0, 0, 0, 	0, 0, 0, 0},
	 {0, 0, 0, 0, 	0, 0, 0, 0},
	 {0, 0, 0, 0, 	0, 0, 0, 0},
	 {0, 0, 0, 0, 	0, 0, 0, 0},
	 {0, 0, 0, 0, 	0, 0, 0, 0},
	 {0, 0, 0, 0, 	0, 0, 0, 0}
 };

 
uint8_t screen_get_bit(uint8_t row, uint8_t col)
{
	return screenBuffer[col][row];
}

void screen_set_bit(uint8_t row, uint8_t col, uint8_t dataBit)
{
	screenBuffer[col][row] = dataBit;
}
 
void screen_show_letter(uint8_t ascii_id)
{
	for(int row=0; row<8; row++)
	{
		for(int col=0; col<8; col++)
		{
			uint8_t dataBit = font8x8_basic[ascii_id][col]; //load byte from correct column
			
			dataBit = dataBit >> row; 	//bit shift down by the amount of "rows"
			dataBit = dataBit & 0x01; 	//keep only the first bit
						
			screenBuffer[row][col] = dataBit;
		}	
	}
}
 
void screen_fill()
{
	for(int row=0; row<8; row++)
	{
		for(int col=0; col<8; col++)
		{
				screenBuffer[row][col] = 1;
		}	
	}
}

void screen_clear()
{
	for(int row=0; row<8; row++)
	{
		for(int col=0; col<8; col++)
		{
				screenBuffer[row][col] = 0;
		}	
	}
}

void screen_interruptUpdate()
{
	static uint8_t col = 0;
	static uint8_t row = 0;
	
	uint8_t dataBit = screenBuffer[row][col];
	if( dataBit != 0)
	{
		matrix_setLed_on(row, col);
	}
	else
	{
		matrix_setLed_allOff();
	}	
		
	col++;
	if(col==8)
	{
		col=0;
		row++;
	}
	if(row==8)
		row=0;
}

// Modified from  https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
void screen_draw_line(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;

    while (screen_set_bit(x0, y0, 1), x0 != x1 || y0 != y1) {
        int e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 <  dy) { err += dx; y0 += sy; }
    }
}

void screen_display_array(uint8_t array[8][8])
{
	for(int row=0; row<8; row++)
	{
		for(int col=0; col<8; col++)
		{
				screenBuffer[row][col] = array[row][col];
		}
	}
}

void screen_display_image(uint64_t image)
{
  screen_clear();
  for (int i = 0; i < 8; i++) {
    uint8_t row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
    	screen_set_bit(i, j, ((row >> j)  & 0x01));
    }
  }
}
