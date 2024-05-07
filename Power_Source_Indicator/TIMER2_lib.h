/*
 * TIMER2_lib.h
 *
 * Created: 10/30/2023 11:03:45 AM
 *  Author: VolodymyrBatih
 */ 


#ifndef TIMER2_LIB_H_
#define TIMER2_LIB_H_

#define F_CPU 1000000UL

#include <avr/interrupt.h>
#include <util/delay.h>

typedef enum pwm_status{
	PWM_OK,
	PWM_ERROR
}pwm_status_t;

void  timer2_pwm_init(void);
pwm_status_t  timer2_pwm_ctl(uint8_t time_pwm);
void pwm_on(void);
void pwm_off(void);

#endif /* TIMER2_LIB_H_ */