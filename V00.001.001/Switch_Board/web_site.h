/*********
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete instructions at https://RandomNerdTutorials.com/build-web-servers-ebook/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

// Import required libraries
//#include <Arduino.h>
//#include <ESP8266WiFi.h>
/*
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
*/ // mit_app.h

//#include <LittleFS.h> // if you use OTA_V4.h , please un comment
#include <Arduino_JSON.h>

#include "Application_var.h"
#include "Relay.h"
#include "MIT_APP.h"

// Create a WebSocket object
AsyncWebSocket ws("/ws");

// Initialize LittleFS
void initFS() {
  if (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  else {
    Serial.println("LittleFS mounted successfully");
  }
}



String getOutputStates(){
  JSONVar myArray;
  for (int i =0; i< MAX_RELAY; i++){
    myArray["gpios"][i]["output"] = String(i);
    myArray["gpios"][i]["state"] = String(get_relay_state(i));
  }
  String jsonString = JSON.stringify(myArray);
  return jsonString;
}


void notifyClients(String state) {
  ws.textAll(state);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "states") == 0) {
      Serial.print("Web_Data : ");
      Serial.println((char*)data);
      notifyClients(getOutputStates());
    }
    else{
      int gpio = atoi((char*)data);
      
      Serial.print("Web_Data : ");
      Serial.println((char*)data);
      /*
      Serial.print("Web_Site GPIO : ");
      Serial.println(gpio);
      */
      Update_relay_state_with_index((gpio - 1), !get_relay_state(gpio-1)); // Relay.h

      notifyClients(getOutputStates());
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);

      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  MIT_server.addHandler(&ws);
}

void web_site_setup(){
  
  initFS();
  
  initWebSocket();
/*
    // Route for root / web page    // mit .h
  MIT_server.on("/app", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html",false);
    MIT_server.serveStatic("/", LittleFS, "/");
    Serial.println("HITTED ....!");
  });

  // Start server
  MIT_server.begin(); //mit.h
*/

}

void web_site_loop() {

  ws.cleanupClients();

}
