#include "StdAfx.h"
#include "Table.h"
#include <string>
#include <iostream>
#include "GENERAL.H"
using namespace std;

SymbolTable::SymbolTable(){
	Top = NULL;
}

int SymbolTable::getScope(){
	return Top->nest;
}

void SymbolTable::SetOffset(string FunctionName, string LocName)
{
	VarCell *Edit = ScopeLookup(FunctionName,Top);
	Edit->offset = atoi(LocName.c_str());
}

void SymbolTable::DeleteTemps(){
	VarCell *Prev = Top->Vars;
	VarCell *Search;
	//cout << "Starting Delete Temps" << endl;
	if (Prev != NULL)
	{
		while (Prev->next != NULL)
		{
			//cout << "This is Prev: " << Prev->Lexeme << endl;
			Search = Prev->next;
			if (Search->Lexeme[0] == '$' && Prev->Lexeme[1] != 's')
			{
				VarCell *Temp = Search;
				//DumpTable();
				//cout << "Deleting " << Temp->Lexeme << endl;
				Prev->next = Search->next;
				delete Temp;
				Top->nextOffset--;
			}
			else
			{
				Prev = Prev->next;
			}
		}
		Prev = Top->Vars;
		if (Prev->Lexeme[0] == '$'&& Prev->Lexeme[1] != 's')
		{
			Top->Vars = Prev->next;
			delete Prev;
			Top->nextOffset--;
		}
	}
}

void SymbolTable::DeleteSpecTemps()
{
	VarCell *Prev = Top->Vars;
	VarCell *Search;
	//cout << "Starting Delete Temps" << endl;
	if (Prev != NULL)
	{
		while (Prev->next != NULL)
		{
			//cout << "This is Prev: " << Prev->Lexeme << endl;
			Search = Prev->next;
			if (Search->Lexeme[0] == '$' && Prev->Lexeme[1] == 's')
			{
				VarCell *Temp = Search;
				//DumpTable();
				//cout << "Deleting " << Temp->Lexeme << endl;
				Prev->next = Search->next;
				delete Temp;
				Top->nextOffset--;
			}
			else
			{
				Prev = Prev->next;
			}
		}
		Prev = Top->Vars;
		if (Prev->Lexeme[0] == '$'&& Prev->Lexeme[1] == 's')
		{
			Top->Vars = Prev->next;
			delete Prev;
			Top->nextOffset--;
		}
	}

}

void SymbolTable::Organize(VarCell *Identifier, ScopeCell *Scope){
	VarCell *Prev = Scope->Vars;
	if (Prev != Identifier){
		while (Prev->next != Identifier)
		{
			Prev = Prev->next;
		}
		Prev->next = Identifier->next;
		Identifier->next = Scope->Vars;
		Scope->Vars = Identifier;
	}
}

int SymbolTable::ParamNum(string Lookup){
	VarCell *Temp = ScopeLookup(Lookup, Top);
	if (Temp == NULL)
	{
		if (Top->next != NULL){
			Temp = ScopeLookup(Lookup, Top->next);
			if (Temp == NULL)
			{
				return -1;
			}
		}
	}
	ParamCell *TempParam = Temp->Params;
	int ParamCount = 0;
	while (TempParam != NULL)
	{
		TempParam = TempParam->next;
		ParamCount++;
	}
	return ParamCount;
}

void SymbolTable::DumpTable(){
	ScopeCell *Temp = Top;
	
	cout << "****************" << endl;
	while (Temp != NULL)
	{
		VarCell *TempVar = Temp->Vars;
		cout << "Name=" << Temp->ScopeName << " Nesting Level=" << Temp->nest << endl;
		while (TempVar != NULL)
		{
			cout << "Lexeme=" << TempVar->Lexeme;
			cout << " ,Type=" << TempVar->type;
			cout << " ,Kind=" << TempVar->kind;
			cout << " ,Offset=" << TempVar->offset;
			cout << " ,Nesting Level " << TempVar->nest << endl;
			if (TempVar->kind == FUNC_ID)
			{
				cout << "	Param-List:" << endl;
				ParamCell *ParamTemp = TempVar->Params;
				while (ParamTemp != NULL)
				{
					cout << "	lexeme=" << ParamTemp->Lexeme;
					cout << ", kind=" << ParamTemp->kind;
					cout << ", Type=" << ParamTemp-> type;
					cout << ", offset=" << ParamTemp->offset<<endl;
					ParamTemp = ParamTemp->next;
				}
			}
			TempVar = TempVar->next;

		}

		Temp = Temp->next;
		cout << endl;
	}
	cout << "******" << endl;
}

