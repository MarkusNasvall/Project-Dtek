#define NUM_ROWS 32
#define NUM_COLUMNS 128

/* Declare a 2D map array that tells us which pixel in the OLED screen will be lit. The screen contains 32 rows and 128 columns. */
uint8_t TwoD_Map[32][128];
/* Declare an array that contains all the pixels in one dimension. It's size is 512 because 32*128=4096 and the size of an unsigned integer is 1 byte = 8 bits, so 4096 / 8 = 512*/
uint8_t OneD_Map[4000];

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
    int speed;
    char direction; 
    int length;
    int tailX[MAX_SNAKE_LENGTH]; 
    int tailY[MAX_SNAKE_LENGTH]; 
};

struct Apple {
    int x;
    int y;
};


void beginDisplay();
