//Brandon Wright, Written Sept 2014.
// This file contains the methods for the Scanner Class.

#include "stdafx.h"
#include "ctype.h"
#include "scanner.h"
#include <string>
#include "filemngr.h"

using namespace std;

int ScannerClass::getScope()
{
	return SymbolTable.getScope();
}

void ScannerClass::setOffset(string FunctionName, string LocName){
	SymbolTable.SetOffset(FunctionName, LocName);
}

VarCell * ScannerClass::ScopeLookup(string Lookup, ScopeCell *Scope){
	return SymbolTable.ScopeLookup(Lookup, Scope);
}

int ScannerClass::ParamNum(string Lookup)
{
	return SymbolTable.ParamNum(Lookup);
}

void ScannerClass::DeleteTemps(){
	SymbolTable.DeleteTemps();
}


VarCell * ScannerClass::FullScopeLookup(string Lookup){
	return SymbolTable.FullScopeLookup(Lookup);
}

bool ScannerClass::TableAddEntry(string Lookup, int Type, int Kind){
	return SymbolTable.TableAddEntry(Lookup, Type, Kind);
}

void ScannerClass::TableAddLevel(string Name){
	SymbolTable.TableAddLevel(Name);
}

void ScannerClass::TableDelLevel(){
	SymbolTable.TableDelLevel();
}

void ScannerClass::DumpTable(){
	SymbolTable.DumpTable();
}

ScannerClass::ScannerClass()
{
	Lexeme = "";
	expressDebug = false;
}

bool ScannerClass::IsDebugOn()
{
	return expressDebug;
}

void ScannerClass::pushBack()
{
	//only thing it does is call the original pushback method from the File Manager.
	theFile.pushBack();
}

ScopeCell * ScannerClass::getTop(){
	return SymbolTable.getTop();

}

void ScannerClass::DeleteSpecTemps()
{
	SymbolTable.DeleteSpecTemps();
}
	

