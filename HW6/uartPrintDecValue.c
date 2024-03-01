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



// Convert and send a number in hexadecimal format
void uart_sendHex(unsigned long num, unsigned char bytes) {
    uart_sendString("0x");
    for (int i = (bytes*2)-1; i >= 0; i--) {
        unsigned char nibble = (num >> (i*4)) & 0x0F;
        uart_transmit(nibble > 9 ? (nibble + 'A' - 10) : (nibble + '0'));
    }
    uart_sendString("\r\n");
}

// Utility function to reverse a string in place
void reverse(char* str, int len) {
    int i = 0, j = len - 1;
    while (i < j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}


// Function to convert an unsigned int to string and print
void uart_sendDecimal(unsigned int num) {
    char str[6]; // Maximum size for a 16-bit integer when converted to string
    int i = 0;

    // Handle 0 explicitly, otherwise the empty string is printed for 0
    if (num == 0) {
        str[i++] = '0';
    }

    // Process individual digits and reverse the resulting string
    while (num != 0) {
        str[i++] = (num % 10) + '0'; // Get the corresponding ASCII character
        num /= 10;
    }
    str[i] = '\0'; // Append string terminator

    reverse(str, i); // Reverse the string

    uart_sendString(str); // Send the string
}

// Send a string
void uart_sendString(const char* str) {
    while (*str) {
        uart_transmit(*str++);
    }
}


int main(void) {
    uart_init(); // Initialize UART with the baud rate set in uart_init

    char c = 13; // Example byte value

    while(1) {
        uart_sendDecimal(c);  // Send the decimal value of c
        uart_sendString("\r\n");  // Send a new line for readability
        delay_ms(1000); // Wait for a bit before sending again
    }
}

