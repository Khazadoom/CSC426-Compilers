#include "StdAfx.h"
#include "PARSER.h"
#include "pStack.h"
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

ParseClass::ParseClass()
{
	Outfile.open("out.pal", ios::out);
}

void ParseClass::Program()
{
	if (TheToken.getType() == PRGRM_T)
	{
		TheToken = scanner.getToken(TheToken);
		if (TheToken.getType() == IDENT_T)
		{
			scanner.TableAddLevel(TheToken.getLexeme());
			TheToken = scanner.getToken(TheToken);
			if (TheToken.getType() == SEMICOLON_T)
			{
				//output assembly code for main
				Outfile << "$junk #1" << endl;
				Outfile << "$main movw SP R0" << endl;

				TheToken = scanner.getToken(TheToken);
				Block();
				scanner.TableDelLevel();
				if (TheToken.getType() != PERIOD_T)
				{
					string x = TheToken.getLexeme();
					cout << "Expecting [.] but found < ";
					cout << x;
					cout << " > instead"<< endl;
					scanner.printCurrentLine();
					cin.get();
					exit(1);
				}
				//File is ended properly output assembly
				Outfile << "inb $junk" << endl;
				Outfile << "end" << endl;
			}
		}
	}
}

void ParseClass::FuncStart(){
	Function();
}

void ParseClass::Block()
{
	int ScopeLevel = scanner.getScope();
	string JumpFuncDec = TempNames();

	if (TheToken.getType() == INT_T || TheToken.getType() == BOOLEAN_T)
	{
		GlobalParam = 0;
		GlobalVarType = TheToken.getType();
		switch (GlobalVarType){
		case INT_T:
			GlobalVarType = INT_TYPE;
			break;
		case BOOLEAN_T:
			GlobalVarType = BOOL_TYPE;
			break;
		}
		

		TheToken = scanner.getToken(TheToken);
		Type();
	}
	if (ScopeLevel == 0)
		{
			//PrintInstructions("Hello", NULL, "", NULL, "");
			PrintInstructions("jmp", NULL, JumpFuncDec, NULL, "");
		}
	if (TheToken.getType() == FUNCTION_T)
	{
		TheToken = scanner.getToken(TheToken);
		FuncStart();
	}
	if (TheToken.getType() == BEGIN_T)
	{
		if (ScopeLevel == 0)
		{
			PrintInstructions(JumpFuncDec, NULL, "movw r0", NULL, "r0");
		}

		TheToken = scanner.getToken(TheToken);
		BlockBegin_Follow();
		
		return;
	}
	else
	{
		if (TheToken.getType() != PERIOD_T)
		{
		cout << "Expected [BEGIN], [FUNCTION] or INT/BOOLEAN declaration" << endl;
		scanner.printCurrentLine();
		cin.get();
		exit(1);
		}
	}
	//TheToken = scanner.getToken(TheToken);
}

void ParseClass::Type()
{
	Type_Follow();
	if (TheToken.getType() == SEMICOLON_T)
	{
		GlobalParam = 0;
		TheToken = scanner.getToken(TheToken);
		if (TheToken.getType() == INT_T || TheToken.getType() == BOOLEAN_T)
		{
			GlobalVarType = TheToken.getType();
			switch (GlobalVarType){
			case INT_T:
				GlobalVarType = INT_TYPE;
				break;
			case BOOLEAN_T:
				GlobalVarType = BOOL_TYPE;
				break;
			}
			TheToken = scanner.getToken(TheToken);
			Type();
		}

		string ParamNum = "#" + to_string(GlobalParam * 4);
		PrintInstructions("addw", NULL, ParamNum, NULL, "sp");

	}
	else
	{
		string x = TheToken.getLexeme();
		cout << "Expected a [;] but found < ";
		cout << x;
		cout << " > instead" << endl;
		scanner.printCurrentLine();
		cin.get();
		exit(1);
	}
}

//Parsed the keyword Function
void ParseClass::Function()
{ 
	GlobalParam = 0;
	//must be followed by an Identifier
	if (TheToken.getType() == IDENT_T)
	{

		//creating a Callable Variable in current Level
		if (scanner.TableAddEntry(TheToken.getLexeme(), FUNC_ID_TYPE, FUNC_ID) == false){
			cout << "Duplicate Identifier Found:" << TheToken.getLexeme() << endl;
			scanner.printCurrentLine();
			cin.get();
			exit(1);
		}

		string LocName = TempNames();
		LocName = LocName.erase(0,1);
		scanner.setOffset(TheToken.getLexeme(), LocName);
		PrintInstructions("$" + LocName, NULL, "movw r0", NULL, "r0");

		//adding level to the Table
		scanner.TableAddLevel(TheToken.getLexeme());

		//now we look at what follows the Identifier
		TheToken = scanner.getToken(TheToken);
		if (TheToken.getType() == LEFTPARENTH_T)  // a Left Parenthesis
		{
			TheToken = scanner.getToken(TheToken);
			Function_Follow(); //calls Function_follow to determine how many variables and what types
			if (TheToken.getType() == RIGHTPARENTH_T)
			{
				TheToken = scanner.getToken(TheToken);
			}
			int Tick = 1;
			string APLOCSTR;
			string FPLocStr;
			int APLOC = (GlobalParam-Tick) * 4;
			while (Tick <= GlobalParam)
			{
				FPLocStr = "+" + to_string((Tick - 1) * 4) + "@fp";
				APLOC = (GlobalParam-Tick)*4;
				APLOCSTR = "-" + to_string(APLOC) + "@ap";
				PrintInstructions("movw", NULL, APLOCSTR, NULL, FPLocStr);
				PrintInstructions("addw", NULL, "#4", NULL, "sp");
				Tick++;
			}




		}
		if (TheToken.getType() == SEMICOLON_T) //or a Semicolon
		{
			TheToken = scanner.getToken(TheToken);
			Block();// a bit of recursion since a function needs it's own block. which puts a block in a block.
			//scanner.TableDelLevel();
			if (TheToken.getType() == SEMICOLON_T)//has to end with a semi colon
			{
				//scanner.printCurrentLine();
				//checking to see if there is another function
				TheToken = scanner.getToken(TheToken);
				if (TheToken.getType() == FUNCTION_T)
				{
					
					TheToken = scanner.getToken(TheToken);
					FuncStart();
				}
			}
		}
		 else
		 {
			 string x = TheToken.getLexeme();
			 cout << "ERROR: Expected a [;] or [(] but found < ";
			 cout << x;
			 cout << " > instead" << endl;
			 scanner.printCurrentLine();
			 cin.get();
			 exit(1);
		 }
	}
	else
	{
		string x = TheToken.getLexeme();
		cout << "ERROR: Expected an Identifier but found < ";
		cout << x;
		cout << " > instead";
		scanner.printCurrentLine();
		cin.get();
		exit(1);
	}
}
void ParseClass::Type_Follow()
{
	if (TheToken.getType() == IDENT_T)
	{
		GlobalParam++;
		bool entered = scanner.TableAddEntry(TheToken.getLexeme(), GlobalVarType, VAR_ID);

		if (entered == false)
		{
			cout << "Duplicate Identifier Found:" << TheToken.getLexeme() << endl;
			scanner.printCurrentLine();
			cin.get();
			exit(1);
		}

		PrintInstructions("addw",NULL,"#4",NULL,"SP");

		TheToken = scanner.getToken(TheToken);
		if (TheToken.getType() == COMMA_T)
		{
			TheToken = scanner.getToken(TheToken);
			Type_Follow();
		}
	}
	else if (TheToken.getType() != IDENT_T)
	{
		string x = TheToken.getLexeme();
		cout << "ERROR: Expected an Identifier but found < ";
		cout << x;
		cout << " > instead" << endl;
		scanner.printCurrentLine();
		cin.get();
		exit(1);
	}
}

