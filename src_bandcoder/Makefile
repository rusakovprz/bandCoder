
ifndef SERIAL_PORT
SERIAL_PORT = /dev/ttyUSB0
endif

MMCU = -mmcu=atmega328p

# OPTIONS - флаги директив препроцоссора
#
# OUTPUT_LEVEL_UP_ON - Включение реле "ВЫСОКИМ" уровнем (лог. 1)
#  
# Защитные полосы 
# LPS  - Lower Protective Strip
# UPS  - Upper Protective Strip

OPTIONS = -DOUTPUT_LEVEL_UP_ON -DLPS=50 -DUPS=50 




bc.hex: obj_all.o 
	avr-objcopy -j .text -j .data -O ihex obj_all.o bc.hex

obj_all.o: bc.o bands.o uart.o
	avr-gcc $(MMCU) -Os bc.o bands.o uart.o -o obj_all.o

bc.o: bc.c
	avr-gcc -c $(MMCU) -Os bc.c -o bc.o 

bands.o: bands.c bands.h 
	avr-gcc -c $(MMCU) $(OPTIONS) -Os bands.c -o bands.o

uart.o: uart.c uart.h
	avr-gcc -c $(MMCU) $(OPTIONS) -Os uart.c -o uart.o

	
clean:
	rm -f *.o

distclean: clean
	rm -f bc.hex

prog_serial: bc.hex
	avrdude -p m328p -b 9600 -P $(SERIAL_PORT) -c dasa -U flash:w:bc.hex:i

prog_usbasp: bc.hex
	avrdude -p m328p -c usbasp -U flash:w:bc.hex:i

#c2n232i	serial port banging, reset=dtr sck=!rts mosi=!txd miso=!cts
#dapa		Direct AVR Parallel Access cable
#dasa		serial port banging, reset=rts sck=dtr mosi=txd miso=cts
#dasa3		serial port banging, reset=!dtr sck=rts mosi=txd miso=cts
#diecimila	alias for arduino-ft232r
