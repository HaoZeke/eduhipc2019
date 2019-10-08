#ifndef __INPUTOUTPUT_H_
#define __INPUTOUTPUT_H_

#include <vector>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <array>
#include <memory>
#include <iostream>

namespace io{

  // --------------------------------------------
  // INLINE FUNCTIONS

  // Checks whether a file exists or not
  inline bool file_exists (const std::string& name) {
      struct stat buffer;   
      return (stat (name.c_str(), &buffer) == 0); 
  }

  // Function for tokenizing line strings into words (strings) delimited
  // by whitespace. This returns a vector with the words in it
  inline std::vector<std::string> tokenizer(std::string line)
  {
    std::istringstream iss(line);
    std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, 
                    std::istream_iterator<std::string>{}};
    return tokens;
  }

  // Function for tokenizing line strings into a vector of doubles
  inline std::vector<double> tokenizerDouble(std::string line)
  {
    std::istringstream iss(line);
    std::vector<double> tokens;
    double number; // Each number being read in from the line
    while (iss >> number) {tokens.push_back(number);}
    return tokens;
  }

  // --------------------------------------------

  // Gets the total number of timesteps in a lammps trajectory file
  int getTotalTimesteps(std::string filename, int *totalSteps);

}


#endif // __INPUTOUTPUT_H_