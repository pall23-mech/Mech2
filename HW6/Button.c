#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BUTTON_PIN PIND2

volatile unsigned int buttonPressCount = 0;

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

void setup() {
    // Initialize UART
    uart_init();

    // Configure BUTTON_PIN as input with pull-up enabled
    DDRD &= ~(1 << BUTTON_PIN); // Set as input
    PORTD |= (1 << BUTTON_PIN); // Enable pull-up resistor

    sei(); // Enable global interrupts
}

void loop() {
    static uint8_t lastButtonState = 1; // The last state of the button
    uint8_t currentButtonState = PIND & (1 << BUTTON_PIN); // Read the button state

    // Check for button press (logical LOW) and that the button state has changed
    if (lastButtonState && !currentButtonState) {
        // Button was pressed, increment count
        buttonPressCount++;

        // Send the current count over UART
        char buffer[20];
        snprintf(buffer, 20, "Count: %u\r\n", buttonPressCount);
        uart_sendString(buffer);
    }

    lastButtonState = currentButtonState; // Update the last state for the next loop iteration

    _delay_ms(50); // Debounce delay
}

int main(void) {
    setup();
    while (1) {
        loop();
    }
}
