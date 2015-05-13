/* fisier: main.c
 * 12/07/2014 vadim@anti
 * Bazat pe codul original de la: https://www.openhub.net/p/avr-pid-solder-station?ref=sample
 * Controllerul pentru statia de lipit se foloseste de doua interfete pentru comenzi.
 * Prima este interfata seriala(baud:9600 start8stop) si trei butoane (- + ok)
 * Controlul temperaturii se face cu un algoritm PID, cu parametrii modificabili
 *
 * Control serial: 	solst set <parameter> <value>	-parametrii configurabili (KP KI KD KT)
 *	    			solst log <0/1>		-in scopuri de debug sau urmarirea comportamentului
 *				solst config		-afiseaza parametrii setati
 *				solst eeprom		-afiseaza valorile salvate in eeprom
 *				solst save		-salveaza noii parametrii in memorie
 *				solst temp		-seteaza temperatura dorita
 *
 * Nota: Programul actual este construit pentru un ATmega8, dar poate rula pe orice procesor AVR cu cel putin 2 timere, PWM,
 *	interfata seriala si, optional, pini pentru butoane si afisaj.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "variables.h"
#include "lcd_char.h"
#include "timer.h"
#include "usart.h"
#include "pid.h"
#include "spi.h"
#include "moving_average.h"
#include "hardware.h"


int main(void)
{
    /*defs*/
    /*periphereal init*/
    cli();
    /* initialize display, cursor off */
    lcd_init(LCD_DISP_ON);    lcd_clrscr();    lcd_home();	lcd_puts("SolSt starting...");
    /* intitialize timers, as follows: timer0-button_input/menu_display, timer1-heater pwm, timer2-temp reading */
    TIMER0_init(); //isr takes care of menu display
    TIMER1_init(); //pwm generator for heater
    TIMER2_init(); //temp reading and pid
    USART_init();  //usart comm setup
    fdevopen(USART_putchar,USART_getchar); 	//setup stdio streams
    
    // read pid parameters from eeprom
	pid_s.KP = eeprom_read_byte(&EEMEM_KP);
	pid_s.KI = eeprom_read_byte(&EEMEM_KI);
	pid_s.KD = eeprom_read_byte(&EEMEM_KD);
	pid_s.KT = eeprom_read_byte(&EEMEM_KT);

	// eeprom is uninitialized - let's fallback to some safe settings
	if (pid_s.KP == 0xff && pid_s.KI == 0xff && pid_s.KD == 0xff && pid_s.KT == 0xff){
		// This one were taken from my own tunnings
		pid_s.KP = 8;		pid_s.KI = 22;		pid_s.KD = 4;		pid_s.KT = 30;
	}
    
    sei();
    
    // Be polite and introduce itself :-)
	printf_P(STATUS, pid_s.KP, pid_s.KI, pid_s.KD, pid_s.KT);
	printf_P(OK);
	lcd_puts("ok.");
	
    while (1) {  
    	menu();
    	usart_comm();
    }
    return 0;
}
