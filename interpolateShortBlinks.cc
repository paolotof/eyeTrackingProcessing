#include "main.ih"

// TrialInfo interpolateShortBlinks(ofstream& outputfile, TrialInfo trialSet, Dataline preBlink, 
// 				 Dataline postBlink, size_t lines2interp, bool charlotte)
size_t interpolateShortBlinks(ofstream& outputfile, TrialInfo trialSet, Dataline preBlink, 
				 Dataline postBlink, size_t lines2interp, bool charlotte)
{
  
//   cout << "S";
  // interpolate
  double dataArray[4] = {0, 0, 0, 0};
  for (double iter = (lines2interp - 1); iter >= 0; iter--)
  {
    dataArray[0] = linearInterpolate(postBlink.g_time(), preBlink.g_time(), iter/lines2interp);
    dataArray[1] = linearInterpolate(postBlink.g_xpos(), preBlink.g_xpos(), iter/lines2interp);
    dataArray[2] = linearInterpolate(postBlink.g_ypos(), preBlink.g_ypos(), iter/lines2interp);
    dataArray[3] = linearInterpolate(postBlink.g_psize(), preBlink.g_psize(), iter/lines2interp);
    
    size_t timeBefore = 400;
    if (dataArray[0] >= (trialSet.g_targetOnset() - timeBefore))
    {
      trialSet.addOneBin();
      AssignAreaAndWriteOut(dataArray, outputfile, trialSet, charlotte);
    }
      //trialSet  = AssignAreaAndWriteOut(dataArray, outputfile, trialSet, charlotte);
    
  } // end for loop "for (double iter = (lines2interp-1); iter > 0; --iter)"
  
  //return(trialSet);
  return lines2interp;
}