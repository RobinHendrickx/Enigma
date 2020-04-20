#include <iostream>
#include <fstream>
#include "errors.h"
#include "reflector.h"

using namespace std;

/* CONSTRUCTOR */  
Reflector::Reflector(int argc, char** argv) {

  reflection = new int[nr_of_mappings];

  // GET REFLECTOR MAPPINGS
  int number;
  char ch;
  char ch2;
  ifstream input;

  // Open file and check if opened correctly
  input.open(*(argv+2));
      
  if (input.fail())
    {
      cerr << "Reflector input file couldn't be opened.\n";
      throw ERROR_OPENING_CONFIGURATION_FILE;
    }
  
  // Loop through opened file and copy into correct array
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
	  cerr << "Non-numeric character in reflector file ";
	  cerr << "reflector.rf\n";
	  input.close();
	  throw NON_NUMERIC_CHARACTER;
	}

      // case 2)
      else if (isspace(input.peek()) && isdigit(ch))
	{
	  q++; // Increment counter

	  // check if max input not reached
	  if (q<=nr_of_mappings)
	    {
	      // Logical chekcs and store in reflection
	      number = static_cast<int>(ch - '0');
	      try{check_index(number);}
	      catch(int &ex){input.close(); throw ex;}
	      try{check_mapping(number,q);}
	      catch(int &ex){input.close(); throw ex;}
	      reflection[q-1] = number;
	    }
	}

      // At least 2 digit char values, possibly more
      else if (isdigit(input.peek()) && input.peek() != EOF && isdigit(ch))
	{
	  // Get second character
	  input.get(ch2);

	  // Case 3)
	  if (isspace(input.peek()))
	    {
	      q++;
	      // Perform logical checks and store value
	      if (q<= nr_of_mappings)
		{
		  number = static_cast<int>(ch - '0')* 10 + static_cast<int>(ch2 - '0');
		  try{check_index(number);}
		  catch(int &ex){input.close(); throw ex;}
		  try{check_mapping(number,q);}
		  catch(int &ex){input.close(); throw ex;}
		  reflection[q-1] = number;
		}
	    }

	  // Case 4
	  else if (isdigit(input.peek()))
	    {
	      cerr << "Invalid index in reflector mapping\n";
	      input.close();
	      throw INVALID_INDEX;
	    }
	}

      // Check if max is not exceeded
      if (q>nr_of_mappings)
	{
	  cerr << "Incorrect (odd) number of parameters ";
	  cerr << "in reflector file reflector.rf\n";
	  input.close();
	  throw INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
	}
      
      input.get(ch);
      
    }

  input.close();
  
  if (q < nr_of_mappings)
    {
      if(q%2)
	{
	  cerr << "Incorrect (odd) number of parameters ";
	  cerr << "in reflector file reflector.rf\n";
	  throw INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
	}
      else
	{
	  cerr << "Insufficient number of mappings in ";
	  cerr << "reflector file: reflector.rf\n";
	  throw INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
	}
    }
  return;
}
/* END OF CONSTRUCTOR */



/* DESTRUCTOR */
Reflector::~Reflector()
{
  delete [] reflection;
}
/* END OF DESTRUCTOR */



/* FUNCTION THAT PASSES VALUE THROUGH REFLECTOR */
void Reflector::swap(int &input) {

  for (int j = 0; j < nr_of_mappings; j++)
    {
      if (input == reflection[j])
	{
	  if (j%2)
	    {
	      input = reflection[j-1];
	      return;
	    }
	  else
	    {
	      input = reflection[j+1];
	      return;
	    }
	}
    }
    
  return;
}
/* END OF FUNCTION */



/* FUNCTION THAT CHECKS INDEX */
void Reflector::check_index(int number)
{
  if(number < 0 || number > 25)
    {
      cerr << "Invalid index in reflector mapping\n";
      throw INVALID_INDEX;
    }
  return;
}
/* END OF FUNCTION */



/* FUNCTION THAT CHECKS MAPPING */
void Reflector::check_mapping(int number, int q)
{
 
  for (int i = 0; i < q-1; i++)
    {
      if(reflection[i] == number)
	{
	  cerr << "Invalid reflector mapping\n";
	  throw INVALID_REFLECTOR_MAPPING;
	}
    }
  return;
}
/* END OF FUNCTION */
