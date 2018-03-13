#include "trialinfo.ih"
/* this function was supposed to set the starting and ending limits to extract
 * but now it only parses the line with the information of the beginning of the 
 * trial and then set the beginning of the target. 
 * Start and end of export interval are set by setExportStarts.cc
 * It is a bit of a waste but I do not have time to fix it right now
*/

//TrialInfo TrialInfo::computeTargetLimits(string line, TrialInfo trialSet)
void TrialInfo::computeTargetLimits(string& line)
{
  istringstream linedata(line);
  string msgType;
  size_t sentenceStarts; 
  linedata >> msgType >> sentenceStarts;
  
//  size_t timeBefore = 400;
  
  //  TrialInfo::setOnsetTarget(sentenceStarts + TrialInfo::g_targetStarts() 
	// 		+ timeBefore); 
  TrialInfo::setOnsetTarget(sentenceStarts + TrialInfo::g_targetStarts()); 
  TrialInfo::setTimeIsUp();
  
//   return(trialSet);
}
