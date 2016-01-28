#include "main.ih"

// Areas are classified as:
// (T)arget, where the target is ()
// (C)ompetitor, where the competitor is
// (D)istractor, where the distractorS were
// (E)lse, blinks and saccades 

size_t classifyArea(double xpos, double ypos, const string& pos2match, size_t posID, bool charlotte)
{
  char posX = classifyXpos(xpos, charlotte);
  char posY = classifyYpos(ypos, charlotte);
  
  size_t valOut = 0;
  
  if ((pos2match.find(posX) != string::npos) && (pos2match.find(posY) != string::npos)) 
  {
    valOut = posID;
  }
  return(valOut);
}

