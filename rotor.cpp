#include <iostream>
#include <fstream>
#include "errors.h"
#include "rotor.h"

using namespace std;

/* CONSTRUCTOR */
Rotor::Rotor(int rotorpos, int argc, char** argv): position(rotorpos), nr_of_rotors(argc-4) {

  rotation = new int[nr_of_mappings];
  
  // PART 1: GET ROTOR MAPPINGS AND NOTCH POSITION
  int number;
  char ch;
  char ch2;
  ifstream input;

  // Open correct input file and check if opened correctly
  input.open(*(argv+3+rotorpos));
      
  if (input.fail())
    {
      cerr << "Input file couldn't be opened.\n";
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
      // Case 1)
      if (!isspace(ch) && !isdigit(ch))
	{
	  cerr << "Non-numeric character for mapping in rotor ";
	  cerr << "file rotor.rot\n";
	  input.close();
	  throw NON_NUMERIC_CHARACTER;
	}

      // Case 2)
      else if (isspace(input.peek()) && isdigit(ch))
	{
	  q++; // Increment counter

	  // Check if max input not reached
	  if (q <= nr_of_mappings+1)
	    {
	      // perform logical checks
	      number = static_cast<int>(ch - '0');
	      try{check_index(number, 0);}
	      catch(int &ex){input.close(); throw ex;}
	      try{check_mapping(number, q);}
	      catch(int &ex){input.close(); throw ex;}

	      // Store digit in correct place and update counter 
	      if (q <= nr_of_mappings)
		rotation[q-1] = number;
	      else if (q == nr_of_mappings+1)
		notch = number;
	    }
	}

	
      // At least 2 digit char values, possibly more  
      else if (isdigit(input.peek()) && input.peek() != EOF && isdigit(ch))
	{
	  // Get second character
	  input.get(ch2);

	  // Case 3) 
	  if (isspace(input.peek())) {
	    q++;
	    if(q<=nr_of_mappings+1)
	      {
		// Perform logical checks and store value
		number = static_cast<int>(ch - '0')* 10 + static_cast<int>(ch2 - '0');
		try{check_index(number, 0);}
		catch(int &ex){input.close(); throw ex;}
		try{check_mapping(number,q);}
		catch(int &ex){input.close(); throw ex;}
		if (q <= nr_of_mappings)
		  rotation[q-1] = number;
		else if (q == nr_of_mappings + 1)
		  notch = number;
	      }
	  }
	  
	  // Case 4)
	  else if (isdigit(input.peek()))
	    {
	      cerr << "Invalid index in rotor configuration files\n";
	      input.close();
	      throw INVALID_INDEX;	    
	    }
	}

      input.get(ch);

      // Check if max is not exceeded
      if(q > nr_of_mappings+1)
	{
	  cerr << "Too many input arguments in rotor.rot\n";
	  input.close();
	  throw INVALID_ROTOR_MAPPING;
	}
    }
  // end of loop
  
  // Close input file
  input.close();

  // Check if all inputs were mapped
  if (q != nr_of_mappings+1) {
    cerr << "Not all inputs mapped in rotor file: rotor.rot\n";
    throw INVALID_ROTOR_MAPPING;
  }


  
  // PART 2: GET POSITION OF ROTOR
  // Open correct file
  input.open(*(argv+argc-1));
      
  if (input.fail())
    {
      cerr << "Input file couldn't be opened.\n";
      throw ERROR_OPENING_CONFIGURATION_FILE;
    }

  q = 0; // Reset counter
  input.get(ch);

  // Same 4 cases for distinct inputs
  // Start of loop
  while(!input.eof() && q <= argc-4)
    {
      // Case 1)
      if (!isspace(ch) && !isdigit(ch))
	{
	  cerr << "Non-numeric character in rotor positions file rotor.pos\n";
	  input.close();
	  throw NON_NUMERIC_CHARACTER;
	}

      // Case 2)
      else if (isspace(input.peek()) && isdigit(ch))
	{ 
	  if(q == position)
	    {
	      // Perform logical checks and store value
	      number = static_cast<int>(ch - '0'); 
	      try{check_index(number, 1);}
	      catch(int &ex){input.close(); throw ex;}
	      top = number;
	    }
	  q++;
	}
      
      // at least 2 digits, possibly more
      else if (isdigit(input.peek()) && input.peek() != EOF && isdigit(ch))
	{
	  // Get second character
	  input.get(ch2);

	  // case 3)
	  if(q == position && isspace(input.peek()))
	    {
	      // Perform logical checks and store value
	      number = static_cast<int>(ch - '0')* 10 + static_cast<int>(ch2 - '0');
	      try{check_index(number, 1);}
	      catch(int &ex){input.close(); throw ex;}
	      top = number;  
	    }
	  
	  // Case 4)
	  else if (isdigit(input.peek()))
	    {
	      cerr << "Invalid index in rotor configuration files\n";
	      input.close();
	      throw INVALID_INDEX;	    	    
	    }
	  q++;
	}

      input.get(ch);
    }

  if (q < nr_of_rotors)
    {
      cerr << "No starting position for rotor ";
      cerr << position;
      cerr << " in rotor position file: rotor.pos\n";
      input.close();
      throw NO_ROTOR_STARTING_POSITION;
    }
  
  input.close();
}
/* END OF CONSTRUCTOR */



