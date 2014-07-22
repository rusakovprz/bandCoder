/*
  File: bands.с
  
*/

#include <avr/io.h>
#include "bands.h"


// Таблица соответствия диапазонов линиям портов ввода вывода
// Поддерживаем 6 основных любительских КВ диапазонов и 3 дополнительных

static struct band_to_pin
{

  int code_band;    // код диапазона
  int out_port;     // адрес регистра записи данных порт ввода вывода на котором расположен пин соответствующий диапазону
  int in_port;      // адрес регистра чтения данных порта ввода вывода (используется для контроля состояния линий)
  int pin;          // пин соответствующий диапазону
  int regisrt;      // имя регистра конфигурации порта ввода вывода

} band_to_pin_tables[9];


void init_band_to_pin_tables()
{
  /*
    Задействованны только 6 основных любительских диапазонов.
    Используются пины 2-8 порта D.
  */
   
  band_to_pin_tables[0].code_band= BAND_160; // код диапазона
  band_to_pin_tables[0].out_port = 0x2B;     // регистр для записи в порт данных 
  band_to_pin_tables[0].in_port  = 0x29;     // регистр для чтения данных из порта 
  band_to_pin_tables[0].pin      = PIND2;    // пин соответствующий диапазону
  band_to_pin_tables[0].regisrt  = 0x2A;     // регистр направления порта (приём/передача) 

  band_to_pin_tables[1].code_band= BAND_80;
  band_to_pin_tables[1].out_port = 0x2B;
  band_to_pin_tables[1].in_port  = 0x29;
  band_to_pin_tables[1].pin      = PIND3;
  band_to_pin_tables[1].regisrt  = 0x2A;   

  band_to_pin_tables[2].code_band= BAND_40;
  band_to_pin_tables[2].out_port = 0x2B;
  band_to_pin_tables[2].in_port  = 0x29;
  band_to_pin_tables[2].pin      = PIND4;
  band_to_pin_tables[2].regisrt  = 0x2A;   

  band_to_pin_tables[3].code_band= BAND_20;
  band_to_pin_tables[3].out_port = 0x2B;
  band_to_pin_tables[3].in_port  = 0x29;
  band_to_pin_tables[3].pin      = PIND5;
  band_to_pin_tables[3].regisrt  = 0x2A;   

  band_to_pin_tables[4].code_band= BAND_15;
  band_to_pin_tables[4].out_port = 0x2B;
  band_to_pin_tables[4].in_port  = 0x29;
  band_to_pin_tables[4].pin      = PIND6;
  band_to_pin_tables[4].regisrt  = 0x2A;   

  band_to_pin_tables[5].code_band= BAND_10;
  band_to_pin_tables[5].out_port = 0x2B;
  band_to_pin_tables[5].in_port  = 0x29;
  band_to_pin_tables[5].pin      = PIND7;
  band_to_pin_tables[5].regisrt  = 0x2A;   

// WARC bands

  band_to_pin_tables[6].code_band= BAND_30;
  band_to_pin_tables[6].out_port = 0x25;
  band_to_pin_tables[6].in_port  = 0x23;
  band_to_pin_tables[6].pin      = PINB0;
  band_to_pin_tables[6].regisrt  = 0x24;   

  band_to_pin_tables[7].code_band= BAND_17;
  band_to_pin_tables[7].out_port = 0x25;
  band_to_pin_tables[7].in_port  = 0x23;
  band_to_pin_tables[7].pin      = PINB1;
  band_to_pin_tables[7].regisrt  = 0x24;   

  band_to_pin_tables[8].code_band= BAND_12;
  band_to_pin_tables[8].out_port = 0x25;
  band_to_pin_tables[8].in_port  = 0x23;
  band_to_pin_tables[8].pin      = PINB2;
  band_to_pin_tables[8].regisrt  = 0x24;   

}


// Возвращает длинну таблицы
// cоответствие диапазонов линиям портов ввода вывода

inline int len_band_to_pin_tables()
{
  return sizeof(band_to_pin_tables)/sizeof(struct band_to_pin);
}


void init_out_pins()
{

  int index;
  uint8_t* ptR;
  
  for(index=0; index<len_band_to_pin_tables(); ++index)
  {
    // задаём режим
    ptR = (uint8_t*) band_to_pin_tables[index].regisrt; 
    *ptR |= 1 << band_to_pin_tables[index].pin;
    
    // выставляем уровень соответствующий режиму "реле выключено"
    ptR = (uint8_t*) band_to_pin_tables[index].out_port;
#ifndef OUTPUT_LEVEL_UP_ON
    *ptR |= ( 1 << band_to_pin_tables[index].pin );
#else
    *ptR &= ~( 1 << band_to_pin_tables[index].pin );
#endif
  }
  
}


void set_band(int band_code)
{

  if(band_code == BAND_NO)
    init_out_pins();
    
  if(band_code == BAND_UNKNOWN)
    return;

  /*
      + вычисляем индекс нового диапазона
      + вычисляем индекс текущего диапазона
      + проверяем индексы на  валидность
        + если индекс нового диапазона не валиден - выходим из функции
        + усли нидекс текущего диапазона не валиден - !!!! 
      + сравниваем индексы
       + если индексы равны, то ничего не делаем
       + если индексы не равны выключаем "текущий" диапазон, и включаем "новый"
  */

  int index_new_band = -1;
  int index;
  uint8_t* ptR;
  
  for(index=0; index<index<sizeof(band_to_pin_tables); ++index)
  {
    if(band_to_pin_tables[index].code_band == band_code)
    {
      index_new_band = index;
      break;
    }
  }

  if(index_new_band < 0)
    return;

  int index_current_band = -1;

  for(index=0; index<index<sizeof(band_to_pin_tables); ++index)
  { 
    ptR = (uint8_t*)band_to_pin_tables[index].in_port;

#ifndef OUTPUT_LEVEL_UP_ON
    if( !(*ptR & (1 << band_to_pin_tables[index].pin)) )
#else
    if( *ptR & (1 << band_to_pin_tables[index].pin) )
#endif
    {
      index_current_band = index;
      break;
    }
  }
  
  if(index_new_band != index_current_band)
  {
    if(index_current_band >= 0)
    {
      ptR = (uint8_t*)band_to_pin_tables[index_current_band].out_port;
#ifndef OUTPUT_LEVEL_UP_ON
      *ptR |= ( 1 << band_to_pin_tables[index_current_band].pin );
#else
      *ptR &= ~( 1 << band_to_pin_tables[index_current_band].pin );
#endif
    }

    ptR = (uint8_t*)band_to_pin_tables[index_new_band].out_port;
#ifndef OUTPUT_LEVEL_UP_ON
    *ptR &= ~( 1 << band_to_pin_tables[index_new_band].pin );      
#else
    *ptR |= ( 1 << band_to_pin_tables[index_new_band].pin );
#endif
  }
}

