#include "functions.h"
#include "sensors.h"
#include <SoftwareSerial.h>
#include <TimerOne.h>
CommandList commands;
double temp, pressure, p0, humidity,height;
float speed,voltage;
String gpsData, miscData;
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
void GetInfoFromSensors(double& t, double& hum,double& pres, double& altit,long& fTime,float& volt,float& sp,String& gps,String& misc);
//Arguments are not finished
String BuildTelemetryMessage(double t, double hum,double pres, double altit,long fTime,float volt,float sp, String& gps, String& misc);
void SendTelemetry(const String& message);

void StartSendingTelemetry();
void StopSendingTelemetry();

SoftwareSerial xb(5,4);
SoftwareSerial gS(7,8);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  InitializeSensors(xb,gS);
  GiveSoundCommand(100,3);
  commands.AddCommand(Command::RELEASE,Release);
  commands.AddCommand(Command::RESET, Reset);
  commands.AddCommand(Command::BEEP_START,TurnOnBuzzer);
  commands.AddCommand(Command::BEEP_STOP,TurnOffBuzzer);
  commands.AddCommand(Command::TAKE_PHOTO,TakePhoto);
  commands.AddCommand(Command::SAVE_PRESSURE, GetAndSavePressure);
  GiveSoundCommand(125,3);
  p0 = GetPressureFromEEPROM();
  p0 = 101000;
  id = GetIDFromEEPROM();
  released = GetReleasedStateFromEEPROM();
  startTimePoint = GetTimeFromEEPROM();
 // Serial.println(p0);
  gS.begin(9600);
  xb.begin(9600);
  GiveSoundCommand(150,3);
}
int timeout = 1000;
void loop() {
  Release();
  //Incremental code
  if(TryGetCommand(commandCode)){
      ExecuteCommand(commands,commandCode);
  }
  GetInfoFromSensors(temp,humidity,pressure,height,flightTime,voltage,speed,gpsData,miscData);
  if(unsigned short int code = CheckAutoCommands(height,released)){
      ExecuteCommand(commands,code);
  }
  telemetryString = BuildTelemetryMessage(temp,humidity,pressure,height,flightTime,voltage,speed,gpsData,miscData);
  id++;
  SaveIDInEEPROM(id);
  /*
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
  SetBuzzerState(true,BUZZER_COOLDOWN);
}
void TurnOffBuzzer(){
  SetBuzzerState(false,BUZZER_COOLDOWN);  
}
void Release(){
  StopSendingTelemetry();
  TurnServo(90,false);
  delay(1000);
  TurnServo(0,true);
  StartSendingTelemetry();
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

void StartSendingTelemetry(){
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(SendTelemetry);
}
void StopSendingTelemetry(){
  Timer1.detachInterrupt();
}
void SendTelemetry(){
  Serial.println(telemetryString);
  //XBeeSend(MSG_TYPES::TELEMETRY,telemetryString);
}
String BuildTelemetryMessage(double t, double hum,double pres, double altit,long fTime,float volt, float sp, String& gps,String& miscData){
  String telemetry;
  telemetry.concat(String(id));
  telemetry.concat(",");
  telemetry.concat(String(fTime));
  telemetry.concat(",");
  if(t != 0)
  telemetry.concat(String(t));
  telemetry.concat(",");
  if(pres != 0)
  telemetry.concat(String(pres));
  telemetry.concat(",");
  if(altit != 0)
  telemetry.concat(String(altit));
  telemetry.concat(",");
  if(hum != 0)
  telemetry.concat(String(hum));
  telemetry.concat(",");
  if(sp != 0);
  telemetry.concat(String(sp));
  telemetry.concat(",");
  if(volt != 0)
  telemetry.concat(String(volt));
  telemetry.concat(",");
  telemetry.concat(gps);
  telemetry.concat(",");
  telemetry.concat(miscData);
  return telemetry;
}
void GetInfoFromSensors(double& t, double& hum,double& pressure, double& altit,long& fTime,float& volt,float& sp,String& gpsData,String& misc){
  
  bool couldRead = GetTemperatureAndHumidity(t,hum));
  GetFlightTime(fTime);
  fTime -= startTimePoint;
  GetPressureAndHeight(p0,t,pressure,altit,!couldRead);
  GetVoltage(volt);
  GetGPSCoordinates(gpsData);
  GetSpeed(sp); 
  GetMISCData(misc);
}

