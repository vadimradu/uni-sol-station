#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "lcd_char.h"


/*
** constant definitions
*/


/*
** function prototypes
*/ 


int main(void)
{
    /*defs*/
    
    /* initialize display, cursor off */
    lcd_init(LCD_DISP_ON);
    lcd_clrscr();
    lcd_home();
    lcd_puts("SolSt starting...");
    /*periphereal init*/
    
    lcd_puts("ok.");
    //a delay should be inserted
    while (1) {  
    /* loop forever */
    //should create menu
    }
}
