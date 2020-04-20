/* PLUGBOARD CLASS DECLARATION */
class Plugboard
{
 public:
  // Constructor
  // Input: argc = number of command line arguments
  // argv = pointer to character arrays of input arguments
  Plugboard(int argc, char** argv);

  // Destructor
  ~Plugboard();

  // Run value through plugboard
  // Input: value to be put through
  void swap(int &input);

 private:
  int* plug; // Mapping of plugboard
  int const max_mappings = 26; // Max amount of mappings
  int length; // Actual amount of mappings

  void check_index(int number); // Check index
  void check_mapping(int number,int q); // Check mapping
};
/* END OF PLUGBOARD CLASS DECLARATION */
