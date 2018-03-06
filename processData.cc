#include "main.ih"

/* 
 * icmbuild program
 * mv tmp/bin/binary /home/paolot/ownCloud/Shared/eye-tracking_rhythm/lianne/contextEffects/eyetracker/debug_processingWithBaselines
 */ 


size_t processData(string& trial2beIncluded, string filename, 
									 bool charlotte, size_t timeBefore, 
									 bool print2screen, string filePrefix)
{
	string interpolationFile;
	string fillers;
  if (filename.find("noFillers") != string::npos) {
		interpolationFile = filePrefix + "noFillers";
		fillers = "no ";
	} else {
		interpolationFile = filePrefix + "withFillers";
		fillers = "fillers";
	}
  interpolationFile.append("_blinksPerSubjectSmaller300.txt"); 
	
	interpolationFile = eliminateRedundantTrials(trial2beIncluded,
																							 interpolationFile);
	cout << interpolationFile << '\n';
	// exit if eliminateRedundantTrials failed
	if (interpolationFile.find("none") != string::npos)
		return 0;
 	Interpdata interpolation;
	ifstream interpInfo(interpolationFile);
	if (! interpInfo.is_open()) {
		cout << "NO interpolation file "<< interpolationFile << '\n'; 
		cout << "Continue without interpolation information\n\n"; 
	} else {
		interpolation.setDoInterpolation(true);
		string line1, line2;
		getline(interpInfo, line1);
		getline(interpInfo, line2);
		interpolation = interpolation.extractInterpData(line1, line2);
	}	
	ifstream trialInfoFile(trial2beIncluded);
	if (not trialInfoFile.is_open()) {
		cout << "Unable To Open "<< trial2beIncluded << '\n';
		return 0;
	}
	string trialInfo;
	getline(trialInfoFile, trialInfo);
	TrialInfo trialSet;
	trialSet.extractInfo(trialInfo);
	trialSet.updateInterp(false); 
// 	trialSet.updateInterp(true); // set to true because the file has not been read once
	string subNum = trialSet.g_subject();  
  
	filename.append("_ALL_medianInterp.asc");
	ofstream outputfile;
  outputfile.open(filename);
// write file header
  outputfile << "pp" << '\t' << "clockTime" << '\t'  << "time" << '\t'
//     << "bin" << '\t'
    <<  "condition" << '\t' << "trial" << '\t' << "item" << '\t' << "exp"
    << '\t' << "psize"  << '\t' << "target" << '\t' << "competitor" 
    << '\t' << "d1" <<  '\t' << "d2" << '\t' << "fix" << '\n';  
    
	while (true) {
		if (trialInfoFile.eof())
			break;
		ifstream eyetrackingFile(subNum.append(".asc"));
    if (not eyetrackingFile.is_open()) {
      cout << "Unable to open eyetracking datafile " << subNum << '\n'; 
      break;
    } else {
			if (print2screen)
				cout  << "\nExtracting " << fillers << ' ' << subNum << ' ';
			
      trialSet.setCurrentTr(0);
      trialSet.resetBinsCounter();
      trialSet.setFix(1);
      Dataline eye, preBlink;
      string line;     
      while (getline(eyetrackingFile, line)) {
				while ((line.find("EFIX") != string::npos) || 
					(line.find("SAC") != string::npos))
					getline(eyetrackingFile, line);
// update trila counter at each onsetSoundStim and 
// skip all trials which shouldn't be included
				if (line.find("onsetSoundStim") != string::npos){
					trialSet.updateCurrentTrial(line, timeBefore);
					// NO! in some cases we need data before the onsetSoundStim
// 					cout << "skipping " << trialSet.g_currentTr() 
// 						<< " to " << trialSet.g_trialIN();
// 					while (trialSet.g_trialIN() > trialSet.g_currentTr()){
// 						getline(eyetrackingFile, line);
// 						if (eyetrackingFile.eof()){
// 							cout << "eyetrackingFile " << subNum << " ended while looping\n";
// 							break; // could this create trouble because of the updating?
// 						}
// 						if (line.find("onsetSoundStim") != string::npos)
// 							trialSet.updateCurrentTrial(line, timeBefore);
// 					}
// 					cout << '\n';
				}
// this must be out of the if trial in inclusion because 
// there are trials where the onsetSoundStim message is within a blink
//skip eyetracking lines between interpolation interval 
// update interpolation info
// NOTE: the two if statements are there because the reading might fail for each line,
// this is also valid for the end of file read
				if (line.find("SBLINK") != string::npos) {
					trialSet = interpolateBlinks(trialInfoFile, eyetrackingFile, outputfile, 
									trialSet, preBlink, interpolation, print2screen, 
									charlotte, timeBefore);
					if (trialSet.g_updateInterp()) {
						// read eye-tracking file up to line after interpolated data 
						// this avoids repetitions
						while (getline(eyetrackingFile, line)) {
							eye.extractData(line);
							if (eye.g_time() >= interpolation.iEnd())
								break;
						}
						string line1, line2;
						if (not interpInfo.eof())
							getline(interpInfo, line1);
						if (not interpInfo.eof()) {
							getline(interpInfo, line2);
							interpolation = interpolation.extractInterpData(line1, line2);
// 							cout << '\n' << line2;
						}
						trialSet.updateInterp(false); // I think we can leave it to false then
						// we can read another line if the subject, trial and time of interpInfo 
						// are earlier than the current subject, time and trial
						while ((interpolation.sub() < trialSet.g_subject()) 
							&& (interpolation.nTrial() < trialSet.g_currentTr())
							&& (interpolation.iEnd() < eye.g_time())
						) {
							if (interpInfo.eof())
								break;
							cout << "\n\nSYNCHING INTERPINFO AND OTHER FILES\n\n";
							string line1, line2;
							if (not interpInfo.eof())
								getline(interpInfo, line1);
							if (not interpInfo.eof()) {
								getline(interpInfo, line2);
								interpolation = interpolation.extractInterpData(line1, line2);
							}
						}
					} // END if (trialSet.g_updateInterp())
				} // END if (line.find("SBLINK") != string::npos)
				if (trialSet.g_currentTr() == trialSet.g_trialIN()) {
					if (line.find("TRIAL ENDS") != string::npos) {
						trialSet.resetAndUpdate(trialInfoFile);
						if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject()) {
							eyetrackingFile.close();
							break;
						} // END if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject()
					} // END if (line.find("TRIAL ENDS") != string::npos) 
				}
				eye.extractData(line);
				if (trialSet.g_currentTr() == trialSet.g_trialIN()) {	
					if ( (eye.isMSG() != true) && 
						(eye.g_time() >= trialSet.startExport()) // && 
// 						(eye.g_time() <= trialSet.timeIsUp())) // we go until the end of the trial
					) {
						if(eye.isValid()) {
							// participants with no interpolation can go without problems
							if (trialSet.g_subject() != interpolation.sub()) {
								double dataArray[4] = {0, 0, 0, 0}; 
								dataArray[0] = eye.g_time();
								dataArray[1] = eye.g_xpos();
								dataArray[2] = eye.g_ypos();
								dataArray[3] = eye.g_psize();
								trialSet.addOneBin();
								AssignAreaAndWriteOut(dataArray, outputfile, trialSet, charlotte);
							} else {  
								// stop exporting data if the data should be interpolated, this is 
								// to export interpolate data from before the SBLINK message:
								// 13-02-2018, it should also export data after interpolation intervals
								if (eye.g_time() <= interpolation.iBegin()
									|| eye.g_time() >= interpolation.iEnd()) {
									double dataArray[4] = {0, 0, 0, 0}; 
									dataArray[0] = eye.g_time();
									dataArray[1] = eye.g_xpos();
									dataArray[2] = eye.g_ypos();
									dataArray[3] = eye.g_psize();
									trialSet.addOneBin();
									AssignAreaAndWriteOut(dataArray, outputfile, trialSet, charlotte);
								} // end (eye.g_time() <= interpolation.iBegin())
							} // end: if (trialSet.g_subject() != interpolation.sub())
						} // end :if(eye.isValid())
					} // end of "if (!eye.isMSG && (eye.g_time() >= trialSet.g_targetOnset()))"
					if ( (eye.g_time() >= trialSet.startExport()) && 
// 						(eye.g_time() <= trialSet.timeIsUp()) && 
						(line.find("SFIX") != string::npos) )
							trialSet.setFix(trialSet.g_Fix() + 1);
				} // end of "if (iTrial == trialSet.g_trialIN())"
				preBlink = eye;
      } // end of "while(getline(eyetrackingFile, line))"
    } // end "if(! eyetrackingFile.is_open()") 
    eyetrackingFile.close();
    subNum = trialSet.g_subject();  
// 		cout << subNum << '\n';
	} // end "while(true)"
  trialInfoFile.close();
  outputfile.close();
	if (print2screen)
		cout  << " finished processData\n"; // get to next participant
  
  return 1;
}
