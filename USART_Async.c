/*	USART_Async.h
*	Created on:		18/02/2008
*	Created by:		Steven Swann - swannonline@googlemail.com
*	Completion:		09/09/2008
*	Device:			ATmega48/162
*						
*	Simple header file to impliment the onboard USART in async mode.
*	USART_Init();			initialises the USART
*	void USART_Out(int);	transfers int to the USART for transmission
*	
* 	See final section on interrupt routines for details of interrupt driven reception
*/

#include "USART_Async.h"


//#include "AVR_LCD.h" ////////////////////////////////////////////////not needeed

/*
*	Macro definitions
*/
#define SETBIT(x,y) (x |= (y)) 		// Set bit y in byte 
#define CLEARBIT(x,y) (x &= (~y)) 	// Clear bit y in byte x
#define CHECKBIT(x,y) (x & (y)) 	// Check bit y in byte x


static struct tx_cbuf volatile * volatile tx_cbuf_init(void)
{
	tx_cbuf = malloc(sizeof(struct tx_cbuf));
	if(tx_cbuf==NULL) {
		//LCD_WData('E');
		return NULL;
	}
	
	tx_cbuf->buf = malloc(TX_CBUF_SIZE);
	if(tx_cbuf->buf==NULL) {
		//LCD_WData('E');
		return NULL;
	}
	tx_cbuf->count=0;
	tx_cbuf->front=0;
	tx_cbuf->end=0;
	tx_cbuf->size=TX_CBUF_SIZE;
	return tx_cbuf;
}

static void rx_cbuf_init(void)
{
	static unsigned char buf[RX_CBUF_SIZE];

	static struct rx_cbuf q;
	q.count=0;
	q.front=0;
	q.end=0;
	q.buf=buf;
	q.size=(RX_CBUF_SIZE);
	rx_cbuf = &q;
return;

}

extern unsigned char tx_isenabled(void)
{	
	return CHECKBIT(UCSR0B,(1<<TXEN0));
}

extern void tx_enable(void)
{	

SETBIT(UCSR0B,(1<<TXEN0));			//set TX enable
SETBIT(UCSR0B,(1<<UDRIE0));			//TX buf empty interrupt enabled
sei();
return;
}

extern void tx_disable(void)
{	
CLEARBIT(UCSR0B,(1<<TXEN0));			//clear TX enable
CLEARBIT(UCSR0B,(1<<UDRIE0));			/* disable data register empty int */
CLEARBIT(UCSR0B,(1<<TXCIE0));			/* disable TX complete int */
return;
}


extern void rx_enable(void)
{	
SETBIT(UCSR0B,(1<<RXEN0));			//Receive enable
SETBIT(UCSR0B,(1<<RXCIE0));			//Receive complete interrupt enabled
return;
}


/*
* USART initilisation routines - prepares SPI for master mode spi comms
*			
*/
void USART_Init(void)
{	
	USART_FrameSet();					//function to set up frame size
	USART_Baud(MYUBRR);					//function to set up baud rate

	tx_cbuf_init();

	return;
}

/*
* Baud rate Initialisation Routine
*/
void USART_Baud(unsigned int ubrr)
{
	UBRR0L = ubrr;				
	UBRR0H = (ubrr>>8);	
	return;
}

/*
* Frame set Initialisation routine
*/
void USART_FrameSet(void)
{
	CLEARBIT(UCSR0C,(1<<UMSEL01));	//async mode UMSEL01:0
	CLEARBIT(UCSR0C,(1<<UMSEL00));		
	
	CLEARBIT(UCSR0C,(1<<UPM01));	//Parity UPM01:0; 00=disabled;10=even;11=odd
	CLEARBIT(UCSR0C,(1<<UPM00));

	
	CLEARBIT(UCSR0C,(1<<USBS0));	//1 stop bit USBS0; 01=2stop bits
	
	SETBIT(UCSR0C,(1<<UCSZ01));	//8-bit mode UCSZ01:0
	SETBIT(UCSR0C,(1<<UCSZ00));
	return;
}

/*
*Transmit Byte Routine 
*									
*	Waits for the data register empty flag and when clear, loads 
*	the transmit data buffer with the byte stored in temp.
*/

extern unsigned char read_buf(unsigned char temp)
{
	return cbuf_read(temp, tx_cbuf);
}

extern unsigned char load_buf(unsigned char temp)
{
if(cbuf_load(temp, (struct cbuf *)tx_cbuf)) 
	return ERROR;
else
	return SUCCESS;
}


extern unsigned char USART_Out(unsigned char temp)
{     
if(!cbuf_load(temp, (struct cbuf *)tx_cbuf)) {
	
	if (!tx_isenabled())
	tx_enable();
	return SUCCESS;
}
return ERROR;
}

/*
*	interrupt vectors
*		Add the following to main source and alter as required, when a 
* 		byte is received in this instance the byte is simply loaded 
*		into the temp variable

#include <interrupt.h>
ISR(USART_RX_vect, __attribute__((signal)))
{
	unsigned char temp = UDR0;
	
	return;		
}	
*
*/
ISR(USART_RX_vect)
{
	return;		
}	

ISR(USART_UDRE_vect, ISR_BLOCK)
{ 
	if (cbuf_isempty((struct cbuf *)tx_cbuf)) {
		//SETBIT(UCSR0B,(1<<TXCIE0));		/* Enable TX complete int */
		CLEARBIT(UCSR0B,(1<<UDRIE0));	/* Disable Data Reg Empty int */
	} else {
//		while(CHECKBIT(UCSR0A,UDRE0));
		UDR0 = cbuf_unload((struct cbuf *)tx_cbuf);
		//._delay_ms(10);
	} 	
}	

ISR(USART_TX_vect)
{ 

//_delay_ms(10);
//	LCD_WData(0x55);
//	_delay_ms(10);
if (cbuf_isempty((struct cbuf *)tx_cbuf)) {
	tx_disable();
} else {
	CLEARBIT(UCSR0B,(1<<TXCIE0));
	tx_enable();
	
}
return;
}	