#include "main.ih"


size_t allAtOnce(bool charlotte, string trial2beIncluded, string outputfile, size_t timeBefore)
{
/*	
 *            NO BASELINE COMPUTATION IN THIS FILE
 * 
	*/
	
// Source data should be kept in the same folder, only processed data which 
//	might yield  different results should be move to a different folder, to not 
//	overwrite existing data.	
//  trial2beIncluded.insert(0, dir2save); 
  
  if (trial2beIncluded.find("noFillers") != string::npos)
    outputfile.append("noFillers");
  else
    outputfile.append("withFillers");
  
//   ifstream trialinfofile(trial2beIncluded);
// 	checkBlinksAndGetInterpBounds(trialinfofile, 1, trial2beIncluded, outputfile);

  trial2beIncluded.replace(trial2beIncluded.find(".txt"), 4, "_noBlinks.txt");
  ifstream updatedTrialInfoFile(trial2beIncluded);
  if (not updatedTrialInfoFile.is_open())
    cout << "Unable To Open "<< trial2beIncluded << '\n';
  else
  {
// //     cout << "Exporting\n\n";
// 		//size_t timeBefore = 200; // time interval before the onset of the target word
//     // add baseline length to the trial2beIncluded 
    outputfile.append("_");
		outputfile += std::to_string(timeBefore);
// // 		cout << outputfile <<'\n';
// 		processData(updatedTrialInfoFile, outputfile, charlotte, timeBefore);
// //     cout << "Binning\n\n";
//     binData(outputfile);
// 		// bin data as in the Barr 2008 JML paper
		size_t timeInt2bin = 50;
		binTimeInterval(outputfile, timeInt2bin);
  }

  return 0;
}