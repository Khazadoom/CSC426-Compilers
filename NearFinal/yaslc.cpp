

//Dave A. Berque, Revised August 2014
//This driver program should be used to test the first part of the YASL
//compiler.

//This version tested in Visual Studio .NET 2014


#include "stdafx.h"  // A visual studio requirement


#include "parser.h"
#include <iostream>

int main(int argc, char* argv[])
{   
	ParseClass parser;	
	parser.ParseProgram();
	cout << "YASLC - BW has just compiled ";
	cout << parser.numLinesProcessed();
	cout << " lines of code." << endl;
    cin.get();

    return (0);
}

