#include "main.ih"

size_t binTimeInterval(string filename, size_t timeInterval)
{
	cout << "BINNING\n";

	timeInterval = round(timeInterval/4); // convert to bins
	
// 	string inputDir = "/home/paolot/ownCloud/Shared/eye-tracking_rhythm/lianne/eyetracker/";
// 	string tmp = inputDir + "tbt_noFillers_1000_ALL_medianInterp.asc";
	string tmp = filename;
  tmp.append("_ALL_medianInterp.asc");
  cout << tmp << '\n';
  ifstream datafile(tmp);
  
//   tmp.insert(tmp.find(".asc"), "_binned");
//   cout << tmp << "\n";
//   binnedData.open(tmp); 
	
  if (! datafile.is_open())
  {
    cout << "datafile to bin does not exists\n";
    return (0);
  }

  
  ofstream binnedData;
  //cout << filename << '\n';
	filename = filename + "_binBarr.asc";
	cout << filename << '\n';
	binnedData.open(filename); 
	if (! binnedData.is_open())
  {
    cout << "binned file does not exists\n";
    return (0);
  }
  
  vector<double> tVect;
	vector<double> newBin;
	vector<double> bVect;
  vector<double> pVect;
  vector<double> targV;
  vector<double> compV;
  vector<double> dOneV;
  vector<double> dTwoV;
  vector<double> fixV;

//   size_t lines2bin = 5;
	
		
  string previousPp = "";
  size_t previousTr = 1;
  string dataline;
// skip the first line which has the file's header
	getline(datafile, dataline);
// 	cout << dataline;
  while (getline(datafile, dataline))
  {
    istringstream linedata(dataline); // extract time information from the eyetracking file
    string currentPp, condition, item, exp;
    size_t clockTime, bin, currentTr;
    int time; // time can also be negative, but it will always be an integer
    double psize, target, competitor, d1, d2, fix;
    linedata >> currentPp >> clockTime >> time >> bin >> condition >> currentTr >> 
      item >> exp >> psize >> target >> competitor >> d1 >> d2 >> fix;
    
    
    // reset everything  
    if ((previousPp != currentPp) || (previousTr != currentTr))
    {
      vector<double>().swap(tVect);
			vector<double>().swap(newBin);
      vector<double>().swap(bVect);
      vector<double>().swap(pVect);
      vector<double>().swap(targV);
      vector<double>().swap(compV);
      vector<double>().swap(dOneV);
      vector<double>().swap(dTwoV);
      vector<double>().swap(fixV);
// 			cout << '\n';
    }

    // this must be done afterwards otherwise all the values of the first timepoint
    // are flushed instead of stored. 
    tVect.push_back(time);
    bVect.push_back(bin);
		double binNew = round(time/4);
		newBin.push_back(binNew);
    pVect.push_back(psize);
    targV.push_back(target);
    compV.push_back(competitor);
    dOneV.push_back(d1);
    dTwoV.push_back(d2);
    fixV.push_back(fix);

    // write mean out
//     if (tVect.size() == lines2bin)
// we need to parse the lines to check the time interval
// 		size_t binnedFrames = 0;
// 		cout << time % timeInterval << '\n';
// 		if ( time % timeInterval == 0 )	
// 		cout << ((int)newBin[newBin.size()]);
// 		cout << newBin[newBin.size()] << ' ' << time/4;
		
		if ( fmod(binNew, timeInterval) == 0 )	
		{
// 			cout << dataline << '\n';
//       << clockTime 
// 			binnedFrames = tVect.size(); // add a line at the end to know on how many lines the average for the binning is based
      binnedData << currentPp << '\t' << vectorMean(tVect) << '\t' 
				<< vectorMean(bVect) << '\t'  << round(vectorMean(newBin)) << '\t' 
				<< condition << '\t' 
				<< currentTr  << '\t' << item << '\t' << exp  << '\t' 
				<< vectorMean(pVect) << '\t' << vectorMean(targV) << '\t' 
				<< vectorMean(compV) << '\t' << vectorMean(dOneV) << '\t' 
				<< vectorMean(dTwoV) << '\t' << vectorMean(fixV) << '\t'
				<< tVect.size() << '\n'; // binnedFrames
	
      vector<double>().swap(tVect);
      vector<double>().swap(bVect);
			vector<double>().swap(newBin);
      vector<double>().swap(pVect);
      vector<double>().swap(targV);
      vector<double>().swap(compV);
      vector<double>().swap(dOneV);
      vector<double>().swap(dTwoV);
      vector<double>().swap(fixV);
	
    }
    
    previousPp = currentPp;
    previousTr = currentTr;
          
  } // end "while(getline(trialInfoFile, trialInfo))"
  
  datafile.close();
  binnedData.close();
  
  return 0;
}