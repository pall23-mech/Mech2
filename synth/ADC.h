/*
 * ADC.h
 *
 *  Created on: Apr 15, 2024
 *      Author: palli
 */

#ifndef ADC_H_
#define ADC_H_
void adc_init();
uint8_t adc_read(uint8_t channel);
void print_adc_value(uint8_t value);


#endif /* ADC_H_ */
