#include "trialinfo.ih"

void TrialInfo::setExportStarts(size_t ExportStarts)
{
// 	d_timeIsUp = TrialInfo::g_targetOnset() + TrialInfo::limit4extraction(); 
	// for the computation of the PERD it could be helpful to add 1000ms here
	d_exportStart = TrialInfo::g_targetOnset() - ExportStarts; 
}
