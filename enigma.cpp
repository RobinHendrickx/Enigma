#include <iostream>
#include <fstream>
#include <cstdlib>
#include "errors.h"
#include "rotor.h"
#include "reflector.h"
#include "plugboard.h"

using namespace std;


/* GLOBAL CONSTANT DECLARATIONS */
int const min_nr_of_args = 4;


/* HELPER FUNCTION DECLARATION */
int check_and_convert_input(char ch);



/* MAIN FUNCTION */
int main(int argc, char** argv) {

  //INITIALISE/DECLARE OBJECTS AND VARIABLES
  // Initialise/declare necessary variables
  int const nr_of_rotors = argc - min_nr_of_args;
  Rotor* rotor_array[nr_of_rotors];
  Reflector *refl;
  Plugboard *plugb;
  int rotordirection; // Direction for value to be passed through rotor
  
  // Check if correct number of parameters
  if (argc < min_nr_of_args)
    {
      cerr << "usage: enigma plugboard-file reflector-file "; 
      cerr << "(<rotor-file>)* rotor-positions\n";
      return INSUFFICIENT_NUMBER_OF_PARAMETERS;
    }

  // Create the rotors; Return correct value if exception
  try
    {
      for (int i = 0; i < nr_of_rotors; i++)
	{
	  rotor_array[i] = new Rotor(i, argc, argv);
	}
    }
  catch(int &ex)
    {
      return ex;
    }
  
  // Create the reflector; Return correct value and free memory
  // if exception
  try
    {
      refl = new Reflector(argc,argv);
    }
  catch(int &ex)
    {
      for (int i = 0; i < nr_of_rotors; i++)
	{
	  delete rotor_array[i];
	}
      return ex;
    }

  
  // Create the plugboard; Return correct value and free memory
  // if exception
  try{
    
    plugb = new Plugboard(argc,argv);
   
  }
  catch(int &ex)
    {
      for (int i = 0; i < nr_of_rotors; i++)
	{
	  delete rotor_array[i];
	}
      delete refl; 
      return ex;
    }


  
  // READ IN INPUT AND CALCULATE OUTPUT 
  char ch; // used to read in characters
  int input; // int equivalent of input
  
  cin >> ws;
  cin >> ch;

  // Start of loop
  while (!cin.eof())
    {
      // Check if input is valid; Free memory and return if not
      try
	{
	  input = check_and_convert_input(ch);
	}
      catch(int &ex)
	{
	  for (int i = 0; i < nr_of_rotors; i++)
	    {
	      delete rotor_array[i];
	    }
	  delete refl;
	  delete plugb;
	  return ex;
	}

      // Run through plugboard
      plugb->swap(input);
            
      // Rotate rightmost rotor
      if (nr_of_rotors>0)
	rotor_array[nr_of_rotors-1]->rotate();

      // Check if notch and rotate if necessary
      for (int j = nr_of_rotors-1; j>0; j--)
	{
	  if (rotor_array[j]->check_notch())
	    rotor_array[j-1]->rotate();

	  else
	    break;
	}

      // Run input value forwards through rotors
      rotordirection = 0; 

      for (int j = nr_of_rotors-1; j>=0; j--)
	{
	  rotor_array[j]->pass(input, rotordirection); 
	}
      
      // Run through reflectors
      refl->swap(input);

      // Run input value backwards through rotors
      rotordirection = 1; 
      
      for (int j = 0; j<nr_of_rotors; j++)
	{
	  rotor_array[j]->pass(input, rotordirection); 
	}

      // Run through plugboard
      plugb->swap(input);

      // Convert and output to std output stream
      ch = static_cast<char>(input)+'A';
      cout << ch;	      
      
      // Read in next character
      cin >> ws;
      cin >> ch;
      
    }
  // End of loop

  
  // Free memory and return correct value
  for (int i = 0; i < nr_of_rotors; i++)
    {
      delete rotor_array[i];
    }
  delete refl;
  delete plugb;
  return NO_ERROR;
}
/* END OF MAIN FUNCTION */




/* HELPER FUNCTION WHICH CHECKS AND CONVERTS INPUT CHAR TO INT */
int check_and_convert_input(char ch){

 if (ch > 'Z' || ch < 'A')
	{
	  cerr << ch << " is not a valid input character ";
	  cerr << "(input characters must be upper case letters A-Z)!\n";
	  throw INVALID_INPUT_CHARACTER;
	}
  return static_cast<int>(ch -'A');
}
/* END OF FUNCTION */
