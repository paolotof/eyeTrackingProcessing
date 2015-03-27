#include "main.ih"

TrialInfo interpolateLongBlinks(ofstream& outputfile, TrialInfo trialSet, Interpdata interpolation, bool charlotte)
{
  
//   cout << trialSet.g_subject() << ' ' << trialSet.g_currentTr() << '\n';
//   cout << interpolation.sub() << '\t';
//   cout << interpolation.nTrial() << '\t';
//   cout << interpolation.iBegin() << '\t';
//   cout << interpolation.begX() << '\t';
//   cout << interpolation.begY() << '\t';
//   cout << interpolation.begP() << '\t';
//   cout << interpolation.endP() << '\t';
//   cout << interpolation.endX() << '\n';
// 
//   cout << 
//   trialSet.g_subject()	<< ' ' <<
//   trialSet.g_currentTr() << '\n';
//   
  
//   for (double iter = interpolation.lines2interp(); iter > 0; --iter)
//   {
//     dataArray[0] = linearInterpolate(interpolation.iEnd(), interpolation.iBegin(), 
// 				     iter/interpolation.lines2interp());
//     dataArray[1] = linearInterpolate(interpolation.endX(), interpolation.begX(), 
// 				     iter/interpolation.lines2interp());
//     dataArray[2] = linearInterpolate(interpolation.endY(), interpolation.begY(), 
// 				     iter/interpolation.lines2interp());
//     dataArray[3] = linearInterpolate(interpolation.endP(), interpolation.begP(), 
// 				     iter/interpolation.lines2interp());

  
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
    
//     size_t old_precision = cout.precision();
//     cout.precision(10);
//     cout << dataArray[0] << ' ' << trialSet.g_targetOnset() << '\n';
//     cout.precision(old_precision);
    
//     if (dataArray[0] >= (trialSet.g_targetOnset()) && 
//       trialSet.g_subject() == interpolation.sub() && 
//       trialSet.g_currentTr() == interpolation.nTrial())
//     {
      
    if ((dataArray[0] >= interpolation.iBegin()) || (dataArray[0] <= interpolation.iEnd()))
    {
//       cout << " interpolation\n";
      trialSet = AssignAreaAndWriteOut(dataArray, outputfile, trialSet, charlotte);
      trialSet.setUpdateInterp(true); // interpolation class sets true for updates here 
      // because there are more types of blinks which get interpolated (e.g. short blinks)
    }
  } // end for loop "for (double iter = (lines2interp-1); iter > 0; --iter)"
  
  
//   if (trialSet.g_updateInterp())
//   {
//     cout << interpolation.sub() << ' ' << interpolation.nTrial() << " interpolated \n";
//   }
//   else
//   {
//     cout <<  " nothing to interpolate \n";
//   }
//   cout << interpolation.sub() << '\t';
//   cout << interpolation.nTrial() << '\t';
//   cout << interpolation.iBegin() << '\t';
//   cout << interpolation.begX() << '\t';
//   cout << interpolation.begY() << '\t';
//   cout << interpolation.begP() << '\t';
//   cout << interpolation.endP() << '\t';
//   cout << interpolation.endX() << '\n';	
//   
//   cout << 
//   trialSet.g_subject()	<< ' ' <<
//   trialSet.g_currentTr() << '\n';
  
  return(trialSet);
  
}