
#define F_CPU 16000000UL
#include<avr/io.h>
#include "DelayMech.h"
int main()
{
	DDRB=0x20;

	while (1)
	{
		PORTB=0x20;
		delay_ms(100);
		PORTB=0;
		delay_ms(100);
	}
	return 0;
}


//#define F_CPU 16000000UL
//#include <avr/io.h>
//#include <util/delay.h>

//int main(void) {
  //  DDRB |= 1<<PB5; /* set PB0 to output */
//    while(1) {
  //      PORTB &= ~(1<<PB5); /* LED on */
       // _delay_ms(1);
    //    PORTB |= 1<<PB5; /* LED off */
     //   _delay_ms(1);
    //}
  //  return 0;
//}