void ParseClass::Function_Follow()
{
	if (TheToken.getType() == INT_T || TheToken.getType() == BOOLEAN_T)
	{

		int VarType = TheToken.getType();//Determining what type of Parameter will be here
		switch (VarType){				//
		case INT_T:						//
			VarType = INT_TYPE;			//
			break;						//
		case BOOLEAN_T:					//
			VarType = BOOL_TYPE;		//
			break;						//
		}

		TheToken = scanner.getToken(TheToken);
		if (TheToken.getType() == AMPERSAND_T)
		{
			GlobalParam++;
			TheToken = scanner.getToken(TheToken);
			if (TheToken.getType() == IDENT_T)
			{
				//Adding Reference Parameter to Scope
				if (scanner.TableAddEntry(TheToken.getLexeme(), VarType, REF_PARAM) == false)
				{
					cout << "Duplicate Identifier Found:" << TheToken.getLexeme() << endl;
					scanner.printCurrentLine();
					cin.get();
					exit(1);
				}
				
				TheToken = scanner.getToken(TheToken);
				if (TheToken.getType() == COMMA_T)
				{
					TheToken = scanner.getToken(TheToken);
					Function_Follow();
				}
			}
		}
		else if (TheToken.getType() == IDENT_T)
		{
			GlobalParam++;
			//adding Value parameter to Scope
			if (scanner.TableAddEntry(TheToken.getLexeme(), VarType, VALUE_PARAM) == false)
			{
				cout << "Duplicate Identifier Found:" << TheToken.getLexeme() << endl;
				scanner.printCurrentLine();
				cin.get();
				exit(1);
			}
			
			TheToken = scanner.getToken(TheToken);
			if (TheToken.getType() == COMMA_T)
			{
				TheToken = scanner.getToken(TheToken);
				Function_Follow();
			}
		}
		else
		{
			string x = TheToken.getLexeme();
			cout << "ERROR: Expected an Identifier or & but found < ";
			cout << x;
			cout << " > instead" << endl;
			scanner.printCurrentLine();
			cin.get();
			exit(1);
		}

	}

}

void ParseClass::ParseProgram()
{
	TheToken = scanner.getToken(TheToken);

	Program();
	Outfile.close();

}

void ParseClass::cout_follow()
{
	if (TheToken.getType() == DOUBLELESSER_T)
	{
		TheToken = scanner.getToken(TheToken);
		if (TheToken.getType() == STRINGCONST_T || TheToken.getType() == ENDL_T)
		{
			if (TheToken.getType() == STRINGCONST_T)
			{
				string LexConst = TheToken.getLexeme();
				for (int i = 1; i < LexConst.length() - 1; i++)
				{
					if (iswspace(LexConst[i]))
					{
						Outfile << "outb #32" << endl;
					}
					else if (!iswspace(LexConst[i]))
					{
						Outfile << "outb ^" << LexConst[i] << endl;
					}
				}
			}
			else if (TheToken.getType() == ENDL_T)
			{
				//assembly for Carriage Return
				Outfile << "outb #10" << endl;
			}

			TheToken = scanner.getToken(TheToken);
			cout_follow();
		}
		else if (TheToken.getType() == END_T || TheToken.getType() == SEMICOLON_T)
		{
			string x = TheToken.getLexeme();
			cout << "ERROR: Expected something to output but found < ";
			cout << x;
			cout << " > instead";
			cin.get();
			exit(1);
		}
		else
		{
			parse_expr();
			PrintInstructions("outw",NULL, "-4@sp", NULL, "");
			PrintInstructions("subw", NULL, "#4", NULL, "sp");
			cout_follow();
		}
	}
	else if (TheToken.getType() == SEMICOLON_T || TheToken.getType() == END_T || TheToken.getType() == ELSE_T)
	{
		return;
	}
	else if (TheToken.getType() != DOUBLELESSER_T)
	{
		string x = TheToken.getLexeme();
		cout << "ERROR: Expected << but found < ";
		cout << x;
		cout << " > instead" << endl;
		scanner.printCurrentLine();
		cin.get();
		exit(1);
	}
}

void ParseClass::expr_follow()
{
	
	if (TheToken.getType() == COMMA_T)
	{
		TheToken = scanner.getToken(TheToken);
		parse_expr();
		GlobalParam++;
		scanner.TableAddEntry(SpecTempNames(), AnswerType,VAR_ID );
		CallTypeCheck(AnswerType, FuncTest);
		expr_follow();
	}
}

void ParseClass::StatementTail()
{
	if (TheToken.getType() == END_T)
	{
		TheToken = scanner.getToken(TheToken);
		return;
	}
	if (TheToken.getType() != END_T){
		//TheToken = scanner.getToken(TheToken);
		if (TheToken.getType() == SEMICOLON_T)
		{
			TheToken = scanner.getToken(TheToken);
			if (TheToken.getType() == END_T)
			{
				cout << "ERROR: END not expected" << endl;
				scanner.printCurrentLine();
				cin.get();
				exit(1);
			}
			Statement();
			StatementTail();
		}
	}
}

void ParseClass::BlockBegin_Follow()
{
	if (TheToken.getType() != END_T)
	{
		Statement();
		StatementTail();
	}
	else if (TheToken.getType() == END_T)
	{
		TheToken = scanner.getToken(TheToken);
	}
	int ScopeLevel = scanner.getScope();
	if (ScopeLevel != 0)
	{
		PrintInstructions("movw", NULL, "fp", NULL, "sp");
		PrintInstructions("ret", NULL, "", NULL, "");
	}
	scanner.TableDelLevel();
	return;

}

