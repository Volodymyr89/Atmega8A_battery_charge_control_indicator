#ifndef CONFIG_H
#define CONFIG_H

#define BATT_FULL 
#define BATT_ALMOST_FULL
#define BATT_MID
#define BATT_LOW

#define CHARGER_PLUGGED_IN  (PINB&(1<<PINB6))

#define SET_LED0 PORTD |= (1<<PORTD0)
#define SET_LED1 PORTD |= (1<<PORTD1)
#define SET_LED2 PORTD |= (1<<PORTD2)
#define SET_LED3 PORTD |= (1<<PORTD3)

#define RESET_LED0 PORTD &= ~(1<<PORTD0)
#define RESET_LED1 PORTD &= ~(1<<PORTD1)
#define RESET_LED2 PORTD &= ~(1<<PORTD2)
#define RESET_LED3 PORTD &= ~(1<<PORTD3)

#endif