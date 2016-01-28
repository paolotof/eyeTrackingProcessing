#include "main.ih"

TrialInfo endTrial(ifstream& trialInfoFile, TrialInfo trialSet)
{
  trialSet.resetBinsCounter(); // in case it hasn't been reset before
  string trialInfo;
  getline(trialInfoFile, trialInfo); 
  trialSet.setFix(1);
  trialSet = trialSetup(trialInfo, trialSet);
//  cout << trialSet.g_subject() << ' ';
//   return(trialSetup(trialInfo, trialSet));
  return(trialSet);
} 
