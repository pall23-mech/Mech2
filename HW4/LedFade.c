

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
		c=1000;
		f = 1000;
		while(c>0)
		{
		PORTB=0x20;
		delay_us(c);
		PORTB=0x00;
		delay_us(f);
		c-=10;
		f+=10;
		}
		c=0;
		f=2000;
		while(c<1000)
		{
			PORTB=0x20;
			c+=10;
			delay_us(c);
			f-=10;
			delay_us(f);
			PORTB=0x00;
		}
	}
	return 0;
}
