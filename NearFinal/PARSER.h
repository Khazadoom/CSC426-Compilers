#ifndef _parser
#define _parser


#include "SCANNER.h"
#include "pStack.h"
class ParseClass
{
public:
	ParseClass();
	int prec(int TopTerm, int TokenType);
	int lastPopped(TokenClass Array[8]);
	bool TerminalPopped(TokenClass Array[8]);
	void ParseProgram();
	void Statement();
	int numLinesProcessed();
	void Program();
	void PrintInstructions(string command, VarCell *ptr1, string str1, VarCell *ptr2, string str2);
	int GlobalVarType;
	int GlobalParam;
	int AnswerType;
	int AnswerKind;
	string AnswerLexeme;
private:
	string ParamLex;
	string Labels();
	bool CallTypeCheck(int Type, VarCell *Function);
	VarCell *FuncTest;
	string SpecTempNames();
	string TempNames();
	fstream Outfile;
	ScannerClass scanner;
	PStackClass TheStack;
	TokenClass TheToken;
	void parse_expr();
	void expr_follow();
	void cout_follow();
	void StatementTail();
	void begin_follow();
	void Type();
	void Block();
	void Function();
	void Function_Follow();
	void Type_Follow();
	void BlockBegin_Follow();
	void FuncStart();
};

#endif