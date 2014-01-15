/*
  File: uart.h
  
  Содержит прототипы функций : 
    - инициализация/настройка UART
    - начало пеердачи через UART строки
     
*/


void init_uart();

void send_string(const char* in_string);

