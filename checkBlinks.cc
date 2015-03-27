#include "main.ih"

string checkBlinks(ifstream& trialInfoFile, size_t reportType, string filename)
{

  ofstream blinksReport;
  blinksReport.open("blinksPerSubject.txt"); // file containing blinks longer than 300 ms after sound onset
  
  ofstream shortBlinksReport;
  shortBlinksReport.open("blinksPerSubjectSmaller300.txt"); // file containing blinks longer than 300 ms 
                                                            // after onset of the target word
  
  bool includeTrial = true;
  ofstream outputfile;
  outputfile.open(filename);
  
  string trialInfo;
  while (getline(trialInfoFile, trialInfo))
  {
    TrialInfo trialSet;
    trialSet = trialSetup(trialInfo, trialSet);  
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
      size_t onsetWord = 0;
      
      string line;
      Dataline eye;
      
      while (getline(eyetrackingFile, line))
      {
	if (line.find("onsetSoundStim") != string::npos)
	{
	  iTrial++;
	  if (iTrial == trialSet.g_trialIN())
	  {
	    istringstream linedata(line); // extract time information from the eyetracking file
	    string msgType;
	    size_t timeZero;
	    linedata >> msgType >> timeZero;
	    onsetWord = timeZero + trialSet.g_targetStarts();
	  } // end of "if (iTrial == trial2include)"
	} // end of "if (line.find("SoundStim") != string::npos)"
	
	//   update trial information
	 if ((line.find("TRIAL ENDS") != string::npos) && (iTrial == trialSet.g_trialIN()))
	{
	  if (reportType == 1)
	  {
	    if (includeTrial)
	      outputfile << trialInfo << '\n';
	    includeTrial = true; // reset include trials
	  }
	  getline(trialInfoFile, trialInfo); 
	  trialSet = trialSetup(trialInfo, trialSet); 
	}

	eye = extractData(line);

	if (line.find("EBLINK") != string::npos)
	{ 
	  if ((iTrial == trialSet.g_trialIN()) && (eye.g_time() >= onsetWord))
	  {
	    istringstream linedata(line); // extract time information from the datafile
	    string msgType, side;
	    size_t timeStart, timeEnd;
	    linedata >> msgType >> side >> timeStart >> timeEnd;
	    
	    if (reportType == 0)
	      blinksReport << subNum << ' ' << iTrial << ' ' 
		<< timeEnd - timeStart << '\n';
	    
	    if (reportType == 1)
	    {
	      if ((timeEnd - timeStart) > 300)
	      {
// 		blinksReport << subNum << ' ' << iTrial << ' ' << timeStart 
// 		  << " blink (ms) = " << timeEnd - timeStart 
// 		  << " from word onset = " << timeEnd - onsetWord << '\n';
		blinksReport << subNum << ' ' << iTrial << ' ' 
		  << timeEnd - timeStart; 
		// add info about interpolation. These are the time for the beginning and 
		// end of interpolation, with the 15 or 25 lines included...
		// 
		
		// note that we also interpolating with the median only segments longer than
	       // 20 ms, shorter segments are interpolated linearly... 
	       // 
		blinksReport << '\n'; 
		includeTrial = false; 
	      } 
	      else
	      {
		shortBlinksReport << subNum << ' ' << iTrial << ' ' 
		<< timeEnd - timeStart << '\n';
	      }
	    }
	  } 
	} // end of "if (iTrial == trial2include)"
      } // end of "while(getline(eyetrackingFile, line))"
    } // end "if(! eyetrackingFile.is_open()") 

    eyetrackingFile.close();

    if (trialInfoFile.eof())
      break;
  } // end "while(getline(trialInfoFile, trialInfo))"
  
  trialInfoFile.close();
  outputfile.close();
  blinksReport.close();
  shortBlinksReport.close();
  
  return "blinks processed";
}