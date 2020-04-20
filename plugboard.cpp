#include <iostream>
#include <fstream>
#include "errors.h"
#include "plugboard.h"

using namespace std;

/* CONSTRUCTOR */
Plugboard::Plugboard(int argc, char** argv) {

  plug = new int[max_mappings];

  // GET PLUGBOARD MAPPINGS
  int number;
  char ch;
  char ch2;
  ifstream input;

  // Open file and check if opened correctly
  input.open(*(argv+1));
      
  if (input.fail())
    {
      cerr << "Input file couldn't be opened.\n";
      throw ERROR_OPENING_CONFIGURATION_FILE;
    }
  
  // loop through opened file and copy into correct array
  int q = 0; // Counter
  input.get(ch);

  // 4 cases for distinct inputs (ie seperated by whitespace)
  // 1) non-integer          2) single digit integer
  // 3) double digit integer 4) >2 digit integer
  // Cases 2 and 3 are checked for their logical validity

  // Start of loop
  while(!input.eof())
    {
      // case 1)
      if (!isspace(ch) && !isdigit(ch))
	{
	  cerr << "Non-numeric character in plugboard file ";
          cerr << "plugboard.pb\n";
	  input.close();
	  throw NON_NUMERIC_CHARACTER;
	}

      // case 2)
      else if (isspace(input.peek()) && isdigit(ch))
	{
	  q++; // Increment counter

	  // Check if max input not reached
	  if(q<=max_mappings)
	    {
	      // Logical checks and store in plug
	      number = static_cast<int>(ch - '0');
	      try{check_index(number);}
	      catch(int &ex){input.close(); throw ex;}
	      try{check_mapping(number,q);}
	      catch(int &ex){input.close(); throw ex;}
	      plug[q-1] = number;
	    }
	}

      // at least 2 digit char values, possibly more
      else if (isdigit(input.peek()) && input.peek() != EOF && isdigit(ch))
	{
	  // Get second character,
	  input.get(ch2);
	  
	  // Case 3)
	  if (isspace(input.peek()))
	    {
	      q++;
	      // Perform logical checks and store value
	      if(q<=max_mappings)
		{
		  number = static_cast<int>(ch - '0')* 10 + static_cast<int>(ch2 - '0');
		  try{check_index(number);}
		  catch(int &ex){input.close(); throw ex;}
		  try{check_mapping(number,q);}
		  catch(int &ex){input.close(); throw ex;}
	          plug[q-1] = number;
		}
	    }
	  
	  // Case 4)
	  else if (isdigit(input.peek()))
	    {
	      cerr << "Invalid index for plugboard ";
	      cerr << "configuration file/n";
	      input.close();
	      throw INVALID_INDEX;
	    }
	}

      input.get(ch);

      // Check if max is not exceeded
      if (q>max_mappings)
	{
	  cerr << "Incorrect number of parameters ";
	  cerr << "in plugboard file plugboard.pb\n";
	  input.close();
	  throw INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
	}
    }
  // End of loop

  input.close();

  // Check if correct amount of mappings (ie even)
  if (q%2)
    {
      cerr << "Incorrect number of parameters in plugboard ";
      cerr << "file plugboard.pb\n";
      throw INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
    }
  
  // Assign the actual length of plugboard 
  length = q;
}
/* END OF CONSTRUCTOR */



/* DESTRUCTOR */
Plugboard::~Plugboard()
{
  delete [] plug;
}
/* END OF DESTRUCTOR */


/* FUNCTION TO PASS VALUE THROUGH PLUGBOARD */
void Plugboard::swap(int &input) {

  for (int j=0; j < length; j++)
    {
      if (input == plug[j])
	{
	  if (j%2)
	    {
	      input = plug[j-1];
	      return;
	    }
	  else
	    {
	      input = plug[j+1];
	      return;
	    }
	}
    }

  return;
}
/* END OF FUNCTION */



/* FUNCTION TO CHECK INDEX */
void Plugboard::check_index(int number)
{
  if(number < 0 || number > 25)
    {
      cerr << "Invalid index for plugboard configuration\n";
      throw INVALID_INDEX;
    }
  return;
}
/* END OF FUNCTION */



/* FUNCTION TO CHECK MAPPING */
void Plugboard::check_mapping(int number, int q)
{
 
  for (int i = 0; i < q-1; i++)
    {
      if(plug[i] == number)
	{
	  cerr << "Impossible plugboard configuration\n";
	  throw IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
	}
    }
  return;
}
/* END OF FUNCTION */
  
    
	      
