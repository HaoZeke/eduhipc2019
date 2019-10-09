#ifndef __INPUTOUTPUT_H_
#define __INPUTOUTPUT_H_

#include <sys/stat.h>
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

// Boost
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

/*! \file inputOutput.hpp
    \brief This header file contains functions for I/O.

    Details.
*/

/*!
 *  \addtogroup io
 *  @{
 */

/*! \brief Handles Input-Output Operations.
 *         This contains general functions for reading in and writing out files.
 *
  Some functions use Boost.

  ### Changelog ###

  - Amrita Goswami [amrita16thaug646@gmail.com]; date modified: Oct 9, 2019
 */

namespace io {

// --------------------------------------------
// INLINE FUNCTIONS

/********************************************/ /**
                                                *  Function for checking if a
                                                *file exists or not.
                                                *  @param[in] name The name of
                                                *the file
                                                ***********************************************/
inline bool file_exists(const std::string &name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

/********************************************/ /**
                                                *  Function for tokenizing line
                                                *strings into words (strings)
                                                *delimited by whitespace. This
                                                *returns a vector with the words
                                                *in it.
                                                *  @param[in] line The string
                                                *containing the line to be
                                                *tokenized
                                                ***********************************************/
inline std::vector<std::string> tokenizer(std::string line) {
  std::istringstream iss(line);
  std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                  std::istream_iterator<std::string>{}};
  return tokens;
}

/********************************************/ /**
                                                *  Function for tokenizing line
                                                *strings into a vector of
                                                *doubles.
                                                *  @param[in] line The string
                                                *containing the line to be
                                                *tokenized
                                                ***********************************************/
inline std::vector<double> tokenizerDouble(std::string line) {
  std::istringstream iss(line);
  std::vector<double> tokens;
  double number;  // Each number being read in from the line
  while (iss >> number) {
    tokens.push_back(number);
  }
  return tokens;
}

// --------------------------------------------

// Gets the total number of timesteps in a lammps trajectory file
int getTotalTimesteps(std::string filename, int *totalSteps);

// Write out the RDF to an output file
int writeRDF(double *rdfArray, double binsize, int nbin,
             std::string filename = "rdf.dat");

}  // namespace io

#endif  // __INPUTOUTPUT_H_