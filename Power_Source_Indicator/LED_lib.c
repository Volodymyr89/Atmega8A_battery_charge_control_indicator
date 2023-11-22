/*
 * LED_lib.c
 *
 * Created: 10/26/2023 12:37:42 PM
 *  Author: VolodymyrBatih
 */ 


#include <avr/io.h>
#include "ADC_lib.h"
#include "config.h"
#include "TIMER1_lib.h"
#include "LED_lib.h"

bool go_up = true;

void leds_and_pins_init(void){
	DDRD |= (1<<PORTD0) | (1<<PORTD1) | (1<<PORTD2) | (1<<PORTD3) | (1<<PORTD5);  // set as outputs
	DDRB |= (1<<PORTB0) | (1<<PORTB1);
	PORTD = 0x00; // set zeros
	PORTB = 0x00; // set zeros
}

void relay_control(relay_control_t relay_control){
	if (relay_control == CHARGER_RELAY_ON){
		CHARGER_TO_BATTERY_RELAY_ON;
	}
	else if (relay_control == CHARGER_RELAY_OFF){
		CHARGER_TO_BATTERY_RELAY_OFF;
	}
}

void leds_check_greeting_startup(void){
	
		for(uint8_t i=0; i<=3; i++){
			_delay_ms(150);
			PORTD |= 1<<i;
		}
		for(uint8_t i=0; i<3; i++){
			_delay_ms(150);
			PORTD &= ~(1<<i);
		}
		for(uint8_t i=3; i>0; i--){
			_delay_ms(150);
			PORTD &= ~(1<<i);
			PORTD |= 1<<(i-1);
		}
		_delay_ms(150);
		PORTD &= ~(0x0F);
}

