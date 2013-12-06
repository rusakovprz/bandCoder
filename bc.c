#include <avr/interrupt.h>
#include<avr/io.h>

#define TCNT1_INIT 34286


/*
  Обработчик прерывания таймера.
*/
ISR( TIMER1_OVF_vect )
{
  TCNT1 = TCNT1_INIT; //выставляем начальное значение TCNT1.
  
  if( PINB & (1 << PB5) )
    PORTB &= ~( 1 << PB5 );
  else
    PORTB |= ( 1 << PB5 );
}


void init()
{
  DDRB |= 1<< PB5;                        // Настраиваем пин PB5 на выход.
  TCCR1B = (1<<CS12)|(0<<CS11)|(1<<CS10); // настраиваем делитель.
  TIMSK0 |= (1<<TOIE1);                   // разрешаем прерывание по переполнению таймера.
  TCNT1 = TCNT1_INIT;                     // выставляем начальное значение TCNT1.
  sei();                                  // выставляем бит общего разрешения прерываний.
}


void main_loop()
{
  while(1);
}


main()
{
  init();
  main_loop();             
}

