#include "sensors.h"

#include <Servo.h>
#include <DHT.h>

Servo servo;
DHT dht(PIN_DHT,DHT_TYPE);

void InitializeSensors(){
  servo.attach(PIN_SERVO);
  dht.begin();
  // ...
}

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

void TurnServo(const float degree){
  servo.write(degree);
}
bool GetTemperatureAndHumidity(float& temp, float& humidity){
  float t = dht.readTemperature();
  if(isnan(t)) return false;
  temp = t;
  humidity = dht.readHumidity();
  return true;
}
