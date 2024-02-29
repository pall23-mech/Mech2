#define F_CPU 16000000UL
#include <avr/io.h>
#include "DelayMech.h"

// Initialize UART
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

// Convert and send a number in hexadecimal format
void uart_sendHex(unsigned long num, unsigned char bytes) {
    uart_sendString("0x");
    for (int i = (bytes*2)-1; i >= 0; i--) {
        unsigned char nibble = (num >> (i*4)) & 0x0F;
        uart_transmit(nibble > 9 ? (nibble + 'A' - 10) : (nibble + '0'));
    }
    uart_sendString("\r\n");
}

int main(void) {
    uart_init(); // Initialize UART with the baud rate set in uart_init

    // Example values
    unsigned long myLong = 0xDEADBEEF;
    int myInt = 0x1234;
    unsigned char myByte = 0xAB;

    while(1) {
        uart_sendHex(myByte, 1);  // Send byte
        uart_sendHex(myInt, 2);   // Send int
        uart_sendHex(myLong, 4);  // Send long
        delay_ms(1000); // Wait for a bit before sending again
    }
}
