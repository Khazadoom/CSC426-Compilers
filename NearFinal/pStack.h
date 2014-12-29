#ifndef _pstack
#define _pstack
#include <string>
#include "scanner.h"

using namespace std;

class StackCell
{
public:
	int Type;
	int SubType;
	string Lexeme;
	StackCell *next;
	VarCell *Variable;


};

class PStackClass
{
public:
	PStackClass();
	void push(TokenClass TheToken);
	void pop();
	bool isThereATerminalOnTop();
	int returnTopMostTerminal();
	int getTopType();
	int getSubType();
	string getLexeme();
	void setPointer(VarCell *Var);
	VarCell * getPointer();
private:
	StackCell *top;

};

#endif