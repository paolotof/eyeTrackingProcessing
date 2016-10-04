#include "main.ih"

size_t binData(string filename)
{
  string tmp = filename;
  tmp.append("_ALL_medianInterp.asc");
  ifstream datafile(tmp);
  
	cout << "BINNING\n";
	
//   cout << tmp << ' ' << filename << '\n';
  
  if (! datafile.is_open())
  {
    cout << "datafile to bin does not exists\n";
    return (0);
  }

  
  ofstream binnedData;
  //cout << filename << '\n';
  tmp.insert(tmp.find(".asc"), "_binned");

//   cout << tmp << "\n";
  binnedData.open(tmp); 
  if (! binnedData.is_open())
  {
    cout << "binned file does not exists\n";
    return (0);
  }
  
//   cout << tmp << '\n';
  
  vector<double> tVect;
  vector<double> bVect;
  vector<double> pVect;
  vector<double> targV;
  vector<double> compV;
  vector<double> dOneV;
  vector<double> dTwoV;
  vector<double> fixV;

  size_t lines2bin = 5;
  string previousPp = "";
  size_t previousTr = 1;
  string dataline;
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
      vector<double>().swap(bVect);
      vector<double>().swap(pVect);
      vector<double>().swap(targV);
      vector<double>().swap(compV);
      vector<double>().swap(dOneV);
      vector<double>().swap(dTwoV);
      vector<double>().swap(fixV);
    }

    // this must be done afterwards otherwise all the values of the first timepoint
    // are flushed instead of stored. 
    tVect.push_back(time);
    bVect.push_back(bin);
    pVect.push_back(psize);
    targV.push_back(target);
    compV.push_back(competitor);
    dOneV.push_back(d1);
    dTwoV.push_back(d2);
    fixV.push_back(fix);

    // write mean out
    if (tVect.size() == lines2bin)
    {
//       << clockTime 
      binnedData << currentPp << '\t' << vectorMean(tVect) << '\t' 
				<< vectorMean(bVect) << '\t' << condition << '\t' 
				<< currentTr  << '\t' << item << '\t' << exp  << '\t' 
				<< vectorMean(pVect) << '\t' << vectorMean(targV) << '\t' 
				<< vectorMean(compV) << '\t' << vectorMean(dOneV) << '\t' 
				<< vectorMean(dTwoV) << '\t' << vectorMean(fixV) << '\n';
	
      vector<double>().swap(tVect);
      vector<double>().swap(bVect);
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