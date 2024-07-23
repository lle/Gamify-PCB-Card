#include "main.h"

//Bypass whole screen
void screen_show_letter(uint8_t ascii_id);
void screen_fill(void);
void screen_clear(void);
void screen_draw_line(int x0, int y0, int x1, int y1);

//Interrupt Handler
void screen_interruptUpdate(void);

//GET and SET functions
uint8_t screen_get_bit(uint8_t row, uint8_t col);
void screen_set_bit(uint8_t row, uint8_t col, uint8_t dataBit);
void screen_display_array(uint8_t array[8][8]);
void screen_display_image(uint64_t image);
