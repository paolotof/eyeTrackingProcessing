#include "trialinfo.ih"

void TrialInfo::setTimeIsUp()
{
  d_timeIsUp = TrialInfo::g_targetOnset() + TrialInfo::limit4extraction(); 
}
