


#define F_CPU 16000000UL
#include<avr/io.h>
#include "DelayMech.h"
int main()
{
	DDRB=0x20;
	int c;
	int f;
	while (1)
	{
		c=10000;
		f = 10000;
		while(c>0)
		{
		PORTB=0x20;
		delay_us(c);
		PORTB=0x00;
		delay_us(f);
		c-=10;
		f+=10;
		}
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
