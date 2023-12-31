/*
 * Power_Source_Indicator.c
 *
 * Created: 10/26/2023 12:05:23 PM
 * Author : VolodymyrBatih
 */ 



#include <avr/io.h>
#include "config.h"
#include "ADC_lib.h"
#include "TIMER1_lib.h"
#include "LED_lib.h"

volatile bool isr_run_adc_convertion = false;

ISR(TIMER1_COMPA_vect)
{
	isr_run_adc_convertion = true;
}


int main(void)
{
	adc_data_t adc_data={0,0};
	relay_control(CHARGER_OFF);	
	COOLER_OFF;
		
	leds_and_pins_init();
	adc_init();
	adc_read(&adc_data);
	
	if (charger_status()){
		if (timer1_delay(TIMER_FOR_CHARGING) == TIMER_OK){}
			else{fail();}
	}
	else{
		if (timer1_delay(TIMER_FOR_DISCHARGING) == TIMER_OK){}
		else{fail();}
	}
	
	leds_check_greeting_startup();

    while (1) 
    {
		if(isr_run_adc_convertion){
			isr_run_adc_convertion=false;
			adc_read(&adc_data);
			leds_show_status(adc_data, charger_status());
		}
	}

}

