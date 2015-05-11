#include "main.ih"

int getFixationsFromSentenceOnset(ifstream& trialInfoFile)
{
  
  ofstream outputfile;
  outputfile.open("fixationsFromOnset.txt");
  
  string trialInfo;
  TrialInfo trialSet;
  while (getline(trialInfoFile, trialInfo))
  {
    trialSet.extractInfo(trialInfo);
    string subNum = trialSet.g_subject();  
    
    ifstream eyetrackingFile(subNum.append(".asc"));
    if (! eyetrackingFile.is_open())
    {
      cout << "Unable to open eyetracking datafile " << subNum << '\n';
      break;
    }
    else
    {
      size_t iTrial = 0;
      string line;
      
      while (getline(eyetrackingFile, line))
      {
	if (line.find("onsetSoundStim") != string::npos)
	  trialSet.updateCurrentTrial(line);

	if (iTrial == trialSet.g_trialIN())
	{
	  if (line.find("TRIAL ENDS") != string::npos)
	  {
	    getline(trialInfoFile, trialInfo); 
	    trialSet.extractInfo(trialInfo); 
	  }
	  
	  if ((line.find("EFIX") != string::npos))
	  {
	    istringstream linedata(line); // extract time information from the datafile
	    string rubbish;
	    size_t stime, etime;
	    double xpos, ypos, psize; //, dur; promoted to rubbish because unreliable
	    
	    linedata >> rubbish >> rubbish >> stime >> etime >> 
	      rubbish >> rubbish >> rubbish >> rubbish >> 
	      xpos >> ypos >> psize;

	    if (etime >= trialSet.g_targetOnset())
	      outputfile << trialSet.g_subject() << '\t' << trialSet.g_trialIN()  << '\t' <<
		trialSet.g_target() << '\t' << xpos << '\t' << ypos << '\t' << 
		psize << '\t' << etime-trialSet.g_targetOnset() << '\n';
		
	  } // if (line.find("EFIX") != string::npos) 
	} // if (iTrial == trialSet.g_trialIN())
      } // while (getline(eyetrackingFile, line))
      
      eyetrackingFile.close();
    } // end "if(! eyetrackingFile.is_open()") 
    if (trialInfoFile.eof())
      break;
  } // end "while(getline(trialInfoFile, trialInfo))"
  trialInfoFile.close();
  outputfile.close();
  cout << "Output stored in fixationsFromOnset.txt\n"; 
  return 1;
}