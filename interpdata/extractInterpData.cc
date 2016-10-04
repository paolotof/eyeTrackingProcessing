#include "interpdata.ih"

Interpdata Interpdata::extractInterpData(string& lineBegin, string& lineEnd)
{
  
  istringstream firstLine(lineBegin);
  string sub;
  size_t ntrial; //ibeg, iend; 
  double ibeg, iend, xBeg, xEnd, yBeg, yEnd, pBeg, pEnd;
  
  firstLine >> sub >> ntrial >> ibeg >> xBeg >> yBeg >> pBeg;

  Interpdata data;
  sub.erase(sub.end()-4,sub.end()); // remove '.asc' from file name
  data.setSub(sub); 
  data.setNtrial(ntrial);
  data.setBegin(ibeg);
  data.setBegX(xBeg);
  data.setBegY(yBeg);
  data.setBegP(pBeg);
  
  istringstream secondLine(lineEnd);
//   linedata.str(lineEnd);
  
  size_t lines2interp;
  secondLine >> sub >> ntrial >> iend >> xEnd >> yEnd >> pEnd >> lines2interp; 

  data.setEnd(iend);
  data.setEndX(xEnd);
  data.setEndY(yEnd);
  data.setEndP(pEnd);
  data.setLines2interp(lines2interp);
  // ps:check if numbers make sense!
	// cout << 
  return data;
}