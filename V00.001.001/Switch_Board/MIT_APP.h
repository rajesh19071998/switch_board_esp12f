#ifndef _MIT_APP_H
#define _MIT_APP_H
const String mit_app_ver = "MIT_APP : V00.002.001";

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "Wifi_functions.h"

#define _MIT_DEBUG 1

extern void mit_app_requist_process(AsyncWebServerRequest *web); // Application_functions.h
void mit_app_cmd_setup();

AsyncWebServer MIT_server(80);

extern String Global_IP;
String Hotspot_IP = "http://192.168.4.1";
String MIT_INFO_DATA;

String MIT_Basic_cmd = "Basic CMD's\nRST = Reset\nOTA = Enable_OTA\nWNQ = Wifi_Name\nWPQ = Wifi_Password\nHNQ = Hotspot_Name\nHPQ = Hotspot_Password\nWIFIUPDATE = to copy wifi/hotspot details EEPROM to local\n";
extern String MIT_Appl_cmd;

String MIT_cmd = MIT_Basic_cmd + MIT_Appl_cmd;
String DATA; // Wifi_functions.h

 String Appl_mit_DATA = ""; // Application_functions.h
 int MIT_flag = 0;  // Application_functions.h

 void my_info()
 {
  String MYIP = "http://"+ MY_IP;
  MIT_INFO_DATA =" Hi Application User\nBOARD Details mentioned below\nBoard_Name : "+BOARD_NAME +"\nBoard_Number : "+BOARD_NUMBER +"\nBoard_Local_IP : "+MYIP+"\nBoard Local DNS : "+"http://"+Local_DNS_Name+".local"+"\nBoard_Global_IP : "+Global_IP+"\nHotspot_IP : "+Hotspot_IP+"\n For MIT Commands type "+MYIP+"/cmd" ;

 }

void MIT_APP_Setup()
{
  mit_app_cmd_setup(); 
  MIT_server.begin();
}


void Enable_OTA()
{
  OTA_flag?OTA_flag = 0:OTA_flag = 1;
   
   EEPROM.write(OTA_addr, OTA_flag);
   EEPROM.commit();
   delay(5000);
   ESP.restart();
}

void mit_request_process(AsyncWebServerRequest *request)
{
  String R = "";
  String data = "";
  Serial.print("URL : ");
  Serial.println(request->url());
  R = request->url();

  if(R.indexOf("OTA") > 0 ){ request->send(200, "text/plain", "OTA_Enabled");Enable_OTA();}
  else if(R.indexOf("RST") > 0 ){request->send(200, "text/plain", "ESP Restarted");delay(5000);ESP.restart();}
  else if(R.indexOf("WNQ") > 0 ){request->send(200, "text/plain", AP_NameChar.c_str());}
  else if(R.indexOf("WPQ") > 0 ){request->send(200, "text/plain", WiFiPassword.c_str());}
  else if(R.indexOf("HNQ") > 0 ){request->send(200, "text/plain", ssid.c_str());}
  else if(R.indexOf("HPQ") > 0 ){request->send(200, "text/plain", password.c_str());}
  else if(R.indexOf("WN") > 0 )
  { 
    data = request->getParam("WN")->value();
    if(data == ""){request->send(200, "text/plain", "WIFI Name should not be empty!"); return;}
    Wifi_Hotspot_Write(data,1);request->send(200, "text/plain", "Wifi Name Updated as : " + data +"\nEEPROM writen : "+ DATA);
  }
  else if(R.indexOf("WP") > 0 )
  {
    data = request->getParam("WP")->value();
    if(data == ""){request->send(200, "text/plain", "WIFI Password should not be empty!"); return;}
    Wifi_Hotspot_Write(data,2);request->send(200, "text/plain", "Wifi Password Updated as : " + data +"\nEEPROM writen : "+ DATA);
  }
  else if(R.indexOf("HN") > 0 )
  {
    data = request->getParam("HN")->value();
    if(data == ""){request->send(200, "text/plain", "Hotspot Name should not be empty!"); return;}
    Wifi_Hotspot_Write(data,3);request->send(200, "text/plain", "Hotspot Name Updated as : " + data +"\nEEPROM writen : "+ DATA);
  }
  else if(R.indexOf("HP") > 0 )
  {
    data = request->getParam("HP")->value();
    if(data == ""){request->send(200, "text/plain", "Hotspot Password should not be empty!"); return;}
    Wifi_Hotspot_Write(data,4);request->send(200, "text/plain", "Hotspot Password Updated as : " + data +"\nEEPROM writen : "+ DATA);
  }
  
  else if(R.indexOf("wifiupdate") > 0 ){ReadWifiCredentials();request->send(200, "text/plain", "Wifi and Hotspot variables updated from EEPROM");}
  //else if(R.indexOf("info") > 0 ){my_info();request->send(200, "text/plain", MIT_INFO_DATA);}
  else if(R.indexOf("cmd") > 0 ){request->send(200, "text/plain", MIT_cmd);}

  else if(R.indexOf("A") > 0 ){
         mit_app_requist_process(request);
         if(MIT_flag == 2 ) { request->send(200, "text/plain", Appl_mit_DATA);} 
         else { request->send(200, "text/plain", "ESP Application Invalid Request....!");  }
         }

  else{
        request->send(200, "text/plain", "ESP Invalid Request....!");
      }       
  
}


void mit_app_cmd_setup()
{
  MIT_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html",false);
    MIT_server.serveStatic("/", LittleFS, "/");
    Serial.println("WEB SITE LOADED ....!");
  });

  MIT_server.on("/info", HTTP_GET, [](AsyncWebServerRequest *request){ my_info();request->send(200, "text/plain", MIT_INFO_DATA); }); // Root //http://192.168.31.146

  MIT_server.on("/OTA", HTTP_GET, [](AsyncWebServerRequest *request){mit_request_process(request); });
  MIT_server.on("/RST", HTTP_GET, [](AsyncWebServerRequest *request){mit_request_process(request); });
  MIT_server.on("/WNQ", HTTP_GET, [](AsyncWebServerRequest *request){mit_request_process(request);});
  MIT_server.on("/WPQ", HTTP_GET, [](AsyncWebServerRequest *request){mit_request_process(request);});
  MIT_server.on("/HNQ", HTTP_GET, [](AsyncWebServerRequest *request){mit_request_process(request);});
  MIT_server.on("/HPQ", HTTP_GET, [](AsyncWebServerRequest *request){ mit_request_process(request);});  // http://192.168.31.146/HPQ
  MIT_server.on("/WN", HTTP_GET, [](AsyncWebServerRequest *request){ mit_request_process(request);});  // http://192.168.31.146/WN?WN=RajeshJio
  MIT_server.on("/WP", HTTP_GET, [](AsyncWebServerRequest *request){mit_request_process(request);});
  MIT_server.on("/HN", HTTP_GET, [](AsyncWebServerRequest *request){mit_request_process(request);});
  MIT_server.on("/HP", HTTP_GET, [](AsyncWebServerRequest *request){mit_request_process(request);});
  MIT_server.on("/wifiupdate", HTTP_GET, [](AsyncWebServerRequest *request){mit_request_process(request);});
  MIT_server.on("/cmd", HTTP_GET, [](AsyncWebServerRequest *request){mit_request_process(request);});

  MIT_server.on("/A", HTTP_GET, [](AsyncWebServerRequest *request){mit_request_process(request);}); // Application  data

}

void MIT_loop()
{
  wifi_check_loop(); // every 20 min
}







#endif //_MIT_APP_H