/* DESTRUCTOR */
Rotor::~Rotor()
{
  delete [] rotation;
}
/* END OF DESTRUCTOR */



/* MEMBER FUNCTION WHICH LETS A VALUE PASS THROUGH THE ROTOR */
void Rotor::pass(int &input,  int direction) {

  // The value on top and the input value determine which mapping is taken
  // Take the positive modulus to ensure correct mapping
  // Forwards mapping
  if (direction == 0)
    {      
      int value = (top+input)%nr_of_mappings;
      input = ((rotation[value]-top)%nr_of_mappings+nr_of_mappings)%nr_of_mappings;
      return;
    }
  // Backwards mapping
  else
    {
      int value = (top+input)%nr_of_mappings;
      for (int i = 0; i < nr_of_mappings; i++)
	{
	 if (rotation[i] == value)
	    {
	      input = ((i-top)%nr_of_mappings+nr_of_mappings)%nr_of_mappings;
	      return;
	    }
	}
    }  
}
/* END OF FUNCTION */



/* FUNCTION WHICH ROTATES THE ROTOR */
void Rotor::rotate() {

  // Rotate the top by one
  top = (top+1)%nr_of_mappings;
  
  return;
}
/* END OF FUNCTION */



/* FUNCTION WHICH CHECKS IF A NOTCH IS REACHED */
bool Rotor::check_notch() {

  // check if notch is reached
  if (top == notch)
    {
      return true;
    }
  return false;
}
/* END OF FUNCTION */



/* FUNCTION WHICH CHECKS INDEX */
void Rotor::check_index(int number, int indicator)
{
  // Indicator check if we are checking for rotor.rot or rotor.pos
  // 0 indicates we are checking for rotor.rot, other value rotor.pos
  if(number < 0 || number > 25)
    {
      if (indicator == 0)
	{
	  cerr << "Invalid index in rotor.rot\n";
	  throw INVALID_INDEX;
	}
      else
	{
	  cerr << "Invalid index in rotor.pos\n";
	  throw INVALID_INDEX;
	}
    }
  return;
}
/* END OF FUNCTION */



/* FUNCTION WHICH CHECKS MAPPING */
void Rotor::check_mapping(int number, int q){

  
  if (q <= nr_of_mappings)
    {
      for (int i = 0; i<q-1; i++)
	{
	  if(rotation[i] == number)
	    {
	      cerr << "Invalid mapping of input " << q-1;
	      cerr << " to output " << number << " (output " << number;
	      cerr << " is already mapped to from input " << i << ") in rotor file rotor.rot\n";
	      throw INVALID_ROTOR_MAPPING;
	    }
	}
    }
  return;
}
/* END OF FUNCTION */
  
