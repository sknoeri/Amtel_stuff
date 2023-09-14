
#include "includs/iom328p.h"
#include "includs/interrupt.h"
#include "includs/uart_hal.h"
#include "includs/wire_hal.h"
#include "includs/spi_hal.h"


/*#define DDRD    *((volatile unsigned char*) 0x2A)
#define PORTD   *((volatile unsigned char*) 0x29)
#define TIMER0A *((volatile unsigned char*) 0x44)
#define TIMER0B *((volatile unsigned char*) 0x45)
#define PORTB   *((volatile unsigned char*) 0x25)
#define DDRB    *((volatile unsigned char*) 0x24)*/

int main() {
  DDRB = 0x00;
  PORTB = 0x00;
  DDRB |= (1<<5);
  PORTB |=(1<<5);
  
  TCCR1A = 0x00;
  TCCR1B = 0x00;
  //TCNT1 = 2625;
  //TCCR1B |=(1<<WGM12); //CTC mode -- compare to OCR1A
  TCCR1B |=(1<<CS12);//Set prescaler to 1024
  //Set the interuupt
  TIMSK1 |=(1<<TOIE1);
  uart_init(9600,0);
  //spi_master_init();

  //enalbe interrupts
  sei();
  //char *h = "Hallo" ;
  unsigned char a[4] = {23,3,4,4};
  unsigned char b[4];
  //OCR0A=200;
  //TCCR0A=0;
  //TCCR0B=0;
  //TCCR0A|=(1<<COM0A1);//|(0<<COM0A0); // INITAILIZE THE COMPARE MATCH
  //TCCR0A|=(1<<WGM01)|(1<<WGM00); // FAST PWM MODE UPDATE THE OUPUT PIN AT BOOTOM
  //TCCR0B|=(0<<WGM02)|(1<<CS02)|(0<<CS01)|(0<<CS00); // Register B CS0x Sets the prescaler of the timer
  //Timer0B|=(1<<2);//|(1<<0)
  uart_send_byte('\n');
  uart_send_string("Hallo");
  uart_send_byte('\n');
  uart_sendU16(400);
  uart_send_byte('\n');
  uart_sendfVal(-99219.345);
  uart_send_byte('\n');
  uart_sendhex8(0x88);
  
  while(1){
    //for (long i = 0; i < 500000; i++){PORTB |=(1<<5);}
    if (uart_read_count()>0){
      unsigned char data = uart_read();
      uart_send_byte(data);
    }
    
    //for (long i = 0; i < 500000; i++){PORTB &=~(1<<5);}
  }
}

ISR(TIMER1_OVF_vect){
  PORTB ^=(1<<5);
  /*uart_send_byte('B');
  uart_send_byte('\n');
  TCNT1 = 2625;*/
}