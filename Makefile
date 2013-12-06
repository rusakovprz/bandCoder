
compile : bc.c
	avr-gcc -mmcu=atmega168 -Os bc.c -o bc.o
	avr-objcopy -j .text -j .data -O ihex bc.o bc.hex

clean:
	rm -f bc.o

distclean: clean
	rm -f bc.hex

prog: bc.hex
	avrdude -p m168 -b 9600 -P /dev/ttyS0 -c dasa -U flash ./bc.hex

#c2n232i	serial port banging, reset=dtr sck=!rts mosi=!txd miso=!cts
#dapa		Direct AVR Parallel Access cable
#dasa		serial port banging, reset=rts sck=dtr mosi=txd miso=cts
#dasa3		serial port banging, reset=!dtr sck=rts mosi=txd miso=cts
#diecimila	alias for arduino-ft232r
