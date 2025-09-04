/*
  -----------------------
  ElegantOTA - Demo Example
  -----------------------
 
  Skill Level: Beginner
 
  This example provides with a bare minimal app with ElegantOTA functionality.
 
  Github: https://github.com/ayushsharma82/ElegantOTA
  WiKi: https://docs.elegantota.pro
 
  Works with both ESP8266 & ESP32
 
  -------------------------------
 
  Upgrade to ElegantOTA Pro: https://elegantota.pro
 
*/
 #ifndef _OTA_H
 #define _OTA_H
 
#if defined(ESP8266)
  
  #ifndef _ESP8266_WIFI_H
    #define _ESP8266_WIFI_H
    #include <ESP8266WiFi.h>
  #endif //_ESP8266_WIFI_H
  #ifndef _WIFI_CLIENT_H
    #define _WIFI_CLIENT_H 
    #include <WiFiClient.h>
  #endif //_WIFI_CLIENT_H
  #ifndef _ESP8266_WEB_SERVER_H
    #define _ESP8266_WEB_SERVER_H
    #include <ESP8266WebServer.h>
  #endif //_ESP8266_WEB_SERVER_H
#elif defined(ESP32)
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <WebServer.h>
#endif
 
#include <ElegantOTA.h>
#include "Wifi_functions.h"

#define _OTA_DEBUG

const String ota_v5_ver = "OTA_V5 : V00.000.001";
extern String Hotspot_IP;
String MYIP = "http://"+ MY_IP;
String OTA_info = " Hi OTA vertion 5 User\nBOARD Details mentioned below\nBoard_Name : "+BOARD_NAME +"\nBoard_Number : "+BOARD_NUMBER +"\nTo Update Board Firmware\nBoard_Local_IP : "+MYIP+"/update"+"\nBoard_Global_IP : "+Global_IP+"/update"+"\nHotspot_IP : "+Hotspot_IP+"/update" ;

#if defined(ESP8266)
  ESP8266WebServer OTA_server(80);
#elif defined(ESP32)
  WebServer OTA_server(80);
#endif


unsigned long ota_progress_millis = 0;

const long interval_OTA = 600000; //6000 * 10 = 60000 60sec or 1 min * 10 = 600000  10 min it will give control OTA to APPL
unsigned long previousMillis_OTA = 0, currentMillis_OTA = 0; 



void Disable_OTA()
{
    OTA_flag = 0;
#ifdef _OTA_DEBUG    
    Serial.println("OTA_loop_TIME OUT give control to Application");
#endif    
    EEPROM.write(OTA_addr, OTA_flag);
    EEPROM.commit();
    delay(5000);
    ESP.restart();
}


void onOTAStart() {
  // Log when OTA has started
#ifdef _OTA_DEBUG
  Serial.println("OTA update started!");
#endif  
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
#ifdef _OTA_DEBUG    
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
#endif    
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
#ifdef _OTA_DEBUG    
    Serial.println("OTA update finished successfully!");
#endif    
    Disable_OTA();
  } else {
#ifdef _OTA_DEBUG    
    Serial.println("There was an error during OTA update!");
#endif    
  }
  // <Add your own code here>
}



void OTA_Setup(void) {
 

  WiFi.mode(WIFI_STA);
  WIFI_Setup();

   OTA_server.on("/", []() {

    OTA_server.send(200, "text/plain",OTA_info.c_str());
   //OTA_server.send(200, "text/plain", "Hi! This is SWITCH_BOX_1 (ESP Board No 5).\nPlease Enter\n For Local -> http://192.168.31.111/update  \n OR \n For Global -> "" \nTo update Firmware.");
  });

  ElegantOTA.begin(&OTA_server);    // Start ElegantOTA

  // ElegantOTA callbacks
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);

  OTA_server.begin();
#ifdef _OTA_DEBUG  
  Serial.println("HTTP OTA_server started");
#endif  
}


void OTA_loop()
{
  OTA_server.handleClient();
  ElegantOTA.loop();

  currentMillis_OTA = millis();

  if (currentMillis_OTA - previousMillis_OTA >= interval_OTA) 
  {
    previousMillis_OTA = currentMillis_OTA;

    Disable_OTA();
   
  }
}



#endif //_OTA_H
