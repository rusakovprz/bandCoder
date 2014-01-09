
ifndef SERIAL_PORT
SERIAL_PORT = /dev/ttyS0
endif

MMCU = -mmcu=atmega328p

bc.hex: obj_all.o 
	avr-objcopy -j .text -j .data -O ihex obj_all.o bc.hex

obj_all.o: bc.o bands.o uart.o
	avr-gcc $(MMCU) -Os bc.o bands.o uart.o -o obj_all.o

bc.o: bc.c
	avr-gcc -c $(MMCU) -Os bc.c -o bc.o 

bands.o: bands.c bands.h 
	avr-gcc -c $(MMCU) -Os bands.c -o bands.o

uart.o: uart.c uart.h
	avr-gcc -c $(MMCU) -Os uart.c -o uart.o

	
clean:
	rm -f *.o

distclean: clean
	rm -f bc.hex

prog: bc.hex
	avrdude -p m328p -b 9600 -P $(SERIAL_PORT) -c dasa -U flash:w:bc.hex:i

#c2n232i	serial port banging, reset=dtr sck=!rts mosi=!txd miso=!cts
#dapa		Direct AVR Parallel Access cable
#dasa		serial port banging, reset=rts sck=dtr mosi=txd miso=cts
#dasa3		serial port banging, reset=!dtr sck=rts mosi=txd miso=cts
#diecimila	alias for arduino-ft232r
