#ifndef _RELAY_H
#define _RELAY_H

 //#define MAX_RELAY 3 // refer Application_var.h

typedef struct st1
{
 bool state;
 bool old_state;
 byte pin;
 String name;
 byte pin_tag; // to map switch

}relay_state;

relay_state RELAY[MAX_RELAY] = {0};
// EEPROM
byte memory_addr = 0;
byte memory_flag = 0;

byte relay_addr = 4;
int relay_stored_val = 0;

void Disable_memory_flag()
{
  memory_flag = 0;
  EEPROM.write(memory_addr, memory_flag);
  EEPROM.commit();
  delay(50);
}


void copy_relay_data_to_memory()  // not working need to check
{
  byte i = 0;
  int val = 0;
  int data = 0;

  for(i = 0; i < MAX_RELAY; i++ )
  {
    val = 0;
    val = RELAY[i].state ;
    val = val << i;
    data = data | val;
  }
  EEPROM.write(relay_addr, data);
  EEPROM.commit();
  delay(50);
}

void Enable_memory_flag()
{
  memory_flag = 1;
  EEPROM.write(memory_addr, memory_flag);
  EEPROM.commit();
  delay(50);
  copy_relay_data_to_memory();
}
String Get_memory_flag_status()
{
  String S = "";
  if(memory_flag == 1)
  {
    S ="Memory flag was Enabled";
  }
  else
  {
    S ="Memory flag was Disabled";
  }
  return S;
}

void Read_relay_stored_state()
{
  byte i = 0;
  bool stored_state = 0;
  relay_stored_val = EEPROM.read(relay_addr);  
  for(i = 0; i < MAX_RELAY ; i++ )
    {
      stored_state = (relay_stored_val >> i)&1 ;
      RELAY[i].state = stored_state;
    }
}
void Write_relay_stored_state(byte index , bool state)
{

  int temp_var = 0;

  temp_var = state;
  temp_var = temp_var << index ;
  
  relay_stored_val = relay_stored_val | temp_var ;

  EEPROM.write(relay_addr, relay_stored_val);
  EEPROM.commit();
  delay(50);

}

String Get_relay_stored_state()
{
  byte i = 0;
  bool stored_state = 0;
  String s = "EEPROM stored state \n";
  for(i = 0; i < MAX_RELAY; i++ )
  {
    stored_state = (relay_stored_val >> i)&1 ;

    if(stored_state)
    {
      s = s + " ON \n";
    }
    else
    {
      s = s + " OFF \n";
    }
  }

  return s;
}

void relay_setup()
{
  byte i = 0;

    for(i = 0; i < MAX_RELAY ; i++ )
    {
      pinMode(RELAY[i].pin, OUTPUT);
    }
    for(i = 0; i < MAX_RELAY ; i++ )
    {
      digitalWrite(RELAY[i].pin, 0); // off state
      RELAY[i].state = 0; // off
    }
  memory_flag = EEPROM.read(memory_addr);

  if(memory_flag == 1)
  {
    Read_relay_stored_state();
  }  
}


void Update_relay_state_with_index(byte index , bool state)
{

  RELAY[index].state = state;

}

void Update_relay_state_with_pin(byte pin , bool state)
{
  byte i = 0;
  
  for(i = 0; i < MAX_RELAY ; i++ )
  {
    if(pin == RELAY[i].pin)
    {
      RELAY[i].state = state;
      break;
    }
  }
}

void Update_relay_state_with_pin_tag(byte pin_tag , bool state)
{
  byte i = 0;
  
  for(i = 0; i < MAX_RELAY ; i++ )
  {
    if(pin_tag == RELAY[i].pin_tag)
    {
      RELAY[i].state = state;

      Serial.print(RELAY[i].name);
      Serial.print(" was updated wit : ");
      Serial.println(RELAY[i].state);
      
      //break;
    }
  }
}

void Update_relay_state_with_name(String name , bool state)
{
  byte i = 0;
  
  for(i = 0; i < MAX_RELAY ; i++ )
  {
    if(name == RELAY[i].name)
    {
      RELAY[i].state = state;

      Serial.print(RELAY[i].name);
      Serial.print(" was updated wit : ");
      Serial.println(RELAY[i].state);
      
      break;
    }
  }
}

byte get_relay_pin(byte index)
{
  byte pin = 255;
  if(index < MAX_RELAY)
  {
    pin = RELAY[index].pin;
  }
  return pin;
}

byte get_relay_pin_tag(byte index)
{
  byte pin_tag = 255;
  if(index < MAX_RELAY)
  {
    pin_tag = RELAY[index].pin_tag;
  }
  return pin_tag;
}

String get_relay_name(byte index)
{
  String name = "";
  if(index < MAX_RELAY)
  {
    name = RELAY[index].name;
  }
  return name;
}

bool get_relay_state(byte index)
{
  bool state = 0;
  if(index < MAX_RELAY)
  {
    state = RELAY[index].state;
  }
  return state;
}



void update_relay_state()
{
  byte i = 0;

  for(i = 0; i < MAX_RELAY ; i++ )
  {
    if(RELAY[i].state != RELAY[i].old_state )
    {
      digitalWrite(RELAY[i].pin , RELAY[i].state);
      if(memory_flag == 1)
      {
        Write_relay_stored_state(i , RELAY[i].state);
      }
      RELAY[i].old_state = RELAY[i].state ;
    }
  }
}

void relay_loop()
{
  update_relay_state();
}


#endif // _RELAY_H