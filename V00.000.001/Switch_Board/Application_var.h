#ifndef _APPLICATION_VARIABLES_H
#define _APPLICATION_VARIABLES_H

#define MAX_SWITCH 3  // Switch.h maximum switches
#define MAX_RELAY  3  // Relay.h maximum switches

#include "PIN_CONFIG.h"
#include "Millis_Timer.h"
#include "google_sheet.h"

#include "Switch.h"
#include "Relay.h"

extern String Appl_mit_DATA;
extern int MIT_flag;
String MIT_Appl_cmd = "Appl CMD's\n";

#define DEBUG 1


#define _ON  1
#define _OFF 0


// Must start timer from 3 (0,1,2 was reserved )
#define SLEEP_TIMER1 3 // 5 sec timer

void switch_pin_setup()
{
  SWITCH[0].pin = SWITCH1 ;
  SWITCH[0].name = "SWITCH_1";
  SWITCH[0].pin_tag = 12; // tag should match with relay

  SWITCH[1].pin = SWITCH2 ;
  SWITCH[1].name = "SWITCH_2";
  SWITCH[1].pin_tag = 13; //  tag should match with relay

  SWITCH[2].pin = SWITCH3 ;
  SWITCH[2].name = "SWITCH_3";
  SWITCH[2].pin_tag = 14; // tag should match with relay

}

void relay_pin_setup()
{
  RELAY[0].pin = RELAY1 ;
  RELAY[0].name = "LIGHT_1";
  RELAY[0].pin_tag = 12; // tag should match with switch

  RELAY[1].pin = RELAY2 ;
  RELAY[1].name = "LIGHT_2";
  RELAY[1].pin_tag = 13; // tag should match with switch

  RELAY[2].pin = RELAY3 ;
  RELAY[2].name = "LIGHT_3";
  RELAY[2].pin_tag = 14; // tag should match with switch

}












#endif //_APPLICATION_VARIABLES_H