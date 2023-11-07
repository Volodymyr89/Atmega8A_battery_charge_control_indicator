#ifndef CONFIG_H
#define CONFIG_H

#define ADC_CHANNEL0  (~(0x0F))
#define ADC_CHANNEL1 (1<<0)

#define TIMER_FOR_CHARGING           170
#define TIMER_FOR_DISCHARGING        4000

#define BATT_FULL 869             //23.9/11/0.0025 (reference Voltage is 2.56V)
#define BATT_FULL_HYSTERESIS 858 //23.6/11/0.0025
#define BATT_ALMOST_FULL 818     //22.5/11/0.0025
#define BATT_MID 781             //21.5/11/0.0025
#define BATT_LOW 727             //20/11/0.0025

#define CHARGER_PLUGGED_IN			 (PINB&(1<<PINB6))
#define CHARGER_TO_BATTERY_RELAY_ON  (PORTB |= 1<<PORTB0)
#define CHARGER_TO_BATTERY_RELAY_OFF (PORTB &= ~(1<<PORTB0))
#define BATTERY_TO_OUT_RELAY_ON      (PORTD |= 1<<PORTD5)
#define BATTERY_TO_OUT_RELAY_OFF	 (PORTD &= ~(1<<PORTD5))
#define COOLER_ON					 (PORTB |= 1<<PORTB1)
#define COOLER_OFF					 (PORTB &= ~(1<<PORTB1))				 

#endif