TokenClass ScannerClass::getToken(TokenClass TheToken)
{
	int state = getNextToken(TheToken);
	switch (state){
	case IDENT_T: // Identifier Token
		pushBack();
		Lexeme.erase(Lexeme.length() - 1, 1); 
		//if and else if's to see if Identifier is actually a keyword.
		// if it is, return the keyword instead.
		if (_stricmp(Lexeme.c_str(), "program") == 0)
		{
			TheToken.setTokenValues(Lexeme, PRGRM_T, NONE_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "function") == 0)
		{
			TheToken.setTokenValues(Lexeme, FUNCTION_T, NONE_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "begin") == 0)
		{
			TheToken.setTokenValues(Lexeme, BEGIN_T, NONE_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "end") == 0)
		{
			TheToken.setTokenValues(Lexeme, END_T, NONE_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "if") == 0)
		{
			TheToken.setTokenValues(Lexeme, IF_T, NONE_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "then") == 0)
		{
			TheToken.setTokenValues(Lexeme, THEN_T, NONE_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "else") == 0)
		{
			TheToken.setTokenValues(Lexeme, ELSE_T, NONE_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "while") == 0)
		{
			TheToken.setTokenValues(Lexeme, WHILE_T, NONE_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "do") == 0)
		{
			TheToken.setTokenValues(Lexeme, DO_T, NONE_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "cout") == 0)
		{
			TheToken.setTokenValues(Lexeme, COUT_T, NONE_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "cin") == 0)
		{
			TheToken.setTokenValues(Lexeme, CIN_T, NONE_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "endl") == 0)
		{
			TheToken.setTokenValues(Lexeme, ENDL_T, NONE_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "int") == 0)
		{
			TheToken.setTokenValues(Lexeme, INT_T, NONE_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "boolean") == 0)
		{
			TheToken.setTokenValues(Lexeme, BOOLEAN_T, NONE_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "true") == 0)
		{
			TheToken.setTokenValues(Lexeme, TRUE_T, NONE_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "false") == 0)
		{
			TheToken.setTokenValues(Lexeme, FALSE_T, NONE_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "or") == 0)
		{
			TheToken.setTokenValues(Lexeme, ADDOP_T, OR_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "and") == 0)
		{
			TheToken.setTokenValues(Lexeme, MULOP_T, AND_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "div") == 0)
		{
			TheToken.setTokenValues(Lexeme, MULOP_T, DIV_ST);
			Lexeme = "";
			return(TheToken);
		}
		else if (_stricmp(Lexeme.c_str(), "mod") == 0)
		{
			TheToken.setTokenValues(Lexeme, MULOP_T, MOD_ST);
			Lexeme = "";
			return(TheToken);
		}
		else {
			if (Lexeme.length() > 12)
			{
				cout << "ERROR: IDENTIFIER LENGTH CANNOT EXCEED 12" << endl;
				printCurrentLine();
				cin.get();
				exit(1);
			}
			// if the identifier isn't too long and it isn't a protected keyword set as Identifier
			TheToken.setTokenValues(Lexeme, IDENT_T, NONE_ST);
			Lexeme = "";
			return(TheToken);
		}
	case INTEGER_T:
		pushBack();
		Lexeme.erase(Lexeme.length() - 1, 1);

		if (Lexeme.length() > 4)
		{
			cout << "ERROR: Integer has Exceeded Maximum Length. Cannot exceed 4 digits" << endl;
			printCurrentLine();
			cin.get();
			exit(1);
		}

		TheToken.setTokenValues(Lexeme, INTEGER_T, NONE_ST);
		Lexeme = "";
		return(TheToken);
	case EOF_T:
		//Lexeme.erase(Lexeme.length() - 1, 1);
		TheToken.setTokenValues(Lexeme, EOF_T, NONE_ST);
		Lexeme = "";
		return(TheToken);
	case ADDITION_ST:
		TheToken.setTokenValues(Lexeme, ADDOP_T, ADDITION_ST);
		Lexeme = "";
		return(TheToken);
	case SETEQUAL_T: // = token
		pushBack();
		Lexeme.erase(Lexeme.length() - 1, 1);
		TheToken.setTokenValues(Lexeme, SETEQUAL_T, NONE_ST);
		Lexeme = "";
		return(TheToken);
	case ISEQUAL_ST: // == token
		TheToken.setTokenValues(Lexeme, RELOP_T, ISEQUAL_ST);
		Lexeme = "";
		return(TheToken);
	case GREATER_ST: // > token
		pushBack();
		Lexeme.erase(Lexeme.length() - 1, 1);
		TheToken.setTokenValues(Lexeme, RELOP_T, GREATER_ST);
		Lexeme = "";
		return(TheToken);
	case DOUBLEGREATER_T:
		TheToken.setTokenValues(Lexeme, DOUBLEGREATER_T, NONE_ST);
		Lexeme = "";
		return(TheToken);
	case SWAP_T:
		TheToken.setTokenValues(Lexeme, SWAP_T, NONE_ST);
		Lexeme = "";
		return(TheToken);
	case GREQUAL_ST:
		TheToken.setTokenValues(Lexeme, RELOP_T, GREQUAL_ST);
		Lexeme = "";
		return(TheToken);
	case LESSER_ST:
		pushBack();
		Lexeme.erase(Lexeme.length() - 1, 1);
		TheToken.setTokenValues(Lexeme, RELOP_T, LESSER_ST);
		Lexeme = "";
		return(TheToken);
	case DOUBLELESSER_T:
		TheToken.setTokenValues(Lexeme, DOUBLELESSER_T, NONE_ST);
		Lexeme = "";
		return(TheToken);
	case LESSEQUAL_ST:
		TheToken.setTokenValues(Lexeme, RELOP_T, LESSEQUAL_ST);
		Lexeme = "";
		return(TheToken);
	case NOTEQUAL_ST:
		TheToken.setTokenValues(Lexeme, RELOP_T, NOTEQUAL_ST);
		Lexeme = "";
		return(TheToken);
	case SUBTRACT_ST:
		TheToken.setTokenValues(Lexeme, ADDOP_T, SUBTRACT_ST);
		Lexeme = "";
		return(TheToken);
	case ASTERISK_ST:
		TheToken.setTokenValues(Lexeme, MULOP_T, ASTERISK_ST);
		Lexeme = "";
		return(TheToken);
	/**case DIRECTIVE_T:
		if (Lexeme == "{$p+}")
			theFile.setPrintStatus(true);
		else
		{
			cout << "WARNING: Compiler Directive not Defined" << endl;
			printCurrentLine();
		}
		//TheToken.setTokenValues(Lexeme, DIRECTIVE_T, NONE_ST);
		Lexeme = "";
		state = 0;
		break;
		**/
	case LEFTPARENTH_T:
		TheToken.setTokenValues(Lexeme, LEFTPARENTH_T, NONE_ST);
		Lexeme = "";
		return(TheToken);
	case RIGHTPARENTH_T:
		TheToken.setTokenValues(Lexeme, RIGHTPARENTH_T, NONE_ST);
		Lexeme = "";
		return(TheToken);
	case STRINGCONST_T:
		if (Lexeme.length() > 52)
		{
			cout << "ERROR: String Constant has Exceeded Maximum Length" << endl;
			printCurrentLine();
			cin.get();
			exit(1);
			//TheToken.setTokenValues("EOF", EOF_T, NONE_ST);
			//return(TheToken);
		}
		TheToken.setTokenValues(Lexeme, STRINGCONST_T, NONE_ST);
		Lexeme = "";
		return(TheToken);
	case SEMICOLON_T:
		TheToken.setTokenValues(Lexeme, SEMICOLON_T, NONE_ST);
		Lexeme = "";
		return(TheToken);
	case COMMA_T:
		TheToken.setTokenValues(Lexeme, COMMA_T, NONE_ST);
		Lexeme = "";
		return(TheToken);
	case PERIOD_T:
		TheToken.setTokenValues(Lexeme, PERIOD_T, NONE_ST);
		Lexeme = "";
		return(TheToken);
	case AMPERSAND_T:
		TheToken.setTokenValues(Lexeme, AMPERSAND_T, NONE_ST);
		Lexeme = "";
		return(TheToken);
		}
}

