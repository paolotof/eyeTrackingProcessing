#include "main.ih"

// Dataline extractData(string line)
// Dataline extractData(istream& line)
Dataline extractData(string& line)
{
  Dataline eye;
  
  string firstLetter = line.substr(0,1);
  
  if (firstLetter.find_first_of("0123456789") == string::npos)
    eye.setMSG(true);
  else
  {
    eye.setMSG(false);
    istringstream linedata(line);
    double time, xpos, ypos, psize;
//     cout << line << '\n';
    linedata >> time >> xpos >> ypos >> psize; 
//     cout << time << ' ' << xpos << ' ' <<  ypos << ' ' <<  psize << '\n';
    if (!xpos) // when the read fails, as for example during blinks
    {
      eye.setValidity(false);
      //eye.setMSG(true);
    }
    else
    {
      eye.setTime(time);
      eye.setX(xpos);
      eye.setY(ypos);
      eye.setSize(psize);
      eye.setValidity(true);
    }
  }
  
  return (eye);
}