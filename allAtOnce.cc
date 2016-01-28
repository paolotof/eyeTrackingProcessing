#include "main.ih"

size_t allAtOnce(bool charlotte, string filename)
{
  string dir2save = "/home/paolot/results/tmptestsanita/";
  dir2save = "";
  string outputfile = "tbt_";;
  outputfile.insert(0, dir2save);

  filename.insert(0, dir2save);
  
  if (filename.find("noFillers") != string::npos)
    outputfile.append("noFillers");
  else
    outputfile.append("withFillers");
  
  ifstream trialinfofile(filename);
  checkBlinksAndGetInterpBounds(trialinfofile, 1, filename, dir2save);

  filename.replace(filename.find(".txt"), 4, "_noBlinks.txt");
  ifstream updatedTrialInfoFile(filename);
  if (not updatedTrialInfoFile.is_open())
    cout << "Unable To Open "<< filename << '\n';
  else
  {
    cout << "Exporting\n\n";
    size_t interval4baseline = 200;
    // add baseline length to the filename 
    outputfile.append("_");
    outputfile += std::to_string(interval4baseline);
    
    processData(updatedTrialInfoFile, outputfile, charlotte, dir2save, interval4baseline);
    cout << "Binning\n\n";
    binData(outputfile, dir2save);
    cout << "Average Baseline Before Stimulus Onset\n\n";
    if (not updatedTrialInfoFile.is_open())
      updatedTrialInfoFile.open(filename, ifstream::in);
    //size_t interval4baseline = 600;
//     size_t interval4baseline = 200;    
    getMeanPsizeBeforeStim(updatedTrialInfoFile, outputfile, interval4baseline);
    cout << "Baseline Before Visual Stimulus Onset\n\n";
    averageFirst200(outputfile);
  }

  return 0;
}