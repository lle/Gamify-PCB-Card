#include "ledHandler.h"

void matrix_setLed_on(uint8_t row, uint8_t col)
{
	//turn on all leds
	matrix_setLed_allOff();
	
	HAL_GPIO_WritePin(GPIOB, 0x01<<row, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, 0x01<<(col+8), GPIO_PIN_RESET);
}

void matrix_setLed_allOff()
{
	HAL_GPIO_WritePin(GPIOB, 0x00FFU, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, 0xFF00U, GPIO_PIN_SET);
}
