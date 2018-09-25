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
bool isCameraActivated = false;
float offset = 0;
void InitializeSensors(SoftwareSerial& xb,SoftwareSerial& gS){
  pinMode(PIN_BUZZER,OUTPUT);
  dht.begin();
  Wire.begin();
  RTC.begin();
  xbee = &xb;
  xbee->begin(9600);
  xbee->setTimeout(200);
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
    pinMode(PIN_MICS_PREHEAT,OUTPUT);
    digitalWrite(PIN_MICS_PREHEAT,OUTPUT);
    delay(MICS_PREHEAT_SECONDS);
    digitalWrite(PIN_MICS_PREHEAT,LOW);
    pinMode(PIN_CAMERA,OUTPUT);
    TurnServo(0,false,500);
    // ...
}
void GiveSoundCommand(const unsigned int t,const unsigned int iter){
    for(int i = 0; i < iter; i++){
      digitalWrite(PIN_BUZZER,HIGH);
      delay(t * (i+1));
      digitalWrite(PIN_BUZZER,LOW);
      delay(t * (i+1));
    }
}

void SetBuzzerState(bool verify,long t,bool force){
  static bool buzzerState = false;
  static int previousValue = 0;
  static long coolDown = 0;
  if(coolDown > millis() && !force){
    return;
  }
  if(verify){ 
    coolDown = millis() + t;
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
void CorrectPressure(double& pressure){
  static double prevValue = 0;
  static double p = 1;
  const double r = 64, q =0.5;
  if(prevValue){
    double _p = p + q;
    double k = _p / (_p + r);
    pressure = (prevValue + k*(pressure - prevValue));
    p = _p * (1-k);
  }
  prevValue = pressure;
}
bool GetPressureAndHeight(const double& p0, double& temp, double& pressure, double& height, const bool findTemp){
  double t = temp;
  char status;
  if(findTemp){
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
  CorrectPressure(pressure);
  height = bmp.altitude(pressure,p0/100);
  pressure *= 100;
  return true;
}
bool GetMISCData(String& miscData){
  int vnox_value = analogRead(PIN_MICS_VNOX);
  int vred_value = analogRead(PIN_MICS_VRED);
  if(vnox_value == -1 || vred_value == -1){
    miscData = ",";
    return false;
  }
  miscData = String(map(vred_value,100,1500,1000,1)*1e-4)+","+String(map(vnox_value*100,80,2000,5,1000)*1e-6);
  return false;
}
void TurnServo(const int servoNum, const float degree,const bool doDetach, const int timeout){
  if(!servo.attached()){
    servo.attach(servoNum);
  }
  servo.write(degree);
  delay(timeout);
  if(doDetach){
    servo.detach();
  }
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
bool GetVoltage(float& vOut){
  int inputVoltage = analogRead(PIN_VOLTAGE_DIVIDER);
  if(inputVoltage == -1){
    Serial.println("Couldn't read");
    vOut = 0;
    return false;
  }
  else{
    vOut = 10*inputVoltage / 1024.0f ;
    return true;
  }  
}
void SavePhoto(){
  if(!isCameraActivated){
    isCameraActivated = true;
    digitalWrite(PIN_CAMERA,HIGH);
    delay(2000);
    digitalWrite(PIN_CAMERA,LOW);
  }
  digitalWrite(PIN_CAMERA,HIGH);
  delay(100);
  digitalWrite(PIN_CAMERA,LOW);
}

bool GetFlightTime(long& time) {
  DateTime DT = RTC.now();
  time = DT.hour()*3600 + DT.minute()*60 + DT.second();
  return true;
  }

