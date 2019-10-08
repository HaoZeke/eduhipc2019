// Standard Library
#include <iostream>
#include <memory>
#include <numeric>

// Internal Libraries
#include <generic.hpp>
#include <inputOutput.hpp>
#include <rdf.hpp>


int main() {
  // -------------------------------------------- // User-input
  // Maybe read the inputs in more elegantly
  std::string lammpsInputTraj = "./../../data/liq-mW"; // relative path
  // All steps start from 1, and are the number, not the actual timestep value
  int numCalcSteps = 1;              // Number of steps for which to do the calculation 1000
  int stepGap = 1;                      // Gap between steps
  // Make equiliSteps one if you don't want to skip anything
  int equiliSteps = 50;                 // Number of steps to skip (last frame exclusive)
  // RDF
  double binsize = 0.01;                // In Angstroms
  double cutoff = 12;                   // Cutoff for the RDF (should be less than half the box)
  // -------------------------------------------- // Variables
  int totalSteps = 0;                   // Starts from 1
  std::vector<int> box;                 // Box lengths
  int nop;                              // Total number of particles in the box
  int dim;                              // Number of dimensions
  // Vector of vectors containing the coordinates
  std::vector< std::vector<double> > coord; 
  std::vector<double> iatomCoord; // Coordinates of a particular iatom
  // File handling and I/O
  std::unique_ptr<std::ifstream> dumpFile;
  dumpFile = std::make_unique<std::ifstream>(lammpsInputTraj);
  int targetFrame;                      // Current frame to process
  int currentFrame;                     // Current frame number
  std::string line;                     // Current line  being read in
  int atomIndex;                        // Column number from which x y z coordinates starts
  bool fillCoord;                       // true if the frame is to be processed
  std::vector<std::string> tokens;      // Vector containing word tokens
  std::vector<double> numbers;          // Vector containing type double numbers
  // -------------------------------------------- // RDF Specific Variables
  int nbin;                             // Number of bins
  int switchVar;                        // equal to 0 for init, 1 for adding and 2 for final normalization
  int nframes;                          // Number of frames; used for normalizing g(r)
  // -------------------------------------------- // Main logic

  // Read in the total number of steps.
  // In this case, nop does not change throughout
  io::getTotalTimesteps(lammpsInputTraj, &totalSteps);

  // ----
  // Allocate the RDF array
  nbin = (int)(cutoff/binsize) + 1;
  double rdf[nbin]; // init
  // Initialize the RDF
  switchVar = 0;
  rdf::gr(rdf, &nframes, binsize, nbin, coord, nop, switchVar);
  // ----

  // Check to make sure that the user has entered valid  steps
  if (equiliSteps+numCalcSteps*equiliSteps > totalSteps){
    // do error handling later
    std::cerr<<"You have entered an unfeasible number of calculation or equilibrium steps.\n";
    std::cerr<<"The total number of frames in the trajectory file is "<<totalSteps<<"\n";
    return 1;
  } // end of check

  // Open the file
  if (dumpFile->is_open()){ 

    // Loop through timesteps
    // ---------------------------
    // Skip the equilibrium steps
    currentFrame = 0; // init

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

      if (currentFrame==equiliSteps){break;}
    } // End of while loop searching for targetFrame

    targetFrame = equiliSteps; // init (includes the last frame of equiliSteps)

    // Loop through the rest of the traj file
    for (int iframe=1; iframe<=numCalcSteps; iframe++){

      getline((*dumpFile), line); // Timestep Number
      getline((*dumpFile), line); // ITEM: NUMBER OF ATOMS
      getline((*dumpFile), line); // Number of atoms
      nop = std::stoi(line);      // Update number of atoms
      getline((*dumpFile), line); // ITEM: BOX BOUNDS pp pp pp
      // Get the box lengths
      box.clear();
      while (std::getline((*dumpFile), line)) {
        // Read in lines and tokenize them
        tokens = io::tokenizer(line);
        // Break out once box lengths have been read in
        if (tokens[0].compare("ITEM:") == 0){break;} // ITEM: ATOMS id type x y z
        // Get the box lengths
        numbers = io::tokenizerDouble(line);
        // Add to the box length vector
        box.push_back(numbers[1]-numbers[0]); 
      } // end of getting the box lengths

      dim = box.size(); // Number of dimensions
      // ---
      // Find the index from which the coordinates will be filled
      // ITEM: ATOMS id type x y z
      tokens = io::tokenizer(line);
      auto it = std::find(tokens.begin(), tokens.end(), "x");
      if (it != tokens.end()){
        int index = std::distance(tokens.begin(), it);
        atomIndex = index-2;
      }
      else{
        std::cerr << "Invalid traj file perhaps!\n";
        return 1;
      }
      // ---

      fillCoord = false;
      // If currentFrame is not equal to targetFrame, skip
      if (currentFrame!=targetFrame){
        // Go through the atom coordinate lines
        for (int iatom=1; iatom<=nop; iatom++){
          getline((*dumpFile), line); // 1 1 0 0 0 etc 
        } // end of loop through atoms
      } // end of skipping when targetFrame is not equal to currentFrame
      // Fill up the coordinate array
      else{
        fillCoord = true;
        // Empty the vector of vectors
        std::vector< std::vector<double> > emptyVec;
        coord.swap( emptyVec );

        // Go through the atom coordinate lines
        for (int iatom=1; iatom<=nop; iatom++){
          getline((*dumpFile), line); // 1 1 0 0 0 etc 
          // Fill stuff
          iatomCoord.resize(0);
          numbers = io::tokenizerDouble(line);
          // Fill from atomIndex onwards
          for (int k=atomIndex; k<atomIndex+dim; k++){
            iatomCoord.push_back(numbers[k]);
          } // end of update of iatomCoord
          // Update coord
          coord.push_back(iatomCoord); 
        } // end of loop through atoms
      } // end of filling up the coordinate array
      
      getline((*dumpFile), line); // ITEM: TIMESTEP
      currentFrame++; // Update the frame number

      // Update the next value of targetFrame
      targetFrame = equiliSteps+iframe*stepGap;

      // ---------------------------
      // Do stuff if the coordinates have been filled
      // Process targetFrame
      if (fillCoord){
        //
      } // End of processing targetFrame
      // ---------------------------
    } // end of loop through calculation frames


  } // end of lammps traj open statement

  dumpFile->close(); // Close the lammps file

  // -------------------------------------------- // Write out the RDF

  // -------------------------------------------- // Fin

  std::cout << "Welcome to the Black Parade \n";
  return 0;
}