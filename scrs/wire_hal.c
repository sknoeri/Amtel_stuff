#include "../includs/wire_hal.h"
volatile unsigned char status=0xF8;

ISR(TWI_vect){
    status = (TWSR & 0xF8);
}


static unsigned char wire_start(void){
    unsigned short i=0;
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE); // Sends start bit, allows enrupt
    //When TWINT is set Start condition was transmitted
    // The interrupt is triggered and sets TWINT to zero
    while(status != WIRE_START){ // chek if status register meet the start condition
        i++;
        if(i>=WIRE_TIMEOUT){
            return WIRE_ERROR_START;
        }
    }
    return WIRE_OK;
}
static void wire_stop(void){
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN)|(1<<TWIE);
}

static unsigned char wire_rstart(void){
    unsigned short i=0;
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE); // Sends start bit, allows enrupt
    //When TWINT is set Start condition was transmitted
    // Cheks if start conditions was meet if not it turns the LED on
    while(status != WIRE_RSTART){
        i++;
        if(i>=WIRE_TIMEOUT){
            return WIRE_ERROR_RSTART;
        }
    }
    return WIRE_OK;
}
static unsigned char wire_addr_write_ack(void){
    unsigned short i = 0;
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE);
    while(status != WIRE_ADDR_ACK){
        i++;
        if(i>=WIRE_TIMEOUT){
            return WIRE_NACK;
        }
    }
    return WIRE_OK;
}

static unsigned char wire_data_write_ack(void){
    unsigned short i = 0;
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE);
    // Clears interrupt flag fucks//enables bus and interupt wirte a data ack bite
    while(status != WIRE_DATA_ACK){
        i++;
        if(i>=WIRE_TIMEOUT){
            return WIRE_NACK;
        }
    }
    return WIRE_OK;
}

static unsigned char wire_addr_read_ack(void){
    unsigned short i = 0;
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE);
    while(status != WIRER_ADDR_ACK){
        i++;
        if(i>=WIRE_TIMEOUT){
            return WIRE_NACK;
        }
    }
    return WIRE_OK;
}

static unsigned char wire_data_read_ack(unsigned char ack){
    unsigned short i = 0;
    
    if(ack!=0){
        //Sends acknoleged bite to slave
        TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE)|(1<<TWEA);
        while(status != WIRER_DATA_ACK){
            i++;
            if(i>=WIRE_TIMEOUT){
                return WIRE_NACK;
            }
        }
    }else{
        TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE);
        while(status != WIRER_DATA_NACK){
            i++;
            if(i>=WIRE_TIMEOUT){
                return WIRE_NACK;
            }
        }
    }
    return WIRE_OK;
}

void wire_clockF(unsigned long freq){
    
    freq = (F_CPU/freq-16)/2;
    TWBR = freq&0xFF;
    TWCR = (1<<TWEN)|(1<<TWIE);
    PORTC |= (1 << PORTC5 | 1 << PORTC4); //Internal pulups activated needed for wire bus
}

unsigned char wire_read(unsigned char sladdr,unsigned char reg,unsigned char *data,unsigned short len){ //Function to do a write operation on theTWI
    unsigned short i = 0;
    unsigned char err=WIRE_OK;
    
    err = wire_start();//Sends start and gets error code
    if(err!=WIRE_OK){
        wire_stop();
        return err;
    }
    TWDR = (sladdr<<1)|0; // loads the slave address to the register, read bit is 0 because of 
    err = wire_addr_write_ack();
    if(err!=WIRE_OK){
        wire_stop();
        return err;
    }
    TWDR = reg;// loads the rgesiter we want to read to the device
    err = wire_data_write_ack();
    if(err!=WIRE_OK){
        wire_stop();
        return err;
    }
    err = wire_rstart();// restarts the bus to do a read operation
    if(err!=WIRE_OK){
        wire_stop();
        return err;
    }
    TWDR = (sladdr<<1)|1; // loads the slave address to the register, read bit is 1 because we want to read
    err = wire_addr_read_ack();
    if(err!=WIRE_OK){
        wire_stop();
        return err;
    }
    for ( i = 0; i < (len-1); i++){
        err = wire_data_read_ack(1);//waits until the data recived and sends the ACK bit to request the next one
        if(err!=WIRE_OK){
            wire_stop();
            return err;
        }
        data[i] = TWDR; // reads the data read out of the TWDR
    }
    err = wire_data_read_ack(0);//No more data request needed so no more ack bit send
    if(err!=WIRE_OK){
        wire_stop();
        return err;
    }
    data[i] = TWDR; // reads the data recived by the
    wire_stop();
    return err;
} 


unsigned char wire_write(unsigned char sladdr,unsigned char reg,unsigned char *data,unsigned short len){ //Function to do a write operation on theTWI
    unsigned short i = 0;
    unsigned char err=WIRE_OK;

    err = wire_start();//Sends start and gets error code
    if(err!=WIRE_OK){
        wire_stop();
        return err;
    }
    TWDR = (sladdr<<1)|0; // loads the slave address to the register, read bit is 0 because of 
    err = wire_addr_write_ack();
    if(err!=WIRE_OK){
        wire_stop();
        return err;
    }
    TWDR = reg;// loads the rgesiter from which we want to read to the device
    err = wire_data_write_ack();
    if(err!=WIRE_OK){
        wire_stop();
        return err;
    }
    for (i = 0; i < len; i++){
        TWDR = data[i];// loads the data we want to send to the device
        err = wire_data_write_ack();
        if(err!=WIRE_OK){
            wire_stop();
            return err;
        }
    }
    wire_stop();
    return err;
}

unsigned char wire_writeByte(unsigned char sladdr,unsigned char reg,unsigned char byte){ //Function to do a write operation on theTWI
    //unsigned short i = 0;
    unsigned char err=WIRE_OK;

    err = wire_start();//Sends start and gets error code
    if(err!=WIRE_OK){
        wire_stop();
        return err;
    }
    TWDR = (sladdr<<1)|0; // loads the slave address to the register, read bit is 0 because of 
    err = wire_addr_write_ack();
    if(err!=WIRE_OK){
        wire_stop();
        return err;
    }
    TWDR = reg;// loads the rgesiter from which we want to read to the device
    err = wire_data_write_ack();
    if(err!=WIRE_OK){
        wire_stop();
        return err;
    }

    TWDR = byte;// loads the data we want to send to the device
    err = wire_data_write_ack();
    if(err!=WIRE_OK){
        wire_stop();
        return err;
    }
    wire_stop();
    return err;
}