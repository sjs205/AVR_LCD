
#include "cbuffer.h"

/*
 * 	Circular buffer initilisation routine
 */
struct cbuf * cbuf_init(void)
{
	struct cbuf *q = malloc(sizeof(struct cbuf));
	if(q==NULL)
		return NULL;
	
	q->buf = malloc(CBUF_SIZE);
	if(q->buf==NULL)
		return NULL;
	
	q->count=0;
	q->front=0;
	q->end=0;
	q->size=CBUF_SIZE;
	return q;
}

/*
 *	Load buffer routine
 */
unsigned char volatile cbuf_load(unsigned char tmp, struct cbuf volatile * volatile tmp_buf)
{
  if (cbuf_isfull(tmp_buf)) {
      return BUF_FULL;
  } else {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
      tmp_buf->buf[tmp_buf->end] = tmp;	
      tmp_buf->end++;
      if(tmp_buf->end>=tmp_buf->size) {
	tmp_buf->end=0;
      }
      
      tmp_buf->count++;
    }
    return SUCCESS;
 
  }
}

/*
 * 	Unload buffer routine
 */
unsigned char volatile cbuf_unload(struct cbuf volatile * volatile tmp_buf)
{
  unsigned char volatile tmp;
	if (cbuf_isempty(tmp_buf)) {
		return BUF_EMPTY;
	} else {
		tmp = tmp_buf->buf[tmp_buf->front];
		tmp_buf->front++;
		if(tmp_buf->front==tmp_buf->size) {
			tmp_buf->front=0;
		} 
	tmp_buf->count--;
	
	return tmp;
	}
}

/*
 * 	Circular buffer test functions
 */
unsigned char cbuf_isfull( struct cbuf *tmp_buf)
{
	return(tmp_buf->count == tmp_buf->size);
}

unsigned char volatile cbuf_isempty( struct cbuf * volatile tmp_buf)
{
	return(tmp_buf->count == 0);
}

unsigned char cbuf_read(unsigned char offset, struct cbuf *tmp_buf)
{
	return (tmp_buf->buf[offset]);
}
	
unsigned char cbuf_size(struct cbuf *tmp_buf)
{
	return tmp_buf->size;
}

unsigned char cbuf_count(struct cbuf *tmp_buf)
{
	return tmp_buf->count;
}

unsigned char cbuf_front(struct cbuf *tmp_buf)
{
	return(tmp_buf->buf[tmp_buf->front]);
}

unsigned char cbuf_end(struct cbuf *tmp_buf)
{
	return(tmp_buf->buf[tmp_buf->end]);
}

unsigned char cbuf_front_pos(struct cbuf *tmp_buf)
{
	return(tmp_buf->front);
}

unsigned char cbuf_end_pos(struct cbuf *tmp_buf)
{
	return(tmp_buf->end);
}