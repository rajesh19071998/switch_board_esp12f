#ifndef _WIFI_CONFIG_H
#define _WIFI_CONFIG_H

#ifndef _EEPROM_H
  #define _EEPROM_H
  #include <EEPROM.h> 
#endif //_EEPROM_H


String BOARD_NUMBER = "5";
String BOARD_NAME = "SWITCH_BOARD_1";

String Global_IP = "https://sw1.rajeshv.in";
/*
String STATIC_IP = "";
String GATEWAY_IP = "";
*/

String STATIC_IP = "192.168.31.111";
String GATEWAY_IP = "192.168.31.1";


String  AP_NameChar = "SWITCH_BOARD_1" ;
String WiFiPassword = "123456789";

String     ssid = "RajeshJioFiber";
String password = "Rajesh@1234";
/*
String     ssid = "Rajesh iPhone";
String password = "123456789";
*/



#endif //_WIFI_CONFIG_H
