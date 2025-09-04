 #ifndef _OTA_H
 #define _OTA_H


#ifndef _ESP8266_WIFI_H
  #define _ESP8266_WIFI_H
  #include <ESP8266WiFi.h>
#endif //_ESP8266_WIFI_H
#ifndef _ESPASYNC_TCP_H
  #define _ESPASYNC_TCP_H  
  #include <ESPAsyncTCP.h>
#endif //_ESPASYNC_TCP_H
#ifndef _ESPASYNC_WEB_SERVER_H
  #define _ESPASYNC_WEB_SERVER_H  
  #include <ESPAsyncWebServer.h>
#endif //_ESPASYNC_WEB_SERVER_H
#ifndef _EEPROM_H
  #define _EEPROM_H
  #include <EEPROM.h> 
#endif //_EEPROM_H


#include <AsyncElegantOTA.h>

#include "Wifi_functions.h"

const String ota_v4_ver = "OTA_V4 : V00.000.001";

extern String Hotspot_IP;
String MYIP = "http://"+ MY_IP;
String OTA_info = " Hi OTA vertion 4 User\nBOARD Details mentioned below\nBoard_Name : "+BOARD_NAME +"\nBoard_Number : "+BOARD_NUMBER +"\nTo Update Board Firmware\nBoard_Local_IP : "+MYIP+"/update"+"\nBoard_Global_IP : "+Global_IP+"/update"+"\nHotspot_IP : "+Hotspot_IP+"/update" ;


AsyncWebServer OTA_server(80);

 static int DEBUG = 1;

const long interval_OTA = 600000; //6000 * 10 = 60000 60sec or 1 min * 10 = 600000  10 min it willgive control OTA to APPL
unsigned long previousMillis_OTA = 0, currentMillis_OTA = 0; 



 /*
  AsyncElegantOTA Demo Example - This example will work for both ESP8266 & ESP32 microcontrollers.
  -----
  Author: Ayush Sharma ( https://github.com/ayushsharma82 )
  
  Important Notice: Star the repository on Github if you like the library! :)
  Repository Link: https://github.com/ayushsharma82/AsyncElegantOTA
*/

void Disable_OTA()
{
    OTA_flag = 0;
    Serial.println("OTA_loop_TIME OUT give control to Application");
    EEPROM.write(OTA_addr, OTA_flag);
    EEPROM.commit();
    delay(5000);
    ESP.restart();
}

void OTA_Setup(void) {
 
  WIFI_Setup();

   OTA_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {

  request->send(200, "text/plain",OTA_info.c_str());
  // request->send(200, "text/plain", "Hi! This is JIO_Remote (ESP Board No 2).\nPlease Enter\n For Local -> http://192.168.31.250/update  \n OR \n For Global -> https://remote.rajeshv.in/update \nTo update Firmware.");
  });

  AsyncElegantOTA.begin(&OTA_server);    // Start AsyncElegantOTA
  OTA_server.begin();
  Serial.println("HTTP OTA_server started");
}


void OTA_loop()
{
  currentMillis_OTA = millis();

  if (currentMillis_OTA - previousMillis_OTA >= interval_OTA) 
  {
    previousMillis_OTA = currentMillis_OTA;

    Disable_OTA();
  }
}




#endif //_OTA_H
