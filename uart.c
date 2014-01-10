/*
  File: uart.c
  
  http://mainloop.ru/avr-atmega/avr-usart-setting.html
  http://www.google.ru/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&ved=0CCoQFjAA&url=http%3A%2F%2Favr-libc.narod.ru%2FDataseets%2F3_Atmega16_32%2F6_USART.doc&ei=YRG_UovPPPT2ygOH64HICg&usg=AFQjCNH03-SPFAIOnt6LvcKvdSLG0SXSmg&bvm=bv.58187178,d.bGQ&cad=rjt
  
*/

#define max_len_buffer_receiver 40

#include <avr/interrupt.h>
#include <avr/io.h>
#include "uart.h"

// приёмный буфер
char buffer_receiver[max_len_buffer_receiver];

// индекс  очередного "свободного" элемента приёмного буфера 
int index_item_receiver;


void init_uart()
{
  
  // Для частоты микроконтроллера 20МГц и скорости 9600 
  // в регистр скорости следует записать значение 129. 
  UBRR0L = 129;
  UBRR0H = 129 >> 8;
  
  // 8 бит данных.
  UCSR0C |= ( 1 << UMSEL01 ) | ( 0 << UCSZ02 ) | ( 1 << UCSZ01 ) | ( 1 << UCSZ00 );

  // Режим с отключенным контролем четности.
  UCSR0C |= ( 1 << UMSEL01 ) | ( 0 << UPM01 ) | ( 0 << UPM00 ) ;
  
  // Режим с одним стоп-битом.
  UCSR0C |= ( 1 << UMSEL01 ) | ( 0 << USBS0 );

  // Разрешение приема и передачи данных по UART.
  UCSR0B |= ( 1 << TXEN0 ) | ( 1 << RXEN0 );

  // Разрешение прерывания по завершению приема, по освобождению регистра данных передтчика.
  UCSR0B |= ( 1 << RXCIE0 ) | ( 1 << UDRIE0 );

  index_item_receiver = 0;
}


ISR( USART_RX_vect )
{
  /*
    + принять очередной символ
    + проверить на равенсво символу ';' код 0x3B
    + проверить на переполнение приёмного буфера
    + проверить, что в буфере лежит ответ 'IF' (0x49 0x46)
    - байты буфера с 6-10 включительно перевести из строки в целое число 
      ( частота в килогерцах см. файл bands.h )
    - сравнить частоту со значениями границ диапазонов и установить
      соответсвующий диапзон ( bands.h void set_band(int band_code); )
     
  */

  char rxbyte = UDR0;
  buffer_receiver[index_item_receiver] = rxbyte;
  
  if ( rxbyte == ';')
  {
    // принят пакет, парсим
    if (buffer_receiver[index_item_receiver] == 'I' &&
        buffer_receiver[index_item_receiver] == 'F')
    {


    } else return;

  } 
    else
    { 
      // проверяем на переполнение буфера  
      if (index_item_receiver >= max_len_buffer_receiver)
      {
        index_item_receiver = 0;
        return;
      }
    }
}


ISR( USART_UDRE_vect )
{

}


