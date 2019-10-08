#include <inputOutput.hpp>

/********************************************/ /**
 *  Function for getting the total number of timesteps from a lammps trajectory
 ***********************************************/
int io::getTotalTimesteps(std::string filename, int *totalSteps)
{
  std::unique_ptr<std::ifstream> dumpFile;
  dumpFile = std::make_unique<std::ifstream>(filename);
  std::string line;                    // Current line being read in
  std::vector<std::string> tokens;     // Vector containing word tokens
  int currentFrame=0;                  // Current frame being read in
  
  // Check if the lammps trajectory exists or not
  if(!(io::file_exists (filename))){
    std::cerr<<"Fatal Error: The file does not exist or you gave the wrong path.\n";
    // Throw exception?
    return 1;
  }

  // The format of the LAMMPS trajectory file is:
  // ITEM: TIMESTEP
  // 0
  // ITEM: NUMBER OF ATOMS
  // 4096
  // ITEM: BOX BOUNDS pp pp pp
  // -7.9599900000000001e-01 5.0164000000000001e+01
  // -7.9599900000000001e-01 5.0164000000000001e+01
  // -7.9599900000000001e-01 5.0164000000000001e+01
  // ITEM: ATOMS id type x y z 
  // 1 1 0 0 0 etc
  if(dumpFile->is_open()){
    // ----------------------------------------------------------
    // At this point we know that the dumpfile is open
    // This loop goes through all the steps
    while (std::getline((*dumpFile), line)) {
      // Read in lines and tokenize them
      tokens = io::tokenizer(line);
      // Find out which timestep number
      // you are inside
      if (tokens[0].compare("ITEM:") == 0){
        if (tokens[1].compare("TIMESTEP") == 0){
          // Now you are in a new timestep. Update frame number
          currentFrame++;
       }
      }

    } // End of while loop
  } // End of the open file if statement
    // ----------------------------------------------------------

  // Update the total number of timesteps
  *totalSteps = currentFrame;

  dumpFile->close();
  return 0;

}