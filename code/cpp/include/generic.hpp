#ifndef __GENERIC_H_
#define __GENERIC_H_

#include <math.h>  
#include <array>
#include <vector> 

namespace gen{

	// Generic function for getting the unwrapped distance
  inline double periodicDist(std::vector< std::vector<double> > coord, std::vector<double> box, int iatom, int jatom){
    int dim = coord[0].size(); // Number of dimensions
    std::vector<double> dr;
    double r2 = 0.0; // Squared absolute distance

    dr.resize(dim); // Resize 

    // Get the squared absolute distance
      for (int k = 0; k < dim; k++) {
        // Get the relative distance
        dr[k] = fabs(coord[iatom][k]-coord[jatom][k]);
        // Correct for periodicity
        dr[k] -= box[k] * round(dr[k] / box[k]);
        r2 += pow(dr[k], 2.0);
      } 

    return sqrt(r2);
  }

}


#endif // __GENERIC_H_
