#include "main.ih"

// TrialInfo interpolateLongBlinks(ofstream& outputfile, TrialInfo trialSet, Interpdata interpolation, bool charlotte)
// size_t interpolateLongBlinks(ofstream& outputfile, TrialInfo trialSet, Interpdata interpolation, bool charlotte)
bool interpolateLongBlinks(ofstream& outputfile, TrialInfo trialSet, Interpdata interpolation, bool charlotte)
{
  
  cout << "L";
  bool updateInterp = false;
  // interpolate
  size_t lines2interp = (interpolation.iEnd() - interpolation.iBegin()) / 4;
  double dataArray[4] = {0, 0, 0, 0};
  for (double iter = lines2interp; iter > 0; --iter)
  {
    dataArray[0] = linearInterpolate(interpolation.iEnd(), interpolation.iBegin(), 
				     iter/lines2interp);
    dataArray[1] = linearInterpolate(interpolation.endX(), interpolation.begX(), 
				     iter/lines2interp);
    dataArray[2] = linearInterpolate(interpolation.endY(), interpolation.begY(), 
				     iter/lines2interp);
    dataArray[3] = linearInterpolate(interpolation.endP(), interpolation.begP(), 
				     iter/lines2interp);
    
    if ((dataArray[0] >= (trialSet.g_targetOnset())) && (dataArray[0] <= trialSet.timeIsUp()))
    { // time of interest boundaries
	trialSet.addOneBin();
	AssignAreaAndWriteOut(dataArray, outputfile, trialSet, charlotte);
	updateInterp = true;
	// because there are more types of blinks which get interpolated (e.g. short blinks)
    }
  } // end for loop "for (double iter = (lines2interp-1); iter > 0; --iter)"
  
//  return(trialSet);
//  return 0;
  return(updateInterp);
  
}