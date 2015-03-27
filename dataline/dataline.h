/* this is the class HEADER */
#ifndef INCLUDED_DATALINE_
#define INCLUDED_DATALINE_

#include <string>
using namespace std;

class Dataline
{
  // data members
  double d_time;  // double instead of size_t because it might be that it must be interpolated 
  double d_xpos;  // position on the horizontal axis
  double d_ypos;  // position on the vertical axis
  double d_psize; // pupil size
  bool d_isMSG;
  bool d_isValid;
  
  public:
    Dataline();

    void setMSG(bool isMSG);
    void setTime(double time);
    void setX(double xpos);
    void setY(double ypos);
    void setSize(double psize);
//    void setValidity(bool valid);
    void setValidity(bool valid);
    
    // there is a g in front of the name because it's then easier to recognize the file names of set versus get functions.
    bool isMSG() const;
    double g_time() const;
    double g_xpos() const;
    double g_ypos() const;
    double g_psize() const;
    bool isValid() const;
    
//     void detValidity(double (*g_psize)());
    
  private:
};
        
#endif
