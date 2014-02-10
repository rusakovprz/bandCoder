/*
  File: bands.h

  Управления пинами портов ввода вывода подключенных к линиям выбора диапазонов.
  
  Содержит: 
    - константы кодирующие радиолюбительские КВ диапазоны
    - описание и "заполнение" таблицы соответствия диапазонов линиям портов ввода вывода 
*/


// Коды радиолюбительских КВ диапазонов.

#define BAND_NO   0
#define BAND_160  1   //  1810 - 2000 кГц
#define BAND_80   2   //  3500 - 3800 кГц
#define BAND_40   3   //  7000 - 7200 кГц 
#define BAND_30   4   // 
#define BAND_20   5   // 14000 - 14350 кГц
#define BAND_17   6   //  
#define BAND_15   7   // 21000 - 21450 кГц
#define BAND_12   8   // 
#define BAND_10   9   // 28000 - 29700 кГц
#define BAND_6    10  //  

#define BAND_UNKNOWN  15


// Соответствие диапазонов линиям портов ввода вывода
// Пока поддерживаем только 6 основных КВ диапазонов

static struct band_to_pin
{

  int code_band;    // код диапазона
  int out_port;     // адрес регистра записи данных порт ввода вывода на котором расположен пин соответствующий диапазону
  int in_port;      // адрес регистра чтения данных порта ввода вывода (используется для контроля состояния линий)
  int pin;          // пин соответствующий диапазону
  int regisrt;      // имя регистра конфигурации порта ввода вывода

} band_to_pin_tables[6];


// Заполнение таблицы соответствия диапазонов линиям портов ввода вывода.
void init_band_to_pin_tables();

// Инициализация пинов портов ввода вывода работающих на управление реле выбора дипазона
void init_out_pins();

// "Включение" диапазона
void set_band(int band_code);

