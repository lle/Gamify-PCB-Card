#include "game_snake.h"
#include "screenBuffer.h"
//#include "usart.h"
#include "ascii_letter.h"

//*************************************************************
//
//	DEFINES
//
//*************************************************************
#define SNAKE_OK 0
#define SNAKE_DEAD 1
#define SNAKE_EATING 2

#define DEFAULT_UPDATE_DELAY 500

#define FOOD_ID 200

#define NO_DIRECTION_CHANGE 255


//*************************************************************
//
//	VARIABLES
//
//*************************************************************
uint8_t curr_snake_direction = GOING_RIGHT;
uint8_t pending_new_direction = NO_DIRECTION_CHANGE;

uint8_t snake_lenght = 4;

uint8_t food_row;
uint8_t food_column;

uint16_t update_delay = DEFAULT_UPDATE_DELAY;

//*************************************************************
//
//	FUNCTIONS PROTOTYPES
//
//*************************************************************
void increment_snake_body(void);
void remove_snake_tail(void);
void update_snake_head(uint8_t col, uint8_t row);
uint8_t validate_direction_change(uint8_t new_direction, uint8_t curr_head_row, uint8_t curr_head_col);
void snake_set_new_direction(uint8_t new_direction);
uint8_t process_new_direction_request(void);
uint8_t collision_detection(uint8_t curr_head_row, uint8_t curr_head_col);
uint8_t food_detection(uint8_t curr_head_row, uint8_t curr_head_col);
void game_over_animation(void);
void generate_food(void);



//*************************************************************
//
//	FUNCTIONS
//
//*************************************************************
void game_startNewGame()
{
	update_delay = DEFAULT_UPDATE_DELAY;
	screen_clear();

	uint8_t startRow = rand()%8;
	uint8_t startCol = rand()%4; //render the snake only on the left side of the screen (food will on the right)
	HAL_Delay(100);
	screen_set_bit(startRow, startCol, 102);
	HAL_Delay(100);
	screen_set_bit(startRow, startCol+1, 101);
	HAL_Delay(100);
	screen_set_bit(startRow, startCol+2, 100);
	HAL_Delay(100);

	snake_lenght = 3;
	curr_snake_direction = GOING_RIGHT;

	generate_food();
}

void generate_food()
{
	uint8_t empty_slot_found = 0;
	while(empty_slot_found == 0)
	{
		food_row = rand()%8;
		food_column = rand()%4 + 3; //render initial food only on the right side of the screen (snake is on the left)
		if(screen_get_bit(food_row, food_column) == 0)
		{
			empty_slot_found = 1;
		}
	}
	screen_set_bit(food_row, food_column, FOOD_ID);
}

void snake_runGame()
{
	static uint32_t ts_gameScheduler = 0;
	static uint32_t ts_blinkScheduler = 0;
	static uint8_t food_blink_state = 0;

	if(HAL_GetTick() - ts_gameScheduler > update_delay)
	{
		ts_gameScheduler = HAL_GetTick();

		//force the food to appear if it is blinking
		screen_set_bit(food_row, food_column, FOOD_ID);

		//FIND SNAKE HEAD
		uint8_t snakeHead_found = 0;

		for(int row=0; row<8; row++)
		{
			for(int col=0; col<8; col++)
			{
				if(screen_get_bit(row, col) == 100)
				{
					snakeHead_found = 1;

					//process new direction change
					curr_snake_direction = validate_direction_change(process_new_direction_request(), row, col); //if this is a valid direction change (ei: user is not trying to go backward) then update snake-direction

					//check if the next update leads to a collision
					if( collision_detection(row, col) == SNAKE_DEAD )
					{
						game_over_animation();
					}
					else
					{
						//has the snake found food?
						if( food_detection(row, col) == SNAKE_EATING )
						{
							snake_lenght++;		//increase the length of the snake
							generate_food(); //pick a new location for food
							//increase speed every 2 points
							if( snake_lenght%2 == 0 && update_delay > 100)
							{
								update_delay = update_delay - 25;
							}
						}

						//increment the IDs for all the snake body parts
						increment_snake_body();

						//add the head into the next position based on the direction
						update_snake_head(row, col);

						//remove the tail
						remove_snake_tail();
					}
				}
				if(snakeHead_found == 1)
					break;
			}
			if(snakeHead_found == 1)
				break;
		}
	}
	else if(HAL_GetTick() - ts_blinkScheduler > 100)
	{
		ts_blinkScheduler = HAL_GetTick();
		if(food_blink_state == 0)
		{
			screen_set_bit(food_row, food_column, 0);
			food_blink_state = 1;
		}
		else
		{
			screen_set_bit(food_row, food_column, FOOD_ID);
			food_blink_state = 0;
		}
	}
}