int ScannerClass::getNextToken(TokenClass TheToken)
{
	
	int state;
	char ch;
	bool IsComment = false;
	state = 0;
	do
	{
		ch = theFile.getNextChar();
		//appends newest read char to the stored Lexeme
		Lexeme = Lexeme + ch;
		string LexBracket = Lexeme.substr(0, 1);//creates a substring of the first character in the lexeme
		string LexSlash = Lexeme.substr(0, 2); // creates a substring of the first two characters in the lexeme


		if (LexBracket == "{" && Lexeme.length() > 5 ) // checks if the lexeme starts with a { and not a Directive as the length is longer than 5
		{
			IsComment = true;
		}
		if (LexSlash == "//") //alternative comment style of beginning with "//"
		{
			IsComment = true;
		}


		if (IsComment && Lexeme.length() > 5) // if the comment is longer than 5 characters long, this will automatically remove whatever has been appended
		{									  // this will keep the size of the comment down to a known manageable size. actual length before erasure is irrelevant.
			Lexeme.erase(Lexeme.length() - 1, 1);
		}

		if (IsComment && ch == '}') //checks to see if the newest character is a } and is Comment is set to true
		{							// if these are both true, IsComment is set to false, and the Lexeme is erased
			Lexeme = "";
			IsComment = false;
		}
		else if (LexSlash == "//" && ch == '\n') //checks to see if newest character is a carriage return and the Lexeme starts with "//"
		{										 // if these are true, IsComment is set to false and Lexeme is erased.
			Lexeme = "";
			IsComment = false;
		}
		else
		{
			IsComment = false;
		}
		 
		state = getNextState(state, ch);
		
			//This will erase all whitespaces if the whitespace is the only thing in the lexeme.
			if (isspace(ch))
			{
				if (Lexeme.length() == 1){
					Lexeme.erase(Lexeme.length() - 1, 1);
				}
			}

		//switch statement for all Final States.
		switch (state){
		case IDENT_T: // Identifier Token
			return(state);
		case INTEGER_T:
			return(state);
		case EOF_T:
			return(state);
		case ADDITION_ST:
			return(state);
		case SETEQUAL_T: // = token
			return(state);
		case ISEQUAL_ST: // == token
			return(state);
		case GREATER_ST: // > token
			return(state);
		case DOUBLEGREATER_T:
			return(state);
		case SWAP_T:
			return(state);
		case GREQUAL_ST:
			return(state);
		case LESSER_ST:
			return(state);
		case DOUBLELESSER_T:
			return(state);
		case LESSEQUAL_ST:
			return(state);
		case NOTEQUAL_ST:
			return(state);
		case SUBTRACT_ST:
			return(state);
		case ASTERISK_ST:
			return(state);
		case DIRECTIVE_T: //Not an actual token state, will just reset to 0 after carrying out directive
			if (Lexeme == "{$p+}") // only currently known directive.
				theFile.setPrintStatus(true);
			else if (Lexeme == "{$p-}")
				theFile.setPrintStatus(false);
			else if (Lexeme == "{$e+}")
				expressDebug = true;
			else if (Lexeme == "{$e-}")
				expressDebug = false;
			else if (Lexeme == "{$s+}")
				DumpTable();
			else
			{
				cout << "WARNING: Compiler Directive not Defined" << endl;
				printCurrentLine();
			}
			Lexeme = "";
			state = 0;
			break;
		case LEFTPARENTH_T:
			return(state);
		case RIGHTPARENTH_T:
			return(state);
		case STRINGCONST_T:
			return(state);
		case SEMICOLON_T:
			return(state);
		case COMMA_T:
			return(state);
		case PERIOD_T:
			return(state);
		case AMPERSAND_T:
			return(state);
		case SLASHERROR_T:
			cout << "ERROR: / must be followed by a second /" << endl;
			printCurrentLine();
			cin.get();
			exit(1);
		case CRERRORCONST_T:
			cout << "ERROR: Cannot have a Carriage Return in String Constant" << endl;
			printCurrentLine();
			cin.get();
			exit(1);
		case EOFERRORCONST_T:
			cout << "ERROR: EOF before end of String Constant" << endl;
			printCurrentLine();
			cin.get();
			exit(1);
		case BRACKETERROR_T:
			cout << "ERROR: EOF found before end of Comment" << endl;
			printCurrentLine();
			cin.get();
			exit(1);
		case OTHERERROR:
			cout << "ERROR: Unrecognized Character found" << endl;
			printCurrentLine();
			cin.get();
			exit(1);

			
		}






	} while (1);

}