void ParseClass::begin_follow()
{
	if (TheToken.getType() != END_T)
	{
		Statement();
		StatementTail();
	}
	else if (TheToken.getType() == END_T)
	{
		TheToken = scanner.getToken(TheToken);
	}

}

void ParseClass::PrintInstructions(string command, VarCell *ptr1, string str1, VarCell *ptr2, string str2)
{
	int Offset1 = -1;
	int Offset2 = -1;

	if (ptr1 != NULL){
		Offset1 = ptr1->offset * 4;
	}
	if (ptr2 != NULL){
		Offset2 = ptr2->offset *4;
	}
	Outfile << command << " ";



	if (Offset1 != -1)
	{
		if (ptr1->nest == 0){
			Outfile << "+" << Offset1;
			if (ptr1->kind == REF_PARAM)
			{
				Outfile << "@";
			}
			Outfile << "@R0 ";
		}
		else{
			Outfile << "+" << Offset1;
			if (ptr1->kind == REF_PARAM)
			{
			Outfile << "@";
			}
			Outfile << "@FP ";
		}
	}
	else
	{
		Outfile << str1 << " ";
	}
	if (Offset2 != -1)
	{
		if (ptr2->nest == 0){
			Outfile << "+" << Offset2;
			if (ptr2->kind == REF_PARAM)
			{
				Outfile << "@";
			}
			Outfile << "@R0 ";
		}
		else{
			Outfile << "+" << Offset2;
			if (ptr2->kind == REF_PARAM)
			{
			Outfile << "@";
			}
			Outfile << "@FP ";
		}
	}
	else
	{
		Outfile << str2;
	}
	Outfile << endl;
	

}

string ParseClass::SpecTempNames(){
	stringstream ss;
	static int nextCt = 0;
	ss << "$" << "s" <<nextCt++;
	return ss.str();

}

