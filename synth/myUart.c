#include <avr/io.h>
#include <avr/interrupt.h>
#include "DelayMech.h"
#include "myUart.h"
#define BAUD 9600
#define MY_UBRR (F_CPU/16/BAUD-1)
/*
 * myUart.c
 *
 *  Created on: Apr 15, 2024
 *      Author: palli
 */
void uart_transmit(unsigned char data) {
    // Wait for the transmit buffer to be empty
    while (!(UCSR0A & (1<<UDRE0)));
    // Put data into buffer, sends the data
    UDR0 = data;
}


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
    if (UCSR0A & (1<<RXC0))
    // Get and return received data from buffer
    return UDR0;
    else return 0;
}

