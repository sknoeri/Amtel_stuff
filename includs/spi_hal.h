#ifndef SPI_HAL_
#define SPI_HAL_

#include "iom328p.h"
#include "interrupt.h"

#define SS   PORTB2
#define MISO PORTB4
#define MOSI PORTB3
#define SCK  PORTB5
#define SPI_DDR DDRB
#define SPI_PORT PORTB

void spi_master_init(void);
void spi_transfer(unsigned char *tx, unsigned char *rx, unsigned char len);

#endif