#include "../includs/uart_hal.h"
#define F_CPU 16000000U
#define BUFFER_SIZE 128
volatile static unsigned char uart_tx_busy = 1;
volatile static unsigned char rx_buffer[BUFFER_SIZE] = {0};
volatile static unsigned char rx_count = 0; // indicates how many data bytes are in the buffer
// 1 is not busy and 0 is busy

ISR(USART_RX_vect){//Recive interrupt vector
    static unsigned char rx_read_count=0;
    rx_buffer[rx_read_count] = UDR0;
    rx_read_count++;
    rx_count++;
    if(rx_read_count>=BUFFER_SIZE){
        rx_read_count = 0;
    }
}
ISR(USART_TX_vect){ //Transmit interrupt vector interups is fired when transmision is complet
    uart_tx_busy=1;
}
void uart_init(unsigned long baud, unsigned char fast_mode){
    static unsigned char mode = 16;
    if(fast_mode !=1){
        UCSR0A |= (1<<U2X0);
        mode = 8;
    }else{
        UCSR0A &= ~(1<<U2X0);
    }
    baud = (F_CPU/mode/baud)-1;
    
    UBRR0H = (unsigned char)(baud >> 8); //0x00;//
    UBRR0L = (unsigned char)baud; //0x08;//
    //Enable transmit and recive
    UCSR0B = (1<<TXEN0)|(1<<RXEN0);
    //Enable interrupt when trasmited or recived
    UCSR0B |= (1<<TXCIE0)|(1<<RXCIE0);
    //Set frame format: 8data, 1 srop bit, not needed to do dat is set like this by deafult
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void uart_send_byte(unsigned char data){
    while (uart_tx_busy == 0);
    while(!(UCSR0A & (1 << UDRE0)));
    uart_tx_busy = 0;
    UDR0 = data;
}
void uart_send_string(char *data){
    while(*data > 0){ // a string can be understood like a pointer: cahr *p = "Hallo"
        uart_send_byte(*data++); // a string can be understood like an char p[6] = "Hallo"
    } // *data++ increments the pointer and returns its old value
}

void uart_sendU16(unsigned short val){
    unsigned char dig1 = '0', dig2 = '0', dig3 = '0', dig4 = '0';

    // count value in 10000s place
    while(val >= 10000){
        val -= 10000;
        dig1++;
    }

    // count value in 1000s place
    while(val >= 1000){
        val -= 1000;
        dig2++;
    }

    // count value in 100s place
    while(val >= 100){
        val -= 100;
        dig3++;
    }

    // count value in 10s place
    while(val >= 10){
        val -= 10;
        dig4++;
    }
    /*while (val>=0.1){
        val -= 0.1
        ldig1++
    }
    */
    
    // was previous value printed?
    unsigned char prevPrinted = 0;

    // print first digit (or ignore leading zeros)
    if(dig1 != '0') {uart_send_byte(dig1); prevPrinted = 1;}

    // print second digit (or ignore leading zeros)
    if(prevPrinted || (dig2 != '0')) {uart_send_byte(dig2); prevPrinted = 1;}

    // print third digit (or ignore leading zeros)
    if(prevPrinted || (dig3 != '0')) {uart_send_byte(dig3); prevPrinted = 1;}

    // print third digit (or ignore leading zeros)
    if(prevPrinted || (dig4 != '0')) {uart_send_byte(dig4); prevPrinted = 1;}

    // print final digit
    uart_send_byte(val + '0');
}
void uart_sendS16(unsigned short val){
    // check for negative number
    if(val & 0x8000)
    {
        // print minus sign
        uart_send_byte('-');

        // convert to unsigned magnitude
        val = ~(val - 1);
    }

    // print unsigned magnitude
    uart_sendU16((unsigned short) val);
}

void uart_sendfVal(float val){
    unsigned char dig1 = '0', dig2 = '0', dig3 = '0', dig4 = '0', dig5 = '0';
    unsigned char ldig1 = '0', ldig2 = '0', ldig3 = '0', ldig4 = '0';
    //Precheck of float
    if(val>=100000||val<=-100000){
        uart_send_string("Float vlaue to high for print out");
    }else{
        if(val < 0){
            val*=-1;
            uart_send_byte('-');
        }
        // count value in 10000s place
        while(val >= 10000){
            val -= 10000;
            dig5++;
        }
        // count value in 1000s place
        while(val >= 1000){
            val -= 1000;
            dig4++;
        }
        // count value in 100s place
        while(val >= 100){
            val -= 100;
            dig3++;
        }
        // count value in 10s place
        while(val >= 10){
            val -= 10;
            dig2++;
        }
        while(val >= 1){
            val -= 1;
            dig1++;
        }
        //past the comma
        while (val>=0.1){
            val -= 0.1;
            ldig1++;
        }
        while (val>=0.01){
            val -= 0.01;
            ldig2++;
        }
        while (val>=0.001){
            val -= 0.001;
            ldig3++;
        }
        while (val>=0.0001){
            val -= 0.0001;
            ldig4++;
        }
        // was previous value printed?
        unsigned char prevPrinted = 0;

        // print first digit (or ignore leading zeros)
        if(dig5 != '0') {uart_send_byte(dig5); prevPrinted = 1;}
        // print second digit (or ignore leading zeros)
        if(prevPrinted || (dig4 != '0')) {uart_send_byte(dig4); prevPrinted = 1;}
        // print third digit (or ignore leading zeros)
        if(prevPrinted || (dig3 != '0')) {uart_send_byte(dig3); prevPrinted = 1;}
        // print third digit (or ignore leading zeros)
        if(prevPrinted || (dig2 != '0')) {uart_send_byte(dig2); prevPrinted = 1;}
        // print third digit (or ignore leading zeros)
        if(prevPrinted || (dig1 != '0')) {uart_send_byte(dig1); prevPrinted = 1;}
        uart_send_byte('.');

        // print first ldigit (or ignore leading zeros)
        if(prevPrinted || (ldig1 != '0')) {uart_send_byte(ldig1); prevPrinted = 1;}
        // print second ldigit (or ignore leading zeros)
        if(prevPrinted || (ldig2 != '0')) {uart_send_byte(ldig2); prevPrinted = 1;}
        // print first ldigit (or ignore leading zeros)
        if(prevPrinted || (ldig3 != '0')) {uart_send_byte(ldig3); prevPrinted = 1;}
        // print second ldigit (or ignore leading zeros)
        if(prevPrinted || (ldig4 != '0')) {uart_send_byte(ldig4); prevPrinted = 1;}
        // print final digit
        //suart_send_byte(val + '0');
    }
}

void uart_sendhex8(unsigned char val){
    // extract upper and lower nibbles from input value
    unsigned char upperNibble = (val & 0xF0) >> 4;
    unsigned char lowerNibble = val & 0x0F;

    // convert nibble to its ASCII hex equivalent
    upperNibble += upperNibble > 9 ? 'A' - 10 : '0';
    lowerNibble += lowerNibble > 9 ? 'A' - 10 : '0';

    // print the characters
    uart_send_byte('0');
    uart_send_byte('x');
    uart_send_byte(upperNibble);
    uart_send_byte(lowerNibble);
}

unsigned char uart_read_count(void){
    return rx_count;
}

unsigned char uart_read(void){
    static unsigned char rx_read_pos = 0;
    unsigned char data;
    data = rx_buffer[rx_read_pos];
    rx_read_pos++;
    rx_count--;
    if (rx_read_pos>=BUFFER_SIZE){
        rx_read_pos = 0;
    }
    return data;
} 

