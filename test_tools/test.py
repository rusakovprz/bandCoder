# -*- coding: utf-8 -*-
"""
  File: test.py
  
  Автоматизированный тест работы декодера диапазонов.

"""

from Tkinter import *
from tkMessageBox import *
import time
import thread
import serial

"""
  'Сценарий' тестирования, набор данных для тестирования.

    freq - 'текущая' частота
    output_code - бинарный код соответсвующий 'текщему' диапазону
                  (заложен на будующее для считывания с устройства 'обратной связи')
    dialog - текст выводимый в диалоговом окне
"""  

data_for_test_script = [

  {"freq":"00003500000", "output_code":0x02, "dialog":"Включен диапазон 3.5 мГц"},     #1
  {"freq":"00001809999", "output_code":0x00, "dialog":"Ни один диапазон не включен."},  
  {"freq":"00001810000", "output_code":0x00, "dialog":"Включен диапазон 1.8 мГц"},
  {"freq":"00003800000", "output_code":0x02, "dialog":"Включен диапазон 3.5 мГц"},
  {"freq":"00002000000", "output_code":0x00, "dialog":"Включен диапазон 1.8 мГц"},     #5
  {"freq":"00003600000", "output_code":0x02, "dialog":"Включен диапазон 3.5 мГц"}, 
  {"freq":"00002001000", "output_code":0x00, "dialog":"Ни один диапазон не включен."},
  {"freq":"00007000000", "output_code":0x04, "dialog":"Включен диапазон 7.0 мГц."},
  {"freq":"00003499999", "output_code":0x02, "dialog":"Ни один диапазон не включен."},
  {"freq":"00014000000", "output_code":0x08, "dialog":"Включен диапазон 14.0 мГц"},    #10
  {"freq":"00003801000", "output_code":0x02, "dialog":"Ни один диапазон не включен."},
  {"freq":"00007100000", "output_code":0x04, "dialog":"Включен диапазон 7.0 мГц."},
  {"freq":"00014350000", "output_code":0x08, "dialog":"Включен диапазон 14.0 мГц"},
  {"freq":"00006999999", "output_code":0x04, "dialog":"Ни один диапазон не включен."},
  {"freq":"00021100000", "output_code":0x10, "dialog":"Включен диапазон 21.0 мГц"},    #15
  {"freq":"00007201000", "output_code":0x04, "dialog":"Ни один диапазон не включен."},
  {"freq":"00029700000", "output_code":0x20, "dialog":"Включен диапазон 28.0 мГц"},
  {"freq":"00013999999", "output_code":0x08, "dialog":"Ни один диапазон не включен."},  
  {"freq":"00021450000", "output_code":0x10, "dialog":"Включен диапазон 21.0 мГц"},
  {"freq":"00014351000", "output_code":0x08, "dialog":"Ни один диапазон не включен."}, #20
  {"freq":"00002000000", "output_code":0x01, "dialog":"Включен диапазон 1.8 мГц"},
  {"freq":"00020999999", "output_code":0x10, "dialog":"Ни один диапазон не включен."},
  {"freq":"00007040000", "output_code":0x04, "dialog":"Включен диапазон 7.0 мГц."},
  {"freq":"00021451000", "output_code":0x10, "dialog":"Ни один диапазон не включен."},
  {"freq":"00027999999", "output_code":0x20, "dialog":"Ни один диапазон не включен."}, #25
  {"freq":"00029700000", "output_code":0x20, "dialog":"Включен диапазон 28.0 мГц"},
  {"freq":"00014200000", "output_code":0x08, "dialog":"Включен диапазон 14.0 мГц"},
  {"freq":"00029701000", "output_code":0x20, "dialog":"Ни один диапазон не включен."},
  {"freq":"00021100000", "output_code":0x10, "dialog":"Включен диапазон 21.0 мГц"},
  {"freq":"00029000000", "output_code":0x20, "dialog":"Включен диапазон 28.0 мГц"}     #30

] 


"""
  Набор данных для 'нагрузочного' тестирования.

    count - число циклов
    delay - задержка между переключениями (в секундах)
    [...] - набор частот
"""  

data_for_load_test = [

  {"count":2, "delay":1},
  ["00001900000", "00003600000", "00007040000", "00014200000", "00021100000", "00029000000"]
]

