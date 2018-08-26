#include <EEPROM.h>

#ifndef __OPTIMIZED__
#include <MD5.h>
#endif

#include "functions.h"
#include "sensors.h"
#include "commandlist.h"

#define RELEASE_HEIGHT 400
#define BUZZER_HEIGHT 10
#define EEPROM_ADDRESS_P0 0x10
#define EEPROM_ADDRESS_PID 0x20
#define EEPROM_ADDRESS_RSTATE 0x30

bool ExecuteCommand(CommandList& commands,const unsigned short int commandCode){
  bool result = false; 
  try{
     commands.ExecuteCommands(commandCode);
     result = true;
   }catch(...){
      result = false;
   } 
   SendReport(commandCode, result);
}
bool TryGetCommand(String& command, unsigned short int& flag){
    unsigned short int flag;
    if(CheckCommand(commands,command, flag)){
      return true;
    }
    else{
     RequestCommand(); 
     return false;
    }
}
bool CheckCommand(const String& command, unsigned short int& commandCode){
  int sep = command.indexOf("|");
  if(sep == -1) return false;
  String strCode = command.substring(0,sep);
  String* hash = CalculateHash(strCode);
  if (*hash != command.substring(sep)) {
	  free(hash);
	  return false;
  }
  try{
    commandCode = (unsigned short int) strCode.toInt();
  }
  catch(...){
	free(hash);
    return false;
  }
  free(hash);
  return true;
}
unsigned short int CheckAutoCommands(const float& height, const bool released){
  unsigned short int flag = 0;  
  if(height > RELEASE_HEIGHT && !released){
      flag |= Command::RELEASE;
  }
  if(height < BUZZER_HEIGHT){
     flag |= Command::BEEP_START; 
  }
  else{
    flag |= Command::BEEP_END;
  }
  return flag;
}

unsigned int GetIDFromEEPROM(){
  byte buffer[] = new buffer[sizeof(unsigned int)];
  for(short i = 0; i < sizeof(unsigned int); i++){
    EEPROM.get(EEPROM_ADDRESS_PID+i,buffer[i]);
  }
  return *(unsigned int*)(void*)&buffer;
}
float GetPressureFromEEPROM(){
  byte buffer[] = new buffer[sizeof(float)];
  for(short i = 0; i < sizeof(unsigned int); i++){
    EEPROM.get(EEPROM_ADDRESS_P0+i,buffer[i]);
  }
  return *(float)(void*)&buffer;
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


String* CalculateHash(const String& msg){
#ifndef __OPTIMIZED__
  String* result = new String(MD5::make_digest(MD5::make_hash(msg.c_str()),16));
  return result;
#endif
}

void RequestCommand(){
  XBeeSend(MSG_TYPES::COMMAND_REQUEST);
}
void Report(const unsigned short int commandCode, const bool executionStatus){
  XBeeSend(MSG_TYPES::COMMAND_RESPONSE,String(commandCode) + "," + String(executionStatus));
}