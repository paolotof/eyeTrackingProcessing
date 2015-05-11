#include "main.ih"

size_t extractPupil(ifstream& trialInfoFile, string filename)
{
  ofstream outputfile;
  filename.append("_pupilSizePreStim.asc");
  outputfile.open(filename);
  // write file header
  outputfile << "pp" << '\t' << "time" << '\t' << "bin" << '\t' 
    <<  "condition" << '\t' << "trial" << '\t' << "item" 
    << '\t' << "exp" << '\t' << "relative time" << '\t' << "psize"  << "fix" << '\n';  
  
  string trialInfo;
  getline(trialInfoFile, trialInfo);
  TrialInfo trialSet;
  trialSet.extractInfo(trialInfo);
  string subNum = trialSet.g_subject();  
  size_t nlines = 125;
  
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
      
      trialSet.setCurrentTr(0);
      trialSet.setBin(0);
      trialSet.setFix(1);

      vector<double> tVect;
      vector<double> pVect;
      
      Dataline eye;
      string line;
      
      cout << subNum << ' ';
      
      while (getline(eyetrackingFile, line))
      {
	
	/* ignore lines which are fix or sac messages - SFIX is used to count fixations */
	while ((line.find("EFIX") != string::npos) || (line.find("SAC") != string::npos))
	  getline(eyetrackingFile, line);
	  
	if (line.find("onsetSoundStim") != string::npos)
	  export1SecFromHere(trialInfoFile, trialSet, line, eyetrackingFile, outputfile, tVect, pVect); 
	
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

      } // end of "while(getline(eyetrackingFile, line))"
    } // end "if(! eyetrackingFile.is_open()") 
    
    eyetrackingFile.close();
    trialSet.setCurrentTr(0);
    cout << " processed \n";
    subNum = trialSet.g_subject();  

    if (trialInfoFile.eof())
      break;
    
  } // end "while(getline(trialInfoFile, trialInfo))"
  
  trialInfoFile.close();
  outputfile.close();
  
  return 1;
}