""" 'Текущая' частота - переменная используется для передачи значения между потоками """
g_s_frequency = 00000000000



class gui(Tk):
  def __init__(self):
    Tk.__init__(self)
    
    self.title("Автоматический тест декодера диапазонов для TS-590")
    #self.geometry('500x200')
    
    self.button_run_test = Button( self, text = "Начать автоматизированное тестирование",
                               command = lambda:self.run_test() )
    self.button_run_test.pack()


    self.button_run_test = Button( self, text = "Начать нагрузочное тестирование",
                               command =  lambda:thread.start_new_thread( self.loading_test, () ) )
    self.button_run_test.pack()

    self.var_check_ansver = IntVar()
    self.var_check_ansver.set(1)
    self.check_ansver = Checkbutton(self, variable = self.var_check_ansver, 
                                    text="Ответы на команду IF;")
    self.check_ansver.pack()
        
    self.result_text = Label(self)
    self.result_text.pack()

    self.frequency_label = Label(self, bg="orange")
    self.frequency_label.pack()    

    self.progress_label = Label(self, text=self.get_progress_text(0, 0), bg="light sky blue")
    self.progress_label.pack()

    self.set_frequency("00000000000")

    self.serial_device = "/dev/ttyS0"
    thread.start_new_thread( self.trx_imitator, () )
        

  def get_progress_text(self, current_value, max_value):
    return "Выполнено " + str(current_value) + " из " + str(max_value) + " тестовых случаев."


  def paint_result(self,successful, failure, total ):

    self.result_text["width"]=50
    self.result_text["height"]=10

    if failure == 0:
      self.result_text["bg"]="green"  
    else:
      self.result_text["bg"]="red"
      
    self.result_text["text"]=" \
    Резльтат тестирования: \n\
    Успешных тестов = " + str(successful) + "\n\
    Сбоев           = " + str(failure) + "\n\
    Всего тестов    = " + str(total)

   
  def run_test(self):
    """
      Автоматизированный тест.
    """ 

    len_list_test_case = len(data_for_test_script)
    successful = 0
    failure = 0
    self.progress_label["text"]=self.get_progress_text(0, len_list_test_case)
        
    for index, item in enumerate(data_for_test_script):

      self.set_frequency( item["freq"] )
      
      ret = askyesno("Тестовый случай #" + str(index+1),
                     "Вы наблюдаете следующее состояние?\n\n" + item["dialog"])

      if ret:
        successful +=1
      else:
        failure +=1

      self.progress_label["text"]=self.get_progress_text(index+1, len_list_test_case)
      
    self.paint_result(successful, failure, len_list_test_case )


  def loading_test(self):
    """
      'Нагрузочный' тест. Разработка не завершена.
      В текущей реализации используется как 'двухкратный прогон' по всем диапазонам 
       
      TODO: 'Вынести' в отдельный поток.
      
    """
    self.progress_label["text"]= "Выполнено циклов : 0 из " + str( data_for_load_test[0]["count"] )

    for iteration in range(data_for_load_test[0]["count"]):  

      for index, item in enumerate(data_for_load_test[1]):
        self.set_frequency( item )
        time.sleep(data_for_load_test[0]["delay"])

      self.progress_label["text"]= "Выполнено циклов : " + str(iteration+1) + " из " + str( data_for_load_test[0]["count"] )

    showinfo("Выполнено", "Нагрузочное тестирование завершено.")
    

  def set_frequency(self, freq):
    if len(freq) != 11:
      showerror("Ошибка", "Значение частоты должно состоять из 11 символов.")
      return
        
    self.s_frequency = freq
    self.frequency_label["text"] = "Текущая частота = " + self.s_frequency
    

  def trx_imitator(self):
    ser = serial.Serial( self.serial_device, baudrate=9600,
                          parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE,
                          bytesize=serial.EIGHTBITS )

    receive_command = ""
  
    while(True):
      receive_char = ser.read()
      receive_command += receive_char
    
      if receive_char == ";":
        if receive_command == "IF;" and self.var_check_ansver.get(): 
          send_command = "IF" + str(self.s_frequency) + "2222233333456789ABCDEEF;"
          ser.write(send_command)
                
        receive_command = ""
    
    
    
if __name__ == '__main__':
  gui().mainloop() 

