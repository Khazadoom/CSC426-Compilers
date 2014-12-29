#ifndef _table
#define _table
#include <string>
using namespace std;

class ParamCell{
public:
	string Lexeme;
	int kind;
	int type;
	int offset;
	ParamCell *next;
};

class VarCell{
public:
	string Lexeme;
	int kind;
	int type;
	int offset;
	int nest;
	VarCell *next;
	ParamCell *Params;
};

class ScopeCell{
public:
	string ScopeName;
	int nest;
	int nextOffset;
	ScopeCell *next;
	VarCell *Vars;

};

class SymbolTable{
public: 
	SymbolTable();
	void DeleteSpecTemps();
	ScopeCell * getTop();
	void TableAddLevel(string Name);
	void TableDelLevel();
	bool TableAddEntry(string Lexeme, int Type, int Kind);
	VarCell * ScopeLookup(string Lexeme, ScopeCell *Scope);
	void DumpTable();
	VarCell * FullScopeLookup(string Lookup);
	int ParamNum(string Lookup);
	void Organize(VarCell *Identifier, ScopeCell *Scope);
	void DeleteTemps();
	int getScope();
	void SetOffset(string FunctionName,string LocName);
private:
	ScopeCell *Top;
};
#endif