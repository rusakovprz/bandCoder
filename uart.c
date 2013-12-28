/*
  File: uart.c
  
  http://mainloop.ru/avr-atmega/avr-usart-setting.html
  http://www.google.ru/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&ved=0CCoQFjAA&url=http%3A%2F%2Favr-libc.narod.ru%2FDataseets%2F3_Atmega16_32%2F6_USART.doc&ei=YRG_UovPPPT2ygOH64HICg&usg=AFQjCNH03-SPFAIOnt6LvcKvdSLG0SXSmg&bvm=bv.58187178,d.bGQ&cad=rjt
  
*/

#include <avr/io.h>
#include "uart.h"


void init_uart()
{
  
  // Для частоты микроконтроллера 20МГц и скорости 9600 
  // в регистр скорости следует записать значение 129. 
  UBRR0L = 129;
  UBRR0H = 129 >> 8;
  
  // 8 бит данных.
  UCSR0C = ( 1 << UMSEL01 ) | ( 0 << UCSZ02 ) | ( 1 << UCSZ01 ) | ( 1 << UCSZ00 );

  // Режим с отключенным контролем четности.
  UCSR0C = ( 1 << UMSEL01 ) | ( 0 << UPM01 ) | ( 0 << UPM00 ) ;
  
  // Режим с одним стоп-битом.
  UCSR0C = ( 1 << UMSEL01 ) | ( 0 << USBS0 );

  // Разрешение приема и передачи данных по UART.
  UCSR0B = ( 1 << TXEN0 ) | ( 1 << RXEN0 );

  // Разрешение прерывания по завершению приема, по освобождению регистра данных передтчика.
  UCSR0B = ( 1 << RXCIE0 ) | ( 1 << UDRIE0 );

}


int get_ch()
{


}


void put_string()
{


}

