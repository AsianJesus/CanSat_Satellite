#include "sensors.h"


void SetBuzzerState(bool verify){
  static bool buzzerState = false;
  static int previousValue = 0;
  if(verify){ 
    unsigned long currentValue = millis();
    if(currentValue - previousValue >= 1000){
        previousValue = currentValue;
        if(!buzzerState){
            tone(buzzer, 100);
            buzzerState = true;
        }
        else{
            noTone(buzzer);
            buzzerState = false;
        }
      } 
    }
    else{
      noTone(buzzer);
    }
  }
