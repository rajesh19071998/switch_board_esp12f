#ifndef _LOCAL_DNS_H
#define _LOCAL_DNS_H

//#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "WIFI_CONFIG.h"

void local_dns_setup()
{
    // Start mDNS with hostname "esp8266"
  if (MDNS.begin(Local_DNS_Name)) {
    Serial.println("mDNS responder started");
  }
}

void local_dns_loop()
{
  MDNS.update();
}








#endif  //_LOCAL_DNS_H