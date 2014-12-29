//Dave A. Berque, Revised August 2014
//This file contains implementations of the 
//fileManagerClass
//Brandon Wright, Edited September 2014
//This is my modifications to the original source code
//I didn't know if you wanted your comment at the top deleted or not.

//The class header is found in filemngr.h

#include "stdafx.h"  // Required for visual studio to work correctly

#include "filemngr.h"


fileManagerClass::fileManagerClass()
//Precondition: None.
//Postcondition: The constructor has opened the file which is to be
//               processed and has inialized the currentLineNumber to 0
{  char filename[MAXCELLSPERSTRING];

   cout << "Enter file name to compile: ";
   cin.getline (filename, MAXCELLSPERSTRING);
   fileToCompile.open(filename);

   if (fileToCompile.fail())
   {  cout << "Error, the source file: " << filename << " was not opened." << endl;
      cout << "Press return to end program." << endl;
      cin.get();
      exit(1);
   }  
  currentLineNumber = 0;
  autoPrintStatus = false;
}

int fileManagerClass::getNextChar()
//Precondition:  The source file associated with the owning object has
//               been prepared for reading.
//Postcondition: The next character from the input file has been read and
//               returned by the function.  If another chacter could not
//               be read because END OF FILE was reached then EOF is
//               returned.
{  if ((currentLineNumber == 0) ||
      (charToReadNext == strlen(currentLine)))
   {  if (fileToCompile.peek() == EOF) 
         return (EOF);
	  fileToCompile.getline(currentLine, MAXCELLSPERSTRING);
      strcat(currentLine, "\n");
      currentLineNumber++;
	  if (autoPrintStatus == true)
	  {
		  printCurrentLine();
	  }
      charToReadNext = 0;
   }
   return(currentLine[charToReadNext++]);
}

void fileManagerClass::pushBack()
{
	
	if (charToReadNext != 0)
	--charToReadNext;
	//sets next character to be read back one to be read again since token has been found.
}

void fileManagerClass::printCurrentLine()
{
	// printing value of current line number and contents of current line
	printf("%d - %s",currentLineNumber, currentLine);
}

void fileManagerClass::setPrintStatus(bool newStatus)
{
	//takes in a bool to modify global autoPrintStatus.
	autoPrintStatus = newStatus;
}

int fileManagerClass::numLinesProcessed()
{
	//returns currentLineNumber
	return currentLineNumber;
}

void fileManagerClass::closeSourceProgram()
//Precondition:  The file belonging to the object owning this routine has
//               been opened.
//Postcondition: The file belonging to the object owning this routine has
//               been closed.
{  fileToCompile.close();
}
