#include "trialinfo.ih"

void TrialInfo::setTimeIsUp()
{
// 	d_timeIsUp = TrialInfo::g_targetOnset() + TrialInfo::limit4extraction(); 
	// for the computation of the PERD it could be helpful to add 1000ms here
	d_timeIsUp = TrialInfo::g_targetOnset() + TrialInfo::g_RT(); 
}
