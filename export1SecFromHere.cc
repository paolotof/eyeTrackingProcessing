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

// TrialInfo export1SecFromHere(ifstream& trialInfoFile, TrialInfo trialSet, string line, ifstream& eyetrackingFile, 
// 			     ofstream& outputfile, vector<double> tVect, vector<double> pVect)
size_t export1SecFromHere(ifstream& trialInfoFile, TrialInfo trialSet, string line, ifstream& eyetrackingFile, 
			     ofstream& outputfile, vector<double> tVect, vector<double> pVect)
{
  trialSet.setCurrentTr(trialSet.g_currentTr() + 1);
  
  if (trialSet.g_currentTr() == trialSet.g_trialIN())
  {
    istringstream linedata(line);
    string msgType;
    size_t sentenceStarts; 
    linedata >> msgType >> sentenceStarts;
    trialSet.setOnsetTarget(sentenceStarts + trialSet.g_targetStarts()); 
  }
  
  trialSet.setFix(1);
  trialSet.resetBinsCounter();
  
  Dataline eye;
  
  if (trialSet.g_currentTr() == trialSet.g_trialIN())
  {
    // first write out the lines before stimulus onset
    size_t nLines = tVect.size();
    for (size_t iloop = 0; iloop < nLines; iloop++)
    {
      eye.setTime(tVect.at(iloop));
      eye.setSize(pVect.at(iloop));
      trialSet.addOneBin(); // remove this to NOT include bin numbers anymore, also line 24 
      writeOut(eye, outputfile, trialSet);
    } // end "for (size_t iloop = 0, iloop < nLines, iloop++)"

    size_t OneSecInLines = 1000/4;
    for (size_t iloop = 0; iloop < OneSecInLines; iloop++)
    {
      if (line.find("TRIAL ENDS") != string::npos)
	trialSet.resetAndUpdate(trialInfoFile);
      
      if (line.find("SFIX") != string::npos)
	trialSet.setFix(trialSet.g_Fix() + 1);
      
      if (line.find("SBLINK") != string::npos)
      {
	while (true)
	{
	  getline(eyetrackingFile, line);
	  if (line.find("EBLINK") != string::npos)
	    break;
	  trialSet.addOneBin();
	    
	}
      }
      
      eye.extractData(line);
      
      if((eye.isMSG() != true) && (eye.g_xpos() > 0.1) 
	&& (eye.g_ypos() > 0.1) && eye.isValid())
      { 
	trialSet.addOneBin();
	writeOut(eye, outputfile, trialSet);
      }
	
    } // end "for (size_t iloop = 0, iloop < nLines, iloop++)"
  }

//   return(trialSet);
  return 0;
}