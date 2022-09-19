#include "main.h"

//Bypass whole screen
void screen_show_letter(uint8_t ascii_id);
void screen_fill(void);
void screen_clear(void);

//Interrupt Handler
void screen_interruptUpdate(void);

//GET and SET functions
uint8_t screen_get_bit(uint8_t row, uint8_t col);
void screen_set_bit(uint8_t row, uint8_t col, uint8_t dataBit);
