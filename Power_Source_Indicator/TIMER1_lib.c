/*
 * TIMER0_lib.c
 *
 * Created: 10/28/2023 11:02:35 PM
 *  Author: VolodymyrBatih
 */ 


#include "TIMER0_lib.h"



timer_status_t  timer1_delay(uint16_t time_period_ms){

	if(time_period_ms <= 4000){
		cli();
		TCCR1B |= 1<<WGM12;
		/* Disable interrupts */
		uint16_t delay = (time_period_ms/64)*1000;
		OCR1A = delay;
		/* Restore Global Interrupt Flag */
		TCCR1B |= 1<<CS10 | 1<<CS11; // start timer, 64 divider
		TIMSK |= (1<<OCIE1A); // overflow interrupt enable
		sei();
		return TIMER_OK;
	}
		return TIMER_ERROR;
}

void fail(void){
	while(1){
		PORTD ^= 1<<PORTD0;
		_delay_ms(70); //fail
	}
}

