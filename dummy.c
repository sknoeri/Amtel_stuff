
/*#ifndef _AVR_IO_H_
# define _AVR_IO_H_
// To avoid the error: "Include <avr/io.h> instead of this file."
#endif

#include "iom328p.h"
#include "interrupt.h"*/
#define PORTB   *((volatile unsigned char*) 0x25)
#define DDRB    *((volatile unsigned char*) 0x24)

/*struct PORT{
  unsigned char PORT0;
  unsigned char PORT1;
  unsigned char PORT2;
  unsigned char PORT3;
  unsigned char PORT4;
  unsigned char PORT5;
  unsigned char PORT6;
  unsigned char PORT7;
};*/





int main() {
  DDRB = 32;
  while(1){
    PORTB = 32;
    for (long i = 0; i < 100000; i++)
    {
    PORTB =32;
    }
    PORTB =0;
    for (long i = 0; i < 100000; i++)
    {
    PORTB = 0;
    }
  }
}
/*ISR(TIMER1_OVF_vect){
    PORTB ^=(1<<5);
    //TCNT1 = 2625;
}*/