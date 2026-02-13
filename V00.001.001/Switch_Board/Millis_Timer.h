#ifndef _MILLIS_TIMER_H
#define _MILLIS_TIMER_H

//#define MAX_TIMERS 10 // refer .ino file
//#define _TIMER_DEBUG

typedef struct
{
  unsigned long previousMillis;
  unsigned long delayMillis;

  byte timeout;
  byte active;
}MillisTimer;

MillisTimer Timer[MAX_TIMERS] = {0};

void Timer_activate(int timer_num)
{
  Timer[timer_num].active = 1;
  Timer[timer_num].timeout = 0;
  Timer[timer_num].previousMillis = millis();
#ifdef _TIMER_DEBUG
    Serial.println(Timer[timer_num].previousMillis);
    Serial.print("Timer Activated in Millis Timer = ");
    Serial.println(timer_num);
#endif 
}

void Timer_deactivate(int timer_num)
{
  Timer[timer_num].active = 0;
  Timer[timer_num].timeout = 0;
#ifdef _TIMER_DEBUG
    Serial.print("Timer Deactivated in Millis Timer = ");
    Serial.println(timer_num);
#endif
}

void Timer_loop()
{
  unsigned long current_millis = millis(); // remove this
  for(int i = 0 ; i < MAX_TIMERS ; i++)
  {
    
    if(Timer[i].active == 1)
    {
      if( ( Timer[i].previousMillis + Timer[i].delayMillis ) <= millis() )
      {
        Timer[i].timeout = 1;
        Timer[i].previousMillis = millis();

#ifdef _TIMER_DEBUG
          Serial.println(Timer[i].delayMillis);
          Serial.println(Timer[i].previousMillis);
          Serial.println(current_millis);
        
          Serial.print("Timer : ");
          Serial.print(i);
          Serial.println(" Expired..!");
#endif
      }
    }

  }



}


















#endif //_MILLIS_TIMER_H
