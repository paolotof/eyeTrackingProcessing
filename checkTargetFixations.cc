#include "main.ih"

/* 
 * before using this file generate the file fileList containing a list of the 
 * eyetracking files to be analysed. The file can be created as:
 * ls *.asc > fileList
 * 
 * move to folder:
 * cd /home/paolot/Dropbox/c++Folder/2014/binEyeFix/countInterpolation/
 * 
 * compile:
 * g++ --std=c++0x -Wall countInterpolation.cc -o counts
 
 * use this program as ./countInterpolation > report.txt
 
 */

int checkTargetFixations(ifstream& trialInfoFile)
{
  
  ofstream outputfile;
  outputfile.open("fixationInfos.txt");
  // file header,  
  outputfile << "subject \t trialnum  \t target \t " 
    << " xpos \t ypos \t psize \t duration \n";
  
  
  string trialInfo;
  while (getline(trialInfoFile, trialInfo))
  {
    TrialInfo i;
    i = trialSetup(trialInfo, i);  
    string subNum = i.g_subject();  
    
    ifstream eyetrackingFile(subNum.append(".asc"));
    if (! eyetrackingFile.is_open())
    {
      cout << "Unable to open eyetracking datafile " << subNum << '\n';
      break;
    }
    else
    {
      size_t iTrial = 0;
      string line;
      size_t onsetWord = 0;// this is to fix the warning, but might induce errors. Do check!
      
      while (getline(eyetrackingFile, line))
      {
	if (line.find("onsetSoundStim") != string::npos)
	{
	  iTrial++;
	  if (iTrial == i.g_trialIN())
	  {
	    istringstream linedata(line); 
	    string rubbish;
	    size_t timeZero;
	    linedata >> rubbish >> timeZero;
	    onsetWord = timeZero + i.g_targetStarts();
	  } // end of "if (iTrial == trial2include)"
	} // end of "if (line.find("SoundStim") != string::npos)"
	
	if (iTrial == i.g_trialIN())
	{
	  if (line.find("TRIAL ENDS") != string::npos)
	  {
	    getline(trialInfoFile, trialInfo); 
	    i = trialSetup(trialInfo, i); 
	  }
	  
	  if ((line.find("EFIX") != string::npos))
	  {
	    istringstream linedata(line); // extract time information from the datafile
// 	    string msgType, eye, stime, etime, axp, ayp, aps;
	    string rubbish;
	    size_t stime, etime;
	    double xpos, ypos, psize; //, dur; promoted to rubbish because unreliable
	    
// 	    linedata >> msgType >> eye >> stime >> etime >> 
// 	      dur >> axp >> ayp >> aps >> 
// 	      axp >> ayp >> aps;
	    linedata >> rubbish >> rubbish >> stime >> etime >> 
	      rubbish >> rubbish >> rubbish >> rubbish >> 
	      xpos >> ypos >> psize;

	    if (etime >= onsetWord)
	      outputfile << i.g_subject() << '\t' << i.g_trialIN()  << '\t' <<
		i.g_target() << '\t' << xpos << '\t' << ypos << '\t' << 
		psize << '\t' << etime-onsetWord << '\n';
		// psize << '\t' << etime-stime << '\n';
		
	  } // if (line.find("EFIX") != string::npos) 
	} // if (iTrial == i.g_trialIN())
      } // while (getline(eyetrackingFile, line))
      
      eyetrackingFile.close();
    } // end "if(! eyetrackingFile.is_open()") 
    if (trialInfoFile.eof())
      break;
  } // end "while(getline(trialInfoFile, trialInfo))"
  trialInfoFile.close();
  outputfile.close();
  cout << "Output stored in fixationInfos.txt\n"; 
  return 1;
}