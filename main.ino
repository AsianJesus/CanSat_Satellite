#include "functions.h"


CommandList commands;
float temp, pressure, p0, humidity,height;
int id;
int commandCode;
bool released = false;
String commandString;
String telemetryString;

void TakePhoto();
void ChangeBuzzerState();
void Release();
void GetAndSavePressure();
void Reset();

//Arguments are not finished
void GetInfoFromSensors(float& temp, float& pressure, float& humidity,float& height);
//Arguments are not finished
String* BuildTelemetryMessage(int id,float temp, float pressure, float humidity,float height);
void SendTelemetry(const String& message);

void setup() {
  // put your setup code here, to run once:
  commands = CommandList();
  commands.AddCommand(Command::RELEASE,Release);
  commands.AddCommand(Command::RESET, Reset);
  commands.AddCommand(Command::BEEP,ChangeBuzzerState);
  commands.AddCommand(Command::TAKE_PHOTO,TakePhoto);
  commands.AddCommand(Command::SAVE_PRESSURE, GetAndSavePressure());
  p0 = GetPressureFromEEPROM();
  id = GetIDFromEEPROM();
  released = GetReleasedFromEEPROM();
}

void loop() {
  // put your main code here, to run repeatedly:
  id = GetIDFromEEPROM();
  if(TryGetCommand(commandString)){
    if(CheckCommand(commandString,commandCode)){
        ExecuteCommand(commands,commandCode);
    }
    else{
        RequestCommand();
    }
  }
  GetInfoFromSensors(temp,pressure,humidity,height);
  if(short int code = CheckAutoCommands(height,released)){
      Report(code,ExecuteCommand(commands,code));
  }
  telemetryString = BuildTelemetryMessage(temp,pressure,humidity,height);
  SendTelemetry(telemetryString);
  id++;
  SaveIDInEEPROM(id);
}

