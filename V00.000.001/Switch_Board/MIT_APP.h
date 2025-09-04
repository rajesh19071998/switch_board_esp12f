#ifndef _MIT_APP_H
#define _MIT_APP_H
const String mit_app_ver = "MIT_APP : V00.000.001";

#include "Wifi_functions.h"

#define _MIT_DEBUG 1

WiFiServer MIT_server(80);
/******** .INO file functions *********************************/

//extern void ino_file_functions();

/*************************************************************/
//static int DEBUG = 1;

String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
String request = "";
extern String Global_IP;
String Hotspot_IP = "http://192.168.4.1";
String MIT_INFO_DATA;

String MIT_Basic_cmd = "Basic CMD's\nRST = Reset\nOTA = Enable_OTA\nWNQ = Wifi_Name\nWPQ = Wifi_Password\nHNQ = Hotspot_Name\nHPQ = Hotspot_Password\nWIFIUPDATE = to copy wifi/hotspot details EEPROM to local\n";
extern String MIT_Appl_cmd;

String MIT_cmd = MIT_Basic_cmd + MIT_Appl_cmd;
 String DATA;
 char DATA_1[20];
 String Appl_mit_DATA = "";
 int MIT_flag = 0;

 void my_info()
 {
  String MYIP = "http://"+ MY_IP;
  MIT_INFO_DATA =" Hi Application User\nBOARD Details mentioned below\nBoard_Name : "+BOARD_NAME +"\nBoard_Number : "+BOARD_NUMBER +"\nBoard_Local_IP : "+MYIP+"\nBoard_Global_IP : "+Global_IP+"\nHotspot_IP : "+Hotspot_IP+"\n For MIT Commands type "+MYIP+"/cmd" ;

 }

void MIT_APP_Setup()
{
  WIFI_Setup();
  MIT_server.begin();
}


void Appliction_MIT_APP_Process()
{

  mit_app_requist_process(request); // refer Application_functions.h

}

void Common_MIT_APP_Process()
{
  if ( request.indexOf("OTA") > 0 )  
     {
       OTA_flag?OTA_flag = 0:OTA_flag = 1;
   
       EEPROM.write(OTA_addr, OTA_flag);
       EEPROM.commit();
       delay(5000);
       ESP.restart();
    }
    else if  ( request.indexOf("WNQ") > 0 ) { MIT_flag = 1; if(_MIT_DEBUG)Serial.println(request);sprintf(DATA_1, "%s%s","WNA ",AP_NameChar.c_str());}
    else if  ( request.indexOf("WPQ") > 0 ) { MIT_flag = 1; if(_MIT_DEBUG)Serial.println(request);sprintf(DATA_1, "%s%s","WPA ",WiFiPassword.c_str()); }
    else if  ( request.indexOf("HNQ") > 0 ) { MIT_flag = 1; if(_MIT_DEBUG)Serial.println(request);sprintf(DATA_1, "%s%s","HNA ",ssid.c_str()); }
    else if  ( request.indexOf("HPQ") > 0 ) { MIT_flag = 1; if(_MIT_DEBUG)Serial.println(request);sprintf(DATA_1, "%s%s","HPA ",password.c_str()); }
    else if  ( request.indexOf("STA") > 0 ) { MIT_flag = 0; if(_MIT_DEBUG)Serial.println(request); /*Last_OTA_Status(); */}
    
    else if  ( request.indexOf("WN") > 0 ) { MIT_flag = 3;if(_MIT_DEBUG) Serial.println(request); Wifi_update(request,1); Read_EEPROM_wifi_data(1); }
    else if  ( request.indexOf("WP") > 0 ) { MIT_flag = 3;if(_MIT_DEBUG) Serial.println(request); Wifi_update(request,2); Read_EEPROM_wifi_data(2); }
    else if  ( request.indexOf("HN") > 0 ) { MIT_flag = 3;if(_MIT_DEBUG) Serial.println(request); Wifi_update(request,3); Read_EEPROM_wifi_data(3); } //HNRajeshJioFiber
    else if  ( request.indexOf("HP") > 0 ) { MIT_flag = 3;if(_MIT_DEBUG) Serial.println(request); Wifi_update(request,4); Read_EEPROM_wifi_data(4); }
    else if  ( request.indexOf("WIFIUPDATE") > 0 ) { MIT_flag = 3;if(_MIT_DEBUG) Serial.println(request); ReadWifiCredentials(); DATA = "WIFI and HOTSPOT data updated from EEPROM to variables"; }
    else if  ( request.indexOf("RST") > 0 ) { MIT_flag = 0;if(_MIT_DEBUG) Serial.println(request);delay(5000);ESP.restart(); }
    else if  ( request.indexOf("RJV") > 0 ) { MIT_flag = 2; } // it will give exesting Appl_mit_DATA buffer data
    
    else if  ( request == "GET / HTTP/1.1" ) { MIT_flag = 4; if(_MIT_DEBUG)Serial.println(request);my_info();} // For INFO
    else if  ( request == "cmd" ) { MIT_flag = 5; if(_MIT_DEBUG)Serial.println(request);} // For INFO
    
}

void MIT_Main_code()
{
  // Check if a client has connected
    WiFiClient MIT_client = MIT_server.available();
     if (!MIT_client)  {  goto L;  }
 
    // Read the first line of the request
    request = MIT_client.readStringUntil('\r');
    
    MIT_flag = 0;
    
    Common_MIT_APP_Process();
    Appliction_MIT_APP_Process();


    MIT_client.flush();
   
    MIT_client.print( header );
    //MIT_client.print(" "); // put data to send mobile

    if (MIT_flag == 1)
    {
     MIT_client.print(DATA_1);
     if(_MIT_DEBUG)Serial.println(DATA_1);
    }
    else if(MIT_flag == 2) // application
    {
     MIT_client.print(Appl_mit_DATA);
     if(_MIT_DEBUG)Serial.println(Appl_mit_DATA); 
    }
    else if(MIT_flag == 3)
    {
     MIT_client.print(DATA);
     if(_MIT_DEBUG)Serial.println(DATA); 
    }
    else if(MIT_flag == 4)
    {
      MIT_client.print(MIT_INFO_DATA);
      if(_MIT_DEBUG)Serial.println(MIT_INFO_DATA);
    }
    else if(MIT_flag == 5)
    {
      MIT_client.print(MIT_cmd);
      if(_MIT_DEBUG)Serial.println(MIT_cmd);
    }
 
    L:
    delay(5);
    wifi_check_loop(); // every 20 min
}







#endif //_MIT_APP_H
