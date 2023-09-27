
#include "includs/iom328p.h"
#include "includs/interrupt.h"
#include "includs/uart_hal.h"
#include "includs/wire_hal.h"

#define WHO_AM_I_MPU9250 0x75


int main() {

    uart_init(115200,0);
    wire_clockF(100000);
    for (long i = 0; i < 100000; i++)
        {
            PORTB =32;
        }
    //unsigned char accel[6] = 0;
    //unsigned char gyro[6] = 0;
    unsigned char wire_error =0;

    //enalbe interrupts
    sei();
    unsigned char data[] = {0x71};
    
    uart_send_string("Programm_start");
    uart_send_byte('\n');

    //wire_writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x00);
    //wire_writeByte(MPU9250_ADDRESS, PWR_MGMT_2, 0x00);
    
    while(1){
        //gyro_sygnals(accel,gyro);

        uart_sendU16(wire_error);
        uart_send_string("\n");
        wire_error = wire_write(0x68, WHO_AM_I_MPU9250,data,1);//wire_read(0x68, WHO_AM_I_MPU9250, data,1);
        //uart_sendhex8(data[0]);
        uart_send_string("\n");
        for (long i = 0; i < 100000; i++)  
        {
            PORTB =32;
        }
    }
}