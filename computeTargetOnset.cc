#include "main.ih"
/*
 * 
 * Compute onset of the target word adding the target onset time to 
 * the value of the onset of the 'soundStim' eyetracking trigger
 * 
 * Since in some analysis we use the 200 ms before the onset of the 
 * target word as a baseline, 200 ms are subtracted from the values of 
 * the target onset. This is then averaged in R and constitutes the 
 * baseline
 * 
 */ 

TrialInfo computeTargetOnset(TrialInfo trialSet, string line)
{
  trialSet.setCurrentTr(trialSet.g_currentTr() + 1);
  
  if (trialSet.g_currentTr() == trialSet.g_trialIN())
  {
    istringstream linedata(line);
    string msgType;
    size_t sentenceStarts; 
    linedata >> msgType >> sentenceStarts;
    
    // size_t msBeforeTarget = 200;
    // trialSet.setOnsetTarget(sentenceStarts + trialSet.g_targetStarts() - msBeforeTarget); // decided to leave it as it was and manage it from the selection of included data // paol8 5-11-14, not anymore since then we have to make a subtraction every line that is read, whereas like this it is done only once
    
    trialSet.setOnsetTarget(sentenceStarts + trialSet.g_targetStarts()); 
  }
  
  trialSet.setFix(1);
  return(trialSet);
}