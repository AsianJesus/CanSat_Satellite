#include "commandlist.h"

bool CommandList::commandSet(Command c) const
{
	return list.keyExists(c);
}

CommandList::CommandList() : list()
{

}

CommandList::CommandList(Dictionary dict) : list(dict)
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
	return commandSet(Command::RELEASE) && commandSet(Command::RESET) && commandSet(Command::BEEP_START) && commandSet(Command::BEEP_STOP) && commandSet(Command::TAKE_PHOTO) && commandSet(Command::SAVE_PRESSURE);
}

void CommandList::AddCommand(Command c, Action<void> act)
{
	list[c] = act;
}

void CommandList::ExecuteCommands(unsigned short int flag){
   if(flag == 0) return;
   if(Command::RELEASE & flag) ((Action<void>)list[Command::RELEASE])();
   if(Command::RESET & flag) ((Action<void>)list[Command::RESET])();
   if(Command::BEEP_START & flag) ((Action<void>)list[Command::BEEP_START])();
   if(Command::BEEP_STOP & flag) ((Action<void>)list[Command::BEEP_STOP])();
   if(Command::TAKE_PHOTO & flag) ((Action<void>)list[Command::TAKE_PHOTO])();
   if(Command::SAVE_PRESSURE & flag) ((Action<void>)list[Command::SAVE_PRESSURE])();
   if(Command::RELEASE_FORCE & flag) ((Action<void>)list[Command::RELEASE_FORCE])();
   if(Command::BEEP_ROUTINE & flag) ((Action<void>)list[Command::BEEP_ROUTINE])();
}

Command GetCommandByID(short int id){
   if(Command::RELEASE & id) return Command::RELEASE;
   if(Command::RESET & id) return Command::RESET;
   if(Command::BEEP_START & id) return Command::BEEP_START;
   if(Command::BEEP_STOP & id) return Command::BEEP_STOP;
   if(Command::TAKE_PHOTO & id) return Command::TAKE_PHOTO;
   if(Command::SAVE_PRESSURE & id) return Command::SAVE_PRESSURE;
   if(Command::RELEASE_FORCE & id) return Command::RELEASE_FORCE;
   if(Command::BEEP_ROUTINE & id) return Command::BEEP_ROUTINE;
}


