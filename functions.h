#ifndef __FUNCTIONS__
#include "commandlist.h"
int GetIDFromEEPROM();
float GetPressureFromEEPROM();
bool GetReleasedStateFromEEPROM();

void SavePressureInEEPROM(const float p0);
void SaveIDInEEPROM(const int id);
void SaveReleasedStateInEEPROM(const bool rState);


bool TryGetCommand(String& command);
bool CheckCommand(const String& command, int& commandCode);
bool ExecuteCommand(CommandList& commands,const int commandCode);
void RequestCommand();
short int CheckAutoCommands(const float& height, const bool released);
void Report(const int commandCode, const bool executionStatus);

#endif


