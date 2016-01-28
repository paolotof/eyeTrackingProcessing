#include "main.ih"

size_t selectProcessing(int narg, char **nameArgs)
{
  cout << "Choose report type:\n"
    "  1 - TARGET fixations\n"
    "  2 - Fixations from sentence onset\n"
    "  3 - BLINKS report\n"
    "  4 - Exclude trials with too long blinks \n"
    "  5 - NORMAL EXPORT\n"
    "  6 - BINNING export\n"
    "  7 - extract pupil size data -500:1500 ms of Stimulus Onset\n"
    "  8 - Average Baseline Before Stimulus Onset\n"
    "  9 - Average Baseline first 200 ms before Visual Onset\n"
    " 10 - exit\n"
    " 11 - 4, 5, 6, 8, and 9 together \n"
    "> ";      
      
    size_t reportType;
    cin >> reportType;
    
    if (narg < 3)
    {
      cout << "The programs runs with 3 arguments:\n" 
	<< "1) program name, \n"
	<< "2) text file (trials2beIncluded_noFillers.txt)\n"
	<< "3) time interval for baseline (600 ms)\n"
	<< "(e.g. ./binary trials2beIncluded_noFillers.txt 600)\n";
      return 0;
    }
    bool charlotte; 
    narg == 4 ? charlotte = true  : charlotte = false;    
    
    string filename = nameArgs[1];
    cout << filename << '\n';
    istringstream ss(nameArgs[2]);
    
    size_t interval4baseline;
    if (!(ss >> interval4baseline))
	cerr << "Invalid number " << nameArgs[2] << '\n';
    
    string outputfile = "tbt_";
    string dir2save = "/home/paolot/results/tmpTestsAnita/";
    dir2save = "";
    //outputfile.insert(outputfile.begin(), dir2save);
    outputfile.insert(0, dir2save);
    filename.insert(0, dir2save);
    
    if (filename.find("noFillers") != string::npos)
      outputfile.append("noFillers");
    else
      outputfile.append("withFillers");
      
    cout << outputfile << '\n';
    
    ifstream trialInfoFile(filename);
    if (!trialInfoFile.is_open())
      cout << "Unable to open "<< filename << '\n';
    else
    {
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
	  checkBlinks(trialInfoFile, 0, filename);
	  break;
	} 
	case 4:
	{
	  cout << "Exclude trials with too long blinks \n\n";
	  checkBlinksAndGetInterpBounds(trialInfoFile, 1, filename, dir2save);
	break;
	}
	case 5:
	{
	  cout << "EXPORTING\n\n";
	  processData(trialInfoFile, outputfile, charlotte, dir2save, interval4baseline);
	  break;
	}
	case 6:
	{
	  cout << "BINNING\n\n";
	  binData(outputfile, dir2save);
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
	  cout << "Average Baseline Before Stimulus Onset\n\n";
	  getMeanPsizeBeforeStim(trialInfoFile, outputfile);
	  break;
	} 
	case 9:
	{
	  cout << "Baseline Before Visual Stimulus Onset\n\n";
	  averageFirst200(outputfile);
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
	  checkBlinksAndGetInterpBounds(trialInfoFile, 1, filename, dir2save);
	  filename.replace(filename.find(".txt"), 4, "_noBlinks.txt");
	  ifstream updatedTrialInfoFile(filename);
	  if (not updatedTrialInfoFile.is_open())
	    cout << "Unable to open "<< filename << '\n';
	  else
	  {
	    cout << "EXPORTING\n\n";
	    processData(updatedTrialInfoFile, outputfile, charlotte, dir2save, interval4baseline);
	    cout << "BINNING\n\n";
	    binData(outputfile, dir2save);
	    cout << "Average Baseline Before Stimulus Onset\n\n";
	    if (not updatedTrialInfoFile.is_open())
	      updatedTrialInfoFile.open(filename, ifstream::in);
	    getMeanPsizeBeforeStim(updatedTrialInfoFile, outputfile);
	    cout << "Baseline Before Visual Stimulus Onset\n\n";
	    averageFirst200(outputfile);
	  }
	  break;
	}
	default:
	{
	  cout << "Unknown option\n\n";
	  break;
	}
      }
    }

    return 0;
  
} // end main
