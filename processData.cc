#include "main.ih"

size_t processData(ifstream& trialInfoFile, string filename, bool charlotte, string dir2save)
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
  trialSet.extractInfo(trialInfo);
  trialSet.updateInterp(false);
  string subNum = trialSet.g_subject();  
  
  string interpolationFile;
  if (filename.find("noFillers") != string::npos)
    interpolationFile = "noFillers";
  else
    interpolationFile = "withFillers";
  
  interpolationFile.append("_blinksPerSubjectSmaller300.txt"); 
  interpolationFile.insert(0, dir2save);
  
  ifstream interpInfo(interpolationFile);
  if (! interpInfo.is_open())
  {
    cout << "Unable to open interpolation file \n"; 
    return 0;
  }
  
  string line1, line2;
  Interpdata interpolation;
  getline(interpInfo, line1);
  getline(interpInfo, line2);
  if (interpInfo.good())
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
      cout << subNum << " opened\n";
      
      if (trialSet.g_subject() > interpolation.sub())
      {
	cout << "fixing " << trialSet.g_subject() << " interpolation\n";
	while (trialSet.g_subject() != interpolation.sub() &&
	  not interpInfo.eof())
	{
	  getline(interpInfo, line1);
	  getline(interpInfo, line2);
	  interpolation.setSub("none");
	  if (interpInfo.good())
	    interpolation = interpolation.extractInterpData(line1, line2);
	}
      }
      trialSet.setCurrentTr(0);
      trialSet.resetBinsCounter();
      trialSet.setFix(1);
      
      Dataline eye, preBlink;
      string line;
      
      while (getline(eyetrackingFile, line))
      {
	/* ignore lines which are fix or sac messages - SFIX is used to count fixations */
	while ((line.find("EFIX") != string::npos) || (line.find("SAC") != string::npos))
	  getline(eyetrackingFile, line);
	
	if (line.find("onsetSoundStim") != string::npos)
	  trialSet.updateCurrentTrial(line);
	  
	if (trialSet.g_currentTr() == trialSet.g_trialIN())
	{
	  
	  if (line.find("TRIAL ENDS") != string::npos)
	  {
	    trialSet.resetAndUpdate(trialInfoFile);
	    // interrupt if the data for another subject has been read
	    if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject())
	    {
	      cout << '\n' << subNum << " stopping with processing at trial " <<
		trialSet.g_currentTr() << '\n';
	      eyetrackingFile.close();
	      break;
	    }
	  }
	  
	  if (line.find("SBLINK") != string::npos)
	  {
	    
	    trialSet = interpolateBlinks(trialInfoFile, eyetrackingFile, outputfile, trialSet, 
			      preBlink, interpolation, charlotte);
	    
	    if (trialSet.g_updateInterp())
	    {
	      // read eyetrackingFile's lines up to the end of the interpolation interval
	      if (trialSet.g_subject() == interpolation.sub() && 
		trialSet.g_currentTr() == interpolation.nTrial())
	      {
		while (eye.g_time() < interpolation.iEnd())
		{
		  getline(eyetrackingFile, line);
		  eye.extractData(line);
		}
	      }
	      
	      // update information about the next interpolation
	      getline(interpInfo, line1);
	      getline(interpInfo, line2);
	      interpolation.setSub("none");
	      if (interpInfo.good())
		interpolation = interpolation.extractInterpData(line1, line2);

	      trialSet.updateInterp(false);
	      getline(eyetrackingFile, line);
// 	      cout << line1 << '\n';
	    }
	  } // end "if (line.find("SBLINK") != string::npos)"

	  eye.extractData(line);

	  if ((eye.isMSG() != true) && (eye.g_time() >= (trialSet.g_targetOnset())) && 
	    (eye.g_time() <= trialSet.timeIsUp()))
	  {
	    if(eye.isValid())
	    {
	      // participants with no interpolation can go without problems
	      if (trialSet.g_subject() != interpolation.sub())
	      {
		double dataArray[4] = {0, 0, 0, 0}; // 4: time, xpos, ypos, psize; 5: N of obs.
		dataArray[0] = eye.g_time();
		dataArray[1] = eye.g_xpos();
		dataArray[2] = eye.g_ypos();
		dataArray[3] = eye.g_psize();
		trialSet.addOneBin();
		AssignAreaAndWriteOut(dataArray, outputfile, trialSet, charlotte);
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
		  trialSet.addOneBin();
		  AssignAreaAndWriteOut(dataArray, outputfile, trialSet, charlotte);
 		}
 	      }
	    }
	    
	    if (line.find("SFIX") != string::npos)
	      trialSet.setFix(trialSet.g_Fix() + 1);
	  } // end of "if (!eye.isMSG && (eye.g_time() >= trialSet.g_targetOnset()))"
	  
	  preBlink = eye;
	  
	  // there are cases when interpolation data are present for trials which should not be interpolated.
	  // For example: a participant blinks multiple times within a trial. The majority of the blinks 
	  // are initially shorter than 300 ms but then there is one that is longer than 300ms. The trial 
	  // should be excluded, but the interpolation data are still there. Interpolation data for that
	  // trial should be ignored if the trial has been excluded.
	  if ( trialSet.g_subject() == interpolation.sub() && 
	    trialSet.g_currentTr() > interpolation.nTrial() &&
	    not interpInfo.eof() )
	  {
	    cout << trialSet.g_trialIN() << " interpolation trial " << interpolation.nTrial() << " skipped";
	    getline(interpInfo, line1);
	    getline(interpInfo, line2);
	    interpolation.setSub("none");
	    if (interpInfo.good())
	      interpolation = interpolation.extractInterpData(line1, line2);
	  }
	} // end of "if (iTrial == trialSet.g_trialIN())"
      } // end of "while(getline(eyetrackingFile, line))"
    } // end "if(! eyetrackingFile.is_open()") 
    
    eyetrackingFile.close();
    subNum = trialSet.g_subject();  

    if (trialInfoFile.eof())
      break;
    
  } // end "while(getline(trialInfoFile, trialInfo))"
  
  trialInfoFile.close();
  outputfile.close();
  
  return 1;
}