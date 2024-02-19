#define F_CPU 16000000UL // Define CPU frequency as 16MHz
#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD 9600
#define MY_UBRR (F_CPU/16/BAUD-1)

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
    PORTB |= (1<<PB5); // Assuming LED is connected to PB5
}

// ISR for Timer0 compare match A: Turns the LED off
ISR(TIMER0_COMPA_vect) {
    PORTB &= ~(1<<PB5);
}

int main(void) {
    // Set PB5 as an output pin
    DDRB = (1<<PB5);
    // Initialize UART
    uart_init();

    // Timer0 configuration for Fast PWM mode
    TCCR0A = (1<<WGM01) | (1<<WGM00); // Set to Fast PWM
    TCCR0A |= (1<<COM0A1); // Clear OC0A on Compare Match, set at BOTTOM (non-inverting mode)
    TCCR0B = (1<<CS02) | (1<<CS00); // Prescaler 1024

    // Enable Timer0 overflow and compare match A interrupts
    TIMSK0 = (1<<TOIE0) | (1<<OCIE0A);

    // Global interrupt enable
    sei();

    unsigned char input;
    while(1) {
        input = uart_receive(); // Read character from UART

        // Adjust OCR0A based on the input to change the LED brightness
        switch(input) {
            case '0':
                OCR0A = 51; // Approx. 20% duty cycle for dimmer light
                break;
            case '1':
                OCR0A = 127; // Approx. 50% duty cycle for medium light
                break;
            case '2':
                OCR0A = 204; // Approx. 80% duty cycle for brighter light
                break;
            default:
                // If input is not 0, 1, or 2, do nothing
                break;
        }
    }
}
