/*
 * TIMER0_lib.c
 *
 * Created: 10/28/2023 11:02:35 PM
 *  Author: VolodymyrBatih
 */ 


#include "TIMER0_lib.h"


void timer1_init(void){
	TCCR1B = 0x00; //timer stopped
	TIMSK |= 1<<TOIE1; // overflow interrupt enable
}

timer_status_t  timer1_delay(uint16_t time_period_ms){
	if(time_period_ms <= 4000){
		uint16_t delay = time_period_ms/64*1000;
		uint8_t low_byte = delay & 0xFF;
		uint8_t high_byte = (delay>>8) & 0xFF;
		TCNT1L = (uint8_t)low_byte;
		TCNT1H = (uint8_t)high_byte;
		TCCR1B |= 1<<CS10 | 1<<CS11; // start timer, 64 divider
		return TIMER_OK;
	}
		return TIMER_ERROR;
}
