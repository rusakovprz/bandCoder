/*
  File: bands.с
  
*/

#include <avr/io.h>
#include "bands.h"


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

}


void init_out_pins()
{

  int index;
  uint8_t* ptR;
  
  for(index=0; index<sizeof(band_to_pin_tables); ++index)
  {
    // задаём режим
    ptR = (uint8_t*) band_to_pin_tables[index].regisrt; 
    *ptR |= 1 << band_to_pin_tables[index].pin;
    
    // выставляем Высокий уровень
    ptR = (uint8_t*) band_to_pin_tables[index].out_port;
    *ptR |= ( 1 << band_to_pin_tables[index].pin );
  }
  
}


void set_band(int band_code)
{
  /*
    Реле срабатывают от подачи низкого уровня на пины портов ввода вывода.

    TODO: 1) Добавить в таблицу band_to_pin_tables поле "уровень срабатывания реле",
          доработать код ниже. 
          Предварительно нужно оценить необходимость такой "универсальности".
          2) Провести декомпозицию функции init_out_pins() на:
             - направление ввода вывода пинов
             - установка логических уровней в "исходном сотоянии" 
  */

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
  
  for(index=0; index<sizeof(band_to_pin_tables); ++index)
  {
    if(band_to_pin_tables[index].code_band == band_code)
    {
      index_new_band = index;
      break;
    }
  };

  if(index_new_band < 0)
    return;

  int index_current_band = -1;

  for(index=0; index<sizeof(band_to_pin_tables); ++index)
  { 
    ptR = (uint8_t*)band_to_pin_tables[index].in_port;

    if( !(*ptR & (1 << band_to_pin_tables[index].pin)) )
    {
      index_current_band = index;
      break;
    }
  };
  
  if(index_new_band != index_current_band)
  {
    if(index_current_band >= 0)
    {
      ptR = (uint8_t*)band_to_pin_tables[index_current_band].out_port;
      *ptR |= ( 1 << band_to_pin_tables[index_current_band].pin );
    }

    ptR = (uint8_t*)band_to_pin_tables[index_new_band].out_port;
    *ptR &= ~( 1 << band_to_pin_tables[index_new_band].pin );      
  }
}

