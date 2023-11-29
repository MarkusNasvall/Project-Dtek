/* game.c

   This file written 2023 by M. Ba Rashd and M. Näsvall
    
    */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include "pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "snake_declarations.h" // For object declarations
#include <stdlib.h>  // For rand() function
#include <stdbool.h>

// Define the parameters for the LCG
#define PRNG_A 1664525
#define PRNG_C 1013904223

// Global variable to store the current state of the PRNG
static uint32_t prng_state = 0;

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}


/*
_____________________________________________________
Function to draw the snake in a 2D array

*/

void draw_snake(struct Snake *snake){
    int k, x, y;

    for (k = 0; k < MAX_SNAKE_LENGTH; k++) {
        x = snake->tailX[k];
        y = snake->tailY[k];

        TwoD_Map[y][x] = 1;
    }
}


/*
_______________________________________________
Snake functions

*/

void initializeSnake(struct Snake *snake) {
    snake->x = getRandomNumber(100);
    snake->y = getRandomNumber(100);
    
    snake->length = 0;  
    snake->direction = 'L';
}

void changeXYDirection(struct Snake *snake) {
    int buttons = getbtns();
    if (buttons) {
        // Save the current head position before updating
        int currentHeadX = snake->x;
        int currentHeadY = snake->y;

        /* Here we will use single characters to resemble directions since it is a bit complicated to compare strings in C:
        U = up
        D = down
        R = right
        L = left*/
        // BTN4: Decrease y-coordinate
        if (snake->direction != 'U') {
            if (buttons & 0x8) {
            snake->direction = 'D';
            }
        }
        // BTN3: Increase y-coordinate
        if (snake->direction != 'D') {
            if (buttons & 0x4) {
            snake->direction = 'U';
            }
        }
        // BTN2: Decrease x-coordinate
        if (snake->direction != 'R') {
            if (buttons & 0x2) {
            snake->direction = 'L';
            }
        }
        // BTN1: Increase x-coordinate, is it correct?
        if (snake->direction != 'L') {
            if (buttons & 0x1) {
            snake->direction = 'R';
            }
        }
    }

    // Change coordinates based on snake's direction
    if (snake->direction == 'D') {
        snake->y--;
    }

    if (snake->direction == 'U') {
        snake->y++;
    }

    if (snake->direction == 'L') {
        snake->x--;
    }

    if (snake->direction == 'R') {
        snake->x++;
    }

    // Update the tail positions by shifting them
    int i;
    for (i = snake->length - 1; i > 0; --i) {
        snake->tailX[i] = snake->tailX[i - 1];
        snake->tailY[i] = snake->tailY[i - 1];
    }

    // Update the first tail segment with the previous head position
    int currentHeadX = snake->tailX[0];
    int currentHeadY = snake->tailY[0];
}

void modifySnakeLength(struct Snake *snake, int newLength) {
    snake->length = newLength;
}

bool wallCollision(const struct Snake *snake) {
    if (snake->x == 0) {
        display_string(1, "Crash");
        return 1;
    }
    if (snake->x == 100) {
        display_string(1, "Crash");
        return 1;
    }
    if (snake->y == 0) {
        display_string(1, "Crash");
        return 1;
    }
    if (snake->y == 100) {
        display_string(1, "Crash");
        return 1;
    }
}

bool selfCollision(const struct Snake *snake) {
    int i;
    for (i = 0; i < snake->length; ++i) {
        if (snake->x == snake->tailX[i] && snake->y == snake->tailY[i]) {
            display_string(1, "Crash");
            return 1;
        }
    }
}

/*
_______________________________________________
Apple functions

*/

void initializeApple(struct Apple *apple) {
    // Set initial coordinates for the apple
    apple->x = getRandomNumber(100);
    apple->y = getRandomNumber(100);
}

int checkAppleCollision(struct Snake snake, struct Apple apple) {
    int i;
    for (i = 0; i < snake.length; ++i) {
        if (snake.tailX[i] == apple.x && snake.tailY[i] == apple.y) {
            return 1; 
        }
    }
    return 0;  
}

void AppleCollisionActions(struct Snake snake, struct Apple *apple) {
    if (checkAppleCollision(snake, *apple)) {
        //Chose randomnumber to max be 100, temporarily
        apple->x = getRandomNumber(100);
        apple->y = getRandomNumber(100);
    }
}

/*
____________________________________________________
Helper functions

*/

// Function to generate a pseudo-random integer given a max integer
int getRandomNumber(int max) {
    // Use the address of a local variable as a seed
    uint32_t seed = (uint32_t)&seed;
    // Update the prng_state
    if(prng_state == 0) {
        prng_state = seed;
    }
    prng_state = PRNG_A * prng_state + PRNG_C;

    // Return a pseudo-random number (you can adjust the range as needed)
    return (int)(prng_state % max);
}


/*
_____________________________________________________
Start, Play & End game functions 

*/

void startGame(struct Snake *snake, struct Apple *apple) {
    int buttons = getbtns();
    int running = 1;
    while(running) {
        while(!(buttons & 0x2)) {
            display_string(1, "Start game by pressing BTN2");
            display_update();
            buttons = getbtns();
        } 
        running=0;
        initializeApple(apple); 
        initializeSnake(snake);
    }

}

void playGame(struct Snake *snake, struct Apple *apple) {
    int running = 1;
    while(running) {
        draw_snake(snake);
        beginDisplay();

        changeXYDirection(snake);

        if(wallCollision(snake)) {
            running = 0;
        }
        if(selfCollision(snake)) {
            running = 0;
        }
        if (checkAppleCollision(*snake, *apple)) {
            AppleCollisionActions(*snake, apple);
        }

    }    
}

void Endgame (void) {
    int buttons = getbtns();
    int running = 1;
    while(running) {
        while(!(buttons & 0x4)) {
            //BTN4
            buttons = getbtns();
            display_string(5, "Game over, End game by pressing BTN4");
        }
        running=0;
    }
}















