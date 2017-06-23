#include "trialinfo.ih"

//TrialInfo TrialInfo::updateCurrentTrial(TrialInfo trialSet, string line)
void TrialInfo::updateCurrentTrial(string& line, size_t timeBefore)
{
  TrialInfo::setCurrentTr(TrialInfo::g_currentTr() + 1);
  
  if (TrialInfo::g_currentTr() == TrialInfo::g_trialIN())
    TrialInfo::computeTargetLimits(line);
  
  TrialInfo::setFix(1);
	TrialInfo::setExportStarts(timeBefore);
// 	cout << TrialInfo::g_currentTr() << ' ' << TrialInfo::g_subject() << ' ';
//  return(TrialInfo);subject
}
