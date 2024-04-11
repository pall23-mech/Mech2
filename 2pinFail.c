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
    PORTB |= (1<<PB1); // Assuming LED is connected to PB5
}

// ISR for Timer0 compare match A: Turns the LED off
ISR(TIMER0_COMPA_vect) {
    PORTB &= ~(1<<PB1);
}

int main(void) {
    // Set PB1 (D9) and PB3 (D11) as output pins
    DDRB |= (1<<PB1) | (1<<PB3);

    // Initialize UART
    uart_init();

    // Timer0 configuration for Fast PWM mode, 50% duty cycle
    TCCR0A = (1<<WGM01) | (1<<WGM00); // Fast PWM
    TCCR0A |= (1<<COM0A1); // Clear OC0A on Compare Match (non-inverting mode)
    OCR0A = 127; // 50% duty cycle
    TCCR0B = (1<<CS02); // Prescaler 256

    // Timer2 configuration for Fast PWM mode, aiming for 50% duty cycle and half frequency of Timer0
    TCCR2A = (1<<WGM21) | (1<<WGM20); // Fast PWM
    TCCR2A |= (1<<COM2A1); // Clear OC2A on Compare Match (non-inverting mode)
    OCR2A = 127; // 50% duty cycle
    // Prescaler for Timer2 to approximately half the frequency of Timer0
    // Assuming Timer0's prescaler is 256 to make Timer2's frequency half,
    // we might not need to change Timer2's prescaler in the switch case if it's already set to produce half frequency.
    TCCR2B = (1<<CS22) | (1<<CS21); // Prescaler 256 for Timer2 as well, adjust if needed

    // Enable global interrupts
    sei();

    unsigned char input;
    while(1) {
        input = uart_receive(); // Read character from UART

        // Adjust the Timer0 prescaler based on the input to decrease the LED frequency
        switch(input) {
            case '0':
                TCCR0B = (1<<CS00); // No prescaling, highest frequency
                break;
            case '1':
                TCCR0B = (1<<CS01); // Prescaler of 8
                break;
            case '2':
                TCCR0B = (1<<CS01) | (1<<CS00); // Prescaler of 64
                break;
            case '3':
                TCCR0B = (1<<CS02); // Prescaler of 256
                break;
            case '4':
                TCCR0B = (1<<CS02) | (1<<CS00); // Prescaler of 1024, lowest frequency
                break;
            default:
                // If input is not 0, 1, 2, 3, or 4, do nothing
                break;
        }
    }
}
