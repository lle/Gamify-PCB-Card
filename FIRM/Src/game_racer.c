#include "screenBuffer.h"
#include "game_racer.h"

#define DEFAULT_REFRESH_MS 1200
#define DEFAULT_CAR_POS 3
#define DEFAULT_OBSTACLE_CNT 1
#define CAR_BLINK_RATE 70
#define BTN_HOLD_INTERVAL 200
#define CAR_ROW_POS 7
#define CAR_MIN_COL_POS 0
#define CAR_MAX_COL_POS 7
#define GAMEOVER_COLLISION_BLINK 20
#define GAMEOVER_FILLBLINK 4

void gameRacer_startNewGame(void);
void initGameOver(uint8_t collision_col, uint8_t collision_row);
void shiftDownObstacle(void);
void generateObstacles(uint8_t numOfObstacle);
void clearTopRow(void);
void updateDifficulty(void);
void updateCarPosition(void);

uint16_t gameRacer_refreshInterval = DEFAULT_REFRESH_MS;
uint32_t gameRacer_level = 0;
uint8_t gameRacer_obstacleCount = DEFAULT_OBSTACLE_CNT;
uint8_t gameRacer_carPosition = DEFAULT_CAR_POS;

void gameRacer_startNewGame()
{
	gameRacer_level = 0;
	gameRacer_obstacleCount = DEFAULT_OBSTACLE_CNT;
	gameRacer_refreshInterval = DEFAULT_REFRESH_MS;
	gameRacer_carPosition = DEFAULT_CAR_POS;
}

void initGameOver(uint8_t collision_row, uint8_t collision_col)
{
	screen_clear();
	for(int i=0; i<GAMEOVER_COLLISION_BLINK; i++)
	{
		screen_set_bit(collision_row, collision_col, 1);	//highlight obstacle hit
		screen_set_bit(CAR_ROW_POS, gameRacer_carPosition, 0);	//highlight car
		HAL_Delay(CAR_BLINK_RATE);
		screen_set_bit(collision_row, collision_col, 0);
		screen_set_bit(CAR_ROW_POS, gameRacer_carPosition, 1);	//highlight obstacle hit
		HAL_Delay(CAR_BLINK_RATE);
	}

	for(int i=0; i<GAMEOVER_FILLBLINK; i++)
	{
		screen_fill();
		HAL_Delay(CAR_BLINK_RATE);
		screen_clear();
		HAL_Delay(CAR_BLINK_RATE);
	}

	gameRacer_startNewGame();
}

void shiftDownObstacle()
{
	for(int r=6; r>=0; r--)	//start from bottom up (row 7,6,5...1,0)
	{
		for(int c=0; c<8; c++)	//go from column left to right
		{
			uint8_t tempBit = screen_get_bit(r, c);

			//check if the car collides with the incoming obstacle
			if(r == 6 && tempBit == 1 && c == gameRacer_carPosition)
			{
				initGameOver(r, c);	//collision detected, start game-over animation
				return;
			}

			//no collision detected, move pixel down 1 row
			screen_set_bit(r+1, c, tempBit);
		}
	}
}

void clearTopRow()
{
	//clear top row once everything has been shifted down
	for(int col=0; col<8; col++)
	{
		screen_set_bit(0, col, 0);
	}
}

void generateNewRow(uint8_t numOfObstacle)
{
	//make sure to limit obstacles to 4 per row
	if(numOfObstacle > 4)
		numOfObstacle = 4;

	//shift all the obstacle down 1 row
	shiftDownObstacle();
	clearTopRow();

	//generate requested number of obstacle
	int8_t firstObstaclePos = -1;
	for(int i=0; i<numOfObstacle; i++)
	{
		int8_t tempCol = -1;
		while(tempCol == -1)
		{
			uint8_t random = rand()%8;
			if(screen_get_bit(0, random) == 0)	//if that pixel is OFF then continue
			{
				if(firstObstaclePos != -1) //if there was already a first obstacle generated, we need to check for edge-cases
				{
					if(random-firstObstaclePos != -1 && random-firstObstaclePos != 1) 	// only accept the proposed 2nd obstacle
						tempCol = random;												// if it is not beside an existing one.
				}																		// (leave at least a 1 pixel gap to avoid un-winnable scenarios)
				else
					tempCol = random;
			}
		}
		screen_set_bit(0, tempCol, 1);
		firstObstaclePos = tempCol;	//save temporarily the position of the first obstacle
	}
}

void updateDifficulty()
{
	if		(gameRacer_level == 5) 	{ gameRacer_obstacleCount=1; 	gameRacer_refreshInterval=1000;	}
	else if	(gameRacer_level == 10) 	{ gameRacer_obstacleCount=1; 	gameRacer_refreshInterval=900;	}
	else if	(gameRacer_level == 15) 	{ gameRacer_obstacleCount=1; 	gameRacer_refreshInterval=800;	}
	else if	(gameRacer_level == 20) 	{ gameRacer_obstacleCount=1; 	gameRacer_refreshInterval=700;	}
	else if	(gameRacer_level == 25) 	{ gameRacer_obstacleCount=1; 	gameRacer_refreshInterval=600; 	}
	else if	(gameRacer_level == 30) 	{ gameRacer_obstacleCount=1; 	gameRacer_refreshInterval=500;	}
	else if	(gameRacer_level == 50) 	{ gameRacer_obstacleCount=1; 	gameRacer_refreshInterval=400;	}
	else if	(gameRacer_level == 75) 	{ gameRacer_obstacleCount=1; 	gameRacer_refreshInterval=300;	}
	else if	(gameRacer_level == 150) 	{ gameRacer_obstacleCount=1; 	gameRacer_refreshInterval=200;	}
	else if	(gameRacer_level == 250) 	{ gameRacer_obstacleCount=2; 	gameRacer_refreshInterval=500;	}
	else if	(gameRacer_level == 270) 	{ gameRacer_obstacleCount=2; 	gameRacer_refreshInterval=400; 	}
	else if	(gameRacer_level == 320) 	{ gameRacer_obstacleCount=2; 	gameRacer_refreshInterval=300;	}
	else if	(gameRacer_level == 400) 	{ gameRacer_obstacleCount=2; 	gameRacer_refreshInterval=200;	}
}

