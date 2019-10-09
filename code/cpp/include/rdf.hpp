#ifndef __RDF_H_
#define __RDF_H_

#include <generic.hpp>
#include <inputOutput.hpp>

/*! \file rdf.hpp
    \brief This contains code specific to radial distribution function
   calculations

    Details.
*/

/*!
 *  \addtogroup rdf
 *  @{
 */

/*! \brief Defines RDF-specific functions.
 *         This calculates the bulk radial distribution function.
 *
 The radial distribution function \f$g(r)\f$, or pair distribution function. It
 can be used to illuminate features of short-range and long-range order. The RDF
 is the probability of finding a particle at a distance of \f$r\f$ from a tagged
 reference particle, relative to that of an ideal gas. For a system of \f$N\f$
 particles, the pair correlation function for \f$N(N-1)\f$ pairs is:

  \f[
  \rho_N^{(2)}(r,r') = ⟨\sum_{i=1}^{N} \sum_{j=1,j \neq i}^{N} \delta
 (r-r_i) \delta (r'-r_j) \langle \f]

 The code essentially bins distances between pairs of particles, and normalizes
 the resulting histogram is normalized with respect to an ideal gas. The
 algorithm for the calculation of \f$g(r)\f$ may be divided into the following
 steps:

 1. <b>Initialization:</b> The \f$g(r)\f$ array is initialized to zero.
 2. <b>Sampling:</b> The histogram is added to for a particular bin, if the
 distance of a pair of atoms falls within the \f$r\f$ associated with the bin.
 3. <b>Normalization:</b> Every bin of the \f$g(r)\f$ array is normalized by the
 product of the number of ideal gas particles in that bin, and the number of
 particles and number of frames.

  ### Changelog ###

  - Amrita Goswami [amrita16thaug646@gmail.com]; date modified: Oct 9, 2019
 */

namespace rdf {

// Calculates the RDF for a bulk volume with particles of a single type only
int gr(double *rdfArray, int *nframes, double binsize, int nbin,
       std::vector<double> box, std::vector<std::vector<double> > coord,
       double cutoff, int nop, int switchVar);

}  // namespace rdf

#endif  //