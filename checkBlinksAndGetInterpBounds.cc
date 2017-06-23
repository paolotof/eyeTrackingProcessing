#include "main.ih"

/* This file does two things:
 * 1 - 	a) creates a file containing blinks longer than 300 ms. These trials are excluded from
 * further analysis
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
 * EXCEPTIONS:
 * 
 * SBLINK - 
 * s27b2.asc Leanne, trial 32 has an sblink with an eblink with less than 25 lines to read, 
 * data should be exported before updating the trial count
 * 
 * includeTrial specifies if the trial is included or not given the length of the blink.
 * It is included because more blinks can happen in one trial, but not all might 
 * be shorter than 300 ms. If one blink in the extraction interval is longer than 300ms
 * all the trial goes out. includeTrial is true when 1) opening an eye-tracking file, 2)
 * every time that a TRIAL ENDS message is fonud; it is false any time a blink is 
 * longer than 300 ms
 * 
 */

//bool checkBlinksAndGetInterpBounds(ifstream& trialInfoFile, size_t reportType, string filename, string dir2save)
string checkBlinksAndGetInterpBounds(string filename, 
																		 string nameOutputfile, bool print2screen, 
																		 size_t timeBefore)
{
	cout << "Blinks and interpolation\n";
	
	ifstream trialInfoFile(filename);
	
	if (filename.find("noFillers") != string::npos)
		nameOutputfile.append("noFillers_blinksPerSubjectSmaller300.txt");
	else
		nameOutputfile.append("withFillers_blinksPerSubjectSmaller300.txt");
	
	// create new file name
  filename.replace(filename.find(".txt"), 4, "_noBlinks.txt");
	
	// file containing blinks shorter than 300 ms
	ofstream shortBlinksReport;
	shortBlinksReport.open(nameOutputfile);  
  // file containing trials to be included without blinks
  ofstream updatedTrialInfoFile;
  updatedTrialInfoFile.open(filename);
  
	string trialInfo;
  TrialInfo trialSet;
	while (getline(trialInfoFile, trialInfo))
  {
    trialSet.extractInfo(trialInfo);
    string subNum = trialSet.g_subject();  
    ifstream eyetrackingFile(subNum.append(".asc"));
    if (! eyetrackingFile.is_open())
    {
      cout << "Unable to open eyetracking datafile " << subNum << '\n';
      break;
    }
    else
    {
			if (print2screen)
				cout << subNum << ' ';
      trialSet.setCurrentTr(0);
      vector<double> tVect;
      vector<double> xVect;
      vector<double> yVect;
      vector<double> pVect;
      size_t lines2include = 25;
			median_t medv;
      string line;
      Dataline eye;
      //size_t onsetWord = 0;
      double blinkDuration = 0;
			bool includeTrial = true;
      while (getline(eyetrackingFile, line)) {
// BEGINNING OF SENTENCE SOUND
				if (line.find("onsetSoundStim") != string::npos){
					trialSet.updateCurrentTrial(line, timeBefore);
					// skip all trials which shouldn't be included
// 					cout << "skipping " << trialSet.g_currentTr() 
// 						<< " to " << trialSet.g_trialIN();
					// NO! there might be data that we need before the onset of the target sound
// 					while (trialSet.g_trialIN() > trialSet.g_currentTr()){
// 						getline(eyetrackingFile, line);
// 						if (eyetrackingFile.eof()){
// 							cout << "eyetrackingFile " << subNum << " ended while looping\n";
// 							break; // could this create trouble because of the updating?
// 						}
// 						if (line.find("onsetSoundStim") != string::npos)
// 							trialSet.updateCurrentTrial(line, timeBefore);
// 					}
				} // if (line.find("onsetSoundStim") != string::npos)
				
// TRIAL ENDS - update trial information/reset arrays
				if ( (line.find("TRIAL ENDS") != string::npos) && 
					(trialSet.g_currentTr() == trialSet.g_trialIN()) ){
					if (includeTrial)
						updatedTrialInfoFile << trialInfo << '\n';
					includeTrial = true;
					blinkDuration = 0;
					getline(trialInfoFile, trialInfo); 
					trialSet.extractInfo(trialInfo);
					// if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject())
					vector<double>().swap(tVect);
					vector<double>().swap(xVect);
					vector<double>().swap(yVect);
					vector<double>().swap(pVect);
					// if the current subject does not have trials that should be 
					// considered anymore stop with this file and get to the next one
					if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject())
					{
						eyetrackingFile.close();
						break;
					}
				} // if ((line.find("TRIAL ENDS") != string::npos) && (trialSet.g_currentTr() == trialSet.g_trialIN()))

			// BLINK STARTS - get median of previously stored values
			// Error the sblink and eblink contains the onset of the target, but the trial is not interpolated
// 				if ((line.find("SBLINK") != string::npos) && 
// 					  (trialSet.g_currentTr() == trialSet.g_trialIN()) && 
// 					  (tVect.size() >= 1) ){
				if ((line.find("SBLINK") != string::npos) && 
// 					(trialSet.g_currentTr() == trialSet.g_trialIN()) && 
					(tVect.size() >= 1) ){
					medv.time = vectorMedian(tVect);
					medv.x = vectorMedian(xVect);
					medv.y = vectorMedian(yVect);
					medv.psize = vectorMedian(pVect);
					// reset vector 
					vector<double>().swap(tVect);
					vector<double>().swap(xVect);
					vector<double>().swap(yVect);
					vector<double>().swap(pVect);
				}
			// BLINK ENDS	
				if ( (line.find("EBLINK") != string::npos) && 
					(trialSet.g_currentTr() == trialSet.g_trialIN()) ){
					istringstream linedata(line); // extract time information from the datafile
					string msgType, side;
					double blinkBegin, blinkEnd;
					linedata >> msgType >> side >> blinkBegin >> blinkEnd;
// 					if ( (blinkEnd >= trialSet.g_targetOnset()) //&&
					if ( (blinkEnd >= trialSet.startExport()) //&& 
// // 						(blinkBegin <= trialSet.timeIsUp()) 
					){
						blinkDuration = blinkEnd - blinkBegin;
						// recompute the duration of the blink
						if (blinkBegin <= trialSet.startExport())
							blinkDuration = blinkEnd - trialSet.startExport();
						bool exportData;
						if (blinkDuration > 300) {
							includeTrial = false; // it needs to be false only once, then the trial 
							//is out, however all the previous interpolation information might have 
							// been written already. 
						} else {
// 							includeTrial = true;
							exportData = true;
// get other 25 lines/points of data to get the median of post blink
							for (size_t lines2skip = 0; lines2skip < lines2include; lines2skip++)	{
								getline(eyetrackingFile, line);
									// interrupt all extrapolation for current blink // 23-05-2017 WHY? 
									// this gives problem, let's interpolate if there is enough to interpolate
								if (line.find("TRIAL ENDS") != string::npos) {
									if (tVect.size() >= 1) {
										median_t medAfter;
										medAfter.time = vectorMedian(tVect);
										medAfter.x = vectorMedian(xVect);
										medAfter.y = vectorMedian(yVect);
										medAfter.psize = vectorMedian(pVect);
										exportInterpolation(shortBlinksReport, trialSet, medv, medAfter);
										// update the line for the new interpolation
										medv.time = medAfter.time;
										medv.x = medAfter.x;
										medv.y = medAfter.y;
										medv.psize = medAfter.psize;
									}
									vector<double>().swap(tVect);
									vector<double>().swap(xVect);
									vector<double>().swap(yVect);
									vector<double>().swap(pVect);
									// do all the operations for 'trials2beIncluded' file update, 
									// since other blinks 
									// in this current trial should be included too
									updatedTrialInfoFile << trialInfo << '\n';
									getline(trialInfoFile, trialInfo); 
									trialSet.extractInfo(trialInfo);
									exportData = false; // data have already been written out
									if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject())
										eyetrackingFile.close();
									break; // breaks the for loop
								} // if (line.find("TRIAL ENDS") != string::npos)
								// in case another blink happens before 25 lines
								if (line.find("SBLINK") != string::npos) {
									exportData = true; 
									break;
								} //if (line.find("SBLINK") != string::npos)
// update array with data for postblink interpolation
								eye.extractData(line);
								if (not eye.isMSG() && eye.isValid()) {
									tVect.push_back(eye.g_time());
									xVect.push_back(eye.g_xpos());
									yVect.push_back(eye.g_ypos());
									pVect.push_back(eye.g_psize());
								} // if (not eye.isMSG() && eye.isValid())
							} // for (size_t lines2skip = 0; lines2skip < lines2include; lines2skip++) 
							// Export data only if 'TRIAL ENDS' message wasn't encountered
							if (exportData) {
// 							if (tVect.size() > 1) {	
								// begin interpolation
								median_t medAfter;
								medAfter.time = vectorMedian(tVect);
								medAfter.x = vectorMedian(xVect);
								medAfter.y = vectorMedian(yVect);
								medAfter.psize = vectorMedian(pVect);
								exportInterpolation(shortBlinksReport, trialSet, medv, medAfter);
								medv.time = medAfter.time;
								medv.x = medAfter.x;
								medv.y = medAfter.y;
								medv.psize = medAfter.psize;
								vector<double>().swap(tVect);
								vector<double>().swap(xVect);
								vector<double>().swap(yVect);
								vector<double>().swap(pVect);
							} // end if (exportData) 
						} // END if (blinkDuration < 300)
					} // END if ( (blinkEnd >= trialSet.startExport())
				} // end if ((line.find("EBLINK") != string::npos) && (trialSet.g_currentTr() == trialSet.g_trialIN()))

			// EXTRACT DATA	
				eye.extractData(line);
				if ((not eye.isMSG()) && (eye.isValid())) {
					if (tVect.size() <= lines2include) {
						tVect.push_back(eye.g_time());
						xVect.push_back(eye.g_xpos());
						yVect.push_back(eye.g_ypos());
						pVect.push_back(eye.g_psize());
					} else {
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
    
    if (print2screen)
			cout  << " processed\n"; 
    eyetrackingFile.close();

    if (trialInfoFile.eof())
      break;
    
  } // end "while(getline(trialInfoFile, trialInfo))"
  
  trialInfoFile.close();
  updatedTrialInfoFile.close();
  shortBlinksReport.close();
	if (print2screen)
		cout << "end\n";
  return "blinks processed";
}
