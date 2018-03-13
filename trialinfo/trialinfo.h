#ifndef INCLUDED_TRIALINFO_
#define INCLUDED_TRIALINFO_

#include <string>
#include <sstream> 
#include <fstream>

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
	string d_timeLocking;// 	TrialInfo::timeLocking;
  size_t d_targetStarts;
  size_t d_onsetTarget;
  size_t d_RT;
  string d_subCond;
  size_t d_iBin;
  size_t d_binCount = 0;
  size_t d_fixCount = 1; // always starts with 1 and then update
  bool d_updateInterp = false; // 26-4-16: assigned a default so that it is valid if running people without interpolation 
  size_t d_limit4extraction = 4000; // this values is never used, only kept for backward compatibility
  size_t d_timeIsUp;
	size_t d_exportStart;
	
  public:
    TrialInfo();
    
// SETTERS    
    void setBin(size_t iBin);
    void setCompetLoc(string CompetLoc);
    void setCondition(string Condition);
    void setCurrentTr(size_t Trial);
    void setDist1Loc(string Dist1Loc);
    void setDist2Loc(string Dist2Loc);
    void setFix(size_t numFixes);
    void setOnsetTarget(size_t onset);
    void setRT(size_t RT);
    void setSubCond(string SubCond);
    void setSubject(string Subject);
    void setTarget(string Target);
    void setTargetLoc(string TargetLoc);
    void setTargetStarts(size_t TargetStarts);
		void setTimeIsUp();
    void setTrialIN(size_t Trial);
    void addOneBin();
    void updateBinCount(size_t nBins);
    void computeTargetLimits(string& line);
    void extractInfo(string& line);
    void resetAndUpdate(ifstream& trialInfoFile);
    void resetBinsCounter();
    void updateCurrentTrial(string& line, size_t timeBefore);
    void updateInterp(bool value); 
		void setExportStarts(size_t ExportStarts);
		void setTimeLocking(string& line);
// GETTERS    
    // there is a g in front of the name because it's then easier to recognize the file names of set versus get functions.
    size_t g_Bin() const;
    string g_competLoc() const;
    string g_condition() const;
    size_t g_currentTr() const;
    string g_dist1Loc() const;
    string g_dist2Loc() const;
    size_t g_Fix() const;
    size_t g_RT() const;
    string g_subCond() const;
    string g_subject() const;
    string g_target() const;
    string g_targetLoc() const;
    size_t g_targetOnset() const;
    size_t g_targetStarts() const;
    size_t g_trialIN() const;
    bool   g_updateInterp() const;
    size_t limit4extraction() const; // this value is set with a default and never modified
    size_t timeIsUp() const;
    size_t totalBins() const;
		size_t startExport() const;
		string timeLocking() const;
		
  private:
};
 
#endif
