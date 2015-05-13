/* fisier: isr.c
 * 13/07/2014 vadim@home
 * Bazat pe codul original de la: https://www.openhub.net/p/avr-pid-solder-station?ref=sample
 *  
 * Rutinele pentru intreruperile timerelor se trateaza aici.
 * de adaugat: -procesare input de la butoane 
 */
 
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <stdio.h>
#include <stdint.h>

#include "moving_average.h"
#include "max31855.h"
#include "pid.h"

#define POWER_SCALE	((PID_TOP/3)+1)
//#define MAX_SETPOINT ((450/4)+1)
//#define MAX_ADC_READOUT ((511/4)+1)
//#define SETPOINT_OFFSET 0

volatile uint16_t temperature_sensor;		// temperature value read from max31855
volatile uint16_t setpoint;					    // setpoint value
volatile uint16_t power;					      // regulator output power
volatile uint8_t display_setpoint;			// setpoint display delay timer
//volatile uint8_t thermocouple_ok;			// thermocouple connected - unused

extern int16_t temperature;
extern uint8_t log_enable;					// should we send log to RS232?
extern pid_t pid_s;							    // pid struct
extern uint8_t button=0;
extern uint8_t button_activity=0;
ISR(TIMER0_OVF_vect){
	//capture button events
	if(bit_is_clear(MINUS_BUTTON, 1)){
		button =1;
		button_activity=1;
	}
	if(bit_is_clear(PLUS_BUTTON, 1)){
		button =2;
		button_activity=1;
	}
	if(bit_is_clear(OK_BUTTON, 1)){
		button =3;
		button_activity=1;
	}
}

ISR(TIMER2_OVF_vect){
	uint16_t temperature_read;
	static uint8_t slow_down;
	static movingaverage_t filter_struct;

	// read temperature from thermocouple
	// smooth readings with moving average filter

	if (slow_down == 0)	{
		temperature_read = read_temperature();
		slow_down = 15;

		// leave out fraction part from readings
		temperature_read = moving_average(&filter_struct,temperature_read,8)>>2;
		power = pid(setpoint,temperature_sensor,&pid_s);
		OCR1A = power;	//set PWM fill factor
	}else{
		slow_down--;
		return;
	}


	if (temperature_sensor != temperature_read)	{
		temperature_sensor = temperature_read;

	if (log_enable)
		printf_P(PSTR("%d\t%d\t%d\n"),setpoint,temperature,power);

}
