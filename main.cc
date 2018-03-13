#include "main.ih"


/*

  inputs:
  1) names of files with trials to process:
  - a) trials2beIncluded_withFillers.txt
  - b) trials2beIncluded_noFillers.txt
  2) third input specifying whether to use Charlotte ROI or not
       if there is no 3rd argument normal ROI is used.

	01-05-2017 removed time is up, now trials go to the end of recording
	
	06-03-2018 added interface to extend interval of baseline, time before for 
	the extraction of the output and to what text the time-locking should be performed
	./binary trials2beIncluded_withFillers.txt 300 200 "onsetSoundStim"
	./binary trials2beIncluded_withFillers.txt 300 200 "onsetSoundStim" "word"
	./binary trials2beIncluded_withFillers.txt 0 200 "onsetSoundStim" "sentence"
	*/

int main(int argc, char **argv)
try{
	
	/*   ---   check program's input, if any   ---   */
	size_t timeBefore = 300;
	size_t baselineInt = 200;
	const char *str2find = "onsetSoundStim";
	string timeLocking = "word";
	// change the names so that the files that it creates also say which 
	// type of string has been used for time locking
	
	
	bool charlotte = false;
	// otherwise it will fail with only 1 input
	if (argc > 1) {
		string secondInput = argv[1];
		secondInput == "c" ? charlotte = true  : charlotte = false;
		cout << "Using " << (charlotte ? "CHARLOTTE's " : " MARLEEN's ") << 
			" areas definition for ROIS\n"; }
	if (argc > 3) {
		timeBefore = stoi(argv[2]);
		baselineInt = stoi(argv[3]);
		str2find = argv[4];
		timeLocking = argv[5]; // 'word', 'sentence'
		timeLocking += "_";
	} 
// if argv[0] contains 'debug' then print out to screen otherwise do not
	bool print2screen = false;
	string programName = argv[0];
	if (programName.find("debug") != string::npos) 
		print2screen = true;
// 	cout << print2screen << '\n';
// 	return 0;
	// -------------------------------------
	
	// define file names for the export
//  	string dir2save = "/mnt/disk2/Data/testFolder/Anita/eyetracker/";
	string dir2save = "";
// 	string outputfile = dir2save + "test_";
	string filePrefix = "RT_" + timeLocking ;
	string outputfile = dir2save + filePrefix;
	
	pid_t pid;
	pid = fork();
	size_t indexOfProcess = 0;
// 	size_t nForks = 2;
	switch(pid)	{
		case 0: /* child */
		{
			cout << "CHILD n " << indexOfProcess << '\n';
			string trial2beIncluded = "trials2beIncluded_withFillers.txt";
			checkBlinksAndGetInterpBounds(trial2beIncluded, outputfile,
																		print2screen, timeBefore, str2find, timeLocking);
			trial2beIncluded.replace(trial2beIncluded.find(".txt"), 4, "_noBlinks.txt");
			outputfile.append("withFillers");
			outputfile.append("_");
			outputfile += std::to_string(timeBefore);
			processData(trial2beIncluded, outputfile, charlotte, timeBefore,
									print2screen, filePrefix, str2find, timeLocking);
			// 			string eye2read = outputfile + "withFillers_" + to_string(timeBefore) 
			// 				+ "_ALL_medianInterp.asc";
			string eye2read = outputfile + "_ALL_medianInterp.asc";
			if (timeLocking.find("word") != string::npos) 
				baselineBeforeTarget((dir2save + filePrefix), eye2read, baselineInt);
			// for timeLocking == "sentence" we do not have an interval we can use, it coincide with average200BeforeVisualOnset
			binData(outputfile);
// 			cout << "bin data as in the Barr 2008 JML paper\n";
// 			size_t timeInt2bin = 50;
// 			binTimeInterval(outputfile, timeInt2bin);
			string outputfileName = filePrefix 
			+ "eyeData_withFillers_Baseline123.asc";
			ERDD_withBase123(outputfileName, timeBefore, baselineInt, filePrefix);
			binData(outputfileName, 5);
			checkInterpolations(("_" + timeLocking + "withFillers"), timeBefore);
			indexOfProcess++;
			exit(0);
		}
		case -1: {
			cout << "Failed to fork " << indexOfProcess <<'\n';
			exit(1); }
		default:  /* parent */
		{
			cout << "PARENT n " << indexOfProcess << '\n';
			string trial2beIncluded = "trials2beIncluded_noFillers.txt";
			checkBlinksAndGetInterpBounds(trial2beIncluded, outputfile,
																		print2screen, timeBefore, str2find, timeLocking);
			trial2beIncluded.replace(trial2beIncluded.find(".txt"), 4,
															 "_noBlinks.txt");
			outputfile.append("noFillers");
			outputfile.append("_");
			outputfile += std::to_string(timeBefore);
			processData(trial2beIncluded, outputfile, charlotte, timeBefore,
									print2screen, filePrefix, str2find, timeLocking);
			// these two can be done only once, not for both fillers and non-fillers they 
			// are pulled here because only fillers take a lot less to process than with fillers too	
			cout << "Baseline Before Visual Stimulus Onset 1st trial ONLY\n\n";
			averageFirst200((dir2save + filePrefix), baselineInt, print2screen);
			cout << "Baseline Before Visual Stimulus Onset Every Trial\n\n";
			average200BeforeVisualOnset((dir2save + filePrefix), baselineInt,
																	print2screen);
			binData(outputfile);
			
// 			cout << "bin data as in the Barr 2008 JML paper\n";
// 			size_t timeInt2bin = 50;
// 			binTimeInterval(outputfile, timeInt2bin);
			
			// it is not necessary to extract data for no fillers per se, 
			// but it is handy because the files are smaller and therefore easier to handle in R
			string outputfileName = filePrefix + "eyeData_noFillers_Baseline123.asc";
			ERDD_withBase123(outputfileName, timeBefore, baselineInt, filePrefix);
			binData(outputfileName, 5);
			checkInterpolations(("_" + timeLocking + "noFillers"), timeBefore);
			// 			cout << "done\n";
			indexOfProcess++;
// 			exit(0);
			// it might be that it needs an exit() here as well
		}
	}  // switch(pid)
	// wait processes to terminate
	cout << "waiting for children to finish\n";
	waitpid(-1, NULL, 0);
  cout << "Finished processing\n";  
  return 0;
} // end main
catch (...) { return 1; }
