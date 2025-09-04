#ifndef _GOOGLE_SHEETS_H
#define _GOOGLE_SHEETS_H

//https://script.google.com/macros/s/AKfycbzc0LEbZWeHH9As_Y2SULaFRaoMF0Oi35Hdi9cvN4cqZBNy8o8a6l7koztRl4dDDGyTBA/exec?Action=AddESP&ESPName=Rajesh_ESP&ESPNum=55&ESPIP=192.168.31.100

#ifndef _ESP8266_WIFI_H
  #define _ESP8266_WIFI_H
  #include <ESP8266WiFi.h>
#endif //_ESP8266_WIFI_H
#ifndef _WIFI_CLIENT_SECURE_H
  #define _WIFI_CLIENT_SECURE_H
  #include <WiFiClientSecure.h>
#endif  //_WIFI_CLIENT_SECURE_H

#include "Millis_Timer.h"

#define G_SHEET_TIMER 1 // timer number
//#define _G_DEBUG
 
String IP_Sheet = "AKfycbzc0LEbZWeHH9As_Y2SULaFRaoMF0Oi35Hdi9cvN4cqZBNy8o8a6l7koztRl4dDDGyTBA"; //--> spreadsheet Insert your script ID

//----------------------------------------Host & httpsPort don't edit
const char* host = "script.google.com";
const int httpsPort = 443;
//----------------------------------------
 
WiFiClientSecure client; //--> Create a WiFiClientSecure object.
  
void google_sheet_setup() {
 
  client.setInsecure();
  Timer[G_SHEET_TIMER].delayMillis = 6000; // 6 sec
}
 
 
// Subroutine for sending data to Google Sheets
void sendDataToGoogleSheet(String G_data) {
#ifdef _G_DEBUG  
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
#endif  
  //----------------------------------------Connect to Google host
  if (!client.connect(host, httpsPort)) {
#ifdef _G_DEBUG     
    Serial.println("connection failed");
#endif    
    return;
  }
  //----------------------------------------
 
  //----------------------------------------Processing data and sending data

 // String url = "/macros/s/" + IP_Sheet + "/exec?temperature=" + string_temperature + "&humidity=" + string_humidity;

  String url = "/macros/s/" + IP_Sheet + "/exec?" + G_data;
#ifdef _G_DEBUG 
  Serial.print("requesting URL: ");
  Serial.println(url);
#endif 
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");
#ifdef _G_DEBUG  
  Serial.println("request sent");
#endif  
  //----------------------------------------
 
  //----------------------------------------Checking whether the data was sent successfully or not
  Timer_activate(G_SHEET_TIMER);
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
#ifdef _G_DEBUG       
      Serial.println("headers received");
#endif      
      Timer_deactivate(G_SHEET_TIMER);
      break;
    }

    if(Timer[G_SHEET_TIMER].timeout == 1)
    {
      Timer_deactivate(G_SHEET_TIMER);
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
#ifdef _G_DEBUG     
    Serial.println("esp8266/Arduino CI successfull!");
#endif    
  } else {
#ifdef _G_DEBUG     
    Serial.println("esp8266/Arduino CI has failed");
#endif    
  }
#ifdef _G_DEBUG   
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
#endif  
  //----------------------------------------
} 
//==============================================================================

/******************************************* Sample code ****************************************************************************/
#if 0

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "DHT.h"
#define DHTTYPE DHT11 // type of the temperature sensor
const int DHTPin = 5; //--> The pin used for the DHT11 sensor is Pin D1 = GPIO5
DHT dht(DHTPin, DHTTYPE); //--> Initialize DHT sensor, DHT dht(Pin_used, Type_of_DHT_Sensor);
 
#define ON_Board_LED 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router
 
const char* ssid = "AndroidAP3DEC"; //--> Your wifi name or SSID.
const char* password = "123456"; //--> Your wifi password.
 
 
//----------------------------------------Host & httpsPort
const char* host = "script.google.com";
const int httpsPort = 443;
//----------------------------------------
 
WiFiClientSecure client; //--> Create a WiFiClientSecure object.
 
String GAS_ID = "AKfycby8Lfzwu-5SBj9tHxbw3_j8ZPZy6P6_3e_BBISC0Jg7SstXLUed"; //--> spreadsheet script ID
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);
 
  dht.begin();  //--> Start reading DHT11 sensors
  delay(500);
  
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
    
  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On Board
 
  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  //----------------------------------------
 
  client.setInsecure();
}
 
void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  int h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor !");
    delay(500);
    return;
  }
  String Temp = "Temperature : " + String(t) + " °C";
  String Humi = "Humidity : " + String(h) + " %";
  Serial.println(Temp);
  Serial.println(Humi);
  
  sendData(t, h); //--> Calls the sendData Subroutine
}
 
