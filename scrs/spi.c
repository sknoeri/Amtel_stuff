#include "../includs/spi_hal.h"

static volatile unsigned char spi_transmitted = 0;
static volatile unsigned char spi_recived = 0;

ISR(SPI_STC_vect){
    if(SPCR & (1<<MSTR)){
        spi_transmitted = 1;
    }else if (!(SPCR & (1<<MSTR))){
        spi_recived = 1;
    }
}

void spi_master_init(void){//Interrupt enabled SPI enalbeld in master mode
    DDRB &=~(1<<DDB2)|~(1<<DDB4);
    PORTB &=~(1<<PORTB2)|~(1<<PORTB4); //Configure SS and MISO pin as input
    DDRB |=(1<<DDB3)|(1<<DDB5);
    PORTB |=(1<<PORTB3)|(1<<PORTB5);//Configures Mosi and SCK pin as output
    SPCR |= (1<<SPIE)|(1<<SPE)|(1<<MSTR)|(1<<CPHA); //Turns SPI on with f_osc/4
}

// muss chip select machen, data, reg , lenge
void spi_send(unsigned char *data, unsigned char pin, unsigned char reg, unsigned char len){

}

unsigned char spi_recive(void){
    if(spi_recived==1){
        spi_recived = 0;
        return SPDR;
    }
}