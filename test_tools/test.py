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
  {"freq":"00029000000", "output_code":0x20, "dialog":"Включен диапазон 28.0 мГц"},    #30

  {"freq":"00010099999", "output_code":0x00, "dialog":"Ни один диапазон не включен."},
  {"freq":"00010100000", "output_code":0x00, "dialog":"Включен диапазон 28.0 мГц"},  
  {"freq":"00010151000", "output_code":0x00, "dialog":"Ни один диапазон не включен."},
  {"freq":"00010150000", "output_code":0x00, "dialog":"Включен диапазон 28.0 мГц"},    #34
  
  {"freq":"00018067999", "output_code":0x00, "dialog":"Ни один диапазон не включен."},
  {"freq":"00018068000", "output_code":0x00, "dialog":"Включен диапазон 18.0 мГц"},
  {"freq":"00018169000", "output_code":0x00, "dialog":"Ни один диапазон не включен."},
  {"freq":"00018168000", "output_code":0x00, "dialog":"Включен диапазон 18.0 мГц"},   #38

  
  {"freq":"00024889999", "output_code":0x00, "dialog":"Ни один диапазон не включен."},
  {"freq":"00024890000", "output_code":0x00, "dialog":"Включен диапазон 25.0 мГц"},
  {"freq":"00025141000", "output_code":0x00, "dialog":"Ни один диапазон не включен."},
  {"freq":"00025140000", "output_code":0x00, "dialog":"Включен диапазон 25.0 мГц"}    #42
] 


"""
  Набор данных для 'нагрузочного' тестирования.

    count - число циклов
    delay - задержка между переключениями (в секундах)
    [...] - набор частот
"""  

data_for_load_test = [

  {"count":2, "delay":1},
  ["00001900000", "00003600000", "00007040000", "00014200000", "00021100000", "00029000000",
   "00010120000", "00018100000", "00025000000"]
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

    self.frame_raw_frequency = LabelFrame(self, text="Ручной ввод частоты")
    self.frame_raw_frequency.pack()

    #---------------------------------------------------------------
    self.label1 = Label(self.frame_raw_frequency, text="частота #1")
    self.label1.grid(row=0, column=0)
    self.entry1 = Entry(self.frame_raw_frequency )
    self.entry1.grid(row=0, column=1)
    self.button1 = Button(self.frame_raw_frequency, text ="Установить", 
                            command=lambda:self.set_frequency( self.norm_frequency(self.entry1.get()) ))
    self.button1.grid(row=0, column=2)
    #---------------------------------------------------------------
    self.label2 = Label(self.frame_raw_frequency, text="частота #2")
    self.label2.grid(row=1, column=0)
    self.entry2 = Entry(self.frame_raw_frequency)
    self.entry2.grid(row=1, column=1)
    self.button2 = Button(self.frame_raw_frequency, text ="Установить", 
                            command=lambda:self.set_frequency( self.norm_frequency(self.entry2.get()) ))
    self.button2.grid(row=1, column=2)
    #---------------------------------------------------------------
    self.label3 = Label(self.frame_raw_frequency, text="частота #3")
    self.label3.grid(row=2, column=0)
    self.entry3 = Entry(self.frame_raw_frequency)
    self.entry3.grid(row=2, column=1)
    self.button3 = Button(self.frame_raw_frequency, text ="Установить", 
                            command=lambda:self.set_frequency( self.norm_frequency(self.entry3.get()) ))
    self.button3.grid(row=2, column=2)
    #---------------------------------------------------------------
    self.label4 = Label(self.frame_raw_frequency, text="частота #4")
    self.label4.grid(row=3, column=0)
    self.entry4 = Entry(self.frame_raw_frequency)
    self.entry4.grid(row=3, column=1)
    self.button4 = Button(self.frame_raw_frequency, text ="Установить", 
                            command=lambda:self.set_frequency( self.norm_frequency(self.entry4.get()) ))
    self.button4.grid(row=3, column=2)
    #---------------------------------------------------------------
    self.label5 = Label(self.frame_raw_frequency, text="частота #5")
    self.label5.grid(row=4, column=0)
    self.entry5 = Entry(self.frame_raw_frequency)
    self.entry5.grid(row=4, column=1)
    self.button5 = Button(self.frame_raw_frequency, text ="Установить", 
                            command=lambda:self.set_frequency( self.norm_frequency(self.entry5.get()) ))
    self.button5.grid(row=4, column=2)
    #---------------------------------------------------------------
    self.label6 = Label(self.frame_raw_frequency, text="частота #6")
    self.label6.grid(row=5, column=0)
    self.entry6 = Entry(self.frame_raw_frequency)
    self.entry6.grid(row=5, column=1)
    self.button6 = Button(self.frame_raw_frequency, text ="Установить", 
                            command=lambda:self.set_frequency( self.norm_frequency(self.entry6.get()) ))
    self.button6.grid(row=5, column=2)

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


  @staticmethod
  def norm_frequency(freq):
    """
      Нормализация частоты - приводит строку к длинне в 11 знаков.
    """
    if len(freq) > 11:
      return freq[-11:]

    while len(freq) < 11:
      freq = "0" + freq

    return freq
        

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

