/*
 * TIMER0_lib.c
 *
 * Created: 10/28/2023 11:02:35 PM
 *  Author: VolodymyrBatih
 */ 


#include "TIMER2_lib.h"

#define PWM_MAX   255

void timer2_pwm_init(void){
	TCCR2 |= 1<<COM21; // Clear OC2 (PB3) on Compare Match, set OC2 at BOTTOM 
	TCCR2 |= 1<<CS21; // clkT2S/8 (From prescaler)
}

void pwm_on(void)
{
	TCCR2 |= 1<<WGM21 | 1<<WGM20; // turn on OC2	
}

void pwm_off(void)
{
	TCCR2 &= ~((1<<WGM21) | (1<<WGM20)); // turn off OC2
}


pwm_status_t  timer2_pwm_ctl(uint16_t time_pwm){
	pwm_status_t status = PWM_ERROR;
	uint8_t pwm = 0;
	uint8_t byte_time_pwm = 0;
	if (time_pwm <= 800)
	{
		time_pwm = 800;
	}
	byte_time_pwm = time_pwm * 0xFF;
	pwm = byte_time_pwm + 10;
	if(pwm >=255)
	{
		pwm = 255;
	}
	OCR2 = pwm;
	status = PWM_OK;
	return status;
}