void ParseClass::Statement()
{

	if (TheToken.getType() == WHILE_T)
	{
		//create top of loop and bottom names and start the loop in A/L with dummy code
		string LoopTop = TempNames();
		string LoopAfter = TempNames();
		PrintInstructions(LoopTop, NULL, "movw r0", NULL, "r0");
		//PrintInstructions("////////////////WHILETOP", NULL, "", NULL, "");
		
		TheToken = scanner.getToken(TheToken);
		if (TheToken.getType() == DO_T)
		{
			cout << "ERROR: Expected an Expression after [While]" << endl;
			scanner.printCurrentLine();
			cin.get();
			exit(1);
		}

		parse_expr();
		if (AnswerType == INT_TYPE)
		{
			cout << "Must use a Boolean for the While Guard" << endl;
			scanner.printCurrentLine();
			cin.get();
			exit(1);
		}


		PrintInstructions("cmpw", NULL, "#0",NULL, "-4@sp");
		PrintInstructions("subw", NULL, "#4", NULL, "sp");
		PrintInstructions("beq", NULL, LoopAfter, NULL, "");

		if (TheToken.getType() == DO_T)
		{
			TheToken = scanner.getToken(TheToken);
			Statement();
		}
		else
		{
			cout << "ERROR: Expecting [DO]" << endl;
			scanner.printCurrentLine();
			cin.get();
			exit(1);
		}
		PrintInstructions("jmp", NULL, LoopTop, NULL, "");
		PrintInstructions(LoopAfter, NULL, "movw r0", NULL, "r0");
		//PrintInstructions("//////////////////WHILE AFTER", NULL, "", NULL, "");
	}
	else if (TheToken.getType() == IDENT_T)
	{
	  //Placeholder for FullLookup
		VarCell *Test = scanner.FullScopeLookup(TheToken.getLexeme());
		int Parameters = 0;
		bool hasParam = false;
		if(Test == NULL)
		{
			cout << "Undeclared Variable:" << TheToken.getLexeme() << endl;
			scanner.printCurrentLine();
			cin.get();
			exit(1);
		}
		if (Test->kind ==FUNC_ID)
		{
			Parameters = scanner.ParamNum(TheToken.getLexeme());
			if (Parameters == -1)
			{
				cout << "Undeclared Variable: " << TheToken.getLexeme() << endl;
				scanner.printCurrentLine();
				cin.get();
				exit(1);
			}


			if (Parameters != 0)
			{
				hasParam = true;
			}
		}
		
		//cout << TheToken.getLexeme() << "WHOO MORE SHIT" << endl;

		TheToken = scanner.getToken(TheToken);
		if (TheToken.getType() == SETEQUAL_T)
		{
			if (Test->kind == FUNC_ID)
			{
				cout << "Cannot set a Function call equal to something" << endl;
				scanner.printCurrentLine();
				cin.get();
				exit(1);
			}

			TheToken = scanner.getToken(TheToken);
			parse_expr();
			if (Test->type != AnswerType)
			{
				if (Test->type == INT_TYPE)
				{
					cout << "Cannot Assign a Boolean to an Integer" << endl;
				}
				else
				{
					cout << "Cannot Assign an Integer to a Boolean" << endl;
				}
				scanner.printCurrentLine();
				cin.get();
				exit(1);
			}
			PrintInstructions("movw", NULL, "-4@sp", Test, "");
			PrintInstructions("movw", NULL, "r1", NULL, "sp");
		}
		else if (TheToken.getType() == LEFTPARENTH_T)
		{
			TheToken = scanner.getToken(TheToken);
			if (TheToken.getType() == RIGHTPARENTH_T)
			{
				cout << "ERROR: Expected Parameters" << endl;
				scanner.printCurrentLine();
				cin.get();
				exit(1);
			}
			FuncTest = Test;
			GlobalParam = 0;
			parse_expr();
			GlobalParam++;
			//This is very test.
			//such test
			//wow
			//much prototype
			string tempname = SpecTempNames();
			scanner.TableAddEntry(tempname,AnswerType,VAR_ID);
			CallTypeCheck(AnswerType, FuncTest);
			expr_follow();
			if (TheToken.getType() == RIGHTPARENTH_T)
			{
				if (GlobalParam != Parameters)
				{
					cout << "Incorrect Number of Parameters: " << Test->Lexeme << endl;
					scanner.printCurrentLine();
					cin.get();
					exit(1);
				}
				TheToken = scanner.getToken(TheToken);
				string LocName = "$" + to_string(Test->offset);
				string ParamString;
				ParamString = "#" + to_string(Parameters);

				PrintInstructions("call", NULL, ParamString, NULL, LocName);
			}
		}
		else if (TheToken.getType() == SWAP_T)
		{
			TheToken = scanner.getToken(TheToken);
			VarCell *Other = scanner.FullScopeLookup(TheToken.getLexeme());

			if (Test->kind != Other->kind)
			{
				cout << "Cannot Swap Variables with different types" << endl;
				scanner.printCurrentLine();
				cin.get();
				exit(1);
			}
			if (Other == NULL)
			{
				cout << "Undeclared Identifier: "<< TheToken.getLexeme() << endl;
				scanner.printCurrentLine();
				cin.get();
				exit(1);
			}
			if (TheToken.getType() != IDENT_T)
			{
				cout << "ERROR: Was Expecting an Identifier" << endl;
				scanner.printCurrentLine();
				cin.get();
				exit(1);
			}
			//string TempNameLex = TempNames();
			//scanner.TableAddEntry(TempNameLex, INT_TYPE, VAR_ID);
			PrintInstructions("addw", NULL, "#4", NULL, "sp");
			PrintInstructions("movw", Test, "", NULL, "-4@sp");
			PrintInstructions("movw", Other, "", Test, "");
			PrintInstructions("movw", NULL, "-4@sp", Other, "");
			PrintInstructions("subw", NULL, "#4", NULL, "sp");

			TheToken = scanner.getToken(TheToken);
		}
		else if (hasParam == true){
			cout << "Incorrect Number of Parameters: " << Test->Lexeme << endl;
			scanner.printCurrentLine();
			cin.get();
			exit(1);
		}
		else if (TheToken.getType() == SEMICOLON_T || TheToken.getType() == END_T)
		{
			string LocName = "$" + to_string(Test->offset);
			PrintInstructions("call", NULL, "#0", NULL, LocName);
			//TheToken = scanner.getToken(TheToken);
		}

		else if (TheToken.getType() != SEMICOLON_T)
		{
			string x = TheToken.getLexeme();
			cout << "ERROR: Expecting =, ><, ( or ; but found < ";
			cout << x;
			cout << " > instead" << endl;
			scanner.printCurrentLine();
			cin.get();
			exit(1);
		}
		
	}
	else if (TheToken.getType() == CIN_T)
	{
		TheToken = scanner.getToken(TheToken);
		if (TheToken.getType() == DOUBLEGREATER_T)
		{
			TheToken = scanner.getToken(TheToken);
			//placeholder for Fulllookup
			VarCell *TempCIN = scanner.FullScopeLookup(TheToken.getLexeme());
			if (TempCIN == NULL)
			{
				cout << "Undeclared Variable:" << TheToken.getLexeme() << endl;
				scanner.printCurrentLine();
				cin.get();
				exit(1);
			}
			if (TempCIN->type == BOOL_TYPE)
			{
				cout << "Cannot input a Boolean" << endl;
				scanner.printCurrentLine();
				cin.get();
				exit(1);
			}

			PrintInstructions("inw", TempCIN, "", NULL, "");

			//cout << TheToken.getLexeme() << " - OH LOOKY AN INPUT" << endl;

			if (TheToken.getType() != IDENT_T)
			{
				cout << "ERROR: Was Expecting an Identifier" << endl;
				scanner.printCurrentLine();
				cin.get();
				exit(1);
			}
			TheToken = scanner.getToken(TheToken);
		}
		else if (TheToken.getType() == END_T || TheToken.getType() == SEMICOLON_T)
		{
			Outfile << "inb $junk" << endl;
		}

		else if (TheToken.getType() != DOUBLEGREATER_T)
		{
			string x = TheToken.getLexeme();
			cout << "ERROR: Expecting >> but found < ";
			cout << x;
			cout << " > instead" << endl;
			scanner.printCurrentLine();
			cin.get();
			exit(1);
		}
	}
	else if (TheToken.getType() == COUT_T)
	{
		TheToken = scanner.getToken(TheToken);
		cout_follow();
	}
	else if (TheToken.getType() == IF_T)
	{
		TheToken = scanner.getToken(TheToken);
		parse_expr();

		if (AnswerType == INT_TYPE)
		{
			cout << "Cannot use an integer for an IF Statement" << endl;
			scanner.printCurrentLine();
			cin.get();
			exit(1);
		}


		if (TheToken.getType() == THEN_T)
		{
			string notIf = TempNames();
			
			PrintInstructions("cmpw", NULL, "#0", NULL, "-4@sp");
			PrintInstructions("subw", NULL, "#4", NULL, "sp");
			PrintInstructions("beq", NULL, notIf, NULL, "");
			TheToken = scanner.getToken(TheToken);
			Statement();
			//TheToken = scanner.getToken(TheToken);
			if (TheToken.getType() == ELSE_T)
			{   
				TheToken = scanner.getToken(TheToken);
				string isIf = TempNames();
				PrintInstructions("jmp", NULL, isIf, NULL, "");
				PrintInstructions(notIf, NULL, "movw r0", NULL, "r0");
				Statement();
				PrintInstructions(isIf, NULL, "movw r0", NULL, "r0");
			}
			else
			{
				PrintInstructions(notIf, NULL, "movw r0", NULL, "r0");
			}
		}
		else
		{
			string x = TheToken.getLexeme();
			cout << "ERROR: Expecting [Then] but found < ";
			cout << x;
			cout << " > instead";
			scanner.printCurrentLine();
			cin.get();
			exit(1);
		}
	}
	else if (TheToken.getType() == BEGIN_T)
	{
		TheToken = scanner.getToken(TheToken);
		begin_follow();
	}
	else
	{
		string x = TheToken.getLexeme();
		cout << "ERROR: Expecting [Identifier] or [While] but found < ";
		cout << x;
		cout << " > instead" << endl;
		scanner.printCurrentLine();
		cin.get();
		exit(1);
	}
}

int ParseClass::lastPopped(TokenClass Array[10])
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			cout << j << endl;
			Array[j].printToken();
		}

		if (Array[i].getType() == RELOP_T || Array[i].getType() == ADDOP_T || Array[i].getType() == MULOP_T || Array[i].getType() == LEFTPARENTH_T || Array[i].getType() == RIGHTPARENTH_T || Array[i].getType() == IDENT_T
			|| Array[i].getType() == INTEGER_T || Array[i].getType() == TRUE_T || Array[i].getType() == FALSE_T)
		{
			return Array[i].getType();
		}
	}
	return E_T;
}

bool ParseClass::TerminalPopped(TokenClass Array[])
{
	for (int i = 0; i < 7; i++)
	{
		if (Array[i].getType() == RELOP_T || Array[i].getType() == ADDOP_T || Array[i].getType() == MULOP_T || Array[i].getType() == LEFTPARENTH_T || Array[i].getType() == RIGHTPARENTH_T || Array[i].getType() == IDENT_T
			|| Array[i].getType() == INTEGER_T || Array[i].getType() == TRUE_T || Array[i].getType() == FALSE_T)
		{
			return true;
		}
	}
	return false;
}

