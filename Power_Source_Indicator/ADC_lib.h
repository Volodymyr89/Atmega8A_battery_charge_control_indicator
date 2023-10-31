/*
 * ADC_lib.h
 *
 * Created: 10/26/2023 12:27:45 PM
 *  Author: VolodymyrBatih
 */ 


#ifndef ADC_LIB_H_
#define ADC_LIB_H_

#include <avr/io.h>
#include <stdbool.h>
	
typedef struct adc_struct{
	uint8_t CH0;
	uint8_t CH1;
}adc_data_t;

extern adc_data_t adc_data;

void adc_init(void);
void adc_select_adc0_channel(void);
void adc_select_adc1_channel(void);
void adc_enable(bool adc_enable);
void adc_start_conversion(void);


#endif /* ADC_LIB_H_ */