VarCell * SymbolTable::FullScopeLookup(string Lookup){
	ScopeCell *TempScope = Top;
	VarCell *Location = new VarCell;
	Location = NULL;
	while (Location == NULL)
	{ 
		if (TempScope != NULL){
			Location = ScopeLookup(Lookup, TempScope);
			TempScope = TempScope->next;
		}
		else
		{
			break;
		}
	}
	return Location;

}

bool SymbolTable::TableAddEntry(string Lookup, int Type, int Kind){
	VarCell *Temp = ScopeLookup(Lookup, Top);
	bool entered = false;

	
	if (Temp == NULL)
	{
		if (Kind == REF_PARAM || Kind == VALUE_PARAM)
		{
			VarCell * IteratorStart = Top->next->Vars;
			ParamCell *Iterator = IteratorStart->Params;
			ParamCell *TempParam = new ParamCell;
			TempParam->Lexeme = Lookup;
			TempParam->type = Type;
			TempParam->kind = Kind;
			TempParam->next = NULL;
			if (Iterator == NULL)
			{
				TempParam->offset = 0;
				IteratorStart->Params = TempParam;
			}
			else
			{
				while (Iterator->next != NULL)
				{
					Iterator = Iterator->next;
				}
				TempParam->offset = Iterator->offset++;
				Iterator->next = TempParam;
			}
			entered = true;
		}

		VarCell *TempVar = new VarCell;
		TempVar->Lexeme = Lookup;
		TempVar->type = Type;
		TempVar->kind = Kind;
		TempVar->offset = Top->nextOffset;
		TempVar->nest = Top->nest;
		TempVar->Params = NULL;
		TempVar->next = Top->Vars;
		Top->Vars = TempVar;
		entered = true;
		if (Kind != FUNC_ID)
		{
			Top->nextOffset++;
		}
	}
	return entered;

}

VarCell * SymbolTable::ScopeLookup(string Lookup, ScopeCell *Scope){
	VarCell *Temp = Scope->Vars;
	if (Temp != NULL)
	{
		string TempLex = Temp->Lexeme;
		while (_stricmp(Lookup.c_str(), TempLex.c_str()) != 0)
		{
			Temp = Temp->next;
			if (Temp == NULL)
			{
				break;
			}
			TempLex = Temp->Lexeme;
		}
		//Organize(Temp, Scope);

	}
	if (Temp != NULL){
		VarCell *Prev = Scope->Vars;
		if (Prev != Temp){
			while (Prev->next != Temp)
			{
				Prev = Prev->next;
			}
			Prev->next = Temp->next;
			Temp->next = Scope->Vars;
			Scope->Vars = Temp;
		}
	}



return Temp;
}

ScopeCell * SymbolTable::getTop(){
	return Top;
}


void SymbolTable::TableAddLevel(string Name){
	if (Top == NULL)
	{
		ScopeCell *Temp = new ScopeCell;
		Temp->nest = 0;
		Temp->next = NULL;
		Temp->nextOffset = 0;
		Temp->ScopeName = "Main";
		Temp->Vars = NULL;
		Top = Temp;
	}
	else
	{
		ScopeCell *Temp = new ScopeCell;
		int tempNest = Top->nest;
		tempNest++;
		Temp->nest = tempNest;
		Temp->next = Top;
		Temp->ScopeName = Top->ScopeName + "." + Name;
		Temp->Vars = NULL;
		Temp->nextOffset = 0;
		Top = Temp;
	}
}

void SymbolTable::TableDelLevel(){

	if(Top != NULL)
	{
		if (Top->nextOffset != 0)
		{
			while (Top->Vars != NULL)
			{
				VarCell *Temp = Top->Vars;
				Top->Vars = Temp->next;
				delete Temp;
			}
		}
		
		ScopeCell *Garbage = Top;
		Top = Top->next;
		delete Garbage;
	}
	

}
