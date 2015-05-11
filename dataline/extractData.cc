#include "dataline.ih"

// Dataline extractData(string line)
// Dataline extractData(istream& line)
// Dataline Dataline::extractData(string& line)
void Dataline::extractData(string& line)
{
  //Dataline eye;
  
  string firstLetter = line.substr(0,1);
  
  if (firstLetter.find_first_of("0123456789") == string::npos)
    Dataline::setMSG(true);
  else
  {
    Dataline::setMSG(false);
    
    double time, xpos, ypos, psize;
    istringstream linedata(line);
    linedata >> time >> xpos >> ypos >> psize; 

    if (!xpos) // when the read fails, as for example during blinks
      Dataline::setValidity(false);
    else
    {
      Dataline::setTime(time);
      Dataline::setX(xpos);
      Dataline::setY(ypos);
      Dataline::setSize(psize);
      Dataline::setValidity(true);
    }
  }
  
  //return (eye);
}