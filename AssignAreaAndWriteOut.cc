#include "main.ih"

//TrialInfo AssignAreaAndWriteOut(double (*dataArray), ofstream& outputfile, TrialInfo trialSet, bool charlotte)
size_t AssignAreaAndWriteOut(double (*dataArray), ofstream& outputfile, TrialInfo trialSet, bool charlotte)
{
  // if time is right: average and define belonging to ROIs  
  int timeNow = static_cast<int>(dataArray[0]);
    
  double propFix2ROIs [4] = {0, 0, 0, 0}; 
  size_t areaInfo;
  areaInfo = 0;
  areaInfo = classifyArea(dataArray[1], dataArray[2], trialSet.g_targetLoc(), 1, charlotte) +
    classifyArea(dataArray[1], dataArray[2], trialSet.g_competLoc(), 2, charlotte) + 
    classifyArea(dataArray[1], dataArray[2], trialSet.g_dist1Loc(), 3, charlotte) + 
    classifyArea(dataArray[1], dataArray[2], trialSet.g_dist2Loc(), 4, charlotte);

  propFix2ROIs[areaInfo - 1] = 1;
  
//   cout << trialSet.totalBins() + 1;
  int timeDif = timeNow - trialSet.g_targetOnset();
  outputfile << trialSet.g_subject() 
    << '\t' << timeNow  
    << '\t' << timeDif
//     << '\t' << trialSet.totalBins()
    << '\t' << trialSet.g_condition() 
    << '\t' << trialSet.g_trialIN() 
		<< '\t' << trialSet.g_target() 
    << '\t' << trialSet.g_subCond() 
		<< '\t' << dataArray[3] 
		<< '\t' << propFix2ROIs[0] 
		<< '\t' << propFix2ROIs[1] 
		<< '\t' << propFix2ROIs[2] 
    << '\t' << propFix2ROIs[3] 
    << '\t' << trialSet.g_Fix() 
		<< '\n';
      
      
    /*
    *
    *this:    
      << '\t' << i.totalBins()
    * has been replace by this:  
      << '\t' << (timeNow - i.g_targetOnset()) / 4 
    * to fix the problem of the negative fixations, otherwise all the further analysis 
    * will be screwed up because the numbering of the bins always starts from 0.  
    */    
  return 0;
}
