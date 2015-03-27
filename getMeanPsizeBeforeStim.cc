#include "main.ih"

size_t getMeanPsizeBeforeStim(ifstream& trialInfoFile, string filename)
{
  
  size_t interval;
  cout << "Specify time interval to use for averaging\n";
  cin >> interval;
  size_t sampInterval = 4; // sampling interval in ms
  size_t nlines = interval/sampInterval;
  
  ofstream outputfile;
  outputfile.open(filename.append("_avePupilSizeBeforeStim.asc"));
  
  string trialInfo;
  getline(trialInfoFile, trialInfo);
  
  TrialInfo i;
  i = trialSetup(trialInfo, i); 
  i.setCurrentTr(0);
  string subNum = i.g_subject();  
  
  while (true)
  {
    // use experiment records to open eyetracking datafile for reading
    ifstream eyetrackingFile(subNum.append(".asc"));
    if (! eyetrackingFile.is_open())
    {
      cout << "Unable to open eyetracking datafile " << subNum << '\n';
      break;
    }
    else
    {
      cout << subNum;
      
      vector<double> tVect;
      vector<double> pVect;
      Dataline eye;
      string line;
      
      while (getline(eyetrackingFile, line))
      {
	eye = extractData(line);

	if ((not eye.isMSG() == true) && eye.isValid()) 
	{
	  tVect.push_back(eye.g_time());
	  pVect.push_back(eye.g_psize());
	  
	  if (tVect.size() >= nlines)
	  {
	    tVect.erase(tVect.begin());
	    pVect.erase(pVect.begin());
	  }
	}
	
	
	if (line.find("onsetVisualStim") != string::npos) // because of the delay it might happen that for the first subject onset of sound file is too late already.
	{
	  i.setCurrentTr(i.g_currentTr() + 1);
	  // average the values subtracting by the number of lines
	  if (i.g_currentTr() == i.g_trialIN())
	  {
	    outputfile << i.g_subject() << '\t' 
	      << i.g_condition()  << '\t' 
	      << i.g_target() << '\t' 
	      << i.g_trialIN() << '\t' 
	      << i.g_subCond() << '\t' 
	      << static_cast<size_t>(vectorMean(tVect)) << '\t'
	      << static_cast<size_t>(vectorMean(pVect)) << '\n';
	  }

	  // reset vector  
	  vector<double>().swap(tVect);
	  vector<double>().swap(pVect);
	  
	  while (getline(eyetrackingFile, line)) // sample next lines until end of the trial
	  {
	    if (line.find("TRIAL ENDS") != string::npos)
	    {
	      if (i.g_currentTr() == i.g_trialIN()) 
		i = endTrial(trialInfoFile, i);
	      break;
	    }
	  }
	  
	}
	
	if (eyetrackingFile.eof())
	  break;
      }
    }
    eyetrackingFile.close();
    i.setCurrentTr(0);
    cout << " processed \n";
    subNum = i.g_subject();  
        
    if (trialInfoFile.eof())
      break;
    
  }
  
  trialInfoFile.close();
  outputfile.close();
  
  return (0);
}