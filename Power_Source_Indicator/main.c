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

ISR(TIMER1_OVF_vect)
{
  isr_run_adc_convertion=true;

}



int main(void)
{
	adc_data_t adc_data={0,0};
	adc_init();
	timer1_init();
	adc_enable(true);
	relay_control(CHARGER_RELAY_OFF);
	relay_control(OUT_RELAY_OFF);
	sei(); // enable global interrupt
	
	if (charger_status()){
		if (timer1_delay(300) == TIMER_OK){}
	}
	else{
		if (timer1_delay(4000) == TIMER_OK){}
	}
	
    while (1) 
    {
			if(isr_run_adc_convertion){
				isr_run_adc_convertion=false;
				adc_select_adc0_channel();
				adc_start_conversion();
				while (ADCSRA&(1<<ADSC)){} //wait until conversion is complete
					if(ADCSRA&(1<<ADIF)){
						adc_data.CH0=ADCL;
					}
				adc_select_adc1_channel();
				adc_start_conversion(); //wait until conversion is complete
				while (ADCSRA&(1<<ADSC)){}
					if(ADCSRA&(1<<ADIF)){
						adc_data.CH1=ADCL;
					}
				if((adc_data.CH0) && (adc_data.CH1) >= BATT_FULL){
					relay_control(CHARGER_RELAY_OFF);
				}
				else if((adc_data.CH0) && (adc_data.CH1) <= BATT_FULL_HYSTERESIS){
					relay_control(CHARGER_RELAY_ON);
				}
				leds_show_status(adc_data, charger_status());
			}
	}

}

