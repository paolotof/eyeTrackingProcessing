#include "main.ih"

size_t average200BeforeVisualOnset(string filename)
{
  ifstream subInfoFile("subnames.txt");
  
  if (!subInfoFile.is_open())
    cout << "Unable to open subnames.txt\n";
  else
  { 
    ofstream outputfile;
    filename.append("_average200_BeforeVisualOnset.asc");
    outputfile.open(filename);
		outputfile << "pp" << '\t'  << "tr" << '\t'  << "x" << '\t' << "y" << '\t' << "psize"  << '\n';  
    
    string subID;
    getline(subInfoFile, subID);
    
    while (true)
    {
      ifstream eyetrackingFile(subID);
      if (! eyetrackingFile.is_open())
      {
				cout << "Unable to open eyetracking datafile " << subID << '\n';
				break;
      }
      else
      {
				Dataline eye;
				string line;
				
				vector<double> xpos;
				vector<double> ypos;
				vector<double> psize;
				
				cout << subID << ' ';
				while (getline(eyetrackingFile, line))
				{
					/* ignore lines which are fix or sac messages */
					while ((line.find("FIX") != string::npos) || (line.find("SAC") != string::npos) || 
						(line.find("BLINK") != string::npos) || (line.find("BUTTON") != string::npos))
						getline(eyetrackingFile, line);
						
					if (line.find("onsetVisualStim") != string::npos)
					{
						string trialNum = line.substr(line.find("=") + 1, 
																					line.find("onset") - (line.find("=") + 1));
						
						if (xpos.size() > 10)
							exportBaseline(xpos, ypos, psize, subID, trialNum, outputfile);
						
						vector<double>().swap(xpos);
						vector<double>().swap(ypos);
						vector<double>().swap(psize);
						
						while (getline(eyetrackingFile, line))
						{
							if (line.find("TRIAL ENDS") != string::npos)
								break;
						}
					}// if (line.find("onsetVisualStim") != string::npos)
						
					eye.extractData(line);
					
// 					if (eye.isMSG() == false)
					if (eye.isValid() == true)
					{
						size_t lines2include = 50; // 200 ms is 50 lines each sampled every 4 seconds
						if (xpos.size() >= lines2include)
						{
							xpos.push_back(eye.g_xpos());
							xpos.erase(xpos.begin());
							ypos.push_back(eye.g_ypos());
							ypos.erase(ypos.begin());
							psize.push_back(eye.g_psize());
							psize.erase(psize.begin());
						}
						else
						{
							xpos.push_back(eye.g_xpos());
							ypos.push_back(eye.g_ypos());
							psize.push_back(eye.g_psize());
						}
					} // if (eye.isMSG() == false)
				} // end of "while(getline(eyetrackingFile, line))"
				
				eyetrackingFile.close();
				getline(subInfoFile, subID); // update subject counter
				cout << " processed \n";
      } // end "if(! eyetrackingFile.is_open()") 
      
      if (subInfoFile.eof())
				break;
      
    } // end "while(getline(subInfoFile, subID))"
    
    subInfoFile.close();
    outputfile.close();
  }
  return 1;
}