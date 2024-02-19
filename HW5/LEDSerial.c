#define F_CPU 16000000UL
#include <avr/io.h>
#include "DelayMech.h"

#define BAUD 9600
#define MY_UBRR F_CPU/16/BAUD-1

void uart_init(void) {
    // Set baud rate
    UBRR0H = (unsigned char)(MY_UBRR>>8);
    UBRR0L = (unsigned char)MY_UBRR;
    // Enable receiver and transmitter
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    // Set frame format: 8 data, 1 stop bit
    UCSR0C = (3<<UCSZ00);
}

unsigned char uart_receive(void) {
    // Wait for data to be received
    while (!(UCSR0A & (1<<RXC0)));
    // Get and return received data from buffer
    return UDR0;
}

int main() {
    DDRB = 0x20; // Set PB5 as an output
    uart_init(); // Initialize UART

    char input;
    int c=1000, f=0;

    while (1) {
        input = uart_receive(); // Read character from UART

        switch (input) {
            case '0':
                c = 1000;
                f = 1000;
                break;
            case '1':
                c = 500;
                f = 1500;
                break;
            case '2':
                c = 1500;
                f = 500;
                break;
            //default:
               // continue; // If input is not 0, 1, or 2, ignore it
        }
        
        PORTB = 0x20; // Turn on LED
        delay_us(c);  // Delay for 'c' microseconds
        PORTB = 0x00; // Turn off LED
        delay_us(f);  // Delay for 'f' microseconds
    }

    return 0;
}
