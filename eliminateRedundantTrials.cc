#include "main.ih"
/* 
 * remove trials that should not be interpolated from the interpolation file
*/

string eliminateRedundantTrials(string& trial2beIncluded, string& interpolationFile)
{
	cout << "deleting trials which do not match between interpInfo and trialInfoFile\n";
	ifstream trialInfoFile(trial2beIncluded);
	if (not trialInfoFile.is_open())
	{
		cout << "Unable To Open "<< trial2beIncluded << '\n';
		return "none";
	}
	ifstream interpInfo(interpolationFile);
	if (not interpInfo.is_open())
	{
		cout << "NO interpolation file "<< interpolationFile << '\n';
		return "none";
	}
	string newInterpolationFile = "clean_"; 
	newInterpolationFile += interpolationFile;
	ofstream outputfile(newInterpolationFile);
	if (not outputfile.is_open())
	{
		cout << "NO OUTPUT FILE\n";
		return "none";
	}
	string subTrial = "none";
	int numTrial = 0;
	while (true)
	{
		string line1, line2;
		getline(interpInfo, line1);
		if (interpInfo.eof())
			break;
		getline(interpInfo, line2);
		if (interpInfo.eof())
			break;
		istringstream secondLine(line2);
		string interpSub;
		int interpTrial;
		secondLine >> interpSub >> interpTrial;
// 		interpSub.erase(interpSub.end()-4, interpSub.end()); // remove '.asc' from name
// the approach above gives problems with Leanne's data
		// remove '.asc' from name
		size_t found = interpSub.find(".asc");
		if (found != string::npos)
			interpSub.erase(found, string::npos);
		if (( subTrial == interpSub) &&
			(numTrial == interpTrial) ) {
			outputfile << line1 << '\n';
			outputfile << line2 << '\n';
		} else {
			string line;
			while (getline(trialInfoFile, line)) {
				istringstream trialLine(line);
				trialLine >> subTrial >> numTrial;
				if ( (subTrial == interpSub) &&
					(numTrial == interpTrial) ) {
					outputfile << line1 << '\n';
					outputfile << line2 << '\n';
					// multiple lines might have interpolation information for one trial
					break;
				}
				if (subTrial > interpSub) 
					break;
				if ((subTrial == interpSub) && (numTrial > interpTrial))
					break;
			}
		}		
	}	
	
	interpInfo.close();
	trialInfoFile.close();
	outputfile.close();
// 	cout << "done";
	return newInterpolationFile;
}
