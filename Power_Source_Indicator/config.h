#ifndef CONFIG_H
#define CONFIG_H

#define ADC_CHANNEL0  (~(0x0F))
#define ADC_CHANNEL1 (1<<0)

#define TIMER_FOR_CHARGING           190
#define TIMER_FOR_DISCHARGING        4000

//thresholds
#define BATT_FULL 944           //25.2/11/0.0025 (reference Voltage is 2.56V) full bat is 25.2V (916)
#define BATT_FULL_HYSTERESIS 927 //24.7/11/0.0025
#define BATT_ALMOST_FULL 890     //23.7/11/0.0025
#define BATT_MID 835             //22.2/11/0.0025
#define BATT_LOW 755             //20/11/0.0025
//thresholds end


#define CHARGER_PLUGGED_IN			 (PINB&(1<<PINB6))
#define CHARGER_TO_BATTERY_ON		 (PORTD |= 1<<PORTD5)
#define CHARGER_TO_BATTERY_OFF		 (PORTD &= ~(1<<PORTD5))
#define COOLER_ON					 (PORTB |= 1<<PORTB1)
#define COOLER_OFF					 (PORTB &= ~(1<<PORTB1))				 

#endif