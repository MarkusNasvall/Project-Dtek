#define NUM_ROWS 32
#define NUM_COLUMNS 128

/*
// Define the structure for the snake
typedef struct {
    int length;
    int speed;
    char direction; 
    uint8_t x[NUM_COLUMNS];
    uint8_t y[NUM_ROWS];
} Snake;

// Define a variable of type Snake
Snake snake;
*/

/*
// Define the structure for the apples
typedef struct {
    uint8_t x[NUM_COLUMNS];
    uint8_t y[NUM_ROWS];
} Apple;

// Define a variable of type snake
Apple apple;
*/

#define MAX_SNAKE_LENGTH 50

struct Snake {
    int x;
    int y;
    int length;
    int tailX[MAX_SNAKE_LENGTH]; 
    int tailY[MAX_SNAKE_LENGTH]; 
};

struct Apple {
    int x;
    int y;
};


