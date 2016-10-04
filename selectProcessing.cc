#include "main.ih"

size_t selectProcessing(int narg, char **nameArgs, string outputfile, size_t timeBefore)
{
	if (narg < 3)
	{
		cout << "The programs runs with at least 3 arguments:\n" 
		<< "1) program name, \n"
		<< "2) text file (trials2beIncluded_noFillers.txt)\n"
		<< "3) time interval for baseline (600 ms)\n"
		<< "(4 | default: 200) time interval to start data export\n"
		<< "(5 | default: not) use Charlotte ROI definition?\n"
		<< "(e.g. ./binary trials2beIncluded_noFillers.txt 600)\n"
		<< "(OR   ./binary trials2beIncluded_noFillers.txt 600 charlotte)\n"
		<< "(OR   ./binary trials2beIncluded_noFillers.txt 600 200)\n"
		<< "(OR   ./binary trials2beIncluded_noFillers.txt 600 200 charlotte)\n";
		return 0;
	}
	
// analysis INPUTS for processing		
// 1 - assign trials2beIncluded
	string trials2beIncludedFile = nameArgs[1];
	
// 2 - assign baseline interval
	istringstream baselineInputValue(nameArgs[2]);
	size_t interval4baseline;
	if (!(baselineInputValue >> interval4baseline))
		cerr << "The values entered for baseline should be size_t data-type" << nameArgs[2] << '\n';
// 3 &| 4	
	bool charlotte; 
	narg == 5 ? charlotte = true  : charlotte = false;    
	if (narg > 3)
	{
		istringstream intPreTarget(nameArgs[4]);
		if (!(intPreTarget >> timeBefore)) // then it's a string
		{
			cout << "assuming data should be processed with ROI of Charlotte\n";
			charlotte = true;
		}
	} // if narg > 3
	if (trials2beIncludedFile.find("noFillers") != string::npos)
		outputfile.append("noFillers");
	else
		outputfile.append("withFillers");
	
	ifstream trialInfoFile(trials2beIncludedFile);
	if (!trialInfoFile.is_open())
		cout << "Unable to open "<< trials2beIncludedFile << '\n';
	else
	{
		// chose analysis type
		cout << "Choose report type:\n"
		"  1 - TARGET fixations\n"
		"  2 - Fixations from sentence onset\n"
		"  3 - BLINKS report\n"
		"  4 - Exclude trials with too long blinks \n"
		"  5 - NORMAL EXPORT\n"
		"  6 - BINNING export\n"
		"  7 - extract pupil size data -500:1500 ms of Stimulus Onset\n"
		"  9 - Average Baseline first 200 ms before Visual Onset 1st Trial\n"
		" 10 - exit\n"
		" 11 - 4, 5, 6, 8, and 9 together \n"
		" 12 - Average Baseline first 200 ms before Each Visual Onset\n"
		"> ";      
//		"  8 - Average Baseline Before Each Sound-Target Onset\n" // this is not implemented yet
		
		size_t reportType;
		cin >> reportType;
		switch (reportType) 
		{
			case 1:
			{
				cout << "FIXATIONS on TARGET\n\n";
				checkTargetFixations(trialInfoFile);
				break;
			}
			case 2:
			{
				cout << "FIXATIONS from SENTENCE onset\n\n";
				getFixationsFromSentenceOnset(trialInfoFile);
				break;
			}
			case 3:
			{
				cout << "BLINKS REPORT\n\n";
				checkBlinks(trialInfoFile, 0, trials2beIncludedFile);
				break;
			} 
			case 4:
			{
				cout << "Exclude trials with too long blinks \n\n";
				checkBlinksAndGetInterpBounds(trialInfoFile, 1, trials2beIncludedFile, outputfile);
			  break;
			}
			case 5:
			{
				cout << "EXPORTING\n\n";
				processData(trialInfoFile, outputfile, charlotte, timeBefore);
				break;
			}
			case 6:
			{
				cout << "BINNING\n\n";
				binData(outputfile);
				break;
			}
			case 7:
			{
				cout << "Extracting pupil size around visual onset\n\n";
				extractPupil(trialInfoFile, outputfile);
				break;
			}
			case 8:
			{	  
				cout << "Baseline Before Visual Stimulus Onset Every Trial\n\n";
				average200BeforeVisualOnset(outputfile, interval4baseline);
				break;
			} 
			case 9:
			{
				cout << "Baseline Before Visual Stimulus Onset 1st trial ONLY\n\n";
				averageFirst200(outputfile, interval4baseline);
				break;
			}
			case 10:
			{
				cout << "exiting NOW!!\n\n";
				break;
			}
			case 11:
			{
				cout << "All processing in a row!!!\n\n";
				cout << "Exclude trials with too long blinks \n\n";
				checkBlinksAndGetInterpBounds(trialInfoFile, 1, trials2beIncludedFile, outputfile);
				trials2beIncludedFile.replace(trials2beIncludedFile.find(".txt"), 4, "_noBlinks.txt");
				ifstream updatedTrialInfoFile(trials2beIncludedFile);
				if (not updatedTrialInfoFile.is_open())
					cout << "Unable to open "<< trials2beIncludedFile << '\n';
				else
				{
					cout << "EXPORTING\n\n";
					// add baseline length to the trials2beIncludedFile 
					outputfile.append("_");
					outputfile += std::to_string(timeBefore);
					processData(updatedTrialInfoFile, outputfile, charlotte, timeBefore);
					cout << "BINNING\n\n";
					binData(outputfile);
					cout << "Baseline Before Visual Stimulus Onset 1st trial ONLY\n\n";
					averageFirst200(outputfile, interval4baseline);
					cout << "Baseline Before Visual Stimulus Onset Every Trial\n\n";
					average200BeforeVisualOnset(outputfile, interval4baseline);
				}
				break;
			}
// 			case 12:
// 			{
// 				cout << "Baseline Before Visual Stimulus Onset Every Trial\n\n";
// 				average200BeforeVisualOnset(outputfile, interval4baseline);
// 				break;
// 			}
			default:
			{
				cout << "Unknown option\n\n";
				break;
			}
		} // end switch
	} // end if (!trialInfoFile.is_open())
	
	return 0;
} // end main
