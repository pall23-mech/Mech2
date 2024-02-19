#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER0_OVF_vect)
{
    PORTB=0x20; // at overflow turn LED on
}

ISR(TIMER0_COMPA_vect)
{
    PORTB=0x00; // at compare level, turn LED off
}

int main()
{
    DDRB=0x20; //make LED pin output
    TCCR0A=0; // normal mode
    TCCR0B=0x05; // f =16MHz/1024
    OCR0A=128; //50% duty cycle
    TIMSK0=0x03; // enable Overflow and Compare A interrupts
    TIFR0=0x03; // clear flags
    sei(); // switch on global interrupt
    while(1)
    {
        asm("nop"); // do something useful
    }
}
