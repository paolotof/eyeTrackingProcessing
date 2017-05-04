#include "main.ih"

/* 
 * 
 * determine blinks length and linear interpolate blink 
 * shorter than 20 ms with after and before values and 
 * use the median for blinks between 20 and 300 ms long
 * 
 * 
 */

// size_t interpolateBlinks(ifstream& trialInfoFile, ifstream& eyetrackingFile, ofstream& outputfile, 
// 			 TrialInfo trialSet, Dataline preBlink, Interpdata interpolation, bool charlotte)
TrialInfo interpolateBlinks(ifstream& trialInfoFile, ifstream& eyetrackingFile, 
														ofstream& outputfile, TrialInfo trialSet, Dataline preBlink,
														Interpdata interpolation, bool charlotte, size_t timeBefore)
{ 
  string line;
  while (getline(eyetrackingFile, line)) // loop until end of blink message is found
  {
    if (line.find("EBLINK") == string::npos)
      break; // stop "while (getline(eyetrackingFile, line))"
   
    if (line.find("TRIAL ENDS") != string::npos)
    {
//       cout << trialSet.g_currentTr() << " ends first loop eblink ";
      trialSet.resetAndUpdate(trialInfoFile);
//       cout << trialSet.g_currentTr() << '\n';
      return(trialSet);
    }

    if (line.find("onsetSoundStim") != string::npos)  
    {
      trialSet.updateCurrentTrial(line);
      return(trialSet);
    }
    
  } // end "while (getline(eyetrackingFile, line))"

  // search first data line after the end-of-blink message
  Dataline postBlink;
  while (getline(eyetrackingFile, line))
  {
    postBlink.extractData(line);
    if (postBlink.isValid() && (not postBlink.isMSG()))
      break;
    
    if (line.find("TRIAL ENDS") != string::npos)
    {
//       cout << trialSet.g_currentTr() << " ends second loop eblink ";// << trialSet.g_trialIN() << ' ';
      trialSet.resetAndUpdate(trialInfoFile);
      return(trialSet);
    }
    
    if (line.find("onsetSoundStim") != string::npos)  
    {
      trialSet.updateCurrentTrial(line);
      return(trialSet);
    }
  } // end "while (true)"
  
  // If the length of the blink is less than 20 ms the linear interpolation is 
	// based on the values of the SBLINK EBLINK messages. 
	// It the length of the blink is between 20 and 300 ms the linear interpolation
	// is based on the median values of the 25 data points reported before and after
	// the SBLINK EBLINK messages. 
	// NOTE: blinkDur checks whether the blink is longer or shorter than 20 ms, 
	// but it considers the messages, not the information from the interpolation!
//   size_t blinkDur = postBlink.g_time() - preBlink.g_time();
  // long blink	  
//   if ((blinkDur > 20) && (interpolation.sub() ==  trialSet.g_subject()) && 
//     (trialSet.g_currentTr() == interpolation.nTrial()))
	if (
		(interpolation.sub() ==  trialSet.g_subject()) && 
		(trialSet.g_currentTr() == interpolation.nTrial())
	)
	{
// 		(7439318 <= 7439398) && 7439350 >= 7439264
// 		cout << trialSet.g_subject() << '.' << trialSet.g_currentTr();
    if ((preBlink.g_time() <= interpolation.iEnd()) && 
			(postBlink.g_time() >= interpolation.iBegin()))
    {
      trialSet.updateInterp(
				interpolateLongBlinks(outputfile, trialSet, interpolation, charlotte, timeBefore)
			);
      if (interpolation.iBegin() < trialSet.g_targetOnset())
				trialSet.updateBinCount((interpolation.iEnd() - trialSet.g_targetOnset()) / 4);
      else
				trialSet.updateBinCount((interpolation.iEnd() - interpolation.iBegin()) / 4);
    }
  }
//   // short blink		  
//   if (blinkDur >= 0 && blinkDur <= 20)
//   {
//     size_t lines2interpolate = blinkDur / 4;
//     interpolateShortBlinks(outputfile, trialSet, preBlink, postBlink, lines2interpolate, charlotte);
//     trialSet.updateBinCount(lines2interpolate);
//   }
    
  return(trialSet);
}
