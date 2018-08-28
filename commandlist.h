#pragma once
#include "dictionary.h"
#include "action.h"

enum Command {
	RELEASE 1,
	RESET 2,
	BEEP_START 4,
  BEEP_STOP 8,
  TAKE_PHOTO 16,
  SAVE_PRESSURE 32
};

class CommandList
{
private:
	Dictionary<Command, Action<void>> list;
	bool commandSet(Command) const;
public:
	CommandList();
	CommandList(Dictionary<Command, Action<void>>);
	CommandList(CommandList&);
	~CommandList();
	void Execute(Command);
	bool AllCommandsSet() const;
	void AddCommand(Command, Action<void>);
    void ExecuteCommands(const unsigned short int);
};

Command GetCommandByID(short int);

