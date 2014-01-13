/*
  File: bands.с
  
*/

#include <avr/io.h>
#include "bands.h"


void init_band_to_pin_tables()
{

  band_to_pin_tables[0].code_band= BAND_160;   // код диапазона
  band_to_pin_tables[0].out_port = PORTD;     // порт ввода вывода на котором расположен пин соответствующий диапазону
  band_to_pin_tables[0].in_port  = PIND;
  band_to_pin_tables[0].pin      = PIND2;      // пин соответствующий диапазону
  band_to_pin_tables[0].regisrt  = DDRD;   

  band_to_pin_tables[1].code_band= BAND_80;
  band_to_pin_tables[1].out_port = PORTD;
  band_to_pin_tables[1].in_port  = PIND;
  band_to_pin_tables[1].pin      = PIND3;
  band_to_pin_tables[1].regisrt  = DDRD;   


  band_to_pin_tables[2].code_band= BAND_40;
  band_to_pin_tables[2].out_port = PORTD;
  band_to_pin_tables[2].in_port  = PIND;
  band_to_pin_tables[2].pin      = PIND4;
  band_to_pin_tables[2].regisrt  = DDRD;   

  band_to_pin_tables[3].code_band= BAND_20;
  band_to_pin_tables[3].out_port = PORTD;
  band_to_pin_tables[3].in_port  = PIND;
  band_to_pin_tables[3].pin      = PIND5;
  band_to_pin_tables[3].regisrt  = DDRD;   

  band_to_pin_tables[4].code_band= BAND_15;
  band_to_pin_tables[4].out_port = PORTD;
  band_to_pin_tables[4].in_port  = PIND;
  band_to_pin_tables[4].pin      = PIND6;
  band_to_pin_tables[4].regisrt  = DDRD;   

  band_to_pin_tables[5].code_band= BAND_10;
  band_to_pin_tables[5].out_port = PORTD;
  band_to_pin_tables[5].in_port  = PIND;
  band_to_pin_tables[5].pin      = PIND7;
  band_to_pin_tables[5].regisrt  = DDRD;   

}


void init_out_pins()
{

  int index;

  for(index=0; index<sizeof(band_to_pin_tables); ++index)
  {
    // задаём режим
    band_to_pin_tables[index].regisrt |= 1 << band_to_pin_tables[index].pin;

    // выставляем низкий уровень
    band_to_pin_tables[index].out_port |= ( 1 << band_to_pin_tables[index].pin );
  }

}


void set_band(int band_code)
{
  if(band_code == BAND_NO && band_code == BAND_UNKNOWN)
    return;

  if(band_code >= sizeof(band_to_pin_tables))
    return;

  /*
      + вычисляем индекс нового диапазона
      + вычисляем индекс текущего диапазона
      + проверяем индексы на  валидность
        + если индекс нового диапазона не валиден, выходим из функции
        + усли нидекс текущего диапазона не валиден (диапазон не включен) приваиваем значение индекса "0",
          чтобы аклгорит ниже не "развалился" на "выключении" текущего диапазона 
      + сравниваем индексы
       + если индексы равны, то ничего не делаем
       + если индексы не равны выключаем "текущий" диапазон, и включаем "новый"
  */

  int index_new_band = -1;
  int index;
  
  for(index=0; index<sizeof(band_to_pin_tables); ++index)
  {
    if(band_to_pin_tables[index].code_band = band_code)
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
    if(band_to_pin_tables[band_code].in_port & (1 << band_to_pin_tables[band_code].pin))
    {
      index_current_band = index;
      break;
    }
  };

  if(index_new_band != index_current_band)
  {
    if(index_current_band >= 0)
      band_to_pin_tables[index_current_band].out_port |= ( 1 << band_to_pin_tables[index_current_band].pin );

    band_to_pin_tables[index_new_band].out_port &= ~( 1 << band_to_pin_tables[index_new_band].pin );      
  }
}