int ParseClass::prec(int TopTerm, int TokenType)
{
	int col = 10;
	int row = 10;
	// creating table to deterimine whether to shift, push, or report an error
	//                   REL  ADD  MUL  (    )    ID   IC   TRU  FLS  ;   OTHER  
	int table[11][11] = {158, 157, 157, 157, 158, 157, 157, 157, 157, 158,160,//RELOP
						 158, 158, 157, 157, 158, 157, 157, 157, 157, 158,160,//ADDOP
						 158, 158, 158, 157, 158, 157, 157, 157, 157, 158,160,//MULOP
						 157, 157, 157, 157, 159, 157, 157, 157, 157, 157,160,// (
						 158, 158, 158, 160, 158, 160, 160, 160, 160, 158,160,// )
						 158, 158, 158, 160, 158, 160, 160, 160, 160, 158,160,// Identifi160
						 158, 158, 158, 160, 158, 160, 160, 160, 160, 158,160,// Integ160 Constant 
						 158, 158, 158, 160, 158, 160, 160, 160, 160, 158,160,// TRUE
						 158, 158, 158, 160, 158, 160, 160, 160, 160, 158,160,//FALSE
						 157, 157, 157, 157, 160, 157, 157, 157, 157, 160,160,// ; 
						 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,160};// OTHER
	switch (TopTerm){
	case RELOP_T:
	{
		row = 0;
		break;
	}
	case ADDOP_T:
		row = 1;
		break;
	case MULOP_T:
		row = 2;
		break;
	case LEFTPARENTH_T:
		row = 3;
		break;
	case RIGHTPARENTH_T:
		row = 4;
		break;
	case IDENT_T:
		row = 5;
		break;
	case INTEGER_T:
		row = 6;
		break;
	case TRUE_T:
		row = 7;
		break;
	case FALSE_T:
		row = 8;
		break;
	case SEMICOLON_T:
	case DO_T:
	case THEN_T:
	case END_T:
	case COMMA_T:
	case DOUBLELESSER_T:
	case ELSE_T:
		row = 9;
		break;
	}


	switch (TokenType){
	case RELOP_T:
	{
		col = 0;
		break;
	}
	case ADDOP_T:
		col = 1;
		break;
	case MULOP_T:
		col = 2;
		break;
	case LEFTPARENTH_T:
		col = 3;
		break;
	case RIGHTPARENTH_T:
		col = 4;
		break;
	case IDENT_T:
		col = 5;
		break;
	case INTEGER_T:
		col = 6;
		break;
	case TRUE_T:
		col = 7;
		break;
	case FALSE_T:
		col = 8;
		break;
	case SEMICOLON_T:
	case DO_T:
	case THEN_T:
	case END_T:
	case COMMA_T:
	case ELSE_T:
	case DOUBLELESSER_T:
		col = 9;
		break;


	}
	return table[row][col];
}

