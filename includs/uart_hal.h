#ifndef UART_HAL_
#define UART_HAL_
#include "iom328p.h"
#include "interrupt.h"


void uart_init(unsigned long baud, unsigned char fast_mode);
void uart_send_byte(unsigned char data);
void uart_send_string(char *data);
void uart_sendU16(unsigned short val);
void uart_sendS16(unsigned short val);
void uart_sendfVal(float val);
void uart_sendhex8(unsigned char val);
unsigned char uart_read_count(void);
unsigned char uart_read(void);
#endif /*UART_HAL_*/