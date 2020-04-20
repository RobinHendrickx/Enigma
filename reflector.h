/* REFLECTOR CLASS DECLARATION */
class Reflector
{
 public:
  // Constructor
  // Input: argc = number of command line arguments
  // argv = pointer to character arrays of input arguments
  Reflector(int argc, char** argv);

  // Destructor
  ~Reflector();

  // Run value through reflector
  // Input: value to be put through
  void swap(int &input);
  
 private:
  int* reflection; // Value mapping of the reflector 
  int const nr_of_mappings = 26; // Number of mappings
  
  void check_index(int number); // Check index
  void check_mapping(int number,int q); // Check mapping
};
/* END OF REFLECTOR CLASS DECLARATION */
