#include <inputOutput.hpp>

/********************************************/ /**
 *  Function for getting the total number of timesteps from a lammps trajectory.
 This also includes 'incomplete' frames, so it should be used with caution.
 *  @param[in] filename The path of the LAMMPS trajectory file
 *  @param[in] totalSteps Pointer to the total number of steps or frames in the
 trajectory
 *  \return an int value of 0 (successful file opening) or 1 (error)
 ***********************************************/
int io::getTotalTimesteps(std::string filename, int *totalSteps) {
  std::unique_ptr<std::ifstream> dumpFile;
  dumpFile = std::make_unique<std::ifstream>(filename);
  std::string line;                 // Current line being read in
  std::vector<std::string> tokens;  // Vector containing word tokens
  int currentFrame = 0;             // Current frame being read in

  // Check if the lammps trajectory exists or not
  if (!(io::file_exists(filename))) {
    std::cerr
        << "Fatal Error: The file does not exist or you gave the wrong path.\n";
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
  if (dumpFile->is_open()) {
    // ----------------------------------------------------------
    // At this point we know that the dumpfile is open
    // This loop goes through all the steps
    while (std::getline((*dumpFile), line)) {
      // Read in lines and tokenize them
      tokens = io::tokenizer(line);
      // Find out which timestep number
      // you are inside
      if (tokens[0].compare("ITEM:") == 0) {
        if (tokens[1].compare("TIMESTEP") == 0) {
          // Now you are in a new timestep. Update frame number
          currentFrame++;
        }
      }

    }  // End of while loop
  }    // End of the open file if statement
       // ----------------------------------------------------------

  // Update the total number of timesteps
  *totalSteps = currentFrame;

  dumpFile->close();
  return 0;
}

/********************************************/ /**
                                                *  Writes out a file containing
                                                *the r and g(r) values
                                                *  @param[in] rdfArray The array
                                                *containing the calculated
                                                *\f$g(r)\f$
                                                *  @param[in] binsize The
                                                *bin-width or bin size of each
                                                *bin, which is user-defined
                                                *  @param[in] nbin The total
                                                *number of bins in the
                                                *\f$g(r)\f$ histogram
                                                *  @param[in] filename (Optional
                                                argument) Specifies a
                                                user-defined file output name,
                                                which will be saved inside a
                                                directory called 'output' in the
                                                top-level directory of the code
                                                folder. \return an int value of
                                                0 (success) or 1
                                                *(error)
                                                ***********************************************/
int io::writeRDF(double *rdfArray, double binsize, int nbin,
                 std::string filename) {
  std::ofstream outputFile;
  double r;  // Distance value
  // ----------------
  // Otherwise create file
  // Create output dir if it doesn't exist already
  const char *path = "../../output";  // relative to the build directory
  boost::filesystem::path dir(path);
  if (boost::filesystem::create_directory(dir)) {
    std::cerr << "Output directory created\n";
  }
  // ----------------
  // Write output to file inside the output directory
  outputFile.open("../../output/" + filename);

  //  Comment line
  // 0.1  3
  // O.2  5

  // Write the comment line
  // Write out the number of atoms
  outputFile << "# r  g(r)\n";

  // Write out the RDF values
  // Loop through the bins
  for (int ibin = 0; ibin < nbin; ibin++) {
    r = binsize * (ibin + 0.5);  // Calculate the r value
    // Write out the RDF
    outputFile << r << " " << rdfArray[ibin] << "\n";
  }  // end of loop through all bins

  // Once the rings have been printed, exit
  return 0;
}