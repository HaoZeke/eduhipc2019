#ifndef __RDF_H_
#define __RDF_H_

#include <inputOutput.hpp>
#include <generic.hpp>

/*! \file mol_sys.hpp
    \brief This contains code specific to radial distribution function calculations
    
    Details.
*/

/*!
 *  \addtogroup rdf
 *  @{
 */

/*! \brief Defines RDF-specific functions.
 *         This calculates the bulk radial distribution function.
 *
 The radial distribution function \f$g(r)\f$, or pair distribution function. It can be used to illuminate
 features of short-range and long-range order. 
  ### Changelog ###

  - Amrita Goswami [amrita16thaug646@gmail.com]; date modified: Oct 9, 2019
 */

namespace rdf{

  int gr(double *rdfArray, int *nframes, double binsize, int nbin, std::vector<double> box, 
    std::vector< std::vector<double> > coord, double cutoff, int nop, int switchVar);

}


#endif // 