# -*- coding: utf-8 -*-
"""
  File: test.py
  
  Автоматизированный тест работы декодера диапазонов.

"""


"""
  'Сценарий' тестирования, набор данных для тестирования.

    freq - 'текущая' частота
    output_code - бинарный код соответсвующий 'текщему' диапазону
                  (заложен на будующее для считывания с устройства 'обратной связи')
    dialog - текст выводимый в диалоговом окне
"""  

data_for_test_script = [
 
  {"freq":"00000000000", "output_code":0x00, "dialog":"message."},
  {"freq":"00001800000", "output_code":0x01, "dialog":"message."},
  {"freq":"00003500000", "output_code":0x02, "dialog":"message."}, 
  {"freq":"00007040000", "output_code":0x04, "dialog":"message."},
  {"freq":"00014000000", "output_code":0x08, "dialog":"message."},
  {"freq":"00021000000", "output_code":0x10, "dialog":"message."},
  {"freq":"00028000000", "output_code":0x20, "dialog":"message."}
]


