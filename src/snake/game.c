/* game.c

   This file written 2023 by M. Ba Rashd and M. Näsvall
    
    */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include "pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "snake_declarations.h" // For object declarations
#include <stdbool.h>

// Define the parameters for the LCG
#define PRNG_A 1664525
#define PRNG_C 1013904223

// Variable to begin the game
int globalBegin = 0;

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

    for (k = 0; k < snake->length; k++) {
        x = snake->tailX[k];
        y = snake->tailY[k];

        TwoD_Map[y][x] = 1;
    }
}

void draw_apple(struct Apple *apple){
    int x, y;
    x = apple->x;
    y = apple->y;
    TwoD_Map[y][x] = 1;
}


/*
_______________________________________________
Snake functions

*/

void initializeSnake(struct Snake *snake) {
    snake->length = 10;  
    snake->direction = 'L';
    int i;
    for (i = 0; i < snake->length; ++i) {
        snake->tailY[i] = 16;
        snake->tailX[i] = 64 + i;
    }
}

void changeXYDirection(struct Snake *snake) {
    int buttons = getbtns();
    if (buttons) {
        /* Here we will use single characters to resemble directions since it is a bit complicated to compare strings in C:
        U = up
        D = down
        R = right
        L = left*/
        // BTN4: Decrease y-coordinate
        if (snake->direction != 'R') {
            if (buttons & 0x8) {
            snake->direction = 'L';
            }
        }
        // BTN3: Increase y-coordinate
        if (snake->direction != 'U') {
            if (buttons & 0x4) {
            snake->direction = 'D';
            }
        }
        // BTN2: Decrease x-coordinate
        if (snake->direction != 'D') {
            if (buttons & 0x2) {
            snake->direction = 'U';
            }
        }
        // BTN1: Increase x-coordinate
        if (snake->direction != 'L') {
            if (buttons & 0x1) {
            snake->direction = 'R';
            }
        }
    }

    //Update the tail positions by shifting them
    int i;
    for (i = snake->length - 1; i > 0; --i) {
        snake->tailX[i] = snake->tailX[i - 1];
        snake->tailY[i] = snake->tailY[i - 1];
    }

    // Change coordinates based on snake's direction
    if (snake->direction == 'D') {
        snake->tailY[0]++;
    }

    if (snake->direction == 'U') {
        snake->tailY[0]--;
    }

    if (snake->direction == 'L') {
        snake->tailX[0]--;
    }

    if (snake->direction == 'R') {
        snake->tailX[0]++;
    }
}

void wallCollision(const struct Snake *snake) {
    if (snake->tailX[0] == 0 || snake->tailX[0] == 127 ||
        snake->tailY[0] == 0 || snake->tailY[0] == 31) {
            globalBegin = 0;
        }
}

void selfCollision(const struct Snake *snake) {
    int i;
    for (i = 1; i < snake->length; ++i) {
        if (snake->tailX[0] == snake->tailX[i] && snake->tailY[0] == snake->tailY[i]) {
            globalBegin = 0;
        }
    }
}

void increaseLength(struct Snake *snake) {
    int i, j;
    int newY, newX;
    
    for (j = 0; j < 5; j++) {
        // This will create a new segment of the snake based on which direction it goes to
        switch (snake->direction) {
            case 'U':
                newY = snake->tailY[0] - 1;
                newX = snake->tailX[0];
                break;
            case 'D':
                newY = snake->tailY[0] + 1;
                newX = snake->tailX[0];
                break;
            case 'L':
                newY = snake->tailY[0];
                newX = snake->tailX[0] - 1;
                break;
            case 'R':
                newY = snake->tailY[0];
                newX = snake->tailX[0] + 1;
                break;
        }

		// Shift existing segments to make room for the new one
        for (i = snake->length; i > 0; i--) {
            snake->tailY[i] = snake->tailY[i - 1];
            snake->tailX[i] = snake->tailX[i - 1];
        }

        // Add the new segment at the front 
        snake->tailY[0] = newY;
        snake->tailX[0] = newX;

        // Increase the length of the snake
        snake->length++;
    }
}

/*
_______________________________________________
Apple functions

*/

void initializeApple(struct Apple *apple) {
    // Set initial coordinates for the apple
    apple->x = getRandomNumber(126) + 1;
    apple->y = getRandomNumber(30) + 1;
}

int checkAppleCollision(struct Snake *snake, struct Apple *apple) {
    if (snake->tailX[0] == apple->x && snake->tailY[0] == apple->y) {
        return 1; 
    }
    return 0;  
}

void AppleCollisionActions(struct Snake *snake, struct Apple *apple) {
    if (checkAppleCollision(snake, apple)) {
        apple->x = getRandomNumber(126) + 1;
        apple->y = getRandomNumber(30) + 1;
        increaseLength(snake);
    }
    snake->score++;
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
            display_string(0, "Start game");
            display_string(1, "by pressing BTN2");
            display_update();
            buttons = getbtns();
        } 
        //Exit loop
        running = 0;
    }
    globalBegin = 1;
    initializeApple(apple); 
    initializeSnake(snake);

}

void playGame(struct Snake *snake, struct Apple *apple) {
    while(globalBegin) {
        displayFrame(snake, apple);
        changeXYDirection(snake);

        wallCollision(snake);
        selfCollision(snake);
        if (checkAppleCollision(snake, apple)) {
            AppleCollisionActions(snake, apple);
        }
    }    
}

void Endgame () {
    int buttons = getbtns();
    int running = 1;
    while(running) {
        while(!(buttons & 0x8)) {
            clearDisplay();
            display_string(0, "Game over,");
            display_string(1, "end game by");
            display_string(2, "pressing BTN4");
            display_update();
            //BTN4
            buttons = getbtns();
        }
        display_string(2, "\n");
        display_update();
        running = 0;

    }
    struct Snake newSnake;
	struct Apple newApple;
    startGame(&newSnake, &newApple);
}















