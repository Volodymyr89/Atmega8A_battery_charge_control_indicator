/*
 * LED_lib.h
 *
 * Created: 10/26/2023 12:37:57 PM
 *  Author: VolodymyrBatih
 */ 


#ifndef LED_LIB_H_
#define LED_LIB_H_

#define F_CPU 1000000UL

#include <util/delay.h>

typedef enum relay_control{
	CHARGER_RELAY_OFF,
	CHARGER_RELAY_ON,
}relay_control_t;


void leds_check_greeting_startup(void);
void leds_and_pins_init(void);
void leds_show_status(const adc_data_t adc_data, bool charger_plugged_in_status);
bool charger_status(void);
void relay_control(relay_control_t relay_control);



#endif /* LED_LIB_H_ */