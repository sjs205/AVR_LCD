/*
 *			User_Interface.c
 *		Created on:		07/03/2008
 *		Created by:		Steven Swann - sjs205@soton.ac.uk
 *		Completion:		pending - 04/03/08
 *		Device:			ATmega48/162
 *
 */

#include <avr/io.h>




#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "AVR_LCD.h"
#include "USART_Async.h"
#include "cbuffer.h"

#if !defined F_CPU
  #define F_CPU = 8000000
#endif

#include <util/delay.h>

/*
 *	Macro Definitions
 */
#define SETBIT(x,y) (x |= (y)) 		// Set bit y in byte 
#define CLEARBIT(x,y) (x &= (~y)) 	// Clear bit y in byte x
#define CHECKBIT(x,y) (x & (y)) 	// Check bit y in byte x


int main(void)
{ 
	wdt_reset();
	wdt_disable();

	char i=0x55;
	
	USART_Init();
	//LCD_Init();
	//LCD_WData('S');
		

//	LCD_WSData("Hello, this is a test!!!");
      

	for(i=0x04A;i<=0x57;i++)
	{
		//LCD_WData(i);
		if (load_buf(i)){
		//	LCD_WData('X');
			_delay_ms(10);

		} 
	}
	for(i=0;i<=100;i++)
		_delay_ms(10);
	tx_enable();
	while(!cbuf_isempty(tx_cbuf));
	//LCD_WData('F');
	//LCD_WData(':');
	for(i=0;i<=100;i++)
		_delay_ms(10);

	int x;
	for(i=0;i<=14;i++)				//loop forever
	{
		
		x = read_buf(i);
		
		while(CHECKBIT(UCSR0A,UDRE0));
		UDR0 = x;
	//	LCD_WData(x);
	}
	for(;;) { /* loop forever */ }		
	
	return(0);
}
/*

ISR(BADISR_vect)
{
   	//LCD_WData('B');
}
*/

