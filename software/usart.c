/* fisier: usart.c
 * 18/07/2014 vadim@home
 * Bazat pe codul original de la: https://www.openhub.net/p/avr-pid-solder-station?ref=sample
 * initializare uart
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "hardware.h"
#include "usart.h"

ISR(USART_RXC_vect)
{
	//got byte
	char c;
	c = UDR;

	// minimal processing to make stdio functions happy
	if (c == '\r')
		c = '\n';

	RxBuf[rx_end++ & RXMASK] = c;

}

ISR(USART_UDRE_vect)
{
	//send byte
	if(tx_end != tx_start)
	{
		UDR = TxBuf[tx_start++ & TXMASK];
	}
	else
	{
		//ring buffer empty
		UCSRB &= ~(1<<UDRIE);
	}
}

int USART_putchar(char c, FILE *fp)
{
	//send char or wait if buffer is full
	while((tx_end-tx_start)>TXBUFSIZE-1);

		TxBuf[tx_end++ & TXMASK] = c;

	UCSRB |= (1<<UDRIE);			// Enable UDR empty interrupt

	return (uint8_t)c;
}

int USART_getchar(FILE *fp)
{

	while((rx_end - rx_start) == 0);

	return RxBuf[rx_start++ & RXMASK];
}

void USART_init(void)
{
	// set baudrate
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;

#ifdef USART_2X
	UCSRA |= (1<<U2X);
#endif

	// enable receiver and transmitter
	// enable receiver interrupt
	// set frame format to 8bit and 1 stop bit
	UCSRB = (1<<RXCIE)|(1<<TXEN)|(1<<RXEN);
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
}

  void usart_comm(void){
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
