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
bool timer_and_adc_set = false;

ISR(TIMER1_OVF_vect)
{
  isr_run_adc_convertion=true;

}



int main(void)
{
	adc_data_t adc_data={0,0};
	adc_init();
	timer1_init();
	sei(); // enable global interrupt
 
    while (1) 
    {
		if (charger_status() && timer_and_adc_set){
			timer_and_adc_set = true;
			if (timer1_delay(300) == TIMER_OK){
				adc_enable(true);
			}
		}
			if(isr_run_adc_convertion){
				isr_run_adc_convertion=false;
				adc_select_adc0_channel();
				adc_start_convertion();
				while (ADCSRA&(1<<ADSC)){}
					if(ADCSRA&(1<<ADIF)){
						adc_data.CH0=ADCL;
					}
				adc_select_adc1_channel();
				adc_start_convertion();
				while (ADCSRA&(1<<ADSC)){}
					if(ADCSRA&(1<<ADIF)){
						adc_data.CH1=ADCL;
					}
				leds_show_status(adc_data, charger_status());
			}
			else{
					if (timer1_delay(4000) == TIMER_OK){
						adc_enable(true);
					}
					leds_show_status(adc_data, charger_status());
				
				}
	}

}