int ScannerClass::getNextState(int state, char ch)
{
	int col;
							// Columns and their corresponding inputs for the 2D Array.
							//   0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23
							//   L, D,   =	 +	 -	 _	 ;	 .	 <	 >	 $	 /	 {	 }	 '	 *   (	 )	 ,	 &   CR	 WS	 EOF OTHER
	static int table[13][24] = { 1,	12,	 4,	 113,114,102,123,125,3,	 2,	 102,10, 5,	 102,9,	 115,118,119,124,126,0,  0,  101,102, //0  States
								 1,	 1,	 100,100,100,1,  100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100, //1  Of
								 106,106,105,106,106,106,106,106,104,103,106,106,106,106,106,106,106,106,106,106,106,106,106,106, //2  The
								 107,107,109,107,107,107,107,107,108,110,107,107,107,107,107,107,107,107,107,107,107,107,107,107, //3  Compiler
								 112,112,111,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112, //4
								 5,	 5,	 5,	 5,	 5,	 5,	 5,	 5,	 5,	 5,	 6,	 5,	 5,  0,  5,	 5,	 5,	 5,	 5,  5,  5,  5,  129,5,   //5
								 7,	 5,	 5,	 5,  5,	 5,	 5,	 5,	 5,	 5,  5,  5,  5,	 5,	 5,	 5,	 5,	 5,	 5,  5,  5,  5,  129,5,   //6
								 5,	 5,	 5,	 8,  8,	 5,	 5,	 5,	 5,	 5,	 5,	 5,	 5,	 5,  5,	 5,	 5,	 5,	 5,  5,  5,  5,  129,5,   //7
								 5,	 5,	 5,	 5,	 5,	 5,	 5,	 5,	 5,	 5,	 5,  5,	 5,	 117,5,	 5,	 5,	 5,	 5,  5,  5,  5,  129,5,   //8
								 9,	 9,  9,	 9,	 9,	 9,	 9,	 9,  9,	 9,	 9,	 9,	 9,	 9,	 120,9,	 9,	 9,	 9,  9,  121,9,  122,9,   //9
								 128,128,128,128,128,128,128,128,128,128,128,11, 128,128,128,128,128,128,128,128,128,128,128,128, //10
								 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 0,  11, 0,  11,   //11
								 130,12, 130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,130  //12
	};



	//mess of IF statements to assign proper col value 
	if (isalpha(ch))
		col = 0;
	else if (isdigit(ch))
		col = 1;
	else if (ch == '=')
		col = 2;
	else if (ch == '+')
		col = 3;
	else if (ch == '-')
		col = 4;
	else if (ch == '_')
		col = 5;
	else if (ch == ';')
		col = 6;
	else if (ch == '.')
		col = 7;
	else if (ch == '<')
		col = 8;
	else if (ch == '>')
		col = 9;
	else if (ch == '$')
		col = 10;
	else if (ch == '/')
		col = 11;
	else if (ch == '{')
		col = 12;
	else if (ch == '}')
		col = 13;
	else if (ch == '\'')
		col = 14;
	else if (ch == '*')
		col = 15;
	else if (ch == '(')
		col = 16;
	else if (ch == ')')
		col = 17;
	else if (ch == ',')
		col = 18;
	else if (ch == '&')
		col = 19;
	else if (ch == '\n')
		col = 20;
	else if (isspace(ch))
		col = 21;
	else if (ch == EOF)
		col = 22;
	else
		col = 23;


	return table[state][col];

}

