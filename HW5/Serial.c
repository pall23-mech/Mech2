#include <avr/io.h>

int main() {
    char cData;
    // baud rate 4800
    UBRR0H = 0;
    UBRR0L = 207;
    // enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // set frame format
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);

    while(1) {
        // wait for data to be received
        while(!(UCSR0A & (1<<RXC0)));
        // get and return data
        cData = UDR0;
        // wait for empty buffer
        while(!(UCSR0A & (1<<UDRE0)));
        // put data into buffer
        UDR0 = cData;
    }
    return 0;
}
