#include "main.ih"
// 640 is the middle of the screen width
//1280 is the screen width
char classifyXpos(double xpos, bool charlotte)
{
//   return((xpos < 640.0) ? 'L' : 'R');
  
  char pos = 'N';
  // if (xpos < 620.0 && xpos > 40.0) paolot: modified dimension to focus more on target
  if (xpos < 620.0 && xpos > 40.0) //  if (xpos < 600.0 && xpos > 100.0)
    pos = 'L';
  if (xpos > 660.0 && xpos < 1240.0)
    pos = 'R';

  if (charlotte) {
    if (xpos < 602.0 && xpos > 72.0) //  if (xpos < 600.0 && xpos > 100.0)
      pos = 'L';
    if (xpos > 678.0 && xpos < 1208.0) // (xpos > 680.0 && xpos < 1180.0)
      pos = 'R';
  }

  
  return(pos);
}
