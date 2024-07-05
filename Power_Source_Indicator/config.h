#ifndef CONFIG_H
#define CONFIG_H

//#define Debug 

#define ADC_CHANNEL0_BATT  (~(0x0F))
#define ADC_CHANNEL1_TEMPERATURE (1<<0)

#define TIMER_FOR_SCAN           (uint8_t)  500
#define DISPLAY_TIME             (uint8_t)  10

//thresholds
#define BATT_FULL            (uint16_t) 385 //4.2V/4.2 = 1000mV/0.0025 = 400 (reference Voltage is 2.56V) 
#define BATT_75_FULL         (uint16_t) 371 //3.9V/4.2 = 928mV/0.0025 = 371
#define BATT_MID             (uint16_t) 333 //3.5V/4.2 = 833mV/0.0025 = 333
#define BATT_LOW             (uint16_t) 310 //3.2V/4.2 = 761mV/0.0025 = 304

#define TEMPERATURE_HIGH      (uint16_t) 1000 // 2.5V temperature is high enough to enable cooler
//thresholds end
#define CHARGER_PLUGGED_IN			 (PINB&(1<<PINB6))			 
#endif