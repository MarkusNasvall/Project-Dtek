/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declarations for these labs */

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

/* Counter for the tick function */
volatile int tick_counter = 0;

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  /* 1 is input and 0 is output */
  /* We need to set the least signifacnt 8 bits to 0 to make them outputs */
  /* Create a volatile pointer to TRISE */
  /* Typecast onto the address to handle the error of trying assign an int onto a uint32_t pointer */
  volatile uint32_t * trise = (volatile uint32_t *) 0xbf886100;

  /* Set the 8 least significant bits to 0, keeping other bits unchanged */
  /* The way this works is by dereferencing the pointer and then performing a logical AND operation. The ~ is used to invert the bits of 0xFF which makes the 8 LSB's equal to zero */
  *trise &= ~0xFF;

  /* Initialize port D so that bits 11 through 5 will be input or set to 1 */
  /* This can be done by using the logical OR operator on the already defined TRISD with the hexadecimal 0x0FE0 */
  TRISD |= 0x0FE0;

  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  delay( 1000 );
  time2string( textstring, mytime );
  display_string( 3, textstring );
  display_update();
  tick( &mytime );

  tick_counter++;  // Increment the counter each time tick is called
  /* We declare the pointer like we did earlier in the labinit function */
  volatile uint32_t * portE = (volatile uint32_t *) 0xbf886110;
  /* We dereference the pointer and set the actual value to the tick counter */
  *portE = tick_counter;

  display_image(96, icon);

  // Check if any button is pressed using getbtns function
  int buttons = getbtns();
  if(buttons) { // If any button is pressed
      int switches = getsw(); // Get the state of the switches

      // For BTN4: Copy SW4-SW1 to the first digit of mytime
      // We check for 0x4 because in binary it looks like 0100 which means that button 4 was pressed
      if(buttons & 0x4) {
          // We mask my time with 0x0FFF because it looks like this in binary: 0000 1111 1111 1111 which will clear the bits of the first digit in "mytime" variable, we then use the logical OR operator | to combine the two values. After that, we move the 4 LSB's in "switches" 12 steps to the left and insert them in the 4 MSB's in "mytime"
          mytime = (mytime & 0x0FFF) | (switches << 12);
      }

      // For BTN3: Copy SW4-SW1 to the second digit of mytime
      if(buttons & 0x2) {
          mytime = (mytime & 0xF0FF) | (switches << 8);
      }

      // For BTN2: Copy SW4-SW1 to the third digit of mytime
      if(buttons & 0x1) {
          mytime = (mytime & 0xFF0F) | (switches << 4);
      }
  }
}
