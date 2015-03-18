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
    uint8_t pwr= 0;
    /* initialize display, cursor off */
    lcd_init(LCD_DISP_ON);
    lcd_home();
    lcd_puts("SolSt starting..");
    /*periphereal init*/
    //pwm init
	TCCR1B |= (1<<CS11)|(1<<CS10);
    TCCR1A |= (1<<COM1A1)|(1<<WGM11);
    OCR1A = 0;
    DDRB |= (1<<PB1);
	//button init
	#define LED_MASK1 (1<<PC0)
	#define LED_MASK2 (1<<PC1)
	#define LED_PORT PORTC
	#define LED_DDR DDRC
	
	lcd_gotoxy(0,1);
    lcd_puts("ok.");
	_delay_ms(1000);
    lcd_clrscr();
	lcd_home();    
	lcd_puts("Power:");
    while (1) {  
    /* loop forever */
		
	}
}
