#include "trialinfo.ih"

//TrialInfo TrialInfo::computeTargetLimits(string line, TrialInfo trialSet)
void TrialInfo::computeTargetLimits(string& line)
{
  istringstream linedata(line);
  string msgType;
  size_t sentenceStarts; 
  linedata >> msgType >> sentenceStarts;
  
//  size_t timeBefore = 400;
  
  //  TrialInfo::setOnsetTarget(sentenceStarts + TrialInfo::g_targetStarts() + timeBefore); 
  TrialInfo::setOnsetTarget(sentenceStarts + 
		TrialInfo::g_targetStarts()
	); 
  TrialInfo::setTimeIsUp();
  
//   return(trialSet);
}