void ParseClass::parse_expr()
{
	PrintInstructions("movw", NULL, "sp", NULL, "r1");
	TokenClass PoppedTerminals[8];
	string poppedvariables[8];
	TokenClass EToken;
	TokenClass SemiToken;
	EToken.setTokenValues("", E_T, NONE_ST);
	SemiToken.setTokenValues(";", SEMICOLON_T, NONE_ST);
	TheStack.push(SemiToken);
	//TheToken = scanner.getToken(TheToken);
	while (1) //IT GOES FOREVER
	{
		VarCell *Test = NULL;

		if (TheToken.getType() == IDENT_T)
		{
			//placeholder for Lookup Should be FullScope
			Test = scanner.FullScopeLookup(TheToken.getLexeme());
			if (Test == NULL)
			{
				cout << "Undeclared Variable:" << TheToken.getLexeme() <<endl;
				scanner.printCurrentLine();
				cin.get();
				exit(1);
			}
			//cout << TheToken.getLexeme() << " : WELL OKAY THEN" << endl;

		}


		for (int i = 0; i < 7; i++)
		{
			PoppedTerminals[i].setTokenValues("", 0, 0);
			poppedvariables[i] = "";
		}
		//cout << TheStack.returnTopMostTerminal() << endl;
		//if the top most terminal is a ; and the newest token is a ;, then return.
		if (TheStack.returnTopMostTerminal() == SEMICOLON_T && (TheToken.getType() == SEMICOLON_T|| TheToken.getType() == RIGHTPARENTH_T || TheToken.getType() == DO_T||TheToken.getType() == THEN_T || TheToken.getType() == END_T||TheToken.getType() == COMMA_T || TheToken.getType() == DOUBLELESSER_T || TheToken.getType() == ELSE_T))  // this kills the loop. 
		{
			VarCell *Answer = scanner.FullScopeLookup(AnswerLexeme);
			AnswerKind = Answer->kind;
			AnswerType = Answer->type;
			PrintInstructions("movw", Answer, "", NULL, "@r1");
			PrintInstructions("movw", NULL, "r1", NULL, "sp");
			PrintInstructions("addw", NULL, "#4", NULL, "sp");
			scanner.DeleteTemps();


			return;
		}
		else //if it ISN'T the end of the line.
		{
			int TopTerm; 

			TopTerm = TheStack.returnTopMostTerminal(); //assign the top most Terminal on the Stack to TopTerm
			if (prec(TopTerm, TheToken.getType()) == LT || prec(TopTerm, TheToken.getType()) == EQ) //This will shift the newest token onto the stack
			{
				TheStack.push(TheToken);
				if (Test != NULL)
				{
					TheStack.setPointer(Test);
				}
				TheToken = scanner.getToken(TheToken);
			}
			else if (prec(TopTerm, TheToken.getType()) == GT) //if we don't shift. WE REDUCE!
			{
				int x = -1;
				int LastPoppedTerm;
				do
				{
					x++;

					if (TheStack.getTopType() == IDENT_T || TheStack.getTopType() == E_T){
						poppedvariables[x] = TheStack.getPointer()->Lexeme;
					}
					else if (TheStack.getTopType() == INTEGER_T){
						poppedvariables[x] = TheStack.getLexeme();
					}

					PoppedTerminals[x].setTokenValues(TheStack.getLexeme(), TheStack.getTopType(), TheStack.getSubType());
					if (TheStack.getTopType() != E_T)
					{
						LastPoppedTerm = TheStack.getTopType();
					}
					TheStack.pop();
					TopTerm = TheStack.returnTopMostTerminal();
					//LastPoppedTerm = lastPopped(PoppedTerminals);
				} while ((!TerminalPopped(PoppedTerminals))||(!TheStack.isThereATerminalOnTop()) || (prec(TopTerm, LastPoppedTerm) != LT));
				
				switch (PoppedTerminals[0].getType())
				{
				//case for Identifiers, Integers, True and False
				case IDENT_T:	
				{
					string TempNameLex = TempNames();
					int TempNameType = scanner.FullScopeLookup(poppedvariables[0])->type;
					int TempNameKind = scanner.FullScopeLookup(poppedvariables[0])->kind;
					scanner.TableAddEntry(TempNameLex, TempNameType, VAR_ID);
					PrintInstructions("addw", NULL, "#4", NULL, "SP");
					PrintInstructions("movw", scanner.FullScopeLookup(poppedvariables[0]), "", scanner.FullScopeLookup(TempNameLex), "");
					if (scanner.IsDebugOn() == true)
					{
						cout << "E -> Identifier" << endl;
					}
					TheStack.push(EToken);
					ParamLex = poppedvariables[0];
					TheStack.setPointer(scanner.FullScopeLookup(poppedvariables[0]));
					AnswerLexeme = TempNameLex;
					break;
				}
				case INTEGER_T:
				{
					string TempNameLex = TempNames();
					scanner.TableAddEntry(TempNameLex, INT_TYPE, VAR_ID);
					PrintInstructions("addw", NULL, "#4", NULL, "SP");
					PrintInstructions("movw", NULL , "#" + poppedvariables[0], scanner.FullScopeLookup(TempNameLex), "");
					if (scanner.IsDebugOn() == true)
					{
						cout << "E -> Integer Constant" << endl;
					}
					TheStack.push(EToken);
					ParamLex = "False";
					TheStack.setPointer(scanner.FullScopeLookup(TempNameLex));
					AnswerLexeme = TempNameLex;
					break;
				}
				case TRUE_T:
				{
					string TempNameLex = TempNames();
					scanner.TableAddEntry(TempNameLex, BOOL_TYPE, VAR_ID);
					PrintInstructions("addw", NULL, "#4", NULL, "SP");
					PrintInstructions("movw", NULL, "#1", scanner.FullScopeLookup(TempNameLex), "");
					if (scanner.IsDebugOn() == true)
					{
						cout << "E -> True" << endl;
					}
					TheStack.push(EToken);
					ParamLex = "False";
					TheStack.setPointer(scanner.FullScopeLookup(TempNameLex));
					AnswerLexeme = TempNameLex;
					break;
				}
				case FALSE_T:
				{
					string TempNameLex = TempNames();
					scanner.TableAddEntry(TempNameLex, BOOL_TYPE, VAR_ID);
					PrintInstructions("addw", NULL, "#4", NULL, "SP");
					PrintInstructions("movw", NULL, "#0", scanner.FullScopeLookup(TempNameLex), "");
					if (scanner.IsDebugOn() == true)
					{
						cout << "E -> False" << endl;
					}
					TheStack.push(EToken);
					ParamLex = "False";
					TheStack.setPointer(scanner.FullScopeLookup(TempNameLex));
					AnswerLexeme = TempNameLex;
					break;
				}
					//Check for Right Parenth at beginning of array, Expression is backwards.  like )E( due to being pushed.
				case RIGHTPARENTH_T:
					if (PoppedTerminals[1].getType() == E_T){
						if (PoppedTerminals[2].getType() == LEFTPARENTH_T)
						{
							string TempNameLex = TempNames();
							ParamLex = "False";
							int TempNameType = scanner.FullScopeLookup(poppedvariables[1])->type;
							int TempNameKind = scanner.FullScopeLookup(poppedvariables[1])->kind;
							scanner.TableAddEntry(TempNameLex, TempNameType, TempNameKind);
							PrintInstructions("addw", NULL, "#4", NULL, "SP");
							PrintInstructions("movw", scanner.FullScopeLookup(poppedvariables[1]), "", scanner.FullScopeLookup(TempNameLex), "");
							AnswerLexeme = TempNameLex;
							if (scanner.IsDebugOn() == true)
							{
								cout << "E -> (E)" << endl;
							}
							TheStack.push(EToken);
							TheStack.setPointer(scanner.FullScopeLookup(poppedvariables[1]));
							break;
						}
					}
					cout << "Expected in Expression" << endl;
					cout << TheToken.getLexeme() << endl;
					scanner.printCurrentLine();
					cin.get();
					exit(1);
				case E_T:
					if (PoppedTerminals[1].getType() == RELOP_T || PoppedTerminals[1].getType() == ADDOP_T || PoppedTerminals[1].getType() == MULOP_T)
					{
						if (PoppedTerminals[2].getType() == E_T)
						{
							string TempNameLex = TempNames();
							string skip;
							string command;
							string TempLex2;
							ParamLex = "False";
							VarCell *First = scanner.FullScopeLookup(poppedvariables[2]);
							VarCell *Second = scanner.FullScopeLookup(poppedvariables[0]);
							switch (PoppedTerminals[1].getSubType())
							{
							case ASTERISK_ST:
								if (First->type == BOOL_TYPE || Second->type == BOOL_TYPE)
								{
									cout << "Invalid Types: Cannot Multiply Booleans" << endl;
									scanner.printCurrentLine();
									cin.get();
									exit(1);
								}

								scanner.TableAddEntry(TempNameLex, INT_TYPE, VAR_ID);
								PrintInstructions("addw", NULL, "#4", NULL, "SP");
								PrintInstructions("movw", scanner.FullScopeLookup(poppedvariables[2]), "", scanner.FullScopeLookup(TempNameLex), "");
								PrintInstructions("mulw", scanner.FullScopeLookup(poppedvariables[0]), "", scanner.FullScopeLookup(TempNameLex), "");
								AnswerLexeme = TempNameLex;
								break;
							case DIV_ST:
								if (First->type == BOOL_TYPE || Second->type == BOOL_TYPE)
								{
									cout << "Invalid Types: Cannot Divide Booleans" << endl;
									scanner.printCurrentLine();
									cin.get();
									exit(1);
								}

								scanner.TableAddEntry(TempNameLex, INT_TYPE, VAR_ID);
								PrintInstructions("addw", NULL, "#4", NULL, "SP");
								PrintInstructions("movw", scanner.FullScopeLookup(poppedvariables[2]), "", scanner.FullScopeLookup(TempNameLex), "");
								PrintInstructions("divw", scanner.FullScopeLookup(poppedvariables[0]), "", scanner.FullScopeLookup(TempNameLex), "");
								AnswerLexeme = TempNameLex;
								break;
							case MOD_ST:
								if (First->type == BOOL_TYPE || Second->type == BOOL_TYPE)
								{
									cout << "Invalid Types: Cannot Mod Booleans" << endl;
									scanner.printCurrentLine();
									cin.get();
									exit(1);
								}
								scanner.TableAddEntry(TempNameLex, INT_TYPE, VAR_ID);
								TempLex2 = TempNames();
								scanner.TableAddEntry(TempLex2, INT_TYPE, VAR_ID);
								PrintInstructions("addw", NULL, "#8", NULL, "SP");
								PrintInstructions("movw", scanner.FullScopeLookup(poppedvariables[2]), "", scanner.FullScopeLookup(TempLex2), "");
								PrintInstructions("movw", scanner.FullScopeLookup(poppedvariables[2]), "", scanner.FullScopeLookup(TempNameLex), "");
								PrintInstructions("divw", scanner.FullScopeLookup(poppedvariables[0]), "", scanner.FullScopeLookup(TempNameLex), "");
								PrintInstructions("mulw", scanner.FullScopeLookup(poppedvariables[0]), "", scanner.FullScopeLookup(TempNameLex), "");
								PrintInstructions("subw", scanner.FullScopeLookup(TempNameLex), "", scanner.FullScopeLookup(TempLex2), "");
								AnswerLexeme = TempLex2;
								break;
							case AND_ST:
								if (First->type == INT_TYPE || Second->type == INT_TYPE)
								{
									cout << "Invalid Types: Cannot perform and with Integers" << endl;
									scanner.printCurrentLine();
									cin.get();
									exit(1);
								}


								scanner.TableAddEntry(TempNameLex, BOOL_TYPE, VAR_ID);
								PrintInstructions("addw", NULL, "#4", NULL, "SP");
								PrintInstructions("movw", scanner.FullScopeLookup(poppedvariables[2]), "", scanner.FullScopeLookup(TempNameLex), "");
								PrintInstructions("mulw", scanner.FullScopeLookup(poppedvariables[0]), "", scanner.FullScopeLookup(TempNameLex), "");
								AnswerLexeme = TempNameLex;
								break;
							case ADDITION_ST:
							{
								if (First->type == BOOL_TYPE || Second->type == BOOL_TYPE)
							{
								cout << "Invalid Types: Cannot ADD Booleans" << endl;
								scanner.printCurrentLine();
								cin.get();
								exit(1);
							}
								scanner.TableAddEntry(TempNameLex, INT_TYPE, VAR_ID);
								PrintInstructions("addw", NULL, "#4", NULL, "SP");
								PrintInstructions("movw", scanner.FullScopeLookup(poppedvariables[2]), "", scanner.FullScopeLookup(TempNameLex), "");
								PrintInstructions("addw", scanner.FullScopeLookup(poppedvariables[0]), "", scanner.FullScopeLookup(TempNameLex), "");
								AnswerLexeme = TempNameLex; 
								break;
							}
							case SUBTRACT_ST:
								if (First->type == BOOL_TYPE || Second->type == BOOL_TYPE)
								{
									cout << "Invalid Types: Cannot Subtract Booleans" << endl;
									scanner.printCurrentLine();
									cin.get();
									exit(1);
								}
								scanner.TableAddEntry(TempNameLex, INT_TYPE, VAR_ID);
								PrintInstructions("addw", NULL, "#4", NULL, "SP");
								PrintInstructions("movw", scanner.FullScopeLookup(poppedvariables[2]), "", scanner.FullScopeLookup(TempNameLex), "");
								PrintInstructions("subw", scanner.FullScopeLookup(poppedvariables[0]), "", scanner.FullScopeLookup(TempNameLex), "");
								AnswerLexeme = TempNameLex;
								break;
							case OR_ST:
								if (First->type == INT_TYPE || Second->type == INT_TYPE)
								{
									cout << "Invalid Types: Cannot Or Integers" << endl;
									scanner.printCurrentLine();
									cin.get();
									exit(1);
								}
								scanner.TableAddEntry(TempNameLex, BOOL_TYPE, VAR_ID);
								PrintInstructions("addw", NULL, "#4", NULL, "sp");
								PrintInstructions("movw", scanner.FullScopeLookup(poppedvariables[2]), "", scanner.FullScopeLookup(TempNameLex), "");
								PrintInstructions("addw", scanner.FullScopeLookup(poppedvariables[0]), "", scanner.FullScopeLookup(TempNameLex), "");
								PrintInstructions("cmpw", NULL ,"#0", scanner.FullScopeLookup(TempNameLex), "");
								skip = TempNames();
								PrintInstructions("beq", NULL, skip, NULL, "");
								PrintInstructions("movw", NULL, "#1", scanner.FullScopeLookup(TempNameLex), "");
								command = skip + " movw";
								PrintInstructions(command, NULL, "R0", NULL, "R0");
								AnswerLexeme = TempNameLex;
								break;
							case ISEQUAL_ST:
								if (First->type != Second->type)
								{
									cout << "Operands must be the same type" << endl;
									scanner.printCurrentLine();
									cin.get();
									exit(1);
								}

								scanner.TableAddEntry(TempNameLex, BOOL_TYPE, VAR_ID);
								PrintInstructions("addw", NULL, "#4", NULL, "sp");
								PrintInstructions("movw", NULL, "#0", scanner.FullScopeLookup(TempNameLex), "");
								PrintInstructions("cmpw", scanner.FullScopeLookup(poppedvariables[2]), "", scanner.FullScopeLookup(poppedvariables[0]), "");
								skip = TempNames();
								PrintInstructions("bneq", NULL, skip, NULL, "");
								PrintInstructions("movw", NULL, "#1", scanner.FullScopeLookup(TempNameLex), "");
								command = skip + " movw";
								PrintInstructions(command, NULL, "R0", NULL, "R0");
								AnswerLexeme = TempNameLex;
								break;
							case NOTEQUAL_ST:
								if (First->type != Second->type)
								{
									cout << "Operands must be the same type" << endl;
									scanner.printCurrentLine();
									cin.get();
									exit(1);
								}

								scanner.TableAddEntry(TempNameLex, BOOL_TYPE, VAR_ID);
								PrintInstructions("addw", NULL, "#4", NULL, "sp");
								PrintInstructions("movw", NULL, "#0", scanner.FullScopeLookup(TempNameLex), "");
								PrintInstructions("cmpw", scanner.FullScopeLookup(poppedvariables[2]), "", scanner.FullScopeLookup(poppedvariables[0]), "");
								skip = TempNames();
								PrintInstructions("beq", NULL, skip, NULL, "");
								PrintInstructions("movw", NULL, "#1", scanner.FullScopeLookup(TempNameLex), "");
								command = skip + " movw";
								PrintInstructions(command, NULL, "R0", NULL, "R0");
								AnswerLexeme = TempNameLex;
								break;
							case LESSEQUAL_ST:
								if (First->type != Second->type)
								{
									cout << "Operands must be the same type" << endl;
									scanner.printCurrentLine();
									cin.get();
									exit(1);
								}
								scanner.TableAddEntry(TempNameLex, BOOL_TYPE, VAR_ID);
								PrintInstructions("addw", NULL, "#4", NULL, "sp");
								PrintInstructions("movw", NULL, "#0", scanner.FullScopeLookup(TempNameLex), "");
								PrintInstructions("cmpw", scanner.FullScopeLookup(poppedvariables[2]), "", scanner.FullScopeLookup(poppedvariables[0]), "");
								skip = TempNames();
								PrintInstructions("bgtr", NULL, skip, NULL, "");
								PrintInstructions("movw", NULL, "#1", scanner.FullScopeLookup(TempNameLex), "");
								command = skip + " movw";
								PrintInstructions(command, NULL, "R0", NULL, "R0");
								AnswerLexeme = TempNameLex;
								break;
							case GREQUAL_ST:
								if (First->type != Second->type)
								{
									cout << "Operands must be the same type" << endl;
									scanner.printCurrentLine();
									cin.get();
									exit(1);
								}
								scanner.TableAddEntry(TempNameLex, BOOL_TYPE, VAR_ID);
								PrintInstructions("addw", NULL, "#4", NULL, "sp");
								PrintInstructions("movw", NULL, "#0", scanner.FullScopeLookup(TempNameLex), "");
								PrintInstructions("cmpw", scanner.FullScopeLookup(poppedvariables[2]), "", scanner.FullScopeLookup(poppedvariables[0]), "");
								skip = TempNames();
								PrintInstructions("blss", NULL, skip, NULL, "");
								PrintInstructions("movw", NULL, "#1", scanner.FullScopeLookup(TempNameLex), "");
								command = skip + " movw";
								PrintInstructions(command, NULL, "R0", NULL, "R0");
								AnswerLexeme = TempNameLex;
								break;
							case LESSER_ST: // <
								if (First->type != Second->type)
								{
									cout << "Operands must be the same type" << endl;
									scanner.printCurrentLine();
									cin.get();
									exit(1);
								}
								scanner.TableAddEntry(TempNameLex, BOOL_TYPE, VAR_ID);
								PrintInstructions("addw", NULL, "#4", NULL, "sp");
								PrintInstructions("movw", NULL, "#0", scanner.FullScopeLookup(TempNameLex), "");
								PrintInstructions("cmpw", scanner.FullScopeLookup(poppedvariables[2]), "", scanner.FullScopeLookup(poppedvariables[0]), "");
								skip = TempNames();
								PrintInstructions("bgeq", NULL, skip, NULL, "");
								PrintInstructions("movw", NULL, "#1", scanner.FullScopeLookup(TempNameLex), "");
								command = skip + " movw";
								PrintInstructions(command, NULL, "R0", NULL, "R0");
								AnswerLexeme = TempNameLex;
								break;
							case GREATER_ST:
								if (First->type != Second->type)
								{
									cout << "Operands must be the same type" << endl;
									scanner.printCurrentLine();
									cin.get();
									exit(1);
								}
								scanner.TableAddEntry(TempNameLex, BOOL_TYPE, VAR_ID);
								PrintInstructions("addw", NULL, "#4", NULL, "sp");
								PrintInstructions("movw", NULL, "#0", scanner.FullScopeLookup(TempNameLex), "");
								PrintInstructions("cmpw", scanner.FullScopeLookup(poppedvariables[2]), "", scanner.FullScopeLookup(poppedvariables[0]), "");
								skip = TempNames();
								PrintInstructions("bleq", NULL, skip, NULL, "");
								PrintInstructions("movw", NULL, "#1", scanner.FullScopeLookup(TempNameLex), "");
								command = skip + " movw";
								PrintInstructions(command, NULL, "R0", NULL, "R0");
								AnswerLexeme = TempNameLex;
								break;
							}

							if (scanner.IsDebugOn() == true)
							{
								cout << "E -> E ";
								switch (PoppedTerminals[1].getSubType())
								{
								case ASTERISK_ST:
									cout << "*";
									break;
								case DIV_ST:
									cout << "div";
									break;
								case MOD_ST:
									cout << "mod";
									break;
								case AND_ST:
									cout << "and";
									break;
								case ADDITION_ST:{
									cout << "+";
									break;
								}
								case SUBTRACT_ST:
									cout << "-";
									break;
								case OR_ST:
									cout << "or";
									break;
								case ISEQUAL_ST:
									cout << "==";
									break;
								case NOTEQUAL_ST:
									cout << "<>";
									break;
								case LESSEQUAL_ST:
									cout << "<=";
									break;
								case GREQUAL_ST:
									cout << ">=";
									break;
								case LESSER_ST:
									cout << "<";
									break;
								case GREATER_ST:
									cout << ">";
									break;
								}
								cout << " E" << endl;
							}
							TheStack.push(EToken);
							if (PoppedTerminals[1].getSubType() == MOD_ST)
							{
								TheStack.setPointer(scanner.FullScopeLookup(TempLex2));
							}
							else{
								TheStack.setPointer(scanner.FullScopeLookup(TempNameLex));
							}
							break;
							}
							
						}
					}

				}
			else //something has come up as an error
			{
				cout << "invalid expression" << endl;
				scanner.printCurrentLine();
				cin.get();
				exit(1);
			}
		}
	}
}

