#include "main.ih"

int main(int argc, char **argv)
try
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
  if (argc == 1)
    testNewClass();
  else
  {
      
      
    size_t reportType;
    cin >> reportType;
    
    bool charlotte; 
    argc >= 3 ? charlotte = true  : charlotte = false;    
    
    string filename = argv[1];
    
    string outputfile = "tbt";
    
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
	  checkBlinksAndGetInterpBounds(trialInfoFile, 1, filename);
	break;
	}
	case 5:
	{
	  cout << "EXPORTING\n\n";
	  processData(trialInfoFile, outputfile, charlotte);
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
	  checkBlinksAndGetInterpBounds(trialInfoFile, 1, filename);
	  filename.replace(filename.find(".txt"), 4, "_noBlinks.txt");
	  ifstream updatedTrialInfoFile(filename);
	  if (not updatedTrialInfoFile.is_open())
	    cout << "Unable to open "<< filename << '\n';
	  else
	  {
	    cout << "EXPORTING\n\n";
	    processData(updatedTrialInfoFile, outputfile, charlotte);
	    cout << "BINNING\n\n";
	    binData(outputfile);
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
  } // ifelse test session  
  
} // end main
catch (...)
{
    return 1;
}

/* ------- EXTRACT ROI FROM ASC FILE -------- */ 
/*
 * use as: ./binary 5 // to bin 5 lines in one 
 * or:
 * ./binary // to not bin anything
 * 
 
./binary trials2beIncluded_withFillers.txt 5                                     

cout << "EXPORTING\n\n";
processData(trialInfoFile, outputfile);
./binary trials2beIncluded_new.txt 5 > blinksPerSubject.txt

*/
