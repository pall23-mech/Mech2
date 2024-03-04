#define F_CPU 16000000UL
#include <avr/io.h>
#include "DelayMech.h"

// Initialize UART with baud rate of 4800
void uart_init(void) {
    // Manually set baud rate to 4800
    UBRR0H = 0;
    UBRR0L = 207;

    // Enable receiver and transmitter
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);

    // Set frame format: 8 data bits, no parity, 1 stop bit
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
}

// Transmit a single byte
void uart_transmit(unsigned char data) {
    while (!(UCSR0A & (1<<UDRE0))); // Wait for empty transmit buffer
    UDR0 = data; // Put data into buffer, sends the data
}

// Send a string
void uart_sendString(const char* str) {
    while (*str) {
        uart_transmit(*str++);
    }
}

// Initialize ADC
void adc_init() {
    // AVcc with external capacitor at AREF pin
    ADMUX = (1<<REFS0);
    // Enable ADC and set prescaler to 128 (16MHz/128 = 125KHz)
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

// Read from ADC
unsigned int adc_read(unsigned char ch) {
    // Select ADC channel with safety mask
    ADMUX = (ADMUX & 0xF0) | (ch & 0x0F);
    // Start single conversion
    ADCSRA |= (1<<ADSC);
    // Wait for conversion to complete
    while(ADCSRA & (1<<ADSC));
    // Return ADC value
    return ADC;
}

void setup() {
    uart_init(); // Initialize UART
    adc_init(); // Initialize ADC
}

void loop() {
    unsigned int potValue = adc_read(0); // Read from ADC0

    // Send the potentiometer value over UART
    char buffer[20];
    snprintf(buffer, 20, "Pool Temp: %u\r\n", potValue/10);  //set to some scale assumed div by 10, assuming 0 is 0°C
    uart_sendString(buffer);

    delay_ms(500); // Add a small delay to limit the number of readings per second
}

int main(void) {
    setup();
    while (1) {
        loop();
    }
}
