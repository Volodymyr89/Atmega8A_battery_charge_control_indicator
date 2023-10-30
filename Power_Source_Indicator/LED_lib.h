/*
 * LED_lib.h
 *
 * Created: 10/26/2023 12:37:57 PM
 *  Author: VolodymyrBatih
 */ 


#ifndef LED_LIB_H_
#define LED_LIB_H_

void leds_check_greeting_startup(void);
void leds_init(void);
void leds_show_status(const adc_data_t adc_data, bool charger_plugged_in_status);
bool charger_status(void);



#endif /* LED_LIB_H_ */