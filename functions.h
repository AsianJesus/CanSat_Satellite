#ifndef __FUNCTIONS__
#include "commandlist.h"

#define RELEASE_HEIGHT 400
#define BUZZER_HEIGHT 10
#define EEPROM_ADDRESS_P0 0x10
#define EEPROM_ADDRESS_PID 0x20
#define EEPROM_ADDRESS_RSTATE 0x30

unsigned int GetIDFromEEPROM();
float GetPressureFromEEPROM();
bool GetReleasedStateFromEEPROM();

void SavePressureInEEPROM(const float p0);
void SaveIDInEEPROM(const unsigned int id);
void SaveReleasedStateInEEPROM(const bool rState);

bool TryGetCommand(unsigned short int& flag);
bool CheckCommand(const String& command, unsigned short int& commandCode);
void ExecuteCommand(CommandList& commands,const unsigned short int commandCode);
void RequestCommand();
unsigned short int CheckAutoCommands(const float& height, const bool released);
void Report(const unsigned short int commandCode);
bool CheckHash(const String& msg,const String& hash);

#endif