uint8_t validate_direction_change(uint8_t new_direction, uint8_t curr_head_row, uint8_t curr_head_col)
{
	uint8_t direction = curr_snake_direction;

	switch(new_direction)
	{
		case GOING_LEFT:
				if(screen_get_bit(curr_head_row, curr_head_col-1) == 101) //if there is the neck of the snake (body_id=101), it means the user is trying back-track (to go reverse), not allowed
					UNUSED(0); //do not change direction, (return the current direction)
				else
					direction = new_direction;			//accept the update, return the proposed new direction
			break;

		case GOING_UP:
				if(screen_get_bit(curr_head_row-1, curr_head_col) == 101) //if there is the neck of the snake (body_id=101), it means the user is trying back-track (to go reverse), not allowed
					UNUSED(0); //do not change direction, (return the current direction)
				else
					direction = new_direction;			//accept the update, return the proposed new direction
			break;

		case GOING_DOWN:
				if(screen_get_bit(curr_head_row+1, curr_head_col) == 101) //if there is the neck of the snake (body_id=101), it means the user is trying back-track (to go reverse), not allowed
					UNUSED(0); //do not change direction, (return the current direction)
				else
					direction = new_direction;			//accept the update, return the proposed new direction
			break;

		case GOING_RIGHT:
				if(screen_get_bit(curr_head_row, curr_head_col+1) == 101) //if there is the neck of the snake (body_id=101), it means the user is trying back-track (to go reverse), not allowed
					UNUSED(0); //do not change direction, (return the current direction)
				else
					direction = new_direction;			//accept the update, return the proposed new direction
			break;
	}

	//if switch case fail, return currect direction
	return direction;
}

void update_snake_head(uint8_t curr_head_row, uint8_t curr_head_col)
{
	switch(curr_snake_direction)
	{
		case GOING_RIGHT:
			if(curr_head_col == 7)	//if the head is at the right edge of the screen, wrap around to the left edge
				screen_set_bit(curr_head_row, 0, 100);
			else
				screen_set_bit(curr_head_row, curr_head_col+1, 100);
		break;

		case GOING_UP:
			if(curr_head_row == 0 )	//if the head is at the top edge of the screen, wrap around to the bottom edge
				screen_set_bit(7, curr_head_col, 100);
			else
				screen_set_bit(curr_head_row-1, curr_head_col, 100);
		break;

		case GOING_LEFT:
			if(curr_head_col == 0 )	//if the head is at the left edge of the screen, wrap around to the right edge
				screen_set_bit(curr_head_row, 7, 100);
			else
				screen_set_bit(curr_head_row, curr_head_col-1, 100);
		break;

		case GOING_DOWN:
			if(curr_head_row == 7 )	//if the head is at the bottom edge of the screen, wrap around to the top edge
				screen_set_bit(0, curr_head_col, 100);
			else
				screen_set_bit(curr_head_row+1, curr_head_col, 100);
		break;
	}
}

uint8_t food_detection(uint8_t curr_head_row, uint8_t curr_head_col)
{
	uint8_t snake_health = SNAKE_OK;
	uint8_t next_tile_dataBit;
	switch(curr_snake_direction)
	{
		case GOING_LEFT:
				if(curr_head_col != 0)
					next_tile_dataBit = screen_get_bit(curr_head_row, curr_head_col-1);
				else
					next_tile_dataBit = screen_get_bit(curr_head_row, 7);	//edge case handling: if the snake head at the left-edge, wrap-around to the right side

				if( next_tile_dataBit == FOOD_ID ) //if there is a snake body (body_id=102 to 164), then you just ate yourself, game over
					snake_health = SNAKE_EATING;
			break;

		case GOING_UP:
				if(curr_head_row != 0)
					next_tile_dataBit = screen_get_bit(curr_head_row-1, curr_head_col);
				else
					next_tile_dataBit = screen_get_bit(7, curr_head_col); //edge case handling: if the snake head at the top-edge, wrap-around to the bottom side

				if( next_tile_dataBit == FOOD_ID ) //if there is a snake body (body_id=102 to 164), then you just ate yourself, game over
					snake_health = SNAKE_EATING;
			break;

		case GOING_DOWN:
				if(curr_head_row != 7)
					next_tile_dataBit = screen_get_bit(curr_head_row+1, curr_head_col);
				else
					next_tile_dataBit = screen_get_bit(0, curr_head_col); //edge case handling: if the snake head at the bottom-edge, wrap-around to the top side

				if( next_tile_dataBit == FOOD_ID ) //if there is a snake body (body_id=102 to 164), then you just ate yourself, game over
					snake_health = SNAKE_EATING;
			break;

		case GOING_RIGHT:
				if(curr_head_col != 7)
					next_tile_dataBit = screen_get_bit(curr_head_row, curr_head_col+1);
				else
					next_tile_dataBit = screen_get_bit(curr_head_row, 0);	//edge case handling: if the snake head at the right-edge, wrap-around to the left side

				if( next_tile_dataBit == FOOD_ID ) //if there is a snake body (body_id=102 to 164), then you just ate yourself, game over
					snake_health = SNAKE_EATING;
			break;
	}

	return snake_health;
}

