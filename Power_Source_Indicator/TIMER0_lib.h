/*
 * TIMER0_lib.h
 *
 * Created: 10/30/2023 11:03:45 AM
 *  Author: VolodymyrBatih
 */ 


#ifndef TIMER0_LIB_H_
#define TIMER0_LIB_H_

#include <avr/interrupt.h>

typedef enum timer_status{
	TIMER_OK,
	TIMER_ERROR
}timer_status_t;

void timer1_init(void);
timer_status_t  timer1_delay(uint16_t time_period_ms);


#endif /* TIMER0_LIB_H_ */