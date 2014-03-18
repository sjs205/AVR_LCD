/*									USART_Async.h
 *		Created on:		18/02/2008
 *		Created by:		Steven Swann - swannonline@googlemail.com
 *		Completion:		09/09/2008
 *		Device:			ATmega48/162
 *						
 *						Simple header file to impliment the onboard USART in async mode.
 *						USART_Init();			initialises the USART
 *						void USART_Out(int);	transfers int to the USART for transmission
 *						
 *						See final section on interrupt routines for details of interrupt driven reception
 */

#include <avr/io.h>
#include <util/delay.h>

ISR(USART_UDRE_vect, ISR_BLOCK);
#include <avr/interrupt.h>

//ISR(USART_TX_vect) __attribute__ ((signal));

#include "cbuffer.h"
/*
 *	Macro definitions
 */
#define SETBIT(x,y) (x |= (y)) 		// Set bit y in byte 
#define CLEARBIT(x,y) (x &= (~y)) 	// Clear bit y in byte x
#define CHECKBIT(x,y) (x & (y)) 	// Check bit y in byte x


/*
 *	SPI bus definitions
 */

#if !defined BAUD
	#define BAUD 9600					//Baud Rate
#endif
#if !defined MYBURR
	#define MYUBRR F_CPU/16/BAUD-1		//UBRR formula
#endif

#define TX_CBUF_SIZE	32
#define RX_CBUF_SIZE	32

#define SUCCESS		0
#define ERROR		0xFF

#define FALSE 		0
#define TRUE		1


struct tx_cbuf {
  unsigned char volatile * volatile  buf;
  unsigned char size;
  unsigned char volatile count;
  unsigned char volatile front;
  unsigned char volatile end;
  
};



struct rx_cbuf {
  unsigned char *buf;
  unsigned char size;
  unsigned char count;
  unsigned char front;
  unsigned char end;
};

static struct tx_cbuf volatile * volatile  tx_cbuf;
static struct rx_cbuf * volatile rx_cbuf;

/*
 * Global pointers, used in interrupt.
 */

//static struct rx_queue *rx_q;

static void rx_queue_init(void);
static struct tx_cbuf volatile * volatile  tx_cbuf_init(void);

extern unsigned char tx_isenabled(void);  
extern void tx_enable(void);
extern void tx_disable(void);
extern void rx_enable(void);
/*
 *	USART initilisation routines
 *			
 */
extern void USART_Init(void);

/*
 *	Baud rate Initialisation Routine
 */
extern void USART_Baud(unsigned int ubrr);

/*
 *	Frame set Initialisation routine
 */
extern void USART_FrameSet(void);

/*
 *Transmit Byte Routine 
 *									
 *									Waits for the data register empty flag and when clear, loads 
 *									the transmit data buffer with the byte stored in temp.
 */

extern unsigned char load_buf(unsigned char temp);
extern unsigned char USART_Out(unsigned char temp);

/*
 *						interrupt vectors
 *		Add the following to main source and alter as required, when a byte is received in this instance the byte is simply loaded 
 *		into the temp variable

#include <interrupt.h>
ISR(USART_RX_vect) //__attribute__((signal))
{
	unsigned char temp = UDR0;
	
	return;		
}	

ISR(USART_TX_vect) //__attribute__((signal))
{
	unsigned char temp = UDR0;
	
	return;		
}	
 *
 */
