bandCoder
=========

Декодер диапазонов к трансиверу KENWOOD TS-590S построенный на базе Arduino Pro Mini v5 (микроконтроллер ATmega328p 16MHz).

Реализовано:
- Поддержка 6 основных любительских диапазонов;
- взаимодействие с трансивером на фиксированной скорости 9600 кб/с;
- темп опроса текущей частоты трансивера (команда IF;) 250 мс;
- “освобождение” диапазонов (не одно реле не включено) в случае отсутствия ответов от трансивера в течении 750 мс;
- “освобождение” диапазонов (не одно реле не включено) в случае если текущая частота трансивера не соответствует границам любительских диапазонов;
- соответсвие пинов портов ввода-вывода определяется программно (file: bands.c, function void init_band_to_pin_tables() ).

Общее описание идеологии аппаратной части, описано [здесь](https://docs.google.com/document/d/1nFi6O7A-nFe-ZUlmCB5rauzhndboRLULFgUdaN8hMWo/edit).

Электрическая схема устройства приведена [здесь в PNG формате]
(https://drive.google.com/file/d/0B4wqi_MPMXo2cWtZbWFmSVNaSTA/edit?usp=sharing) и [здесь в SCH формате]
(https://drive.google.com/file/d/0B4wqi_MPMXo2TnVRXzRRLUc3bjg/edit?usp=sharing).

Принципиальная электрические схемы программатора, соответствующего используемым опциям avrdude (см. Makefile, цель prog) приведена [здесь в PNG формате](https://drive.google.com/file/d/0B4wqi_MPMXo2Z1lnZEhLMENQLUk/edit?usp=sharing) и [здесь в SCH формате] (https://drive.google.com/file/d/0B4wqi_MPMXo2THBNUnQtTEhYSmM/edit?usp=sharing).

Схемы в SCH формате, нарисованы в [gEDA](http://www.geda-project.org/).
