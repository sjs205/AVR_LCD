/*
 *			AVR_SPI.c
 *		Created on:		09/03/2008
 *		Created by:		Steven Swann - sjs205@soton.ac.uk
 *		Completion:		pending - 
 *		Device:			ATmega48
 *			
 *		Simple c file to impliment the SPI interface
 *
 */
#inclue "AVR_SPI.h"

#include <avr/io.h>
#include <util/delay.h>

/*
 *	Macro definitions
 */
#define SETBIT(x,y) (x |= (y)) 		// Set bit y in byte 
#define CLEARBIT(x,y) (x &= (~y)) 	// Clear bit y in byte x
#define CHECKBIT(x,y) (x & (y)) 	// Check bit y in byte x

/*
 *	Serial port master initilisation routines - prepares SPI for master mode spi comms
 *			
 */
 void SPI_M_Init(void)
{
	char bin;

	SETBIT(DDRB,(SS));			//SPI DDRs setup				
	SETBIT(DDRB,(MOSI));
	SETBIT(DDRB,(SCK));

	SETBIT(SPCR,(1<<SPE));			//SPI enabled
	SETBIT(SPCR,(1<<MSTR));			//Master mode enabled

	bin = SPSR;				//flag clearing buy reading
	bin = SPDR;				//as above
	return;
}

/*
 *	Serial port slave initilisation routines - prepares SPI for slave mode spi comms
 *			
 */
void SPI_S_Init(void)
{	
	char bin;
	
	SETBIT(DDRB,(MOSI));			//SPI DDRs setup
	
	SETBIT(SPCR,(1<<SPE));			//SPI enabled

	SETBIT(SPCR,(1<<SPIE));			//SPI interupt enabled

	bin = SPSR;						//flag clearing buy reading
	bin = SPDR;						//as above

	SETBIT(SREG,(0x80));			//global interupts enabled **** Maybe shouldn't be here
	return;

}

/*
 *	Serial port transmit routine
 *
 *	SPI_M_PollTX(Char_to_Send)			
 */
void SPI_M_PollTX(char txbit)
{
	SPDR = txbit;				//load SPI data register 
	do
	{
	}
	while(!CHECKBIT(SPSR,(1<<SPIF)));	//wait until byte in SPDR is sent
	
	return;
}

/*
 *	The following code needs to be copied to the main file of the SPI slave device, 
 *	it is the ISR for data reception
 *
ISR(SPI_STC_vect, __attribute__((signal)))
{
	PORTB = PINB ^= 0xff;
	CHECKBIT(SPSR,0x01);			//flag clearing buy reading
	CHECKBIT(SPDR,0x01);			//as above
	return;		
}
*/
