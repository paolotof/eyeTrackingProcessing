#include "main.ih"

size_t processData(ifstream& trialInfoFile, string filename, bool charlotte)
{
  ofstream outputfile;
  filename.append("_ALL_medianInterp.asc");
  outputfile.open(filename);
  // write file header
  outputfile << "pp" << '\t' << "clockTime" << '\t'  << "time" << '\t'
    << "bin" << '\t'
    <<  "condition" << '\t' << "trial" << '\t' << "item" << '\t' << "exp"
    << '\t' << "psize"  << '\t' << "target" << '\t' << "competitor" 
    << '\t' << "d1" <<  '\t' << "d2" << '\t' << "fix" << '\n';  
  
  string trialInfo;
  getline(trialInfoFile, trialInfo);
    
  TrialInfo trialSet;
  trialSet = trialSetup(trialInfo, trialSet);
  trialSet.setUpdateInterp(false);
  string subNum = trialSet.g_subject();  
  
  string interpolationFile;
  if (filename.find("noFillers") != string::npos)
    interpolationFile = "noFillers";
  else
    interpolationFile = "withFillers";
  
  
  ifstream interpInfo(interpolationFile.append("_blinksPerSubjectSmaller300.txt"));
  string line1, line2;
  Interpdata interpolation;
  getline(interpInfo, line1);
  getline(interpInfo, line2);
  interpolation = interpolation.extractInterpData(line1, line2);
 
   
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
      
      Dataline eye, preBlink;
      string line;
      
      cout << subNum << ' ';
      
      while (getline(eyetrackingFile, line))
      {
	
	/* ignore lines which are fix or sac messages - SFIX is used to count fixations */
	while ((line.find("EFIX") != string::npos) || (line.find("SAC") != string::npos))
	  getline(eyetrackingFile, line);
	  
	if (line.find("onsetSoundStim") != string::npos)
	  trialSet = computeTargetOnset(trialSet, line); 
	  
	if (trialSet.g_currentTr() == trialSet.g_trialIN())
	{
	  
	  if (line.find("TRIAL ENDS") != string::npos)
	    trialSet = endTrial(trialInfoFile, trialSet);
	  
	  if (line.find("SBLINK") != string::npos)
	    trialSet = interpolateBlinks(trialInfoFile, eyetrackingFile, outputfile, 
					 trialSet, preBlink, interpolation, charlotte);
	    
	  if (trialSet.g_updateInterp() )
	  {
	    getline(interpInfo, line1);
	    getline(interpInfo, line2);
	    if (interpInfo.good())
	      interpolation = interpolation.extractInterpData(line1, line2);
	    
	    cout << interpolation.sub() << '\n';;
	    trialSet.setUpdateInterp(false);
	    getline(eyetrackingFile, line);
	  }
	    
	  eye = extractData(line);
	  if (eye.g_time() >= (trialSet.g_targetOnset()))
	  {
	    if((eye.isMSG() != true) && (eye.g_xpos() > 0.1) 
	      && (eye.g_ypos() > 0.1) && eye.isValid())
	    {
	      // lines interpolated with median shall not be repeated
	      
	      if (subNum != interpolation.sub())
	      {
		double dataArray[4] = {0, 0, 0, 0}; // 4: time, xpos, ypos, psize; 5: N of obs.
		dataArray[0] = eye.g_time();
		dataArray[1] = eye.g_xpos();
		dataArray[2] = eye.g_ypos();
		dataArray[3] = eye.g_psize();
		trialSet = AssignAreaAndWriteOut(dataArray, outputfile, trialSet, charlotte);
	      }
	      else
	      {  
		if (eye.g_time() <= interpolation.iBegin())
		{
		  double dataArray[4] = {0, 0, 0, 0}; // 4: time, xpos, ypos, psize; 5: N of obs.
		  dataArray[0] = eye.g_time();
		  dataArray[1] = eye.g_xpos();
		  dataArray[2] = eye.g_ypos();
		  dataArray[3] = eye.g_psize();
		  trialSet = AssignAreaAndWriteOut(dataArray, outputfile, trialSet, charlotte);
		}
	      }
	    }
	    
	    if (line.find("SFIX") != string::npos)
	      trialSet.setFix(trialSet.g_Fix() + 1);
	  } // end of "if (!eye.isMSG && (eye.g_time() >= trialSet.g_targetOnset()))"
	  preBlink = eye;
	} // end of "if (iTrial == trialSet.g_trialIN())"
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