void updateCarPosition()
{
	if(HAL_GPIO_ReadPin(BTN4_GPIO_Port, BTN4_Pin) == GPIO_PIN_RESET)	//move right button
	{
		//COLLISION CHECK
		if(gameRacer_carPosition == CAR_MAX_COL_POS)	//edge case: check for collision during wrap-around
		{
			if(screen_get_bit(CAR_ROW_POS, CAR_MIN_COL_POS) == 1)	//collision detected on the other side?
			{
				initGameOver(CAR_ROW_POS, CAR_MIN_COL_POS);			//start game-over animation and highlight obstacle X,Y coordinate
				return;
			}
		}
		else if(screen_get_bit(CAR_ROW_POS, gameRacer_carPosition+1) == 1)
		{
			initGameOver(CAR_ROW_POS, gameRacer_carPosition+1);	//collision detected, start game-over animation
			return;
		}

		//no collision detected, proceed
		screen_set_bit(CAR_ROW_POS, gameRacer_carPosition, 0);	//clear pixel in previous car position
		if(gameRacer_carPosition == CAR_MAX_COL_POS)
			gameRacer_carPosition = CAR_MIN_COL_POS; 			//wrap around to the right side
		else
			gameRacer_carPosition++;
	}
	else if(HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin) == GPIO_PIN_RESET)	//move left button
	{
		//CHECK COLLISION
		if(gameRacer_carPosition == CAR_MIN_COL_POS)	//edge case: check for collision during wrap-around
		{
			if(screen_get_bit(CAR_ROW_POS, CAR_MAX_COL_POS) == 1)	//collision detected on the other side?
			{
				initGameOver(CAR_ROW_POS, CAR_MAX_COL_POS);			//start game-over animation and highlight obstacle X,Y coordinate
				return;
			}
		}
		else if(screen_get_bit(CAR_ROW_POS, gameRacer_carPosition-1) == 1)
		{
			initGameOver(CAR_ROW_POS, gameRacer_carPosition-1);	//collision detected, start game-over animation
			return;
		}

		//no collision detected, proceed
		screen_set_bit(CAR_ROW_POS, gameRacer_carPosition, 0);	//clear pixel in previous car position
		if(gameRacer_carPosition == CAR_MIN_COL_POS)
			gameRacer_carPosition = CAR_MAX_COL_POS; 			//wrap around to the left side
		else
			gameRacer_carPosition--;
	}
}

void gameRacer_run()
{
	static uint32_t ts_lastExec = 0;
	if(HAL_GetTick() - ts_lastExec > gameRacer_refreshInterval)
	{
		ts_lastExec = HAL_GetTick();
		generateNewRow(gameRacer_obstacleCount);
		gameRacer_level++;
		updateDifficulty();
	}

	static uint32_t ts_blinkingCar = 0;
	if(HAL_GetTick() - ts_blinkingCar > CAR_BLINK_RATE)
	{
		ts_blinkingCar = HAL_GetTick();
		if(HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin) == GPIO_PIN_RESET || HAL_GPIO_ReadPin(BTN4_GPIO_Port, BTN4_Pin) == GPIO_PIN_RESET)
		{
			screen_set_bit(CAR_ROW_POS, gameRacer_carPosition, 1);	//force the LED to stay ON when the player is moving the car
		}
		else
		{
			if(screen_get_bit(CAR_ROW_POS, gameRacer_carPosition) == 1)	//IF led is ON then turned it off
				screen_set_bit(CAR_ROW_POS, gameRacer_carPosition, 0);
			else
				screen_set_bit(CAR_ROW_POS, gameRacer_carPosition, 1);
		}
	}

	static uint32_t ts_btnListen = 0;
	static uint8_t btn_isPressed = 0;
	if(btn_isPressed == 0)
	{
		if(HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin) == GPIO_PIN_RESET || HAL_GPIO_ReadPin(BTN4_GPIO_Port, BTN4_Pin) == GPIO_PIN_RESET) //button press detected
		{
			btn_isPressed = 1;
		}
	}
	else if(btn_isPressed == 1)	//decide what happens after the button is pressed. Determine if the user is holding or doing a single-press
	{
		if(HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin) == GPIO_PIN_SET && HAL_GPIO_ReadPin(BTN4_GPIO_Port, BTN4_Pin) == GPIO_PIN_SET)//if the user releases the button, car-update-position should be immediately available
		{
			updateCarPosition();
			HAL_Delay(1);
		}
		else if(HAL_GetTick() - ts_btnListen > BTN_HOLD_INTERVAL)	//if the user holds the buttons, the car-update-position will be called every 200ms
		{
			ts_btnListen = HAL_GetTick();
			updateCarPosition();
		}
	}
}
