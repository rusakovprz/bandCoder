/*
  File: bands.с
  
*/

#include <avr/io.h>
#include "bands.h"


void init_band_to_pin_tables()
{

  band_to_pin_tables[0].codeband = BAND_160;   // код диапазона
  band_to_pin_tables[0].io_port  = PORTD;      // порт ввода вывода на котором расположен пин соответствующий диапазону
  band_to_pin_tables[0].pin      = PIND2;      // пин соответствующий диапазону
  band_to_pin_tables[0].regisrt  = DDRD;   

  band_to_pin_tables[1].codeband = BAND_80;
  band_to_pin_tables[1].io_port  = PORTD;
  band_to_pin_tables[1].pin      = PIND3;
  band_to_pin_tables[1].regisrt  = DDRD;   


  band_to_pin_tables[2].codeband = BAND_40;
  band_to_pin_tables[2].io_port  = PORTD;
  band_to_pin_tables[2].pin      = PIND4;
  band_to_pin_tables[2].regisrt  = DDRD;   

  band_to_pin_tables[3].codeband = BAND_20;
  band_to_pin_tables[3].io_port  = PORTD;
  band_to_pin_tables[3].pin      = PIND5;
  band_to_pin_tables[3].regisrt  = DDRD;   

  band_to_pin_tables[4].codeband = BAND_15;
  band_to_pin_tables[4].io_port  = PORTD;
  band_to_pin_tables[4].pin      = PIND6;
  band_to_pin_tables[4].regisrt  = DDRD;   

  band_to_pin_tables[5].codeband = BAND_10;
  band_to_pin_tables[5].io_port  = PORTD;
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
    band_to_pin_tables[index].io_port &= ~( 1 << band_to_pin_tables[index].pin );
  }

}



