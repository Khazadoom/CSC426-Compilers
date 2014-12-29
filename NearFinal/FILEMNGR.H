//Dave A. Berque, Revised August 2014
//This file contains a class definition for managing the
//file containing the YASL source program to be compiled.
//Brandon Wright, Edited September 2014
// this is the class definition file.
//The definitions of these routines can be found in filemngr.cpp

#ifndef _filemngr
#define _filemngr

#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>    //to allow exit
#include "general.h"

using namespace std;
using std::ifstream;

class fileManagerClass
{ public:
     fileManagerClass();
     int getNextChar();
	 int numLinesProcessed();
     void closeSourceProgram();
	 void pushBack();
	 void printCurrentLine();
	 void setPrintStatus(bool newStatus);
  private:
     char currentLine[MAXCELLSPERSTRING];
     int charToReadNext;
     int currentLineNumber;
     ifstream fileToCompile;
	 bool autoPrintStatus;
}  ;

#endif
