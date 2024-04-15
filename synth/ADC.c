#include <avr/io.h>
#include <avr/interrupt.h>
#include "DelayMech.h"
#include "myUart.h"
#include "ADC.h"

#define BAUD 9600
#define MY_UBRR (F_CPU/16/BAUD-1)/*
 * ADC.c
 *
 *  Created on: Apr 15, 2024
 *      Author: palli
 */
void adc_init() {
    // Reference voltage = AVcc, left adjust result (for 8-bit precision)
    ADMUX = (1 << REFS0) | (1 << ADLAR);
    // Enable the ADC and set the prescaler to 128 (for 16MHz clock, 125KHz ADC clock)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint8_t adc_read(uint8_t channel) {
    // Select ADC channel (0-7)
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07);
    // Start the conversion
    ADCSRA |= (1 << ADSC);
    // Wait for the conversion to finish
    while (ADCSRA & (1 << ADSC));
    // Return the 8-bit result
    return ADCH; // Using 8-bit precision
}

void print_adc_value(uint8_t value) {
    // Convert the value to a string (assuming a maximum of 255)
    char buffer[4];
    itoa(value, buffer, 10); // Convert integer to string (decimal base)
    // Transmit each character of the string
    for (char* p = buffer; *p; p++) {
        uart_transmit(*p);
    }
    uart_transmit('\n'); // New line for readability
}

