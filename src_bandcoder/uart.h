/*
  File: uart.h
  
  Содержит прототипы функций : 
    - инициализация/настройка UART
    - начало пеердачи через UART строки
    - проверка счётчика запросов (команда IF;)
    - проверка строки на предмет содержания не числовых символов     
*/


void init_uart();

void send_string(const char* in_string);

void check_count_request();

int check_numeric_string(char* s_frequency);

