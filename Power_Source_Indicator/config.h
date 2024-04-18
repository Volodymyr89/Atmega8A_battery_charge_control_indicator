#ifndef CONFIG_H
#define CONFIG_H

#define ADC_CHANNEL0  (~(0x0F))
#define ADC_CHANNEL1 (1<<0)

#define TIMER_FOR_CHARGING           (uint8_t) 190
#define TIMER_FOR_DISCHARGING        (uint16_t) 500

//thresholds
#define BATT_FULL            (uint16_t) 940 //25.2/11/0.0025 (reference Voltage is 2.56V) full bat is 25.2V (916)
#define BATT_FULL_HYSTERESIS (uint16_t) 927 //24.7/11/0.0025
#define BATT_ALMOST_FULL     (uint16_t) 890 //23.7/11/0.0025
#define BATT_MID             (uint16_t) 835 //22.2/11/0.0025
#define BATT_LOW             (uint16_t) 755 //20/11/0.0025
#define TEMPERATURE_LOW      (uint16_t) 950 // temperature is low enough to disable cooler
//thresholds end


#define CHARGER_PLUGGED_IN			 (PINB&(1<<PINB6))
#define CHARGER_TO_BATTERY_ON		 (PORTD |= 1<<PORTD5)
#define CHARGER_TO_BATTERY_OFF		 (PORTD &= ~(1<<PORTD5))
#define COOLER_ON					 (PORTB |= 1<<PORTB1)
#define COOLER_OFF					 (PORTB &= ~(1<<PORTB1))				 

#endif