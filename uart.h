/*
  File: uart.h
  
  Содержит: 
    - инициализация/настройка UART
    - чтение символа из UART
    - звпись строки в UART 
*/


void init_uart();

int get_ch();

void put_string();

