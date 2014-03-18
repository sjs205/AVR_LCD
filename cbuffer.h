#include <stdlib.h>
#include <util/atomic.h>

#if !defined(CBUF_H)
#define CBUF_H

#define BUF_EMPTY	0
#define BUF_FULL	1

#if !defined(CBUF_SIZE)
  #define CBUF_SIZE	32
#endif


#define SUCCESS		0
#define ERROR		0xFF
					/* -1 => signed char => 16-bit */

#define FALSE 		0
#define TRUE		1


struct cbuf {
  unsigned char *buf;
  unsigned char size;
  unsigned char count;
  unsigned char front;
  unsigned char end;
};
  
/*
 * 	USART buffer initilisation routine
 */
struct cbuf * cbuf_init(void);

/*
 *	load buffer routine
 *  
 *	end = next free space
 *	end - 1 =  Last item in cbuf
 */
unsigned char volatile cbuf_load(unsigned char temp, struct cbuf volatile * volatile buf);

/*	Unload buffer routine
 *
 *	it is a good idea to call USART_cbuf_isempty(buf)
 *	before calling this function, otherwise, we don't 
 *	know if return = 0x00 is a valid data. e.g:
 * 	
 *	if (!USART_cbuf_isempty(txq)) 
 *		item = USART_cbuf_decbuf(txq);
 */ 
unsigned char cbuf_unload(struct cbuf volatile * volatile buf);
/*
 * 	USART buffer initilisation routine
 */
unsigned char cbuf_isfull(struct cbuf *buf);
unsigned char volatile cbuf_isempty(struct cbuf * volatile buf);

unsigned char cbuf_read(unsigned char offset, struct cbuf *tmp_buf);



unsigned char cbuf_size(struct cbuf *buf);

unsigned char cbuf_count(struct cbuf *buf);
unsigned char cbuf_front(struct cbuf *buf);
unsigned char cbuf_front_pos(struct cbuf *buf);

unsigned char cbuf_end_pos(struct cbuf *buf);
unsigned char cbuf_end(struct cbuf *buf); 


#endif	/*CBUF_H*/