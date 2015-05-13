/* fisier: main.c
 * 12/07/2014 vadim@anti
 * Bazat pe codul original de la: https://www.openhub.net/p/avr-pid-solder-station?ref=sample
 * Controllerul pentru statia de lipit se foloseste de doua interfete pentru comenzi.
 * Prima este interfata seriala(baud:9600 start8stop) si trei butoane (- + ok)
 * Controlul temperaturii se face cu un algoritm PID, cu parametrii modificabili
 *
 * Control serial: 	solst set <parameter> <value>	-parametrii configurabili (KP KI KD KT)
 *	    			solst log <0/1>					-in scopuri de debug sau urmarirea comportamentului
 *					solst config					-afiseaza parametrii setati
 *					solst eeprom					-afiseaza valorile salvate in eeprom
 *					solst save						-salveaza noii parametrii in memorie
 *					solst temp						-seteaza temperatura dorita
 *
 * Nota: Programul actual este construit pentru un ATmega8, dar poate rula pe orice procesor AVR cu cel putin 2 timere, PWM,
 *	interfata seriala si, optional, pini pentru butoane si afisaj.
*/

#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "variables.h"
#include "lcd_char.h"
#include "timer.h"


int main(void)
{
    /*defs*/
    /*periphereal init*/
    /* initialize display, cursor off */
    lcd_init(LCD_DISP_ON);    lcd_clrscr();    lcd_home();
    /* intitialize timers, as follows: timer0-button_input/menu_display, timer1-heater pwm, timer2-temp reading */
    TIMER0_init(); //isr takes care of menu display
    TIMER1_init(); //pwm generator for heater
    TIMER2_init(); //temp reading and pid
    
    while (1) {  
    /* loop forever */
    //should create menu
    }
}
