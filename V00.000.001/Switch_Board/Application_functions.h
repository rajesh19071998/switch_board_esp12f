#ifndef _APPLICATION_FUNCTIONS_H
#define _APPLICATION_FUNCTIONS_H

#include "Application_var.h"

// This file related Header files , Macros , variables , structures must be declared in  "Application_var.h" file





void Application_Setup()
{
  switch_pin_setup(); // Application_var.h
  relay_pin_setup(); // Application_var.h
  switch_setup(1); // 1 for IPUT_PULLUP, 0 for INPUT
  relay_setup();
 
  Timer[SLEEP_TIMER1].delayMillis = 5000; // 5 sec
  

     /*
  EEPROM.write(head_light_addr, 0);
  EEPROM.write(auto_lock_addr, 0);
  EEPROM.write(auto_sleep_addr, 0);
  EEPROM.commit();
  delay(100);
  */

} //Application_Setup









void All_sleep()
{
  if(Timer[SLEEP_TIMER1].active == 0)
    Timer_activate(SLEEP_TIMER1);

  Appl_mit_DATA = "Sleep Activated in 5 sec...!";

}

void Timer_timeout_check()
{

  if(Timer[SLEEP_TIMER1].timeout == 1) // 5 sec timer
  {
  
    Timer_deactivate(SLEEP_TIMER1);
    delay(1000);
    Serial.println("SLEEP Timer 1 Expired! Going to Sleep...!");
    ESP.deepSleep(0); 
  } 


  

}



void Update_IP_On_Google_Sheet()
{
  static int OneCall = 0; // it will execute one time in RST or power cycle
  if(OneCall == 0)
  {
     // String string_temperature =  String(tem); //float to string

     // String string_humidity =  String(hum, DEC); // int to string

    // String G_Data = "Action=AddESP&ESPName=" + BOARD_NAME + "&ESPNum=" + BOARD_NUMBER + "&ESPIP=" + WiFi.localIP().toString();
     String G_Data = "Action=AddESP&ESPName=" + BOARD_NAME + "&ESPNum=" + BOARD_NUMBER + "&ESPIP=" + MY_IP;
    sendDataToGoogleSheet(G_Data);
    OneCall = 1;
  }

}

void check_switch_status()
{
  bool changed = 0;
  byte i = 0;

  for(i = 0; i < MAX_SWITCH ; i++ )
  {
    changed = Switch_state_changed(i);

    if(changed) // 1
    {
      Update_relay_state_with_pin_tag( SWITCH[i].pin_tag , SWITCH[i].state );
    }
  }
}

void update_relay_as_switch()
{

  byte i = 0;
  for(i = 0; i < MAX_SWITCH ; i++ )
  {

      Update_relay_state_with_pin_tag( SWITCH[i].pin_tag , SWITCH[i].state );
   
  }
  Appl_mit_DATA = "All LIGHTS are updated as per local Switches State ...!";
}



void Application_loop()
{
  // keep watching the push button:
  switch_loop();
  relay_loop();
  check_switch_status();
  Timer_timeout_check();
  Timer_loop(); // Millis_Timer.h

  Update_IP_On_Google_Sheet(); // only one time

}
void relayon(byte index )
{
  Update_relay_state_with_index(index , 1 );// index , state  
  Appl_mit_DATA = get_relay_name(index) + " was turned ON ";
}
void relayoff(byte index )
{
  Update_relay_state_with_index(index , 0 );// index , state
  Appl_mit_DATA = get_relay_name(index) + " was turned OFF ";
}

void relaystatus(byte index)
{
  bool state;
  state = get_relay_state(index);
 if(state)
    Appl_mit_DATA = get_relay_name(index) + " was in ON state ";
  else
    Appl_mit_DATA = get_relay_name(index) + " was in OFF state ";

}

void allon()
{
 relayon( 0 ); 
 relayon( 1 ); 
 relayon( 2 );
 Appl_mit_DATA = "All LIGHTS are ON";
}

void alloff()
{
 relayoff( 0 ); 
 relayoff( 1 ); 
 relayoff( 2 ); 
 Appl_mit_DATA = "All LIGHTS are OFF";
}

void switchstatus(byte index)
{
  bool state;
  state = get_switch_state(index);
 if(state)
    Appl_mit_DATA = get_switch_name(index) + " was in ON state ";
  else
    Appl_mit_DATA = get_switch_name(index) + " was in OFF state ";

}

void mit_app_requist_process(String request) // refer MIT_APP.h
{

  if  ( request.indexOf("Sleep") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); All_sleep(); }
  else if ( request.indexOf("l1on") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); relayon(0); }//index
  else if ( request.indexOf("l1off") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); relayoff(0); }
  else if ( request.indexOf("l1sta") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); relaystatus(0); }

  else if ( request.indexOf("l2on") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); relayon(1); }//index
  else if ( request.indexOf("l2off") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); relayoff(1); }
  else if ( request.indexOf("l2sta") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); relaystatus(1); }

  else if ( request.indexOf("l3on") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); relayon(2); }//index 
  else if ( request.indexOf("l3off") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); relayoff(2); }
  else if ( request.indexOf("l3sta") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); relaystatus(2); }  

  else if ( request.indexOf("sw1sta") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); switchstatus(0); }
  else if ( request.indexOf("sw2sta") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); switchstatus(1); }
  else if ( request.indexOf("sw3sta") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); switchstatus(2); }

  else if ( request.indexOf("allon") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); allon(); }  
  else if ( request.indexOf("alloff") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); alloff(); }

  else if ( request.indexOf("memoryon") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); Enable_memory_flag(); Appl_mit_DATA = "Memory flag Enabled"; } 
  else if ( request.indexOf("memoryoff") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); Disable_memory_flag(); Appl_mit_DATA = "Memory flag Disabled"; } 

  else if ( request.indexOf("getmemory") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); Appl_mit_DATA = Get_relay_stored_state(); } 
  else if ( request.indexOf("memorysta") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); Appl_mit_DATA = Get_memory_flag_status(); }

  else if ( request.indexOf("local") > 0 ) { MIT_flag = 2; if(DEBUG) Serial.println(request); update_relay_as_switch(); }

}



















#endif //_APPLICATION_FUNCTIONS_H
