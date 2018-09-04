#include <EEPROM.h>

#ifndef __OPTIMIZED__
#include <MD5.h>
#endif

#include "functions.h"
#include "sensors.h"
#include "commandlist.h"

void ExecuteCommand(CommandList& commands,const unsigned short int commandCode){
  commands.ExecuteCommands(commandCode);
  Report(commandCode);
}
bool TryGetCommand(unsigned short int& flag){
    String command = XBeeRead();
    if(command == "") return false;
    if(CheckCommand(command, flag)){
      Serial.println("We did it");
      return true;
    }
    else{
     Serial.println(command);
     Serial.println("Some error");
     RequestCommand();
     return false;
    }
}
bool CheckCommand(const String& command, unsigned short int& commandCode){
  int sep = command.indexOf("|");
  if(sep == -1) return false;
  String strCode = command.substring(0,sep);
  //CheckHash(strCode,command.substring(sep+1));
  /*
  if (hash != command.substring(sep+1)) {
     Serial.println("Hash is wrong");
     Serial.println(hash);
     Serial.println(command.substring(sep+1));
     Serial.println(sep);
     Serial.println(command);
	   return false;
  }*/
  commandCode = (unsigned short int) strCode.toInt();
  return commandCode == 0 ? false : true;
}
unsigned short int CheckAutoCommands(const float& height, const bool released){
  unsigned short int flag = 0;  
  if(height > RELEASE_HEIGHT && !released){
      flag |= Command::RELEASE;
  }
  if(height < BUZZER_HEIGHT){
     flag |= Command::BEEP_START; 
  }
}

unsigned int GetIDFromEEPROM(){
  byte* buf = new byte[sizeof(unsigned int)];
  for(short i = 0; i < sizeof(unsigned int); i++){
    EEPROM.get(EEPROM_ADDRESS_PID+i,buf[i]);
  }
  unsigned int result = *(unsigned int*)(void*)buf;
  delete buf;
  return result;
}
float GetPressureFromEEPROM(){
  byte* buf = new byte[sizeof(float)];
  for(short i = 0; i < sizeof(unsigned int); i++){
    EEPROM.get(EEPROM_ADDRESS_P0+i,buf[i]);
  }
  return *(float*)(void*)&buf;
}
bool GetReleasedStateFromEEPROM(){
  bool rState = false;
  EEPROM.get(EEPROM_ADDRESS_RSTATE,rState);
  return rState;
}

void SavePressureInEEPROM(const float p0){
  byte* ptr = (byte*)(void*)&p0;
  for(short i = 0; i < sizeof(p0);i++)
    EEPROM.write(EEPROM_ADDRESS_P0+i,*(ptr++));
}
void SaveIDInEEPROM(const unsigned int id){
  byte* ptr = (byte*)(void*)&id;
  for(short i = 0; i < sizeof(id);i++)
    EEPROM.write(EEPROM_ADDRESS_PID+i,*(ptr++));
}
void SaveReleasedStateInEEPROM(const bool rState){
  EEPROM.write(EEPROM_ADDRESS_RSTATE,rState);
}


bool CheckHash(const String& msg,const String& hash){
#ifndef __OPTIMIZED__
  char* h1 = MD5::make_digest(MD5::make_hash(msg.c_str()),16);
  const char* h2 = hash.c_str();
  bool result = true;
  if(strlen(h1) != strlen(h2)) result = false;
  for(int i = 0; i < strlen(h1) && result; i++){
    if(*h1 != *h2) result = false;
    h1++;
    h2++;
  }
  delete h1;
  delete h2;
  return result;
#endif
}

void RequestCommand(){
  XBeeSend(MSG_TYPES::COMMAND_REQUEST);
}
void Report(const unsigned short int commandCode){
  XBeeSend(MSG_TYPES::COMMAND_RESPONSE,String(commandCode));
}
long GetTimeFromEEPROM() {
  byte buf = 0;
  long result = 0;
  EEPROM.get(EEPROM_ADDRESS_RTC, buf);
  result += buf*3600;
  EEPROM.get(EEPROM_ADDRESS_RTC + 1, buf);
  result += buf*60;
  EEPROM.get(EEPROM_ADDRESS_RTC + 2, buf);
  result += buf;

  return result;
}

void SaveTimeInEEPROM(const long time) {
  long temp = time;
  EEPROM.write(EEPROM_ADDRESS_RTC + 2, (byte)(temp%60));
  temp /= 60;
  EEPROM.write(EEPROM_ADDRESS_RTC + 1, (byte)(temp%60));
  temp /= 60;
  EEPROM.write(EEPROM_ADDRESS_RTC, (byte)(temp));
}

