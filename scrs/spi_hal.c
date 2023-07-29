#include "../includs/spi_hal.h"

volatile unsigned char spi_rx_data = 1;
volatile unsigned char spi_txrx_done = 1;

ISR(SPI_STC_vect){ // interrupt triggerd after transmission done
    if(SPSR & (1<<WCOL)){ // if write colision we must clear the WCOL flag by dummy data read
        spi_rx_data = SPDR;
        spi_rx_data = 0;
        spi_txrx_done = 1;
    }else{
        spi_rx_data = SPDR;
        spi_txrx_done = 1;
    }
}

void spi_master_init(void){//Interrupt enabled SPI enalbeld in master mode
    SPI_DDR &=~(1<<MISO);
    SPI_PORT &=~(1<<MISO); //Configure SS and MISO pin as input
    SPI_DDR |=(1<<SCK)|(1<<MOSI)|(1<<SS); //Configures Mosi and SCK pin as output
    SPI_PORT |=(1<<SS); // initatilze slave select as high because we are in master mode
    SPCR = (1<<SPIE)|(1<<SPE)|(1<<MSTR)|(1<<CPOL)|(0<<CPHA); //Turns SPI on with f_osc/4
    SPCR |=(1<<SPR1)|(1<<SPR0); //fck/128, slowest speed
}
static void spi_send_byte(unsigned char *tx, unsigned char *rx){
    if(spi_txrx_done == 1){
        spi_txrx_done = 0;
        SPDR = *tx; // sends the data 
        while(spi_txrx_done == 0);
        *rx = spi_rx_data; // reads the SPDR register
    }
}
// muss chip select machen, data, reg , lenge
void spi_transfer(unsigned char *tx, unsigned char *rx, unsigned char len){
    SPI_PORT &=~(1<<SS);
    for (unsigned char i = 0; i < len; i++){
        spi_send_byte(&tx[i],&rx[i]);
    }
    PORTB |= (1<<SS);
}
