/*
 * Power_Source_Indicator.c
 *
 * Created: 10/26/2023 12:05:23 PM
 * Author : VolodymyrBatih
 */ 



#include <avr/io.h>
#include "config.h"
#include "ADC_lib.h"
#include "TIMER0_lib.h"
#include "LED_lib.h"

volatile bool isr_run_adc_convertion = false;

ISR(TIMER1_COMPA_vect)
{
	isr_run_adc_convertion = true;
}


int main(void)
{
	adc_data_t adc_data={0,0};
		
	COOLER_OFF;
		
	leds_and_pins_init();
	adc_init();
	
	relay_control(CHARGER_RELAY_OFF);
	relay_control(OUT_RELAY_OFF);

	leds_check_greeting_startup();
	
	if (charger_status()){
		relay_control(OUT_RELAY_ON);
		if (timer1_delay(TIMER_FOR_CHARGING) == TIMER_OK){}
			else{fail();}
	}
	else{
		if (timer1_delay(TIMER_FOR_DISCHARGING) == TIMER_OK){}
		else{fail();}
	}

    while (1) 
    {
			if(isr_run_adc_convertion){
				isr_run_adc_convertion=false;
				if(charger_status()){
					relay_control(OUT_RELAY_ON);
				}else{
					relay_control(OUT_RELAY_OFF);
				}
				adc_read(&adc_data);
				leds_show_status(adc_data, charger_status());
			}
	}

}

