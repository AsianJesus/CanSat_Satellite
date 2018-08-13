#pragma once
#include "dictionary.h"
#include "action.h"

enum Command {
	RELEASE 1,
	RESET 2,
	BEEP 4,
  TAKE_PHOTO 8,
  SAVE_PRESSURE 16
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
  void ExecuteCommands(unsigned short int);
};

Command GetCommandByID(short int);

