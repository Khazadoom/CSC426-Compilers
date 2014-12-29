//Brandon Wright, Written Sept. 2014
//This File will implement the Scanner class of the YASL Compiler.

#ifndef _scanner
#define _scanner


#include "filemngr.h"
#include <string.h>
#include "Table.h"



class TokenClass
{
public:
	TokenClass();
	void printToken();
	void setTokenValues(string InputLexeme, int FinalState, int SubTypeToken);
	int getType();
	int getSubType();
	string getLexeme();
private:
	int TokenType;
	int SubType;
	string Lexeme;


};

class ScannerClass
{
public:
	ScannerClass();
	TokenClass getToken(TokenClass TheToken);
	int getNextToken(TokenClass TheToken);
	void closeSourceProgram();
	int getNextState(int state, char ch);
	void pushBack();
	void printCurrentLine();
	int numLinesProcessed();
	bool IsDebugOn();

	void DeleteSpecTemps();
	int ParamNum(string Lookup);
	bool TableAddEntry(string Lookup, int Type, int Kind);
	VarCell * ScopeLookup(string Lookup, ScopeCell *Scope);
	VarCell * FullScopeLookup(string Lookup);
	void DumpTable();
	void TableAddLevel(string Name);
	void TableDelLevel();
	void DeleteTemps();
	int getScope();
	void setOffset(string FunctionName, string LocName);
	ScopeCell * getTop();
private:
	fileManagerClass theFile;
	string Lexeme;
	bool expressDebug;
	SymbolTable SymbolTable;


};

#endif
