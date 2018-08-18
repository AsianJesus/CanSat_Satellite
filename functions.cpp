#include "functions.h"
#include "commandlist.h"

#define RELEASE_HEIGHT 400
#define BUZZER_HEIGHT 10

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
    if(CheckCommand(command, flag)){
      return true;
    }
    else{
     RequestCommand(); 
     return false;
    }
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
