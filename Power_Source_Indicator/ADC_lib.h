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
	uint16_t ADC_CH0;
	uint16_t ADC_CH1;
}adc_data_t;

extern adc_data_t adc_data;

typedef enum adc_conf{
	ADC_START,
	ADC_ENABLE,
	ADC_DISABLE
}adc_conf_t;

void adc_init(void);
void adc_read(adc_data_t *adcdata);

#endif /* ADC_LIB_H_ */