// Subroutine for sending data to Google Sheets
void sendData(float tem, int hum) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  
  //----------------------------------------Connect to Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  //----------------------------------------
 
  //----------------------------------------Processing data and sending data
  String string_temperature =  String(tem);
  // String string_temperature =  String(tem, DEC); 
  String string_humidity =  String(hum, DEC); 
  String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + string_temperature + "&humidity=" + string_humidity;
  Serial.print("requesting URL: ");
  Serial.println(url);
 
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");
 
  Serial.println("request sent");
  //----------------------------------------
 
  //----------------------------------------Checking whether the data was sent successfully or not
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  //----------------------------------------
} 
//==============================================================================
/******************************************************************************************************************/

/*********************************************** Google Script **************************************************/

//how to use Example
//https://script.google.com/macros/s/AKfycbzc0LEbZWeHH9As_Y2SULaFRaoMF0Oi35Hdi9cvN4cqZBNy8o8a6l7koztRl4dDDGyTBA/exec?Action=AddESP&ESPName=Rajesh_ESP&ESPNum=55&ESPIP=192.168.31.100

var result = "";
var Data_pos = "";

function doGet(e) { 
var ss = SpreadsheetApp.openByUrl("https://docs.google.com/spreadsheets/d/1UNbi_AR6szoaBBUkKiyMhwXmqGe6pRnLv8FFL6BndKQ/edit?gid=0#gid=0");

var sheet = ss.getSheetByName("Sheet1");

 if(e.parameter.Action == "AddESP")
 {
   addUser(e,sheet);
 }

 return ContentService.createTextOutput(result); // Data send to user (MIT APP)
}

function doPost(e) { 

  var ss = SpreadsheetApp.openByUrl("https://docs.google.com/spreadsheets/d/1UNbi_AR6szoaBBUkKiyMhwXmqGe6pRnLv8FFL6BndKQ/edit?gid=0#gid=0");
  var sheet = ss.getSheetByName("Sheet1"); 
  
   if(e.parameter.Action == "AddESP")
 {
   addUser(e,sheet);
 }

  return ContentService.createTextOutput(result); // Data send to user (MIT APP)
}


function VerifyESP(e,sheet) {
  var ESPName = e.parameter.ESPName ; 
  var ESPNum = e.parameter.ESPNum ;

 var ESPName_Found = 0;
 var ESPNum_Found = 0;
 
 Data_pos = "";


 for(let i=2; i<100; i++)
  {
    var i_num = i.toString();
	
    var ESPName_Cell = "A"+i_num;
    var ESPNum_Cell = "B"+i_num;
    
    var ESPNameCellVal = sheet.getRange(ESPName_Cell).getValue();
    var ESPNumCellVal = sheet.getRange(ESPNum_Cell).getValue();
    

    if(ESPName_Found == 0)
    {
      if(ESPNameCellVal == ESPName)
      {
        ESPName_Found = 1;
        Data_pos = i_num;
      }
    }
    if(ESPNum_Found == 0)
    {
      if(ESPNumCellVal == ESPNum)
      {
        ESPNum_Found = 1;
      }
    }
    

    if(ESPNameCellVal == "")
    {
      break; // exit loop if cell is blank
    }

  }

}


function addUser(e,sheet) {
  var ESPName = e.parameter.ESPName ; 
  var ESPNum = e.parameter.ESPNum ;
  var ESPIP  = e.parameter.ESPIP;
  
  VerifyESP(e,sheet);


  var Curr_Date = Utilities.formatDate(new Date(), "Asia/Kolkata", 'dd-MM-yyyy');
  var Curr_Time = Utilities.formatDate(new Date(), "Asia/Kolkata", 'HH:mm:ss');

  var First_SignUp_Time = Curr_Date+" @ "+ Curr_Time ;
  
  if(Data_pos == "") // ESP not found so adding in new line
  {
    sheet.appendRow([ESPName,ESPNum,ESPIP,First_SignUp_Time ]);
    result = "New ESP BOARD Added ....!";
  }	
  else
  {
     var IPCellPos = "C"+Data_pos;
	 var TimeCellPos = "D"+Data_pos;

     sheet.getRange(IPCellPos).setValue(ESPIP);
	 sheet.getRange(TimeCellPos).setValue(First_SignUp_Time);
	 
	 result = "ESP IP Updated Successfully..!";
	 
  }
}



/********************************************************************************************************************/

#endif


/************************************************************************************************************************/


#endif //_GOOGLE_SHEETS_H
