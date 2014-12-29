//Dave A. Berque, Revised August 2014
//This file contains some general constant definitions that will
//be useful in several parts of the compiler.  Feel free to add
//additional definitions here.

#ifndef _general
#define _general

const int MAXCELLSPERSTRING = 82;  //80 characters as well as \n plus \0.

//declaring constants for use in switch statements.
const int IDENT_T = 100;
const int EOF_T = 101;
const int OTHERERROR = 102; //ERROR State, NOT an actual Token.
const int DOUBLEGREATER_T = 103;
const int SWAP_T = 104;
const int GREQUAL_ST = 105;
const int GREATER_ST = 106;
const int LESSER_ST = 107;
const int DOUBLELESSER_T = 108;
const int LESSEQUAL_ST = 109;
const int NOTEQUAL_ST = 110;
const int ISEQUAL_ST = 111;
const int SETEQUAL_T = 112;
const int ADDITION_ST = 113;
const int SUBTRACT_ST = 114;
const int ASTERISK_ST = 115;
const int COMMENT_T = 116; //Technically not a token, will be ignored and NOT STORED.
const int DIRECTIVE_T = 117;
const int LEFTPARENTH_T = 118;
const int RIGHTPARENTH_T = 119;
const int STRINGCONST_T = 120;
const int CRERRORCONST_T = 121; //ERROR STATE Carriage return in a String Constant
const int EOFERRORCONST_T = 122;//ERROR STATE EOF in a string constant
const int SEMICOLON_T = 123;
const int COMMA_T = 124;
const int PERIOD_T = 125;
const int AMPERSAND_T = 126;
const int SLASHCOMMENT_T = 127;
const int SLASHERROR_T = 128; //ERROR STATE / is not followed by another /
const int BRACKETERROR_T = 129; // Comment is attempting to end without a }
const int INTEGER_T = 130;
const int SPECIALID_T = 131;
const int OR_ST = 132; //Key word OR
const int AND_ST = 133; // Keyword AND
const int DIV_ST = 134; // Keyword DIV
const int MOD_ST = 135; // Keyword MOD
const int PRGRM_T = 136; //Keyword program
const int FUNCTION_T = 137; // Keyword Function
const int BEGIN_T = 138; // Keyword Function
const int END_T = 139; // Keyword end
const int IF_T = 140;
const int THEN_T = 141;
const int ELSE_T = 142;
const int WHILE_T = 143;
const int DO_T = 144;
const int COUT_T = 145;
const int CIN_T = 146;
const int ENDL_T = 147;
const int INT_T = 148;
const int BOOLEAN_T = 149;
const int TRUE_T = 150;
const int FALSE_T = 151;
const int NONE_ST = 152;
const int RELOP_T = 153;
const int ADDOP_T = 154;
const int MULOP_T = 155;
const int E_T = 156;
const int LT = 157;
const int GT = 158;
const int EQ = 159;
const int ER = 160;
const int FUNC_ID = 161;
const int VAR_ID = 162;
const int REF_PARAM = 163;
const int VALUE_PARAM = 164;
const int FUNC_ID_TYPE = 165;
const int INT_TYPE = 166;
const int BOOL_TYPE = 167;
#endif