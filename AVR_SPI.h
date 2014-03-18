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
 *	SPI bus definitions
 */
#if !defined MISO
	#define MISO 0x10	
#endif
#if !defined MOSI
	#define MOSI 0x08
#endif
#if !defined SCK
	#define SCK 0x20	
#endif
#if !defined SS
	#define SS  0x04
#endif

/*
 *	Serial port master initilisation routines - prepares SPI for master mode spi comms
 *			
 */
 extern void SPI_M_Init(void)

/*
 *	Serial port slave initilisation routines - prepares SPI for slave mode spi comms
 *			
 */
extern void SPI_S_Init(void)

/*
 *	Serial port transmit routine
 *
 *	SPI_M_PollTX(Char_to_Send)			
 */
extern void SPI_M_PollTX(char txbit)

/*
 *	The following code needs to be copied to the main file of the SPI slave device, it
 *	is the ISR for data reception
 *
ISR(SPI_STC_vect, __attribute__((signal)))
{
	PORTB = PINB ^= 0xff;
	CHECKBIT(SPSR,0x01);				//flag clearing buy reading
	CHECKBIT(SPDR,0x01);				//as above
	return;		
}
*/
