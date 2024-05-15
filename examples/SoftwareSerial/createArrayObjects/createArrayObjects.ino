// ПРИМЕР ЧТЕНИЯ ДАННЫХ СО ВСЕХ МОДУЛЕЙ КЛИМАТИЧЕСКИХ ДАТЧИКОВ НА ШИНЕ:            //
// без указания их адресов в скетче.                                               //
                                                                                   //
#include <SoftwareSerial.h>                                                        //   Подключаем библиотеку для работы с программной шиной UART.
#include <iarduino_Modbus.h>                                                       //   Подключаем библиотеку для работы по протоколу Modbus.
#include <iarduino_MB_ShtSgpLtr.h>                                                 //   Подключаем библиотеку для работы с модулем климатических датчиков.
                                                                                   //
SoftwareSerial         rs485(8,9);                                                 //   Создаём объект для работы с программной шиной UART-RS485 указывая выводы RX, TX.
ModbusClient           modbus(rs485, 2);                                           //   Создаём объект для работы по протоколу Modbus указывая объект программной шины UART-RS485 и вывод DE конвертера UART-RS485.
iarduino_MB_ShtSgpLtr* sensor;                                                     //   Создаём указатель который будет переопределён в массив объектов работы с модулями климатических датчиков.
                                                                                   //
uint8_t sum=0;                                                                     //   Определяем переменную для хранения количества найденных модулей климатических датчиков.
                                                                                   //
void setup(){                                                                      //
     Serial.begin(9600);   while(!Serial);                                         //   Инициируем передачу данных в монитор последовательного порта, указав его скорость.
     rs485.begin(9600); // while(!rs485 );                                         //   Инициируем работу с программной шиной UART-RS485 указав её скорость.
     modbus.begin();                                                               //   Инициируем работу по протоколу Modbus.
//   modbus.setTimeout(10);                                                        //   Указываем максимальное время ожидания ответа по протоколу Modbus.
//   modbus.setDelay(4);                                                           //   Указываем минимальный интервал между отправляемыми сообщениями по протоколу Modbus.
//   modbus.setTypeMB( MODBUS_RTU );                                               //   Указываем тип протокола Modbus: MODBUS_RTU (по умолчанию), или MODBUS_ASCII.
     uint8_t i,j;                                                                  //
                                                                                   //
//   Выполняем поиск всех модулей на шине с 2 датчиками (занимает несколько сек):  //
     Serial.print("Поиск модулей с 2 датчиками ... " );                            //
     uint8_t sumID2 = modbus.findID( DEF_MODEL_SHT_LTR ); sum+=sumID2;             //   Ищем адреса всех устройств с идентификатором модуля "2 датчика SHT,LTR" = DEF_MODEL_SHT_LTR.
     uint8_t arrID2[sumID2];                                                       //   Объявляем массив arrID2 для хранения найденных адресов.
     i=0; while( modbus.available() ){ arrID2[i++]=modbus.read(); }                //   Заполняем массив arrID2 найденными адресами.
     Serial.print("найдено "); Serial.print(sumID2); Serial.println(" модулей.");  //
                                                                                   //
//   Выполняем поиск всех модулей на шине с 3 датчиками (занимает несколько сек):  //
     Serial.print("Поиск модулей с 3 датчиками ... " );                            //
     uint8_t sumID3 = modbus.findID( DEF_MODEL_SHT_SGP_LTR ); sum+=sumID3;         //   Ищем адреса всех устройств с идентификатором модуля "3 датчика SHT,SGP,LTR" = DEF_MODEL_SHT_SGP_LTR.
     uint8_t arrID3[sumID3];                                                       //   Объявляем массив arrID3 для хранения найденных адресов.
     i=0; while( modbus.available() ){ arrID3[i++]=modbus.read(); }                //   Заполняем массив arrID3 найденными адресами.
     Serial.print("найдено "); Serial.print(sumID3); Serial.println(" модулей.");  //
                                                                                   //
     if( sum ){                                                                    //
     //  Переопределяем указатель sensor в массив объектов:                        //
         sensor=(iarduino_MB_ShtSgpLtr*)malloc(sizeof(iarduino_MB_ShtSgpLtr)*sum); //   Выделяем под массив sensor требуемый объем памяти.
         i=0;                                                                      //
         for(j=0;j<sumID2;j++,i++){sensor[i]=modbus; sensor[i].begin(arrID2[j]);}  //   Инициируем работу с модулями имеющими 2 датчика.
         for(j=0;j<sumID3;j++,i++){sensor[i]=modbus; sensor[i].begin(arrID3[j]);}  //   Инициируем работу с модулями имеющими 3 датчика.
     }                                                                             //
                                                                                   //
//   Выводим сообщение:                                                            //
     Serial.print((String)"Всего найдено "+sum+" модулей датчиков, с адресами: "); //
     for(i=0; i<sum; i++){ Serial.print( sensor[i].getID() ); Serial.print(", ");} //
     Serial.println();                                                             //
}                                                                                  //
                                                                                   //
void loop(){                                                                       //
     for(uint8_t i=0; i<sum; i++){                                                 //   Проходим по всем модулям климатических датчиков.
         Serial.print( (String) "ID="   + sensor[i].getID()   + ", "       );      //   Выводим ID текущего модуля климатических датчиков.
         Serial.print( (String) "TEM="  + sensor[i].getTEM()  + "°C, "     );      //   Выводим температуру.
         Serial.print( (String) "HUM="  + sensor[i].getHUM()  + "%, "      );      //   Выводим относительную влажность.
         Serial.print( (String) "HUM="  + sensor[i].getHUMA() + "г/м3, "   );      //   Выводим абсолютную влажность.
         Serial.print( (String) "LUX="  + sensor[i].getLUX()  + "лк, "     );      //   Выводим освещённость.
         Serial.print( (String) "CO2="  + sensor[i].getCO2()  + "ppm, "    );      //   Выводим количество эквивалента СО2 (углекислого газа).
         Serial.print( (String) "TVOC=" + sensor[i].getTVOC() + "ppb.\r\n" );      //   Выводим общее количество летучих органических соединений.
         delay(1000);                                                              //
     }                                                                             //
}                                                                                  //
