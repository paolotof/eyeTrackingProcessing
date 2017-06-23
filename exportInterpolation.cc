#include "main.ih"

void exportInterpolation(ofstream& shortBlinksReport, TrialInfo trialSet, 
												 median_t medB4, median_t medAfter)
{

	shortBlinksReport << trialSet.g_subject() << ' ' 
		<< trialSet.g_currentTr() << ' '; 
	size_t old_precision = shortBlinksReport.precision();
	shortBlinksReport.precision(10);
	shortBlinksReport << medB4.time ;
	shortBlinksReport.precision(old_precision);
	shortBlinksReport << ' ' << medB4.x << ' ' << medB4.y << ' ' 
		<< medB4.psize << '\n';
	// end interpolation
	shortBlinksReport << trialSet.g_subject() << ' ' 
		<< trialSet.g_currentTr() << ' ';
	shortBlinksReport.precision(10);
	shortBlinksReport << medAfter.time; 
	shortBlinksReport.precision(old_precision);
	shortBlinksReport << ' ' << medAfter.x << ' ' << medAfter.y  << ' ' 
		<< medAfter.psize << ' ' 
		<< round((medAfter.time - medB4.time) / 4) << '\n'; // lines to interpolate
}
