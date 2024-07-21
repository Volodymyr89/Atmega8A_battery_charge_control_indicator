/*
 * TIMER0_lib.c
 *
 * Created: 10/28/2023 11:02:35 PM
 *  Author: VolodymyrBatih
 */ 


#include "TIMER1_lib.h"

timer_status_t  timer1_delay_scan(uint16_t time_period){
	if(time_period <= ((uint16_t)14001)){
		cli(); //Disable interrupts 
		TCCR1B = 0; // clear dividers
		TCCR1B |= 1<<WGM12;
		uint16_t delay = (uint16_t)time_period;
		OCR1A = delay;
		/* Restore Global Interrupt Flag */
		TCCR1B |= 1<<CS11; // start timer, 8 divider
		TIMSK |= (1<<OCIE1A); // overflow interrupt enable
		sei();
		return TIMER_OK;
	}
		return TIMER_ERROR;
}

timer_status_t  timer1_delay_stby(uint16_t time_period_ms){
	if(time_period_ms <= ((uint16_t)60001)){
		cli(); //Disable interrupts
		TCCR1B = 0; // clear dividers
		TCCR1B |= 1<<WGM12;
		uint16_t delay = (uint16_t)time_period_ms;
		OCR1A = delay;
		/* Restore Global Interrupt Flag */
		TCCR1B |= 1<<CS10 | CS12; // start timer, 1024 divider
		TIMSK |= (1<<OCIE1A); // overflow interrupt enable
		sei();
		return TIMER_OK;
	}
	return TIMER_ERROR;
}


void fail(void){
	while(1){
		PORTD ^= 1<<PORTD0;
		_delay_ms(100); //fail
	}
}

