#include "main.ih"

/*
 find whether the specific trial which we analysing should be interpolated or not
 
 */

Interpdata interpolationInterval(string& interpolationFile, 
																 TrialInfo trialSet, Dataline eye)
{
	Interpdata interpolation;
	ifstream interpInfo(interpolationFile);
	if (! interpInfo.is_open()) {
		cout << "NO interpolation file "<< interpolationFile << '\n'; 
		cout << "Continue without interpolation information\n\n"; 
  } else {
		interpolation.setDoInterpolation(true);
		string line1, line2;
		while (getline(interpInfo, line1))
		{
			getline(interpInfo, line2);
			if (interpInfo.good())
				interpolation = interpolation.extractInterpData(line1, line2);
			if ((interpolation.sub() ==  trialSet.g_subject()) && 
				(trialSet.g_currentTr() == interpolation.nTrial()) && 
				(eye.g_time() <= interpolation.iEnd()) && 
				(eye.g_time() >= interpolation.iBegin())){
					cout << interpolation.iBegin() << ' ' << 
						eye.g_time() << ' ' << interpolation.iEnd()<< '\n'; 
					break;
			}
		}
	}
	interpInfo.close();
	return (interpolation);
}
