#include <rdf.hpp>

/********************************************/ /**
 *  Function for calculating the RDF or \f$g(r)\f$
 *
 * The is calculated for a bulk system, containing identical particles of a
 single type. Depending on the value of switchVar, either initialization,
 sampling or normalization of the \f$g(r)\f$ is performed.
 *
 *  @param[in] rdfArray The array for holding the histogram for the \f$g(r)\f$
 values
 *  @param[in] nframes The total number of frames for which the \f$g(r)\f$ is
 sampled
 *  @param[in] binsize The user-specified bin-width or bin size
 *  @param[in] nbin The total number of bins in the \f$g(r)\f$ histogram
 *  @param[in] box The simulation box lengths, required for calculating the
 total volume
 *  @param[in] coord A vector of vectors, holding the coordinates of the
 particles
 *  @param[in] cutoff This is the maximum length upto which the \f$g(r)\f$ will
 be calculated, and should be less than half the box length
 *  @param[in] nop The total number of particles in the simulation box
 *  @param[in] switchVar Int whose value determines whether initialization (0),
 sampling (1) or normalization (2) will be performed
 *  \return an int value of 0 (success) or 1 (failure)
 ***********************************************/
int rdf::gr(double *rdfArray, int *nframes, double binsize, int nbin,
            std::vector<double> box, std::vector<std::vector<double>> coord,
            double cutoff, int nop, int switchVar) {
  //
  double r_ij;            // Distance between iatom and jatom
  int ibin;               // Current bin being filled
  double binVolume;       // Volume between the i^th and (i+1)^th bins
  double rho;             // Number density
  double boxVolume;       // Total box volume
  double nIdeal;          // Number of ideal gas particles in the binVolume
  double pi = 3.14159265; // Value of pi
  //
  double r2 = 0.0;          // Squared distance
  std::vector<double> dist; // Distance

  // -------------------------------------// Init
  if (switchVar == 0) {
    //
    *nframes = 0; // Init of number of frames
    // Fill the entire array with zeros
    for (int ibin = 0; ibin < nbin; ibin++) {
      rdfArray[ibin] = 0;
    } // end of looping through every bin
    return 0;
  } // end of init
  // -------------------------------------// Accumulation
  else if (switchVar == 1) {
    *nframes = *nframes + 1; // Add to the number of accumulated frames
    double somethinBig[nop][3];
    for (int i = 0; i < nop; i++) {
      for (int j = 0; j < 3; j++) {
        somethinBig[i][j] = coord[i][j];
      }
    }
#pragma acc kernels
    //  Loop over all pairs of atoms

    for (int iatom = 0; iatom < nop - 1; iatom++) {
      // Loop through jatom
      for (int jatom = iatom + 1; jatom < nop; jatom++) {
        // ---
        // Get the periodic distance r_ij
        dist.resize(box.size());
        r2 = 0.0;
        for (int k = 0; k < box.size(); k++) {
          dist[k] = somethinBig[iatom][k] - somethinBig[jatom][k];
          // Apply PBCs
          dist[k] -= box[k] * round(dist[k] / box[k]);
          r2 += pow(dist[k], 2);
        }
        r_ij = sqrt(r2);
        // r_ij = gen::periodicDist(somethinBig, box, iatom, jatom);
        // ---

        // Only add if r_ij is within the cutoff
        if (r_ij < cutoff) {
          ibin = (int)(r_ij / binsize);
          rdfArray[ibin] += 2;
        } // end of check

      } // end of loop through jatom
    }   // end of loop through every iatom

    return 0;
  } // end of accumulation
  // -------------------------------------// Normalization
  else if (switchVar == 2) {
    // Calculating the number density
    // Get the box volume
    boxVolume = 1;
    for (int k = 0; k < box.size(); k++) {
      boxVolume = boxVolume * box[k];
    }
    rho = nop / (boxVolume); // Number density

    // Normalize the RDF
    for (int ibin = 0; ibin < nbin; ibin++) {
      binVolume =
          (pow((ibin + 1), 3.0) - pow((ibin), 3.0)) * pow((binsize), 3.0);
      nIdeal =
          (4. / 3.) * pi * binVolume * rho; // Number of ideal gas particles
      // Normalize
      rdfArray[ibin] = rdfArray[ibin] / ((*nframes) * nop * nIdeal);
    } // end of loop through all bins
    return 0;
  } // end of normalization
  // or there is some error
  else {
    std::cerr << "You entered the wrong value of switchVar.\n";
    return 1;
  }
}
