#ifndef __RDF_H_
#define __RDF_H_

#include <inputOutput.hpp>
#include <generic.hpp>

namespace rdf{

  int gr(double *rdfArray, int *nframes, double binsize, int nbin, std::vector<double> box, 
    std::vector< std::vector<double> > coord, double cutoff, int nop, int switchVar);

}


#endif // 