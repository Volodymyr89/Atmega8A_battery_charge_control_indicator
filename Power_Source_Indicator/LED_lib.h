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

typedef enum charger_control{
	CHARGER_OFF,
	CHARGER_ON,
}charger_control_t;


void leds_check_greeting_startup(void);
void leds_and_pins_init(void);
void leds_show_status(const adc_data_t *adc_data, bool charger_plugged_in_status);
bool charger_status(void);
void charger_control(charger_control_t charger_control);



#endif /* LED_LIB_H_ */