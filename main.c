#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void scheduler()
{

}



int main()
{
	sei(); 			// global interrupt elabled
	
	//////////////////  set for timer1 overflow interrupt
	TCCR1B =0x01;
	TIMSK = 0x07;

	while(1)
	{

	}
	return 0;
}

ISR(TIMER1_OVF_vect)
{

	scheduler();


}