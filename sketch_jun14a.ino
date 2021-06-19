#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h> // Библиотека для OTA-прошивки

#include "define.h"




const char* ssid = "Tenda_D0B9D0";
const char* password = "guesttell609";

 const char*  ip_server = "91.77.164.46";// (mtbw.ru)
 uint16_t     port_server = 30547;

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // локальный порт для прослушки 
char incomingPacket[255];          // буфер для входящих пакетов
char  replyPacekt[] = "Artem, sosi huy! :P";  // ответ
                  //  "Привет! Сообщение получено :-)"


void setup()
{
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(ssid, password);
  // check for the presence of the shield:
  Serial.print("Connecting :P ");




  
while(WiFi.status() !=  WL_CONNECTED)
  {
    switch(WiFi.status()){
      case  WL_NO_SHIELD:
    Serial.print("1");
      break;
       case   WL_IDLE_STATUS:
    Serial.print("2");
      break;
        case  WL_NO_SSID_AVAIL:
    Serial.print("3");
      break;
       case   WL_SCAN_COMPLETED:
    Serial.print("4");
      break;
       case   WL_CONNECTED:
    Serial.print("5");
      break;
       case   WL_CONNECT_FAILED:
    Serial.print("6");
      break;
       case   WL_CONNECTION_LOST:
    Serial.print("7");
      break;
       case   WL_DISCONNECTED:
    Serial.print("8");
      break;
       case   WL_WRONG_PASSWORD:
    Serial.print("9");
      break;
      }
    delay(2000);
    //Serial.print(".");
  }
  Serial.println();
  if(WiFi.status() == WL_CONNECTED){
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    
    Udp.begin(localUdpPort);
    Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
              //  "Теперь прослушиваем IP-адрес %s, UDP-порт %d"
      // отправляем ответ на IP-адрес и порт, с которых пришел пакет: 
  
   /*ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });*/
    ArduinoOTA.setHostname("ESP8266-00001"); // Задаем имя сетевого порта
    //ArduinoOTA.setPassword((const char *)"0000"); // Задаем пароль доступа для удаленной прошивки
    ArduinoOTA.begin(); // Инициализируем OTA

  
    Udp.beginPacket(ip_server, port_server);
    Udp.write(replyPacekt);
    Udp.endPacket();
  
    Serial.print("Init");
  }
  else
    Serial.print(":(");
    //ESP.restart();
}

void loop() {
  ArduinoOTA.handle(); // Всегда готовы к прошивке
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // получаем входящие UDP-пакеты:
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
              //  "Получено %d байт от %s, порт %d%"
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
              //  "Содержимое UDP-пакета: %s"

  }
  
}
