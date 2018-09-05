//#include <MemoryFree.h>
#include "functions.h"
#include "sensors.h"
#include <SoftwareSerial.h>
//#include <TinyGPS.h>
CommandList commands;
double temp, pressure, p0, humidity,height;
float speed,voltage;
String gpsData;
int id;
unsigned short int commandCode;
bool released = false;
String telemetryString;
long startTimePoint,flightTime; 

void TakePhoto();
void TurnOnBuzzer();
void TurnOffBuzzer();
void Release();
void GetAndSavePressure();
void Reset();

//Arguments are not finished
void GetInfoFromSensors(double& t, double& hum,double& pres, double& altit,long& fTime,float& volt,float& sp,String& gps);
//Arguments are not finished
String BuildTelemetryMessage(double t, double hum,double pres, double altit,long fTime,float volt,float sp, String& gps);
void SendTelemetry(const String& message);
SoftwareSerial xb(5,4);
SoftwareSerial gS(7,8);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  commands.AddCommand(Command::RELEASE,Release);
  commands.AddCommand(Command::RESET, Reset);
  commands.AddCommand(Command::BEEP_START,TurnOnBuzzer);
  commands.AddCommand(Command::BEEP_STOP,TurnOffBuzzer);
  commands.AddCommand(Command::TAKE_PHOTO,TakePhoto);
  commands.AddCommand(Command::SAVE_PRESSURE, GetAndSavePressure);
  InitializeSensors(xb,gS);
  p0 = GetPressureFromEEPROM();
  p0 = 101000;
  id = GetIDFromEEPROM();
  released = GetReleasedStateFromEEPROM();
  startTimePoint = GetTimeFromEEPROM();
 // Serial.println(p0);
  gS.begin(9600);
  xb.begin(9600);

  
}
int timeout = 1000;
void loop() {
  Release();
  //Incremental code
  if(TryGetCommand(commandCode)){
      ExecuteCommand(commands,commandCode);
  }
  GetInfoFromSensors(temp,humidity,pressure,height,flightTime,voltage,speed,gpsData);
  if(unsigned short int code = CheckAutoCommands(height,released)){
      ExecuteCommand(commands,code);
  }
  telemetryString = BuildTelemetryMessage(temp,humidity,pressure,height,flightTime,voltage,speed,gpsData);
  if(telemetryString)
    SendTelemetry(telemetryString);
  id++;
  SaveIDInEEPROM(id);
  delay(500);/*
  TurnServo(100);
  delay(500);
  TurnServo(-100);
  delay(500);*/
  /*
  GetInfoFromSensors(temp,humidity,pressure,height);
  if(unsigned short int code = CheckAutoCommands(height,released)){
      ExecuteCommand(commands,code);
  }
  telemetryString = BuildTelemetryMessage(temp,pressure,humidity,height);
  if(telemetryString)
    SendTelemetry(telemetryString);
  id++;
  SaveIDInEEPROM(id);
  */
}


void TakePhoto(){
  SavePhoto();
}
void TurnOnBuzzer(){
  SetBuzzerState(true);
}
void TurnOffBuzzer(){
  SetBuzzerState(false);  
}
void Release(){
  TurnServo(20);
  delay(1000);
  TurnServo(-20);
  delay(1000);
  TurnServo(90);
  }
void GetAndSavePressure(){
  double p,h;
  GetPressureAndHeight(p0,temp,p,h);
  SavePressureInEEPROM(p);
  p0 = p;
  //Serial.println("p0");
}
void Reset(){
  SaveIDInEEPROM(0);
  id = 0;
  GetFlightTime(startTimePoint);
  SaveTimeInEEPROM(startTimePoint);
}

void SendTelemetry(const String& msg){
  XBeeSend(MSG_TYPES::TELEMETRY,msg);
}
String BuildTelemetryMessage(double t, double hum,double pres, double altit,long fTime,float volt, float sp, String& gps){
  String telemetry;
  telemetry.concat(String(id));
  telemetry.concat(",");
  telemetry.concat(String(fTime));
  telemetry.concat(",");
  telemetry.concat(String(t));
  telemetry.concat(",");
  telemetry.concat(String(pres));
  telemetry.concat(",");
  telemetry.concat(String(altit));
  telemetry.concat(",");
  telemetry.concat(String(hum));
  telemetry.concat(",");
  telemetry.concat(String(sp));
  telemetry.concat(",");
  telemetry.concat(String(volt));
  telemetry.concat(",");
  telemetry.concat(gps);
  return telemetry;
}
void GetInfoFromSensors(double& t, double& hum,double& pressure, double& altit,long& fTime,float& volt,float& sp,String& gpsData){
  
  if(!GetTemperatureAndHumidity(t,hum)){
    t = 0;
  }
  GetFlightTime(fTime);
  fTime -= startTimePoint;
  GetPressureAndHeight(p0,t,pressure,altit);
  //GetVoltage(volt);
  GetGPSCoordinates(gpsData);
  GetSpeed(sp); 
  
}

