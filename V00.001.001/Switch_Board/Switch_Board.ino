#ifndef _INO_
#define _INO_

#define MAX_TIMERS 10

#define NEW_IDE
//#define OLD_IDE

#if defined OLD_IDE // Arduino IDE version 1.xx
  #include "OTA_V4.h"
#elif defined NEW_IDE  // Arduino IDE version 2.xx
  #include "OTA_V5.h"
#endif
//#include <EEPROM.h>
#include "Application_functions.h"

 #include "MIT_APP.h" //please include at last


const String ino_ver = "INO : V00.000.001";
#if defined OLD_IDE // Arduino IDE version 1.xx
  const String VER[] = {ino_ver, mit_app_ver, ota_v4_ver,  wifi_conf_ver};
#elif defined NEW_IDE  // Arduino IDE version 2.xx
  const String VER[] = {ino_ver , mit_app_ver, ota_v5_ver,  wifi_conf_ver};
#endif



void setup() {
 Serial.begin(115200);
 EEPROM.begin(512); 
 WIFI_Setup();
 Application_Setup();
#if 0
 Serial.println("**** SW Versions *****");
 for(byte i = 0 ; i < 4 ; i ++)
    Serial.println(VER[i]);

  Serial.println(AP_NameChar);
  Serial.println(WiFiPassword);
  Serial.println(ssid);
  Serial.println(password);
#endif 
 //  WriteWifiCredentials();
 
  ReadWifiCredentials();   
 
  OTA_flag = EEPROM.read(OTA_addr);
 //OTA_flag = 0;
 if(OTA_flag)
  {
      OTA_Setup();
  }
 else
  {
    MIT_APP_Setup();
  
    google_sheet_setup();
    
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    MY_IP = WiFi.localIP().toString();
    
  }
  
  local_dns_setup();
}




void loop() {
 unsigned long currentMillis = millis();
  wifi_check_loop(); // every 20 min
  local_dns_loop();
  if(OTA_flag)
  {
     OTA_loop();
  }
  else
  {
  //  MIT_loop();
    Application_loop();
   
  } 

}



#endif //_INO_
