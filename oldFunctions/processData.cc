#include "main.ih"

size_t processData(ifstream& trialInfoFile, string filename, 
									 bool charlotte, size_t timeBefore)
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
  
	string filePrefix = "RT_";
  string interpolationFile;
	string fillers;
  if (filename.find("noFillers") != string::npos)
	{
		interpolationFile = filePrefix + "noFillers";
		fillers = "no ";
	}
  else
	{
		interpolationFile = filePrefix + "withFillers";
		fillers = "fillers";
	}
  interpolationFile.append("_blinksPerSubjectSmaller300.txt"); 
	
	interpolationFile = eliminateRedundantTrials(trialInfoFile, interpolationFile);
// 	cout << interpolationFile << '\n';
	if (interpolationFile.find("none") != string::npos)
		return 0;
// 	return 0;
	
// 	cout << interpolationFile << '\n';
  ifstream interpInfo(interpolationFile);
	Interpdata interpolation; // interpolation.interpolate() set to false unless interpolation file exists
	if (! interpInfo.is_open())
  {
		cout << "NO interpolation file "<< interpolationFile << '\n'; 
		cout << "Continue without interpolation information\n\n"; 
  } else {
		interpolation.setDoInterpolation(true); //interpolation.interpolate() set to true
		string line1, line2;
		getline(interpInfo, line1);
		getline(interpInfo, line2);
		if (interpInfo.good())
			interpolation = interpolation.extractInterpData(line1, line2);
	}
	
	string trialInfo;
	getline(trialInfoFile, trialInfo);
	TrialInfo trialSet;
	trialSet.extractInfo(trialInfo);
	trialSet.updateInterp(false);
	string subNum = trialSet.g_subject();  
	while (true)
  {
// use experiment records to open eyetracking datafile for reading
    ifstream eyetrackingFile(subNum.append(".asc"));
    if (not eyetrackingFile.is_open())
    {
      cout << "Unable to open eyetracking datafile " << subNum << '\n'; 
      break;
    }
    else
    {
			cout << "Extracting "<< fillers <<" "<< subNum <<" ";
// 			if (interpolation.interpolate() && 
// 				(trialSet.g_subject() > interpolation.sub()))
//       {
// 				cout << "fixing " << trialSet.g_subject() << " interpolation\n";
						// 					interpolation.interpolate() && 
			// synchronize the files when the eye-tracking files are 
			// further in the processing than the interpolation files
// 			if (trialSet.g_subject() > interpolation.sub()){ 
			while (
				trialSet.g_subject() > interpolation.sub() && 
// 				trialSet.g_currentTr() > interpolation.nTrial() &&
				not interpInfo.eof())
			{
				cout << "sink PP AND trials " 
					<< trialSet.g_subject() << ' ' << trialSet.g_currentTr() << ' ' 
					<< interpolation.sub() << ' ' << interpolation.nTrial() << '\n';
				string line1, line2;
				getline(interpInfo, line1);
				getline(interpInfo, line2);
// 				interpolation.setSub("none"); // why set to none? it shouldn't require initialization
				if (interpInfo.good())
					interpolation = interpolation.extractInterpData(line1, line2);
			} // end while
// 			} // end if (trialSet.g_subject() > interpolation.sub())
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
							eyetrackingFile.close();
							break;
						}
						// update interpolation file, some trials in the interpolation file should
						// be excluded 
						if (
							trialSet.g_subject() == interpolation.sub() && 
							trialSet.g_currentTr() > interpolation.nTrial() &&
							not interpInfo.eof())
						{
							cout << "sink TRIALS " 
								<< trialSet.g_subject() << ' ' << trialSet.g_currentTr() << ' ' 
								<< interpolation.sub() << ' ' << interpolation.nTrial() << '\n';
							string line1, line2;
							getline(interpInfo, line1);
							getline(interpInfo, line2);
							interpolation.setSub("none");
							if (interpInfo.good())
								interpolation = interpolation.extractInterpData(line1, line2);
						} // end if/while
						
					}
					if (line.find("SBLINK") != string::npos)
					{
						trialSet = interpolateBlinks(trialInfoFile, eyetrackingFile, 
																				 outputfile, trialSet, preBlink, 
																	 interpolation, charlotte, timeBefore);
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
							string line1, line2;
							getline(interpInfo, line1);
							getline(interpInfo, line2);
							interpolation.setSub("none");
							if (interpInfo.good())
								interpolation = interpolation.extractInterpData(line1, line2);

							trialSet.updateInterp(false);
							getline(eyetrackingFile, line);
						} // end: if (trialSet.g_updateInterp())
					} // end "if (line.find("SBLINK") != string::npos)"

					eye.extractData(line);
			//	  size_t timeBefore = 200;
					if ((eye.isMSG() != true) && 
						(eye.g_time() >= (trialSet.g_targetOnset() - timeBefore))
					) // && (eye.g_time() <= trialSet.timeIsUp())) // we go until the end of the trial
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
								} // end if-else: if (trialSet.g_subject() != interpolation.sub())
							} // end: if(eye.isValid())
						} // end : if((eye.isMSG() != true) && (eye.g_time() >= (trialSet.g_targetOnset() - timeBefore)) && (eye.g_time() <= trialSet.timeIsUp()))
					} // end of "if (!eye.isMSG && (eye.g_time() >= trialSet.g_targetOnset()))"
					// update number of fixations
					if ( 
						(eye.g_time() >= (trialSet.g_targetOnset() - timeBefore)) && 
// 						(eye.g_time() <= trialSet.timeIsUp()) && 
						(line.find("SFIX") != string::npos) 
					)
							trialSet.setFix(trialSet.g_Fix() + 1);
							
					preBlink = eye;
				} // end of "if (iTrial == trialSet.g_trialIN())"
				/* there are cases when interpolation data are present for 
				* trials which should not be interpolated.
				* For example: a participant blinks multiple times within a trial. 
				* The majority of the blinks are initially shorter than 300 ms but 
				* then there is one that is longer than 300ms. The trial should 
				* be excluded, but the interpolation data are still there. 
				* Interpolation data for that
				* trial should be ignored if the trial has been excluded.
				*/
// 					if (interpolation.interpolate() &&
// 				while ( // the loops goes many times through this so an if is 
// enough
      } // end of "while(getline(eyetrackingFile, line))"
    } // end "if(! eyetrackingFile.is_open()") 
    
    eyetrackingFile.close();
    subNum = trialSet.g_subject();  
		cout << " end\n"; // get to next participant
    if (trialInfoFile.eof())
      break;
    
	} // end "while(true)"
  
  trialInfoFile.close();
  outputfile.close();
  
  return 1;
}
