/*
  File: uart.h
  
  Содержит прототипы функций : 
    - инициализация/настройка UART
    - начало пеердачи через UART строки
    - проверка счётчика запросов (команда IF;)
     
*/


void init_uart();

void send_string(const char* in_string);

void check_count_request();