string ParseClass::TempNames(){
	stringstream ss;
	static int nextCt = 0;
	ss << "$" << nextCt++;
	return ss.str();
	
}

string ParseClass::Labels(){
	stringstream ss;
	static int nextCt = 0;
	ss << "$" << nextCt++;
	return ss.str();
}

bool ParseClass::CallTypeCheck(int Type, VarCell *Function){
	ParamCell *Check = Function->Params;

	int i = 1;
	while (GlobalParam > i)
	{
		if (Check->next != NULL){
			Check = Check->next;
		}
		i++;
	}

	/**for (int i = 1; i > GlobalParam; i++)
	{
		if (Check->next != NULL){
			Check = Check->next;
		}
	}**/

	if (Check->kind == REF_PARAM)
	{
		if (ParamLex == "False")
		{
			cout << "Cannot pass an expression or constant as a reference Parameter" << endl;
			scanner.printCurrentLine();
			cin.get();
			exit(1);
		}
		VarCell* Param = scanner.FullScopeLookup(ParamLex);
		PrintInstructions("subw", NULL, "#4", NULL, "sp");
		if (Param->kind == REF_PARAM)
		{
		PrintInstructions("movw", Param, "", NULL, "@sp");
		}
		else{
		PrintInstructions("mova", Param, "", NULL, "@sp");
		}
		PrintInstructions("addw", NULL, "#4", NULL, "sp");

	}


	if (Check->type != Type)
	{
		cout << "Value passed must be same type as Parameter" << endl;
		scanner.printCurrentLine();
		cin.get();
		exit(1);
	}
	

}

int ParseClass::numLinesProcessed()
{
	return scanner.numLinesProcessed();
}

