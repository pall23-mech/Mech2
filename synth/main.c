#define F_CPU 16000000UL // Define CPU frequency as 16MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include "DelayMech.h"
#include "myUart.h"
#include "ADC.h"

#define BAUD 9600
#define MY_UBRR (F_CPU/16/BAUD-1)



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
    DDRB |= (1<<PB1) | (1<<PB3);    // Initialize UART
    uart_init();
    adc_init(); // Initialize ADC


    // Timer2 configuration for Fast PWM mode, aiming for 50% duty cycle and half frequency of Timer0
    TCCR2A = (1<<WGM21) | (1<<WGM20); // Fast PWM
    TCCR2A |= (1<<COM2A1); // Clear OC2A on Compare Match (non-inverting mode)
    OCR2A = 127; // 50% duty cycle
    // Prescaler for Timer2 to approximately half the frequency of Timer0
    // Assuming Timer0's prescaler is 256 to make Timer2's frequency half,
    // we might not need to change Timer2's prescaler in the switch case if it's already set to produce half frequency.
    TCCR2B = (1<<CS22) | (1<<CS21); // Prescaler 256 for Timer2 as well, adjust if needed

    // Timer0 configuration for Fast PWM mode
    TCCR0A = (1<<WGM01) | (1<<WGM00); // Set to Fast PWM
    TCCR0A |= (1<<COM0A1); // Clear OC0A on Compare Match, set at BOTTOM (non-inverting mode)
    OCR0A = 127; // Set compare match value for a 50% duty cycle
    TCCR0B = (1<<CS02); // Prescaler 256

    // Enable Timer0 overflow and compare match A interrupts
    TIMSK0 = (1<<TOIE0) | (1<<OCIE0A);

    // Global interrupt enable
    sei();

    unsigned char input;
    while(1) {
        input = uart_receive(); // Read character from UART
        uint8_t adcValue = adc_read(0); // Read the value from A0
        print_adc_value(adcValue); // Print the value over serial

        delay_ms(200); // Delay a bit to not flood the serial output
        // Adjust both Timer0 and Timer2 prescalers based on the input
        switch(input) {
			case '0':
				TCCR0B = (1<<CS00); // No prescaling for Timer0, highest frequency
				TCCR2B = (1<<CS21); // Set Timer2 prescaler to 8, to approximate half the frequency of Timer0
				TCCR0A |= (1<<COM0A1); // Ensure OC0A (D9) is connected
				TCCR2A |= (1<<COM2A1); // Ensure OC2A (D11) is connected
            break;
            case '1':
                TCCR0B = (1<<CS01); // Prescaler of 8 for Timer0
                TCCR2B = (1<<CS22); // Set Timer2 prescaler to 64, to keep the frequency roughly half of Timer0's
                break;
            case '2':
                TCCR0B = (1<<CS01) | (1<<CS00); // Prescaler of 64 for Timer0
                TCCR2B = (1<<CS22) | (1<<CS20); // Set Timer2 prescaler to 128, adjusting for half frequency
                break;
            case '3':
                TCCR0B = (1<<CS02); // Prescaler of 256 for Timer0
                TCCR2B = (1<<CS22) | (1<<CS20); // Set Timer2 prescaler to 128, adjusting for half frequency
                break;
            case '4':
            	TCCR0B = (1<<CS02); // Prescaler of 256 for Timer0
                // For Timer2, maintaining a half frequency relationship with the lowest Timer0 frequency setting
                // may not be practical due to prescaler limits, so we'll match Timer0's lowest frequency setting.
                // This will need adjustment based on your specific frequency requirements and available prescaler options.
                TCCR2B = (1<<CS22) | (1<<CS21) | (1<<CS20); // Also setting to 1024, adjust as needed
                break;
            case '5':
                // Disconnect OC0A (D9) and OC2A (D11) to make the synth silent
                TCCR0A &= ~(1<<COM0A1);
                TCCR2A &= ~(1<<COM2A1);

                // Optionally, set the pins to LOW to ensure they are silent
                PORTB &= ~(1<<PB1); // Set D9 to LOW
                PORTB &= ~(1<<PB3); // Set D11 to LOW
                break;
            default:
                // If input is not recognized, do nothing
                break;
        }

    }

}
