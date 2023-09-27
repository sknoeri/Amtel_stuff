#ifndef WIRE_HAL_
#define WIRE_HAL_
#include "iom328p.h"
#include "interrupt.h"
#include "uart_hal.h"


#define F_CPU 16000000U
#define WIRE_TIMEOUT 1600

//Write operation TWSR codes
#define WIRE_START      0x08
#define WIRE_RSTART     0x10
#define WIRE_ADDR_ACK   0x18
#define WIRE_ADDR_NACK  0x20
#define WIRE_DATA_ACK   0x28
#define WIRE_DATA_NACK  0x30
#define WIRE_ERROR      0x38

//Read operation TWSR codes
#define WIRER_ADDR_ACK  0x40
#define WIRER_ADDR_NACK 0x48
#define WIRER_DATA_ACK  0x50
#define WIRER_DATA_NACK 0x58
#define WIRE_NONE       0xF8

enum{
    WIRE_OK,
    WIRE_ERROR_START,
    WIRE_ERROR_RSTART,
    WIRE_NACK
};

void wire_clockF(unsigned long freq);
unsigned char wire_read(unsigned char sladdr,unsigned char reg,unsigned char *data,unsigned short len);
unsigned char wire_write(unsigned char sladdr,unsigned char reg,unsigned char *data,unsigned short len);
unsigned char wire_writeByte(unsigned char sladdr,unsigned char reg,unsigned char byte);

#endif/*WIRE_HAL_*/