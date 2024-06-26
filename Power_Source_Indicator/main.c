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
#include "TIMER2_lib.h"
#include "LED_lib.h"

volatile bool isr_run_adc_convertion = false;

ISR(TIMER1_COMPA_vect)
{
	isr_run_adc_convertion = true;
}


int main(void)
{
	
	adc_data_t adc_data={0, 0};	
	
	leds_and_pins_init();
	adc_init();
	timer2_pwm_init();
	pwm_off();
	leds_check_greeting_startup();
		
    while (1) 
    {	
		#ifdef Debug
		timer1_delay(TIMER_FOR_CHARGING);
		#else
      	if (charger_status() == true){
	      	if (timer1_delay(TIMER_FOR_CHARGING) == TIMER_OK){}
	      	else{fail();}
      	}
      	else{
	      	if (timer1_delay(TIMER_FOR_DISCHARGING) == TIMER_OK){}
	      	else{fail();}
      	}
		#endif	
		if(isr_run_adc_convertion == true){
			isr_run_adc_convertion = false;
			adc_read(&adc_data);
			leds_show_status(&adc_data, charger_status());
		}
		
	}
	

}

