// ПРИМЕР ЧТЕНИЯ ДАННЫХ БЕЗ ПРОВЕРКИ:                                              //
// Чтение данных с проверкой доступно в примере "getData"                          //
                                                                                   //
#include <SoftwareSerial.h>                                                        //   Подключаем библиотеку для работы с программной шиной UART.
#include <iarduino_Modbus.h>                                                       //   Подключаем библиотеку для работы по протоколу Modbus.
#include <iarduino_MB_ShtSgpLtr.h>                                                 //   Подключаем библиотеку для работы с модулем климатических датчиков.
                                                                                   //
SoftwareSerial        rs485(8,9);                                                  //   Создаём объект для работы с программной шиной UART-RS485 указывая выводы RX, TX.
ModbusClient          modbus(rs485, 2);                                            //   Создаём объект для работы по протоколу Modbus указывая объект программной шины UART-RS485 и вывод DE конвертера UART-RS485.
iarduino_MB_ShtSgpLtr sensor(modbus);                                              //   Создаём объект для работы с модулем климатических датчиков указывая объект протокола Modbus.
                                                                                   //
void setup(){                                                                      //
     Serial.begin(9600); while(!Serial);                                           //   Инициируем передачу данных в монитор последовательного порта, указав его скорость.
     rs485.begin(9600); while(!rs485);                                             //   Инициируем работу с программной шиной UART-RS485 указав её скорость.
     modbus.begin();                                                               //   Инициируем работу по протоколу Modbus.
//   modbus.setTimeout(10);                                                        //   Указываем максимальное время ожидания ответа по протоколу Modbus.
//   modbus.setDelay(4);                                                           //   Указываем минимальный интервал между отправляемыми сообщениями по протоколу Modbus.
//   modbus.setTypeMB( MODBUS_RTU );                                               //   Указываем тип протокола Modbus: MODBUS_RTU (по умолчанию), или MODBUS_ASCII.
     sensor.begin(3);                                                              //   Инициируем работу с модулем климатических датчиков, указав его адрес. Если адрес не указан sensor.begin(), то он будет найден, но это займёт некоторое время.
//   sensor.setPeriod( SENSOR_SHT, 0.5f );                                         //   Указываем модулю обновлять данные датчика влажности и температуры каждые 0.5 секунд.
//   sensor.setPeriod( SENSOR_LTR, 1.0f );                                         //   Указываем модулю обновлять данные датчика освещённости каждую секунду.
//   sensor.setPeriod( SENSOR_SGP, 1.0f );                                         //   Указываем модулю обновлять данные датчика углекислого газа каждую секунду.
}                                                                                  //
                                                                                   //
void loop(){                                                                       //
     Serial.print( (String) "TEM="  + sensor.getTEM()  + "°C, "     );             //   Выводим температуру.
     Serial.print( (String) "HUM="  + sensor.getHUM()  + "%, "      );             //   Выводим относительную влажность.
     Serial.print( (String) "HUM="  + sensor.getHUMA() + "г/м3, "   );             //   Выводим абсолютную влажность.
     Serial.print( (String) "LUX="  + sensor.getLUX()  + "лк, "     );             //   Выводим освещённость.
     Serial.print( (String) "CO2="  + sensor.getCO2()  + "ppm, "    );             //   Выводим количество эквивалента СО2 (углекислого газа).
     Serial.print( (String) "TVOC=" + sensor.getTVOC() + "ppb.\r\n" );             //   Выводим общее количество летучих органических соединений.
     delay(1000);                                                                  //
}                                                                                  //