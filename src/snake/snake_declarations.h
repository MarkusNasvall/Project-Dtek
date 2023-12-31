#define NUM_ROWS 32
#define NUM_COLUMNS 128

/* Declare a 2D map array that tells us which pixel in the OLED screen will be lit. The screen contains 32 rows and 128 columns. */
uint8_t TwoD_Map[32][128];
/* Declare an array that contains all the pixels in one dimension. It's size is 512 because 32*128=4096 and the size of an unsigned integer is 1 byte = 8 bits, so 4096 / 8 = 512*/
uint8_t OneD_Map[512];

#define MAX_SNAKE_LENGTH 512
#define INITIAL_SNAKE_LENGTH 10  // Initial size of the snake
extern int globalBegin;
extern int score;
extern int speed;

// Snake object
struct Snake {
    char direction; 
    int length;
    int tailX[MAX_SNAKE_LENGTH]; 
    int tailY[MAX_SNAKE_LENGTH]; 
};

// Apple object
struct Apple {
    int x;
    int y;
};

void displayFrame(struct Snake *snake, struct Apple *apple);
void clearDisplay();
void draw_snake(struct Snake *snake);
void startGame(struct Snake *snake, struct Apple *apple);
void playGame(struct Snake *snake, struct Apple *apple);
void Endgame ();
void draw_apple(struct Apple *apple);
