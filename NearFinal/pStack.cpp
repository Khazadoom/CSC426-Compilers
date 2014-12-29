#include "stdafx.h"
#include "pstack.h"
#include "SCANNER.h"


using namespace std;


PStackClass::PStackClass(){
	top = NULL;
}

string PStackClass::getLexeme(){
	return top->Lexeme;
}


int PStackClass::getTopType()
{
	StackCell *tempCell = new StackCell;
	tempCell = top;
	int x;
	x = tempCell->Type;
	return x;
}

VarCell * PStackClass::getPointer()
{
	StackCell *tempCell = new StackCell;
	tempCell = top;
	return tempCell->Variable;
}

int PStackClass::getSubType()
{
	StackCell *TempCell = new StackCell;
	TempCell = top;
	int x;
	x = TempCell->SubType;
	return x;
}

void PStackClass::setPointer(VarCell *Var){
	top->Variable = Var;
}

void PStackClass::push(TokenClass TheToken)
{
	StackCell *tempCell = new StackCell;
	tempCell->Lexeme = TheToken.getLexeme();
	tempCell->next = top;
	top = tempCell;
	tempCell->Type = TheToken.getType();
	tempCell->SubType = TheToken.getSubType();
}

void PStackClass::pop()
{
	StackCell *tempCell = new StackCell;
	tempCell = top;
	top = top->next;
	delete tempCell;
}
bool PStackClass::isThereATerminalOnTop()
{
	if (top != NULL){
		if (top->Type != E_T)
		{
			return true;
		}
	}
	return false;
}

int PStackClass::returnTopMostTerminal()
{
	StackCell *tempCell = new StackCell;
	tempCell = top;
	while (tempCell != NULL)
	{
		if (tempCell->Type != E_T)
		{
			return tempCell->Type;
		}
		tempCell = tempCell->next;
	}
}