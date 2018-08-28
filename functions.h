#ifndef __FUNCTIONS__
#include "commandlist.h"

unsigned int GetIDFromEEPROM();
float GetPressureFromEEPROM();
bool GetReleasedStateFromEEPROM();

void SavePressureInEEPROM(const float p0);
void SaveIDInEEPROM(const unsigned int id);
void SaveReleasedStateInEEPROM(const bool rState);

bool TryGetCommand(unsigned short int& flag);
bool CheckCommand(const String& command, unsigned short int& commandCode);
bool ExecuteCommand(CommandList& commands,const unsigned short int commandCode);
void RequestCommand();
unsigned short int CheckAutoCommands(const float& height, const bool released);
void Report(const unsigned short int commandCode, const bool executionStatus);
String* CalculateHash(const String& msg);

#endif


