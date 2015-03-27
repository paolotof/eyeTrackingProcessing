#include "main.ih"

/* This file does two things:
 * 1 - 	a) creates a file containing blinks longer than 300 ms. These trials are excluded from
 * further analaysis
 * 	b) updates the file with the 'trials2beIncluded' excluding the trials with blinks longer 
 * than 300 ms. Trials not included in that file are excluded from further analysis. The information
 * for inclusion or exclusion is decided when the 'TRIAL ENDS' message is encounterd.
 * 
 * 2 - Export informations about interpolation of blinks using median of N data lines
 * remove trials with blinks longer than 300 ms
 * leave alone trials with blinks shorter than 20 ms (the will be interpolated linearly)
 * compute median of 25 line before and after blink for boundaries of linear interpolation for
 * blinks longer than 20 ms and shorter than 300
 * 
 * 
 * 
 */



string checkBlinksAndGetInterpBounds(ifstream& trialInfoFile, size_t reportType, string filename)
{
  
  filename.replace(filename.find(".txt"), 4, "_noBlinks.txt");
  
  string nameOutputfile;
  if (filename.find("noFillers") != string::npos)
    nameOutputfile = "noFillers";
  else
    nameOutputfile = "withFillers";
  
  
  ofstream longBlinksReport;// file containing blinks longer than 300 ms
  longBlinksReport.open(nameOutputfile.append("_blinksPerSubject.txt")); 
  
  ofstream shortBlinksReport;// file containing blinks shorter than 300 ms
  shortBlinksReport.open(nameOutputfile.replace(nameOutputfile.find(".txt"), 4,
    "Smaller300.txt"));  
  
  bool includeTrial;
  ofstream updatedTrialInfoFile;
  updatedTrialInfoFile.open(filename);
  
  string trialInfo;
  while (getline(trialInfoFile, trialInfo))
  {
    TrialInfo trialSet;
    trialSet = trialSetup(trialInfo, trialSet);  
    string subNum = trialSet.g_subject();  
    cout << subNum << ' ';  
    ifstream eyetrackingFile(subNum.append(".asc"));
    if (! eyetrackingFile.is_open())
    {
      cout << "Unable to open eyetracking datafile " << subNum << '\n';
      break;
    }
    else
    {
      size_t iTrial = 0;
      //size_t onsetWord = 0;
      
      vector<double> tVect;
      vector<double> xVect;
      vector<double> yVect;
      vector<double> pVect;
      
      size_t lines2include = 25;
      double medianT;
      double medianX;
      double medianY;
      double medianP;
             
      string line;
      Dataline eye;
      size_t onsetWord = 0;
      double blinkDuration = 0;
      
      while (getline(eyetrackingFile, line))
      {

// BEGINNING OF SENTENCE SOUND
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
	
// TRIAL ENDS - update trial information/reset arrays
	if ((line.find("TRIAL ENDS") != string::npos) && (iTrial == trialSet.g_trialIN()))
	{
	  if (reportType == 1)
	  {
	    if (includeTrial)
	      updatedTrialInfoFile << trialInfo << '\n';
	    includeTrial = true; // reset include trials
	    blinkDuration = 0;
	  }
	  getline(trialInfoFile, trialInfo); 
	  trialSet = trialSetup(trialInfo, trialSet);
	  vector<double>().swap(tVect);
	  vector<double>().swap(xVect);
	  vector<double>().swap(yVect);
	  vector<double>().swap(pVect);
	}

// BLINK STARTS - get median of previously stored values
	if ((line.find("SBLINK") != string::npos) && (iTrial == trialSet.g_trialIN()))
	{
	  medianT = vectorMedian(tVect);
	  medianX = vectorMedian(xVect);
	  medianY = vectorMedian(yVect);
	  medianP = vectorMedian(pVect);
	  // reset vector 
	  vector<double>().swap(tVect);
	  vector<double>().swap(xVect);
	  vector<double>().swap(yVect);
	  vector<double>().swap(pVect);

	}
// BLINK ENDS	
// 	if ((line.find("EBLINK") != string::npos) && (iTrial == trialSet.g_trialIN()) && 
// 	  (onsetWord > 0) && (eye.g_time() >= onsetWord))
	if ((line.find("EBLINK") != string::npos) && (iTrial == trialSet.g_trialIN()))
	{
	  istringstream linedata(line); // extract time information from the datafile
	  string msgType, side;
	  size_t timeStart, timeEnd;
	  linedata >> msgType >> side >> timeStart >> timeEnd;
	  
	  // double blinkDuration = timeEnd - timeStart;
	  
	  // 1) check whether blink contains onset of target word
	  if ((onsetWord >= timeStart) && (onsetWord <= timeEnd))
	    blinkDuration = timeEnd - onsetWord;
	  // if it 1.a) doesn't ignore, 
	  //       1.b) does check the length of the blink from onset of target word. 
	  // 2) If length of the blink from onset word is 
	  // 	2.b > 300 ms - exclude 
	  //    2.b < 300 ms - interpolate
	  
 	  bool exportData;
	  if (reportType == 0)
	    longBlinksReport << subNum << ' ' << iTrial << ' ' << (timeEnd - timeStart) << '\n';
	  else
	  {
	    cout << ' ' << iTrial;
	    if (blinkDuration > 300)
	    {
	      cout << '-';
	      longBlinksReport << subNum << ' ' << iTrial << ' ' << blinkDuration << '\n'; 
	      includeTrial = false; 
	    } 
	    else
	    {
	      includeTrial = true;
	      // do nothing for blinks smaller than 20 ms!
	      // Extrapolate data for the interpolation using median values 
	      if (blinkDuration < 20)
		cout << '+';
	      else
	      {
		cout << '.'; // add a dot to filename for each blink needing interpolation with median
		exportData = true;
		// get other 25 lines of data to get the median of
		for (size_t lines2skip = 0; lines2skip < lines2include; lines2skip++) 
		{
		  getline(eyetrackingFile, line);
		  if (line.find("TRIAL ENDS") != string::npos)
		  {
		    // interrupt all extrapolation for current blink
		    vector<double>().swap(tVect);
		    vector<double>().swap(xVect);
		    vector<double>().swap(yVect);
		    vector<double>().swap(pVect);
		    // do all the operation for 'trials2beIncluded' file update, since other blinks 
		    // in this current trial should be included
		    updatedTrialInfoFile << trialInfo << '\n';
		    getline(trialInfoFile, trialInfo); 
		    trialSet = trialSetup(trialInfo, trialSet);

		    exportData = false; // interrupt interpolation for this specific blink
		    
		    break; // breaks the for loop
		  }
		  
		  eye = extractData(line);
		  if (not eye.isMSG() && eye.isValid())
		  {
		    tVect.push_back(eye.g_time());
		    xVect.push_back(eye.g_xpos());
		    yVect.push_back(eye.g_ypos());
		    pVect.push_back(eye.g_psize());
		  }
		} // end for loop to collect data after end of the blink
		
		if (exportData) // Export data only if 'TRIAL ENDS' message wasn't encountered
		{
		  // begin interpolation
		  shortBlinksReport << subNum << ' ' << iTrial << ' '; 
		  size_t old_precision = shortBlinksReport.precision();
		  shortBlinksReport.precision(10);
		  shortBlinksReport << medianT ;
		  shortBlinksReport.precision(old_precision);
		  shortBlinksReport << ' ' << medianX << ' ' << medianY << ' ' << medianP << '\n';
		  // end interpolation
		  shortBlinksReport << subNum << ' ' << iTrial << ' ';
		  shortBlinksReport.precision(10);
		  shortBlinksReport << vectorMedian(tVect); 
		  shortBlinksReport.precision(old_precision);
		  shortBlinksReport << ' ' << vectorMedian(xVect) 
		    << ' ' << vectorMedian(yVect)  << ' ' << vectorMedian(pVect)  
		    << ' ' << blinkDuration / 4 << '\n'; // this last line represents the lines to interpolate
		  
		  vector<double>().swap(tVect);
		  vector<double>().swap(xVect);
		  vector<double>().swap(yVect);
		  vector<double>().swap(pVect);
		}
	      }
	    
	    } // blinks longer than 300 go to the other file, and trials with them are excluded
	  } 
	} // end of "if (iTrial == trial2include)"

// EXTRACT DATA	
	eye = extractData(line);
	if ((not eye.isMSG()) && (eye.isValid()))
	{
	  if (tVect.size() <= lines2include)
	  {
	    tVect.push_back(eye.g_time());
	    xVect.push_back(eye.g_xpos());
	    yVect.push_back(eye.g_ypos());
	    pVect.push_back(eye.g_psize());
	  }
	  else
	  {
	    tVect.push_back(eye.g_time());
	    tVect.erase(tVect.begin());
	    xVect.push_back(eye.g_xpos());
	    xVect.erase(xVect.begin());
	    yVect.push_back(eye.g_ypos());
	    yVect.erase(yVect.begin());
	    pVect.push_back(eye.g_psize());
	    pVect.erase(pVect.begin());
	  }
	} // end of "if (not eye.isMSG() && eye.isValid()) 
      } // end of "while(getline(eyetrackingFile, line))"
    } // end "if(! eyetrackingFile.is_open()") 
    cout << " processed\n"; 
    eyetrackingFile.close();

    if (trialInfoFile.eof())
      break;
  } // end "while(getline(trialInfoFile, trialInfo))"
  
  trialInfoFile.close();
  updatedTrialInfoFile.close();
  longBlinksReport.close();
  shortBlinksReport.close();
  
  return "blinks processed";
}