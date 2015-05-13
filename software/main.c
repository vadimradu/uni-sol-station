/* fisier: main.c
 * 12/07/2014 vadim@anti
 * Bazat pe codul original de la: https://www.openhub.net/p/avr-pid-solder-station?ref=sample
 * Controllerul pentru statia de lipit se foloseste de doua interfete pentru comenzi.
 * Prima este interfata seriala(baud:9600 start8stop) si trei butoane (- + ok)
 * Controlul temperaturii se face cu un algoritm PID, cu parametrii modificabili
 *
 * Control serial: 	solst set <parameter> <value>	-parametrii configurabili (KP KI KD KT)
 *	    			solst log <0/1>			-in scopuri de debug sau urmarirea comportamentului
 *					solst config			-afiseaza parametrii setati
 *					solst eeprom		    -afiseaza valorile salvate in eeprom
 *					solst save				-salveaza noii parametrii in memorie
 *					solst temp				-seteaza temperatura dorita
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
    cli();
    /* initialize display, cursor off */
    lcd_init(LCD_DISP_ON);    lcd_clrscr();    lcd_home();
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
	//should be completed with some lcd printing as well
	printf_P(STATUS, pid_s.KP, pid_s.KI, pid_s.KD, pid_s.KT);
	printf_P(OK);
	
    while (1) {  
    	if (fgets(line,LINE_SIZE-1,stdin)){
			if(strncmp_P(line, PSTR("solst"), 5) == 0){
				if (strncmp_P(line,PSTR("set KP "),7) == 0){ //set KP n
					sscanf_P(line,PSTR("set KP %d"),&tmp); 

					if (tmp < MAX_PID_CONSTANT && tmp >= 0){
						pid_s.KP = (uint8_t)tmp;	printf_P(OK);
					}else{
						printf_P(PARAM_RANGE_MSG,MAX_PID_CONSTANT);
					}
				}else if (strncmp_P(line,PSTR("set KI "),7) == 0){ //set KI n
					sscanf_P(line,PSTR("set KI %d"),&tmp);

					if (tmp < MAX_PID_CONSTANT && tmp >= 0){
						pid_s.KI = (uint8_t)tmp;	printf_P(OK);
					}else{
						printf_P(PARAM_RANGE_MSG,MAX_PID_CONSTANT);
					}
				}else if (strncmp_P(line,PSTR("set KD "),7) == 0){ //set KD n
					sscanf_P(line,PSTR("set KD %d"),&tmp);

					if (tmp < MAX_PID_CONSTANT && tmp >= 0){
						pid_s.KD = (uint8_t)tmp;	printf_P(OK);
					}else{
						printf_P(PARAM_RANGE_MSG,MAX_PID_CONSTANT);
					}
				}else if (strncmp_P(line,PSTR("set KT "),7) == 0){ //set KT n
					sscanf_P(line,PSTR("set KT %d"),&tmp);

					if (tmp < 255 && tmp >= 0){
						pid_s.KT = (uint8_t)tmp;	printf_P(OK);
					}else{
						printf_P(PARAM_RANGE_MSG,255);
					}
				}else if (strncmp_P(line,PSTR("log "),3) == 0){ // log 0/1
					sscanf_P(line,PSTR("log %d"),&tmp);

					if (tmp == 0 || tmp == 1)	log_enable = (uint8_t)tmp;
					if (log_enable)				printf_P(PSTR("setpoint\ttemperature\tpower\n"));
					else						printf_P(OK);
				}else if (strncmp_P(line,PSTR("save"),4) == 0){
					cli();
					eeprom_busy_wait();
					eeprom_write_byte(&EEMEM_KP, pid_s.KP);
					eeprom_write_byte(&EEMEM_KI, pid_s.KI);
					eeprom_write_byte(&EEMEM_KD, pid_s.KD);
					eeprom_write_byte(&EEMEM_KT, pid_s.KT);
					sei();
					printf_P(OK);
				}else if (strncmp_P(line,PSTR("config"),6) == 0){
					printf_P(STATUS,pid_s.KP,pid_s.KI,pid_s.KD,pid_s.KT);
					printf_P(OK);
				}else if (strncmp_P(line,PSTR("eeprom"),6) == 0){
					printf_P(STATUS, \
						eeprom_read_byte(&EEMEM_KP), \
						eeprom_read_byte(&EEMEM_KI), \
						eeprom_read_byte(&EEMEM_KD), \
						eeprom_read_byte(&EEMEM_KT));
					printf_P(OK);
				}else{
					printf_P(PSTR("?\n>"));
				}
			}
		}
    }
    return 0;
}
