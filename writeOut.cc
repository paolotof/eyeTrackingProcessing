#include "main.ih"

TrialInfo writeOut(Dataline eye, ofstream& outputfile, TrialInfo trialSet)
{
  // if time is right: average and define belonging to ROIs  
    
    
  double timeDif = eye.g_time() - trialSet.g_targetOnset();

  outputfile << trialSet.g_subject() 
    << '\t' << timeDif
    << '\t' << trialSet.totalBins()
    << '\t' << trialSet.g_condition() 
    << '\t' << trialSet.g_trialIN() 
    << '\t' << trialSet.g_target() 
    << '\t' << trialSet.g_subCond() 
    << '\t' << eye.g_time() 
    << '\t' << eye.g_psize()
    << '\t' << trialSet.g_Fix()
    << '\n';
      
  return(trialSet);
}
