#define F_CPU 16000000UL // Define CPU frequency as 16MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include "DelayMech.h"

#define BAUD 9600
#define MY_UBRR (F_CPU/16/BAUD-1)

const uint16_t noteFrequencies[] = {262, 294, 330, 349, 392, 440, 494}; // C4, D4, E4, F4, G4, A4, B4
// Initialize UART
void uart_init(void) {
    // Set baud rate
    UBRR0H = (MY_UBRR >> 8);
    UBRR0L = MY_UBRR;
    // Enable receiver and transmitter
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);
    // Set frame format to 8 data bits, no parity, 1 stop bit
    UCSR0C = (3<<UCSZ00);
}

// Function to receive a single byte via UART
unsigned char uart_receive(void) {
    // Wait for data to be received
    while (!(UCSR0A & (1<<RXC0)));
    // Get and return received data from buffer
    return UDR0;
}

// ISR for Timer0 overflow: Turns the LED on at overflow
ISR(TIMER0_OVF_vect) {
    PORTB |= (1<<PB1); // Assuming LED is connected to PB5
}

// ISR for Timer0 compare match A: Turns the LED off
ISR(TIMER0_COMPA_vect) {
    PORTB &= ~(1<<PB5);
}
void initTimer1ForTone(uint16_t frequency) {
    // Set Timer1 for Fast PWM mode, with ICR1 as TOP
    TCCR1A = (1 << COM1A1) | (1 << WGM11); // Clear OC1A on compare match, set at BOTTOM
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10); // No prescaling

    ICR1 = F_CPU / (2 * frequency) - 1; // Calculate TOP value for desired frequency

    DDRB |= (1 << PB1); // Set PB1 (OC1A) as output for PWM signal
}

void playTone(uint16_t frequency, uint16_t duration) {
    initTimer1ForTone(frequency); // Initialize Timer1 for the current tone frequency
    delay_ms(500); // Corrected to use _delay_ms for AVR
    TCCR1A &= ~(1 << COM1A1); // More targeted way to stop the tone
}

int main(void) {
    uart_init();
    // Removed sei() if not needed
    while(1) {
        for (int i = 0; i < sizeof(noteFrequencies) / sizeof(noteFrequencies[0]); i++) {
            playTone(noteFrequencies[i], 500); // Play each note for 500ms
            delay_ms(100); // Corrected to use _delay_ms
        }
    }

}


