#ifndef INCLUDED_TRIALINFO_
#define INCLUDED_TRIALINFO_

#include <string>
using namespace std;

class TrialInfo
{
  string d_subject;
  size_t d_trialIN;
  size_t d_currentTr = 0;  
  string d_targetLoc;
  string d_competLoc;
  string d_dist1Loc;
  string d_dist2Loc;
  string d_target;
  string d_condition;
  size_t d_targetStarts;
  size_t d_onsetTarget;
  size_t d_RT;
  string d_subCond;
  size_t d_iBin;
  size_t d_binCount = 0;
  size_t d_fixCount = 1; // always starts with 1 and then update
  bool d_updateInterp; 
  
  public:
    TrialInfo();
    
    void setSubject(string Subject);
    void setTrialIN(size_t Trial);
    void setCurrentTr(size_t Trial);
    void setOnsetTarget(size_t onset);
    void setTargetLoc(string TargetLoc);
    void setCompetLoc(string CompetLoc);
    void setDist1Loc(string Dist1Loc);
    void setDist2Loc(string Dist2Loc);
    void setTarget(string Target);
    void setCondition(string Condition);
    void setTargetStarts(size_t TargetStarts);
    void setRT(size_t RT);
    void setSubCond(string SubCond);
    void setBin(size_t iBin);
    void addOneBin();
    void resetBinsCounter();
    void setFix(size_t numFixes);
    void setUpdateInterp(bool value); 
    
    // there is a g in front of the name because it's then easier to recognize the file names of set versus get functions.
    string g_subject() const;
    size_t g_trialIN() const;
    string g_targetLoc() const;
    string g_competLoc() const;
    string g_dist1Loc() const;
    string g_dist2Loc() const;
    string g_target() const;
    string g_condition() const;
    size_t g_targetStarts() const;
    size_t g_RT() const;
    string g_subCond() const;
    size_t g_currentTr() const;
    size_t g_targetOnset() const;
    size_t g_Bin() const;
    size_t totalBins() const;
    size_t g_Fix() const;
    bool g_updateInterp() const;
    
  private:
};
 
#endif
