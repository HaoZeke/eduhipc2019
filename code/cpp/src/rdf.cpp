#include <rdf.hpp>

/********************************************/ /**
 *  Calculates the RDF
 If switchVar=0, initializes the RDF
 If swicthVar=1, adds frames to the RDF
 If switchVar=2, normalizes the RDF
 ***********************************************/
int rdf::gr(double *rdfArray, int *nframes, double binsize, int nbin, 
    std::vector< std::vector<double> > coord, int nop, int switchVar){
  //

  // -------------------------------------// Init
  if (switchVar==0){
    //
    *nframes = 0; // Init of number of frames
    // Fill the entire array with zeros
    for (int ibin=0; ibin<nbin; ibin++){
      rdfArray[ibin] = 0;
    } // end of looping through every bin
    return 0;
  } // end of init
  // -------------------------------------// Accumulation
  else if (switchVar==1){
    //
    return 0;
  } // end of accumulation
  // -------------------------------------// Normalization
  else if (switchVar==2){
    //
    return 0;
  } // end of normalization
  // or there is some error
  else{
    std::cerr<<"You entered the wrong value of switchVar.\n";
    return 1;
  }

}