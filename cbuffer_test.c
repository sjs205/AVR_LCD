/*
 *			User_Interface.c
 *		Created on:		07/03/2008
 *		Created by:		Steven Swann - sjs205@soton.ac.uk
 *		Completion:		pending - 04/03/08
 *		Device:			ATmega48/162
 *						
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#include "AVR_LCD.h"
#include "USART_Async.h"

#include "cbuffer.h"

/*
 *	Macro Definitions
 */
#define SETBIT(x,y) (x |= (y)) 		// Set bit y in byte 
#define CLEARBIT(x,y) (x &= (~y)) 	// Clear bit y in byte x
#define CHECKBIT(x,y) (x & (y)) 	// Check bit y in byte x


char command = 0x00;


int main(void) 
{	
	struct cbuf *buf = cbuf_init();
	
	
	USART_Init();
	SETBIT(UCSR0B,(1<<TXEN0));		//tx enable
	LCD_Init();			//init LCD
	_delay_ms(10);
	LCD_WData('T');
	_delay_ms(10);
	LCD_WData(':');
	_delay_ms(10);
	
	
	int i;
		
	
	
	//USART_Init();
	//sei();				//global interrupts enabled

	
	//LCD_Clear();
	for(i=0x04A;i<=0x57;i++)				//loop forever
	{
		cbuf_load(i, buf);
	
		}
	int x;
	for(i=0;i<=14;i++)				//loop forever
	{
		x = cbuf_read(i, buf);
		while(CHECKBIT(UCSR0A,UDRE0));
		UDR0 = x;
		LCD_WData(x);
		_delay_ms(10);
		}
	for(;;){}
		
	return(0);
}