uint8_t collision_detection(uint8_t curr_head_row, uint8_t curr_head_col)
{
	uint8_t snake_health = SNAKE_OK;
	uint8_t next_tile_dataBit;
	switch(curr_snake_direction)
	{
		case GOING_LEFT:
				if(curr_head_col != 0)
					next_tile_dataBit = screen_get_bit(curr_head_row, curr_head_col-1);
				else
					next_tile_dataBit = screen_get_bit(curr_head_row, 7);	//edge case handling: if the snake head at the left-edge, wrap-around to the right side

				if( next_tile_dataBit >= 102 && next_tile_dataBit <= 164 ) //if there is a snake body (body_id=102 to 164), then you just ate yourself, game over
					snake_health = SNAKE_DEAD;
			break;

		case GOING_UP:
				if(curr_head_row != 0)
					next_tile_dataBit = screen_get_bit(curr_head_row-1, curr_head_col);
				else
					next_tile_dataBit = screen_get_bit(7, curr_head_col); //edge case handling: if the snake head at the top-edge, wrap-around to the bottom side

				if( next_tile_dataBit >= 102 && next_tile_dataBit <= 164 ) //if there is a snake body (body_id=102 to 164), then you just ate yourself, game over
					snake_health = SNAKE_DEAD;
			break;

		case GOING_DOWN:
				if(curr_head_row != 7)
					next_tile_dataBit = screen_get_bit(curr_head_row+1, curr_head_col);
				else
					next_tile_dataBit = screen_get_bit(0, curr_head_col); //edge case handling: if the snake head at the bottom-edge, wrap-around to the top side

				if( next_tile_dataBit >= 102 && next_tile_dataBit <= 164 ) //if there is a snake body (body_id=102 to 164), then you just ate yourself, game over
					snake_health = SNAKE_DEAD;
			break;

		case GOING_RIGHT:
				if(curr_head_col != 7)
					next_tile_dataBit = screen_get_bit(curr_head_row, curr_head_col+1);
				else
					next_tile_dataBit = screen_get_bit(curr_head_row, 0);	//edge case handling: if the snake head at the right-edge, wrap-around to the left side

				if( next_tile_dataBit >= 102 && next_tile_dataBit <= 164 ) //if there is a snake body (body_id=102 to 164), then you just ate yourself, game over
					snake_health = SNAKE_DEAD;
			break;
	}

	return snake_health;
}

void game_over_animation()
{
	uint8_t head_row, head_col;

	//hide food
	screen_set_bit(food_row, food_column, 0);

	//find the head
	for(int row=0; row<8; row++)
	{
		for(int col=0; col<8; col++)
		{
			if(screen_get_bit(row, col) == 100)
			{
				head_row = row;
				head_col = col;
			}
		}
	}

	//blink the head
	for(int i=0; i<30; i++)
	{
		screen_set_bit(head_row, head_col, 0);
		HAL_Delay(50);
		screen_set_bit(head_row, head_col, 100);
		HAL_Delay(50);
	}


	//flash the screen 10 times
	for(int i=0; i<3; i++)
	{
		screen_clear();
		HAL_Delay(100);
		screen_fill();
		HAL_Delay(100);
	}
	screen_clear();

	//regenerate new snake
	game_startNewGame();
}

void increment_snake_body()
{
	for(int row=0; row<8; row++)
	{
		for(int col=0; col<8; col++)
		{
			uint8_t current_dataBit = screen_get_bit(row, col);
			if(current_dataBit >= 100 && current_dataBit <= 164)
			{
				uint8_t new_dataBit = current_dataBit + 1;
				screen_set_bit(row, col, new_dataBit);
			}
		}
	}
}

void remove_snake_tail()
{
	for(int row=0; row<8; row++)
	{
		for(int col=0; col<8; col++)
		{
			if(screen_get_bit(row, col) == 100 + snake_lenght)
			{
				screen_set_bit(row, col, 0);
			}
		}
	}
}

void snake_set_new_direction(uint8_t new_direction)
{
	pending_new_direction = new_direction;
}

uint8_t process_new_direction_request()
{
	uint8_t temp = pending_new_direction;
	pending_new_direction = NO_DIRECTION_CHANGE;
	return temp;
}
