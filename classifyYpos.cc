#include "main.ih"
// 512 is the middle of the screen height
// 1024 is the height of the screen
char classifyYpos(double ypos, bool charlotte)
{
//   return((ypos < 512.0) ? 'U' : 'B');
     char pos = 'N';
  if (ypos > 40.0 && ypos < 502.0)//(ypos > 100.0 && ypos < 492.0)
    pos = 'U';
  if (ypos > 522.0 && ypos < 984.0) // (ypos > 532.0 && ypos < 924.0)
    pos = 'B';
  
  if (charlotte)
  {
    if (ypos > 57.0 && ypos < 445.0)//(ypos > 100.0 && ypos < 492.0)
      pos = 'U';
    if (ypos > 579.0 && ypos < 967.0) // (ypos > 532.0 && ypos < 924.0)
      pos = 'B';
  }
  
  return(pos);

}
