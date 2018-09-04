#include "sensors.h"
#include <RTClib.h>
#include <SFE_BMP180.h>
#include <Servo.h>
#include <DHT.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Wire.h>
Servo servo;
DHT dht(PIN_DHT,DHT_TYPE);
SoftwareSerial* gpsSerial;
SoftwareSerial* xbee;
TinyGPS gps;
SFE_BMP180 bmp;
RTC_DS1307 RTC;
float offset = 0;
void InitializeSensors(SoftwareSerial& xb,SoftwareSerial& gS){
  servo.attach(PIN_SERVO);
  dht.begin();
  Wire.begin();
  RTC.begin();
  xbee = &xb;
  xbee->begin(9600);
  xbee->setTimeout(100);
  gpsSerial = &gS;
  gpsSerial->begin(9600);
  delay(1000);
  bmp.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
 RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  int sensorValue = 0;
  int sum = 0;
   for(int i=0;i<10;i++)
    {
         sensorValue = analogRead(PIN_PITOTTUBE)-512;
         sum+=sensorValue;
    }
    offset=sum/10.0;
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
            tone(PIN_BUZZER, 100);
            buzzerState = true;
        }
        else{
            noTone(PIN_BUZZER);
            buzzerState = false;
        }
      } 
    }
    else{
      noTone(PIN_BUZZER);
    }
}
bool GetGPSCoordinates(String& gpsData){
  gpsSerial->listen();
  bool newdata = false;
  unsigned long start = millis();
  // Every 5 seconds we print an update
  while (millis() - start < 500) 
  {
    if (gpsSerial->available())  
    {
      char c = gpsSerial->read();
      //Serial.print(c);  // uncomment to see raw GPS data
      if (gps.encode(c))
      {
        newdata = true;
        break;  // uncomment to print new data immediately!
      }
    }
  }
  if(!newdata){
    gpsData = ",,,,";
    return false;
  }
  float lat, lon;
  unsigned long age;
  gps.f_get_position(&lat, &lon, &age);
  unsigned long date, time;
  gps.get_datetime(&date,&time,&age);
  gpsData = String(lat) + "," + String(lon) + "," + String(gps.satellites()) + "," + String(date) + ","+String(time);
  return true;
}
bool GetPressureAndHeight(const double& p0, double& temp, double& pressure, double& height){
  double t = temp;
  char status;
  if(!t){
    status = bmp.startTemperature();
    if(!status) return false;
    delay(status);
    status = bmp.getTemperature(t);
    temp = t;
    if(!status)return false;
  }
  status = bmp.startPressure(3);
  if(!status)return false;
  delay(status);
  status = bmp.getPressure(pressure,t);
  if(!status) return false;
  height = bmp.altitude(pressure,p0/100);
  pressure *= 100;
  return true;
}

void TurnServo(const float degree){
  servo.write(degree);
}
bool GetTemperatureAndHumidity(double& temp, double& humidity){
  double t = dht.readTemperature();
  if(isnan(t)) return false;
  temp = t;
  humidity = dht.readHumidity();
  return true;
}
void XBeeSend(MSG_TYPES type,const String& msg){
  xbee->println(String(type) + "/" + msg);
}
void XBeeSend(MSG_TYPES type){
  xbee->println(String(type));
}
String XBeeRead(){
  xbee->listen();
  String res = xbee->readStringUntil('\n');
  return res;
}
bool GetSpeed(float& sp){
  int sensorValue = analogRead(PIN_PITOTTUBE)-offset; 
  float P =(5*sensorValue)/1024.0 - 2.5;
  sp = sqrt((2*abs(P))/1.225);
  return true;
}
bool GetVoltage(float&){
  return 2* map(analogRead(PIN_VOLTAGE_DIVIDER),0,1024,0,2.5);
}
void SavePhoto(){
  digitalWrite(PIN_CAMERA,HIGH);
  delay(100);
  digitalWrite(PIN_CAMERA,LOW);
}

bool GetFlightTime(long& time) {
  DateTime DT = RTC.now();
  time = DT.hour()*3600 + DT.minute()*60 + DT.second();
  return true;
  }