void ScannerClass::printCurrentLine()
{
	theFile.printCurrentLine();
}

void ScannerClass::closeSourceProgram()
{
	theFile.closeSourceProgram();
}

TokenClass::TokenClass()
{
	TokenType = 0;
	SubType = 0;
	Lexeme = "";
	//initializing the Token class as a blank Lexeme and 0 for Types/SubTypes
	setTokenValues("", 0, 0);
	
	//just had to make sure here. didn't know if it would stick. HOORAY FOR REDUNDANCY!
}

void TokenClass::printToken()
{
	switch (TokenType)
	{
	case IDENT_T:
		cout << Lexeme;
		cout << "       IDENT_T";
		cout << "		NONE_T" << endl;
		break;
	case INTEGER_T:
		cout << Lexeme;
		cout << "		INTEGER_T";
		cout << "		NONE_T" << endl;
		break;
	case SETEQUAL_T:
		cout << Lexeme;
		cout << "		SETEQUAL_T";
		cout << "		NONE_T" << endl;
		break;
	case RELOP_T:
		cout << Lexeme;
		cout << "       RELOP_T";
		switch (SubType) //switch to figure out the subtype of the RELOP
		{
		case GREATER_ST:
			cout << "           GREATER_ST" << endl;
			break;
		case GREQUAL_ST:
			cout << "           GREQUAL_ST" << endl;
			break;
		case LESSER_ST:
			cout << "           LESSER_ST" << endl;
			break;
		case LESSEQUAL_ST:
			cout << "           LESSEQUAL_ST" << endl;
			break;
		case ISEQUAL_ST:
			cout << "           ISEQUAL_ST" << endl;
			break;
		case NOTEQUAL_ST:
			cout << "           NOTEQUAL_ST" << endl;
			break;
		}
		break;
	case ADDOP_T:
		cout << Lexeme;
		cout << "        ADDOP_T";
		switch (SubType) //switch to figure out the subtype of the ADDOP
		{
		case OR_ST:
			cout << "        OR_ST" << endl;
			break;
		case ADDITION_ST:
			cout << "        ADDITION_ST" << endl;
			break;
		case SUBTRACT_ST:
			cout << "         SUBTRACT_ST" << endl;
			break;
		}
		break;
	case MULOP_T:
		cout << Lexeme;
		cout << "         MULOP_T";
		switch (SubType) // switch to figure out the subtype of the MULOP
		{
		case AND_ST:
			cout << "          AND_ST" << endl;
			break;
		case ASTERISK_ST:
			cout << "          ASTERISK_ST" << endl;
			break;
		case DIV_ST:
			cout << "           DIV_ST" << endl;
			break;
		case MOD_ST:
			cout << "           MOD_ST" << endl;
			break;
		}
		break;
	case DOUBLEGREATER_T:
		cout << Lexeme;
		cout << "       DOUBLEGREATER_T";
		cout << "       NONE_ST" << endl;
		break;
	case DOUBLELESSER_T:
		cout << Lexeme;
		cout << "       DOUBLESSER_T";
		cout << "       NONE_ST" << endl;
		break;
	case SWAP_T:
		cout << Lexeme;
		cout << "       SWAP_T";
		cout << "       NONE_ST" << endl;
		break;
	case DIRECTIVE_T:
		cout << Lexeme;
		cout << "       DIRECTIVE_T";
		cout << "       NONE_ST" << endl;
		break;
	case LEFTPARENTH_T:
		cout << "(";
		cout << "       LEFTPARENTH_T";
		cout << "       NONE_ST" << endl;
		break;
	case RIGHTPARENTH_T:
		cout << ")";
		cout << "       RIGHTPARENTH_T";
		cout << "       NONE_ST" << endl;
		break;
	case STRINGCONST_T:
		cout << Lexeme;
		cout << "       STRINGCONST_T";
		cout << "       NONE_ST" << endl;
		break;
	case SEMICOLON_T:
		cout << ";";
		cout << "        SEMICOLON_T";
		cout << "        NONE_ST" << endl;
		break;
	case COMMA_T:
		cout << ",";
		cout << "        COMMA_T";
		cout << "        NONE_ST" << endl;
		break;
	case PERIOD_T:
		cout << ".";
		cout << "        PERIOD_T";
		cout << "        NONE_ST" << endl;
		break;
	case AMPERSAND_T:
		cout << "&";
		cout << "        AMPERSAND_T";
		cout << "        NONE_ST"<< endl;
		break;
	case PRGRM_T:
		cout << Lexeme;
		cout << "        PRGRM_T";
		cout << "        NONE_ST" << endl;
		break;
	case FUNCTION_T:
		cout << Lexeme;
		cout << "        FUNCTION_T";
		cout << "        NONE_ST" << endl;
		break;
	case BEGIN_T:
		cout << Lexeme;
		cout << "        BEGIN_T";
		cout << "        NONE_ST" << endl;
		break;
	case END_T:
		cout << Lexeme;
		cout << "        END_T";
		cout << "         NONE_ST" << endl;
		break;
	case ENDL_T:
		cout << Lexeme;
		cout << "        ENDL_T";
		cout << "        NONE_ST"<< endl;
		break;
	case IF_T:
		cout << Lexeme;
		cout << "        IF_T";
		cout << "        NONE_ST" << endl;
		break;
	case THEN_T:
		cout << Lexeme;
		cout << "        THEN_T";
		cout << "        NONE_ST" << endl;
		break;
	case ELSE_T:
		cout << Lexeme;
		cout << "        ELSE_T";
		cout << "        NONE_ST" << endl;
		break;
	case WHILE_T:
		cout << Lexeme;
		cout << "        WHILE_T";
		cout << "        NONE_ST" << endl;
		break;
	case DO_T:
		cout << Lexeme;
		cout << "        DO_T";
		cout << "        NONE_ST" << endl;
		break;
	case COUT_T:
		cout << Lexeme;
		cout << "        COUT_T";
		cout << "        NONE_ST" << endl;
		break;
	case CIN_T:
		cout << Lexeme;
		cout << "        CIN_T";
		cout << "        NONE_ST" << endl;
		break;
	case INT_T:
		cout << Lexeme;
		cout << "        INT_T";
		cout << "        NONE_ST" << endl;
		break;
	case BOOLEAN_T:
		cout << Lexeme;
		cout << "        BOOLEAN_T";
		cout << "        NONE_ST" << endl;
		break;
	case TRUE_T:
		cout << Lexeme;
		cout << "        TRUE_T";
		cout << "        NONE_ST" << endl;
		break;
	case FALSE_T:
		cout << Lexeme;
		cout << "        FALSE_T";
		cout << "        NONE_ST" << endl;
		break;
	}

	
}

int ScannerClass::numLinesProcessed(){
	return(theFile.numLinesProcessed());
}

 void TokenClass::setTokenValues(string InputLexeme, int FinalState, int SubTypeToken)
 {
	 TokenType = FinalState;
	 SubType = SubTypeToken;
	 Lexeme = InputLexeme;
 }


int TokenClass::getType()
{
	return TokenType;
}

int TokenClass::getSubType()
{
	return SubType;
}

string TokenClass::getLexeme()
{
	return Lexeme;
}
