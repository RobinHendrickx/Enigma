/* ROTOR CLASS DECLARATION */
class Rotor
{
 public:
  // Constructor
  // Input: rotorpos = rotor position (0 = leftmost)
  // argc = number of command line arguments
  // argv = pointer to character arrays of input arguments
  Rotor(int rotorpos, int argc, char** argv); 

  // Destructor
  ~Rotor(); 

  // Pass value through rotor
  // Input: input = value to be passed through
  // direction = direction (0 indicates forwards, 0 backwards)
  void pass(int &input, int direction);

  // Rotate rotor
  void rotate();

  // Check if notch on top
  // Output: true if notch on top; false otherwise
  bool check_notch();

 private:
  int position; // Position with respect to others (0 = leftmost)
  int const nr_of_rotors; // Number of rotors 
  int const nr_of_mappings = 26; // Number of rotor mappings
  int *rotation; // Mapping of rotors
  int notch; // Position of notch
  int top; // Value currently on top of rotor

  void check_index(int number, int indicator); // Check index
  void check_mapping(int number, int q); // Check mapping
};

/* END OF ROTOR CLASS DECLARATION */
