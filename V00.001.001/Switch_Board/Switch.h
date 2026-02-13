#ifndef _SWITCH_H
#define _SWITCH_H

 //#define MAX_SWITCH 3 // refer Application_var.h

typedef struct st
{
 bool state;
 bool old_state;
 bool invert;
 byte pin;
 String name;
 byte pin_tag; // to map relay

}switch_state;

switch_state SWITCH[MAX_SWITCH] = {0} ;

void switch_setup(byte op)
{
  byte i = 0;
  if(op == 1)
  {
    for(i = 0; i < MAX_SWITCH ; i++ )
    {
      pinMode(SWITCH[i].pin, INPUT_PULLUP);
      SWITCH[i].invert = 1;
    }

  }

  if(op == 0)
  {
    for(i = 0; i < MAX_SWITCH ; i++ )
    {
      pinMode(SWITCH[i].pin, INPUT);
      SWITCH[i].invert = 0;
    }

  }
}


void Update_switch_state()
{
  byte i = 0;
  bool Current_State = 0;

  for(i = 0; i < MAX_SWITCH ; i++ )
    {
      Current_State = digitalRead(SWITCH[i].pin);

      if(SWITCH[i].invert)
      {
        Current_State = !Current_State ;
      }

      if(Current_State != SWITCH[i].state)
      {
        SWITCH[i].state = Current_State;

        Serial.print(SWITCH[i].name);
        Serial.print(" was updated wit : ");
        Serial.print(SWITCH[i].state);
        Serial.print(" pin : ");
        Serial.print(SWITCH[i].pin);

        Serial.print(" index : ");
        Serial.println(i);

       }
    }
}

String get_switch_name(byte index)
{
  String name = "";
  if(index < MAX_SWITCH)
  {
    name = SWITCH[index].name;
  }
  return name;
}

bool get_switch_state(byte index)
{
  bool state = 0;
  if(index < MAX_SWITCH)
  {
    state = SWITCH[index].state;
  }
  return state;
}

bool Switch_state_changed(byte sw)
{
  bool change = 0;
 
  if( SWITCH[sw].state != SWITCH[sw].old_state )
  {
    change = 1;
    SWITCH[sw].old_state = SWITCH[sw].state;
  }

  return change;
}


void switch_loop()
{
  
 Update_switch_state();


}







#endif //_SWITCH_H