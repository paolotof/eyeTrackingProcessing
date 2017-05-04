#include "trialinfo.ih"

void TrialInfo::extractInfo(string& line)
{
  
  istringstream linedata(line);
  
  string Subject, TargetLoc, CompetLoc, Dist1Loc,
    Dist2Loc, Target, Condition, SubCond;

  size_t Trial, TargetStarts, RTtime;
  
  linedata >> Subject >> Trial >> TargetLoc >> CompetLoc >>
		Dist1Loc >> Dist2Loc >> Target >> Condition >> 
		TargetStarts >> RTtime >> SubCond; 
      
//   cout << Trial << ' ' << TrialInfo::g_trialIN() << " extract info ";
  TrialInfo::setSubject(Subject);
  TrialInfo::setTrialIN(Trial);
  TrialInfo::setTargetLoc(TargetLoc);
  TrialInfo::setCompetLoc(CompetLoc);
  TrialInfo::setDist1Loc(Dist1Loc);
  TrialInfo::setDist2Loc(Dist2Loc);
  TrialInfo::setTarget(Target);
  TrialInfo::setCondition(Condition);
  TrialInfo::setTargetStarts(TargetStarts);
  TrialInfo::setRT(RTtime);
  TrialInfo::setSubCond(SubCond);
  TrialInfo::setBin(0); // reset counter
  TrialInfo::setCurrentTr(TrialInfo::g_currentTr());
  
}
