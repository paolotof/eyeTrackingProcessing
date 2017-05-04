#include "main.ih"


/*

  inputs:
  1) names of files with trials to process:
  - a) trials2beIncluded_withFillers.txt
  - b) trials2beIncluded_noFillers.txt
  2) third input specifying whether to use Charlotte ROI or not
       if there is no 3rd argument normal ROI is used.

 */

int main(int argc, char **argv)
try
{
//  	string dir2save = "/mnt/disk2/Data/testFolder/Anita/eyetracker/";
	string dir2save = "";
// 	string outputfile = dir2save + "test_";
	string filePrefix = "RT_";
	string outputfile = dir2save + filePrefix;
	size_t timeBefore = 1000;
	size_t baselineInt = 200;
	
	if (argc > 2)
		selectProcessing(argc, argv, outputfile, timeBefore);
  else
  {
		string filename = "";
		bool charlotte = false;
		if (argc > 1) // otherwise it will fail with only 1 input
		{
			string secondInput = argv[1];
			secondInput == "c" ? charlotte = true  : charlotte = false;
			cout << "Using " << (charlotte ? "Charlotte " : " normal") << 
				" areas definition for ROIS\n"; 
		}
		allAtOnce(charlotte, "trials2beIncluded_noFillers.txt", outputfile, timeBefore);
		allAtOnce(charlotte, "trials2beIncluded_withFillers.txt", outputfile, timeBefore);
		// this needs to be done only once, not for both
		cout << "Baseline Before Visual Stimulus Onset 1st trial ONLY\n\n";
		averageFirst200(outputfile, baselineInt);
		cout << "Baseline Before Visual Stimulus Onset Every Trial\n\n";
		average200BeforeVisualOnset(outputfile, baselineInt);
		cout << "Baseline Before Target (Sound Stimulus) Onset Every Trial\n\n";
		string eye2read = outputfile + "withFillers_" + to_string(timeBefore) +
			"_ALL_medianInterp.asc";
		baselineBeforeTarget(outputfile, eye2read, baselineInt);
		
		// this is not necessary because the withFillers file gets both of them, but it might be handier because the files would be smaller
		string outputfileName = filePrefix + "eyeData_noFillers_Baseline123.asc";
		ERDD_withBase123(outputfileName, timeBefore);
		binData(outputfileName, 5);
		outputfileName = filePrefix + "eyeData_withFillers_Baseline123.asc";
		ERDD_withBase123(outputfileName, timeBefore);
		binData(outputfileName, 5);
	}// if (argc > 2) else
  cout << "Finished processing\n";  
  return 0;
} // end main
catch (...)
{
    return 1;
}
