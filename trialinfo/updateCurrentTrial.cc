#include "trialinfo.ih"

//TrialInfo TrialInfo::updateCurrentTrial(TrialInfo trialSet, string line)
void TrialInfo::updateCurrentTrial(string& line)
{
  TrialInfo::setCurrentTr(TrialInfo::g_currentTr() + 1);
  
  if (TrialInfo::g_currentTr() == TrialInfo::g_trialIN())
    TrialInfo::computeTargetLimits(line);
  
  TrialInfo::setFix(1);
  
//  cout << TrialInfo::g_currentTr() << ' ';
//  return(TrialInfo);
}