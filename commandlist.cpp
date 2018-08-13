#include "stdafx.h"
#include "commandlist.h"

bool CommandList::commandSet(Command c) const
{
	return list.keyExists(c);
}

CommandList::CommandList() : list()
{

}

CommandList::CommandList(Dictionary<Command, Action<void>> dict) : list(dict)
{
}

CommandList::CommandList(CommandList & other)
{
	list = other.list;
}

CommandList::~CommandList()
{
}

void CommandList::Execute(Command c)
{
	if (commandSet(c)) {
		list[c]->Execute();
	}
}

bool CommandList::AllCommandsSet() const
{
	return commandSet(Command::RELEASE) && commandSet(Command::RESET) && commandSet(Command::BEEP);
}

void CommandList::AddCommand(Command c, Action<void> act)
{
	list[c] = act;
}

void CommandList::ExecuteCommands(unsigned short int flag){
   if(flag == 0) return;
   if(Command::RELEASE & flag) list[Command::RELEASE]();
   if(Command::RESET & flag) list[Command::RESET]();
   if(Command::BEEP & flag) list[Command::BEEP]();
   if(Command::TAKE_PHOTO & flag) list[Command::TAKE_PHOTO]();
   if(Command::SAVE_PRESSURE & flag) list[Command::SAVE_PRESSURE]();
}

Command GetCommandByID(short int id){
   if(Command::RELEASE & id) return Command::RELEASE;
   if(Command::RESET & id) return Command::RESET;
   if(Command::BEEP & id) return Command::BEEP;
   if(Command::TAKE_PHOTO & id) return Command::TAKE_PHOTO;
   if(Command::SAVE_PRESSURE & id) return Command::SAVE_PRESSURE;
  
}


