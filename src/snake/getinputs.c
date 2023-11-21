#include <stdint.h>
#include "pic32mx.h"
#include "mipslab.h"  /* Declatations for these labs */

/* Function to read the status of the toggle-switches on the board and return it */
int getsw(void) {
    /* Since SW4 through SW1 are connected to bits 11 through 8, we need to mask PORTD with 0x0F00 which will set all bits other than 11-8 to zero. Then we will shift bits 11-8 8 steps to the right which will put them in bits 3 through 0. Also, we use PORTD instead of TRISD because we only want to read the current status and not configure the direction of the pins */
    int switches = (PORTD & 0x0F00) >> 8; 
    return switches;
}

/* Function to read the current status of the push-buttons BTN2, BTN3, and BTN4 and return it */
int getbtns (void) {
    /* We will do the same as we did in function getsw but this time we will use 0x00E0 since it has the binary representation 0000000011100000 */
    int buttons = (PORTD & 0x00E0) >> 5; 
    return buttons;
}