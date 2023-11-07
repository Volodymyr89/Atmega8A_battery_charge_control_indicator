/*
 * TIMER0_lib.h
 *
 * Created: 10/30/2023 11:03:45 AM
 *  Author: VolodymyrBatih
 */ 


#ifndef TIMER0_LIB_H_
#define TIMER0_LIB_H_

#define F_CPU 1000000UL

#include <avr/interrupt.h>
#include <util/delay.h>

typedef enum timer_status{
	TIMER_OK,
	TIMER_ERROR
}timer_status_t;

timer_status_t  timer1_delay(uint16_t time_period_ms);
void fail(void);

#endif /* TIMER0_LIB_H_ */