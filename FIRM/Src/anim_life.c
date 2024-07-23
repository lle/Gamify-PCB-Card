/*
 * anim_life.c
 *
 *  Created on: Aug 8, 2023
 *      Author: Ang3l
 */
#include "screenBuffer.h"

//Code from https://github.com/mtagius/game-of-life-8x8-arduino-matrix
//Modified to work with this project.

//this 8x8 array represents the matrix. True means the LED will light up, False means it wont
static uint8_t matrix[8][8];

#define SIZE 8

static char world[SIZE][SIZE][2];
long density = 20;

uint8_t change = 0;
static uint8_t predeterminedDeath = 0;
static int iterations = 0;
static int threeCells = 0;

const static uint8_t mode = 0;

int neighbours(int x, int y) {
  int numberOfNeighbours = 0;
  //if the borders overlap to the other side of the matrix
  if(mode) {
    numberOfNeighbours = world[(x + 1) % SIZE][y][0] +
         world[x][(y + 1) % SIZE][0] +
         world[(x + SIZE - 1) % SIZE][y][0] +
         world[x][(y + SIZE - 1) % SIZE][0] +
         world[(x + 1) % SIZE][(y + 1) % SIZE][0] +
         world[(x + SIZE - 1) % SIZE][(y + 1) % SIZE][0] +
         world[(x + SIZE - 1) % SIZE][(y + SIZE - 1) % SIZE][0] +
         world[(x + 1) % SIZE][(y + SIZE - 1) % SIZE][0];
  } else  {
  //if the borders are solid
    if(x != 7) {
      numberOfNeighbours += world[x + 1][y][0];
    }
    if(y != 7) {
      numberOfNeighbours += world[x][y + 1][0];
    }
    if(x != 0) {
      numberOfNeighbours += world[x - 1][y][0];
    }
    if(y != 0) {
      numberOfNeighbours += world[x][y - 1][0];
    }
    if(x != 7 && y != 7) {
      numberOfNeighbours += world[x + 1][y + 1][0];
    }
    if(x != 7 && y != 0) {
      numberOfNeighbours += world[x + 1][y - 1][0];
    }
    if(x != 0 && y != 7) {
      numberOfNeighbours += world[x - 1][y + 1][0];
    }
    if(x != 0 && y != 0) {
      numberOfNeighbours += world[x - 1][y - 1][0];
    }
  }
  return numberOfNeighbours;
}


void worldCreation() {
   for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (rand()%100 < density) {
        world[i][j][0] = 1;
      }
      else {
        world[i][j][0] = 0;
      }
      world[i][j][1] = 0;
    }
  }
}

void anim_life_setup() {

  //never really figured out if bool[][] initializes to false by default
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      matrix[i][j] = 0;
    }
  }

  worldCreation();

}

void anim_life_run() {
  // Display current generation
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      matrix[i][j] = world[i][j][0];
    }
  }

  screen_display_array(matrix);

  change = 0;
  int liveCells = 0;

  // Birth and death cycle
  for (int x = 0; x < SIZE; x++) {
    for (int y = 0; y < SIZE; y++) {
      // Default is for cell to stay the same
      world[x][y][1] = world[x][y][0];
      int count = neighbours(x, y);
      if (count == 3 && world[x][y][0] == 0) {
        // A new cell is born
        world[x][y][1] = 1;
        change = 1;
      }
      if ((count < 2 || count > 3) && world[x][y][0] == 1) {
        // Cell dies
        world[x][y][1] = 0;
        change = 1;
      }
    }
  }

  // Copy next generation into place
  for (int x = 0; x < SIZE; x++) {
    for (int y = 0; y < SIZE; y++) {
      world[x][y][0] = world[x][y][1];
      if(world[x][y][0]) {
        liveCells++;
      }
    }
  }

  if(change == 0 && predeterminedDeath == 0) {
    predeterminedDeath = 1;
    //if everything stops moving the grid will reset after 1 iteration (no cells alive) or 6 iterations (some cells alive)
    if(liveCells == 0) {
      iterations = 99;
    } else {
      iterations = 94;
    }
  }

  if(change == 1 && predeterminedDeath == 0) {
    if(liveCells == 3) {
      threeCells++;
    }
    if(threeCells >= 5) {
      threeCells = 0;
      predeterminedDeath = 1;
      iterations = 96;
    }
  }

  //forced reset
  if(iterations >= 100) {
    worldCreation();
    iterations = 0;
    predeterminedDeath = 0;
  } else {
    iterations++;
  }

	HAL_Delay(200);
	screen_clear();

}

