#include "main.ih"

size_t getMeanPsizeBeforeStim(ifstream& trialInfoFile, string filename, size_t interval)
{
  
  size_t sampInterval = 4; // sampling interval in ms
  size_t nlines = interval/sampInterval;
  
  ofstream outputfile;
  outputfile.open(filename.append("_avePupilSizeBeforeStim.asc"));
  
  string trialInfo;
  getline(trialInfoFile, trialInfo);
  
  TrialInfo trialSet;
  trialSet.extractInfo(trialInfo); 
  trialSet.setCurrentTr(0);
  string subNum = trialSet.g_subject();  
  
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
				eye.extractData(line);

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
					trialSet.setCurrentTr(trialSet.g_currentTr() + 1);
					// average the values subtracting by the number of lines
					if (trialSet.g_currentTr() == trialSet.g_trialIN())
					{
						outputfile << trialSet.g_subject() << '\t' 
							<< trialSet.g_condition()  << '\t' 
							<< trialSet.g_target() << '\t' 
							<< trialSet.g_trialIN() << '\t' 
							<< trialSet.g_subCond() << '\t' 
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
							if (trialSet.g_currentTr() == trialSet.g_trialIN()) 
					trialSet.resetAndUpdate(trialInfoFile);
							break;
						}
					}
				}
				if (eyetrackingFile.eof())
					break;
      }
		} // if (! eyetrackingFile.is_open())
    eyetrackingFile.close();
    trialSet.setCurrentTr(0);
    cout << " processed \n";
    subNum = trialSet.g_subject();  
        
    if (trialInfoFile.eof())
      break;
  }
  trialInfoFile.close();
  outputfile.close();
  return (0);
} // main