void leds_show_status(const adc_data_t adc_data, bool charger_plugged_in_status){
	static bool timer_4s_set = false;
	static bool timer_300ms_set = false;
	static uint8_t toggle_state = 0;
	static uint8_t toggle = 0;

	if(charger_plugged_in_status){
			toggle ^= 1;
			if ((adc_data.ADC_CH0<=BATT_LOW)&&(adc_data.ADC_CH1<=BATT_LOW)){
				COOLER_ON;
				go_up = true;
				relay_control(CHARGER_RELAY_ON);
					if(!timer_300ms_set){
						timer_300ms_set = true;
						timer_4s_set = false;
						if (timer1_delay(TIMER_FOR_CHARGING) == TIMER_OK){} // set timer to blink for 300ms
						else{fail();}
					}
				toggle_state = 0;//LED0
			}else if (((adc_data.ADC_CH0>BATT_LOW)&&(adc_data.ADC_CH1>BATT_LOW))&&((adc_data.ADC_CH0<=BATT_MID)&&(adc_data.ADC_CH1<=BATT_MID))){		
				COOLER_ON;
				go_up = true;
				relay_control(CHARGER_RELAY_ON);
					if(!timer_300ms_set){
						timer_300ms_set = true;
						timer_4s_set = false;
						if (timer1_delay(TIMER_FOR_CHARGING) == TIMER_OK){} // set timer to blink for 300ms
						else{fail();}
					}				
				toggle_state = 1; //LED1
			}else if ((adc_data.ADC_CH0>BATT_MID)&&(adc_data.ADC_CH1>BATT_MID)&&(adc_data.ADC_CH0<=BATT_ALMOST_FULL)&&(adc_data.ADC_CH1<=BATT_ALMOST_FULL)){	
				COOLER_ON;
				go_up = true;
				relay_control(CHARGER_RELAY_ON);
					if(!timer_300ms_set){
						timer_300ms_set = true;
						timer_4s_set = false;
						if (timer1_delay(TIMER_FOR_CHARGING) == TIMER_OK){} // set timer to blink for 300ms
						else{fail();}
					}			
				toggle_state = 2; //LED2
			}else if ((adc_data.ADC_CH0>BATT_ALMOST_FULL)&&(adc_data.ADC_CH1>BATT_ALMOST_FULL)&&(adc_data.ADC_CH0<BATT_FULL)&&(adc_data.ADC_CH1<BATT_FULL)&&(go_up==true)){
				go_up = true;
				COOLER_ON;
				relay_control(CHARGER_RELAY_ON);
					if(!timer_300ms_set){
						timer_300ms_set = true;
						timer_4s_set = false;
						if (timer1_delay(TIMER_FOR_CHARGING) == TIMER_OK){} // set timer to blink for 300ms
						else{fail();}
					}
				toggle_state = 3; //LED3
			}else if ((adc_data.ADC_CH0>BATT_ALMOST_FULL)&&(adc_data.ADC_CH1>BATT_ALMOST_FULL)&&(adc_data.ADC_CH0<BATT_FULL)&&(adc_data.ADC_CH1<BATT_FULL)&&(adc_data.ADC_CH0<BATT_FULL_HYSTERESIS)&&(adc_data.ADC_CH1<BATT_FULL_HYSTERESIS)&&(go_up == false)){
				relay_control(CHARGER_RELAY_ON);
			}else if ((adc_data.ADC_CH0>=BATT_FULL)&&(adc_data.ADC_CH1>=BATT_FULL)){
				go_up = false;
				relay_control(CHARGER_RELAY_OFF);
				if(!timer_4s_set){
					timer_4s_set = true;
					timer_300ms_set = false;
					COOLER_OFF;
					if (timer1_delay(TIMER_FOR_DISCHARGING) == TIMER_OK){} // set timer to 4s
					else{fail();}
				}
				toggle_state = 4;			 
			}
		}
			else{
				if(!timer_4s_set){
				    COOLER_OFF;
				    relay_control(CHARGER_RELAY_ON);
					timer_4s_set = true;
					timer_300ms_set = false;
					if (timer1_delay(TIMER_FOR_DISCHARGING) == TIMER_OK){} // set timer to blink for 4s
					else{fail();}
				}
				if ((adc_data.ADC_CH0<BATT_LOW)&&(adc_data.ADC_CH1<BATT_LOW)){
					toggle_state = 5;
				}else if ((adc_data.ADC_CH0>=BATT_LOW)&&(adc_data.ADC_CH1>=BATT_LOW)&&(adc_data.ADC_CH0<BATT_MID)&&(adc_data.ADC_CH1<BATT_MID)){
					toggle_state = 6;
				}else if ((adc_data.ADC_CH0>=BATT_MID)&&(adc_data.ADC_CH1>=BATT_MID)&&(adc_data.ADC_CH0<BATT_ALMOST_FULL)&&(adc_data.ADC_CH1<BATT_ALMOST_FULL)){
					toggle_state = 7;
				}else if ((adc_data.ADC_CH0>=BATT_ALMOST_FULL)&&(adc_data.ADC_CH1>=BATT_ALMOST_FULL)&&(adc_data.ADC_CH0<BATT_FULL)&&(adc_data.ADC_CH1<BATT_FULL)){
					toggle_state = 8;
				}else if ((adc_data.ADC_CH0>=BATT_FULL)&&(adc_data.ADC_CH1>=BATT_FULL)){
					toggle_state = 9;
				}
			}
						
		switch(toggle_state){
			case 0: 
				PORTD &= ~(1<<0x00) & (1<<0x01) & (1<<0x02);
				toggle ? (PORTD |= (1<<0x03)) : (PORTD &= ~(1<<0x03)); //BAT_LOW
				break;
			case 1:
				PORTD |= (1<<0x03); // BAT_MID
				PORTD &= ~(1<<0x00) & (1<<0x01);
				toggle ? (PORTD |= (1<<0x02)) : (PORTD &= ~(1<<0x02));
				break;
			case 2:
				PORTD |= (1<<0x03) | (1<<0x02); // BAT_ALMOST_FULL
				PORTD &= ~(1<<0x00);
				toggle ? (PORTD |= (1<<0x01)) : (PORTD &= ~(1<<0x01));
				break;
			case 3:
				PORTD |= (1<<0x01) | (1<<0x02) | (1<<0x03); 
				toggle ? (PORTD |= (1<<0x00)) : (PORTD &= ~(1<<0x00));
				break;
			case 4:		
				PORTD |= 0x0F;
				_delay_ms(200);
				PORTD &= ~(0x0F);//reset all LEDs
				break;
             case 5:
				 PORTD &= ~(0x0F); //reset all LEDs
				 break;
			 case 6:
				 PORTD |= (1<<0x03); // LED0
				 _delay_ms(200);
				 PORTD &= ~(0x0F); //reset all LEDs
				 break;
			 case 7:
				 PORTD |= (1<<0x03) | (1<<0x02); // LED0, 1
				 _delay_ms(200);
				 PORTD &= ~(0x0F); //reset all LEDs				 
				 break;
			 case 8:
				PORTD |= (1<<0x01) | (1<<0x02) | (1<<0x03); // LED0, 1, 2
				 _delay_ms(200);
				 PORTD &= ~(0x0F); //reset all LEDs
				 break;
			 case 9:
			   PORTD |= 0x0F; // LED0, 1, 2, 3
				_delay_ms(200);
			   PORTD &= ~(0x0F); //reset all LEDs	
		}	
	}


bool charger_status(void){
	
	static bool charger_plugged_in;
	
	if(CHARGER_PLUGGED_IN){
		charger_plugged_in = true;
	}
	else{
		charger_plugged_in = false;
	}
		return charger_plugged_in;
}

