#include "main.ih"

/* 
 * 
 * determine blinks length and linear interpolate blink 
 * shorter than 20 ms with after and before values and 
 * use the median for blinks between 20 and 300 ms long
 * 
 * 
 */

TrialInfo interpolateBlinks(ifstream& trialInfoFile, ifstream& eyetrackingFile, ofstream& outputfile, 
			    TrialInfo trialSet, Dataline preBlink, Interpdata interpolation, bool charlotte)
{ 
  
  Dataline postBlink;
  string line;
  while (getline(eyetrackingFile, line))
  {
    if (line.find("TRIAL ENDS") != string::npos)
      return(endTrial(trialInfoFile, trialSet));
    if (line.find("onsetSoundStim") != string::npos)  
      return(computeTargetOnset(trialSet, line));
    if (line.find("EBLINK") == string::npos)
    {
      istringstream linedata(line); // extract time information from the datafile
      string msgType, side;
      size_t timeStart, timeEnd;
      linedata >> msgType >> side >> timeStart >> timeEnd;
      
      getline(eyetrackingFile, line); 
      postBlink = extractData(line);
      while (true)
      {
	getline(eyetrackingFile, line);
	
	if (line.find("TRIAL ENDS") != string::npos)
	  return(endTrial(trialInfoFile, trialSet));
	
	if (line.find("onsetSoundStim") != string::npos)  
	  return(computeTargetOnset(trialSet, line));
	
	postBlink = extractData(line);
	
	if (not postBlink.isMSG())
	{
	  size_t blinkDur = postBlink.g_time() - preBlink.g_time();
	  size_t lines2interpolate = blinkDur / 4;
	  
	  if ((blinkDur > 20) && (interpolation.sub() ==  trialSet.g_subject()) 
	    && (trialSet.g_currentTr() == interpolation.nTrial()))
	  {
	    trialSet = interpolateLongBlinks(outputfile, trialSet, interpolation, charlotte);
	    Dataline eye;
	    while (eye.g_time() < interpolation.iEnd())
	    {
	      getline(eyetrackingFile, line);
	      eye = extractData(line);
	    }
	  }
	  
	  if (blinkDur >= 0 && blinkDur <= 20)
	    trialSet = interpolateShortBlinks(outputfile, trialSet, preBlink, postBlink, lines2interpolate, charlotte);
	  
	  return(trialSet);  
	} // end "if (not postBlink.isMSG())"
      } // end "while (true)"
      break; // stop "while (getline(eyetrackingFile, line))"
    } // if EBLINK
  
    
  } // end "while (getline(eyetrackingFile, line))"

  
}