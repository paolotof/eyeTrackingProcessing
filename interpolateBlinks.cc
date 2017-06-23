#include "main.ih"

/* 
 * 
 * determine blinks length and linear interpolate blink 
 * shorter than 20 ms with after and before values and 
 * use the median for blinks between 20 and 300 ms long
 * 
 * 
 */

TrialInfo interpolateBlinks(ifstream& trialInfoFile, 
														ifstream& eyetrackingFile, 
														ofstream& outputfile, TrialInfo trialSet, 
														Dataline preBlink, Interpdata interpolation, 
														bool print2screen, 
														bool charlotte, size_t timeBefore)
{ 
  string line;
	// loop until end of blink message is found
  while (getline(eyetrackingFile, line))
  {
    if (line.find("EBLINK") == string::npos)
      break; // stop "while (getline(eyetrackingFile, line))"
    if (line.find("TRIAL ENDS") != string::npos)
    {
			if (print2screen)
				cout << "\ntrial ends before blink termination\n";
			if (trialSet.g_currentTr() == trialSet.g_trialIN())
				trialSet.resetAndUpdate(trialInfoFile);
			// get a new line before exiting to prevent double processing of the same line
			getline(eyetrackingFile, line); 
      return(trialSet);
    }
    if (line.find("onsetSoundStim") != string::npos)  
    {
			if (print2screen)
				cout << "\nnew trial starts before blink termination\n";
			trialSet.updateCurrentTrial(line, timeBefore);
			// get a new line before exiting to prevent double processing of the same line
			getline(eyetrackingFile, line); 
			return(trialSet);
    }
  } // end "while (getline(eyetrackingFile, line))"

  // search first data line after the end-of-blink message
  Dataline postBlink;
  while (getline(eyetrackingFile, line)) {
    postBlink.extractData(line);
    if (postBlink.isValid() && (not postBlink.isMSG()))
      break;
    if (line.find("TRIAL ENDS") != string::npos) {
			if (print2screen){
				cout << "\ntrial " << trialSet.g_currentTr() 
					<< " ENDS before enough postblink data gathered\n";
			}
// 			cout << line << '\n';
			// update trialInfoFile only if the "TRIAL ENDS" is encountered for 
			// the current trial
			if (trialSet.g_currentTr() == trialSet.g_trialIN())
				trialSet.resetAndUpdate(trialInfoFile);
			trialSet.updateInterp(false);
// 			cout << trialSet.g_currentTr() << " " << trialSet.g_trialIN() << '\n';
			// get a new line before exiting to prevent double processing of the same line
			getline(eyetrackingFile, line);
			return(trialSet);
    }
    if (line.find("onsetSoundStim") != string::npos) {
			trialSet.updateCurrentTrial(line, timeBefore);
			trialSet.updateInterp(false);
			if (print2screen){
				cout << "\ntrial " << trialSet.g_currentTr()
					<< " STARTS before enough postblink data gathered\n";
			}
// 			cout << line << '\n';
				// get a new line before exiting to prevent double processing of the same line
			// check if this blink should be interpolated or whether we should exiting
			//	5218300 <= 5218350
			//	5220146 >= 5218350
// 			cout << interpolation.iBegin() << ' ' << preBlink.g_time() << ' ' 
// 				<< interpolation.iEnd() << ' ' << trialSet.g_currentTr() << ' ' 
// 				<< trialSet.g_trialIN() << '\n';
			if (not ((preBlink.g_time() >= interpolation.iBegin())
				&& (preBlink.g_time() <= interpolation.iEnd())) ){
				getline(eyetrackingFile, line);
				return(trialSet);
			} /*else {
				cout << "I am doing " << trialSet.g_currentTr();
			}*/
    }
  } // end "while (true)"
  
// // 	cout << '.'; //<< '\n';
// 	if ( (interpolation.sub() ==  trialSet.g_subject()) && 
// 		(trialSet.g_currentTr() == interpolation.nTrial()) &&
// // 		(preBlink.g_time() >= interpolation.iBegin()) && 
// // 		(postBlink.g_time() <= interpolation.iEnd()) )
// // DO NOT TOUCH THIS, DO NOT TRY TO BE SMART, YOU ARE FUCKING STUPID AND 
// // YOU ARE NOT GOING TO UNCOMMENT THE PIECE BEFORE TO COMMENT THIS ONE
// // BELOW
// // YOU ARE JUST FUCKING STUPID! DON'T DO IT!!!
// 		(postBlink.g_time() >= interpolation.iBegin()) && 
// 		(preBlink.g_time() <= interpolation.iEnd()) ){
// 			trialSet.updateInterp(
// 				interpolateLongBlinks(outputfile, trialSet, interpolation, charlotte)
// 			);
// // 		if (interpolation.iBegin() < trialSet.g_targetOnset())
// // 			trialSet.updateBinCount((interpolation.iEnd() - trialSet.g_targetOnset()) / 4);
// // 		else
// // 			trialSet.updateBinCount((interpolation.iEnd() - interpolation.iBegin()) / 4);
//   }
	// 	cout << '.'; //<< '\n';
// 	if ( (interpolation.sub() ==  trialSet.g_subject()) && 
// 		(trialSet.g_currentTr() == interpolation.nTrial()) ){
// 		if ( (postBlink.g_time() >= interpolation.iBegin()) || 
// 			(preBlink.g_time() <= interpolation.iEnd()) )
// 			trialSet.updateInterp(interpolateLongBlinks(outputfile, 
// 																									trialSet, interpolation, charlotte) );
// 	}
	
// 	5003874 >= 5004552
// 	5003770 <= 5004778
// 	5004602 <= 5004778
// 	5004734 >= 5004552
	if ( (interpolation.sub() ==  trialSet.g_subject()) && 
		(trialSet.g_currentTr() == interpolation.nTrial()) && 
		(preBlink.g_time() <= interpolation.iEnd()) && 
		(postBlink.g_time() >= interpolation.iBegin()) )
			trialSet.updateInterp(interpolateLongBlinks(outputfile, 
																									trialSet, interpolation, charlotte) );
		
  return(trialSet);
}
// 32B1, 11, 2358098, 2358138
// 32B1, 11, 2361144, 2361282

// 32B1, 11, 2358138 > 2361144
// 32B1, 11, 2358098 < 2361282

// 2361238 > 2361144
// 2361198 < 2361282

