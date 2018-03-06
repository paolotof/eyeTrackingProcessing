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

//     if (!xpos) // when the read fails, as for example during blinks
//       Dataline::setValidity(false);
		if (linedata.fail())
			Dataline::setValidity(false);
    else
    {
      Dataline::setTime(time);
      Dataline::setX(xpos);
      Dataline::setY(ypos);
      Dataline::setSize(psize);
      Dataline::setValidity(true);
    }
    // 30/1/2018 - it can happen that xpos is 0.0, does it get skipped then? 
    // yes but adding it does not solve the problem, then we get stuck with 
    // blinks
//     if (xpos == 0.0 && ypos > 0.0){
// 			Dataline::setTime(time);
// 			Dataline::setX(xpos);
// 			Dataline::setY(ypos);
// 			Dataline::setSize(psize);
// 			Dataline::setValidity(true);
// 		}
// 		if (xpos > 0.0 && ypos == 0.0){
// 			Dataline::setTime(time);
// 			Dataline::setX(xpos);
// 			Dataline::setY(ypos);
// 			Dataline::setSize(psize);
// 			Dataline::setValidity(true);
// 		}
		
  }
  //return (eye);
}
