PROGRAMMER 			= arduino
CHIP 				= atmega328p
PORT 				= COM4
#-b57600 for arduino nano
BAUD 				= 57600
FILE_NAME           = linked_file
CONFIG_FILE 		= "C:\Program Files (x86)\Arduino\hardware\tools\avr\etc\avrdude.conf"
COMPILE 			= avr-gcc -Wall -Os -mmcu=$(CHIP)
COMPILE_NO_LINK 	= avr-gcc -c -Wall -Os -mmcu=$(CHIP)
default: compile uploade clean

compile:
	$(COMPILE_NO_LINK) accel.c -o accel.o
	$(COMPILE_NO_LINK) scrs/uart_hal.c -o uart_hal.o
	$(COMPILE_NO_LINK) scrs/wire_hal.c -o wire_hal.o
#	$(COMPILE_NO_LINK) scrs/spi_hal.c -o spi_hal.o
	$(COMPILE) accel.o uart_hal.o wire_hal.o -o $(FILE_NAME).out
#Wall truns wrnings on -Os opimisating code
	avr-objcopy -O ihex -j.text -j.data $(FILE_NAME).out $(FILE_NAME).hex
#hexdump $(FILE_NAME).hex
uploade:
	avrdude -C $(CONFIG_FILE) -v -p$(CHIP) -c $(PROGRAMMER) -P$(PORT) -b$(BAUD) -U flash:w:$(FILE_NAME).hex:i
# -v for verbose mode
clean:
	del accel.o 
	del uart_hal.o
	del wire_hal.o
	del $(FILE_NAME).out
	del $(FILE_NAME).hex