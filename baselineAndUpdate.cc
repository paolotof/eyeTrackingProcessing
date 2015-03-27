#include "main.ih"
/*
 * 
 * 
 */ 

string baselineAndUpdate(vector<double> xpos, vector<double> ypos, vector<double> psize, ifstream& subInfoFile, string subID, ofstream& outputfile)
{
  // make average baseline and write result to file
  outputfile << subID << '\t' 
    << vectorMean(xpos) << '\t' 
    << vectorMean(ypos) << '\t' 
    << vectorMean(psize) << '\n';

  // update subject counter
  getline(subInfoFile, subID);
  return(subID);
}