#include "main.ih"

TrialInfo trialSetup(string& line, TrialInfo i)
{
  
  istringstream linedata(line);
  
  string Subject, TargetLoc, CompetLoc, Dist1Loc,
    Dist2Loc, Target, Condition, SubCond;

  size_t Trial, TargetStarts, RTtime;
  
  linedata >> Subject >> Trial >> TargetLoc >> CompetLoc >> Dist1Loc >>
    Dist2Loc >> Target >> Condition >> TargetStarts >> RTtime >> SubCond; 
      
  i.setSubject(Subject);
  i.setTrialIN(Trial);
  i.setTargetLoc(TargetLoc);
  i.setCompetLoc(CompetLoc);
  i.setDist1Loc(Dist1Loc);
  i.setDist2Loc(Dist2Loc);
  i.setTarget(Target);
  i.setCondition(Condition);
  i.setTargetStarts(TargetStarts);
  i.setRT(RTtime);
  i.setSubCond(SubCond);
  i.setBin(0); // reset counter
  i.setCurrentTr(i.g_currentTr());
  
  return (i);
}