#pragma once
#include "action.h"

enum Command {
	RELEASE = 1,
  RELEASE_FORCE = 2,
	RESET = 4,
	BEEP_START = 8,
  BEEP_STOP = 16,
  TAKE_PHOTO = 32,
  SAVE_PRESSURE = 64
};

class ArrayCursor;

class Dictionary {
  friend class ArrayCursor;
private:
  
  struct Node {
    Command key;
    Action<void> value;
    Node* next;
    Node(Command k, Action<void> v, Node* n = nullptr) : key(k), value(v), next(n){}
  };
  Node* dictStart;
  Node& findItem(Command value);
public:
  Dictionary();
  Dictionary(Dictionary& other);
  ~Dictionary();
  ArrayCursor operator[](Command key);
  Dictionary& operator=(Dictionary& other);
  bool keyExists(Command key) const;
};
class ArrayCursor {
  friend class Dictionary;
  typedef Dictionary::Node Node;
private:
  Node* node;
  Command* key;
  Dictionary& caller;
  ArrayCursor(Dictionary& c, Node& node);
  ArrayCursor(Dictionary& c, Command key);
public:
  operator Action<void>();
  ArrayCursor& operator =(Action<void> v) {
    if (node == nullptr) {
      Node* curr = new Node(*key,v,caller.dictStart);
      caller.dictStart = curr;
    }
    else {
      node->value = v;
    }
    return *this;
  }
  Action<void>* operator->();
};


inline Dictionary::Node& Dictionary::findItem(Command key)
{
  Node* curr = dictStart;
  while (curr != nullptr) {
    if (curr->key == key)
      return *curr;
    curr = curr->next;
  }
  return *dictStart;
}


inline Dictionary::Dictionary()
{
  dictStart = nullptr;
}


inline Dictionary::Dictionary(Dictionary& other)
{
  Node* iter = other.dictStart;
  if (iter == nullptr) {
    dictStart = nullptr;
    return;
  }
  Node* prev = new Node(iter->key,iter->value);
  dictStart = prev;
  Node* curr;
  iter = iter->next;
  while (iter != nullptr) {
    curr = new Node(iter->key, iter->value);
    prev->next = curr;
    prev = curr;
    iter = iter->next;
  }
}


inline Dictionary::~Dictionary()
{
  Node* curr = dictStart;
  Node* next;
  while (curr != nullptr) {
    next = curr->next;
    delete curr;
    curr = next;
  }
}


inline ArrayCursor Dictionary::operator[](Command key)
{
  return keyExists(key) ? ArrayCursor(*this, findItem(key)) : ArrayCursor(*this, key);
}


inline Dictionary & Dictionary::operator=(Dictionary& other)
{
  if (&other == this) {
    return *this;
  }
  delete dictStart;
  Node* iter = other.dictStart;
  if (iter == nullptr) {
    dictStart = nullptr;
    return *this;
  }
  Node* prev = new Node(iter->key, iter->value);
  dictStart = prev;
  Node* curr;
  iter = iter->next;
  while (iter != nullptr) {
    curr = new Node(iter->key, iter->value);
    prev->next = curr;
    prev = curr;
    iter = iter->next;
  }
  return *this;
}


inline bool Dictionary::keyExists(Command key) const
{
  Node* node = dictStart;
  while (node != nullptr) {
    if (node->key == key) {
      return true;
    }
    node = node->next;
  }
  return false;
}


inline ArrayCursor::ArrayCursor(Dictionary& c, Node& node)
  : caller(c), node(&node),key(nullptr)
{
}


inline ArrayCursor::ArrayCursor(Dictionary& c, Command key) 
  : caller(c),key(new Command(key)), node(nullptr)
{
}


inline ArrayCursor::operator Action<void>()
{
  return node->value;
}

inline Action<void> * ArrayCursor::operator->()
{
  return !node ? (Action<void>*)node : &node->value;
}


class CommandList
{
private:
	Dictionary list;
	bool commandSet(Command) const;
public:
	CommandList();
	CommandList(Dictionary);
	CommandList(CommandList&);
	~CommandList();
	void Execute(Command);
	bool AllCommandsSet() const;
	void AddCommand(Command, Action<void>);
    void ExecuteCommands(const unsigned short int);
};

Command GetCommandByID(short int);

