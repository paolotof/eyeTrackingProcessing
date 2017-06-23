#include "trialinfo.ih"

//TrialInfo TrialInfo::resetAndUpdate(ifstream& trialInfoFile)
void TrialInfo::resetAndUpdate(ifstream& trialInfoFile)
{
  TrialInfo::resetBinsCounter(); // in case it hasn't been reset before
  TrialInfo::setFix(1);
  string trialInfo;
  getline(trialInfoFile, trialInfo); 
  TrialInfo::extractInfo(trialInfo);
//  return(trialSet);
} 
