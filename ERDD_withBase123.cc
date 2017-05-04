#include "main.ih"

void ERDD_withBase123(string& outputfileName, size_t timeBefore)
{
	string fillersOrNot = "withFillers";
	if (outputfileName.find("noFillers") != string::npos)
		fillersOrNot = "noFillers";

	string file;
	file += string("RT_") + fillersOrNot + "_" + std::to_string(timeBefore) + 
		"_ALL_medianInterp.asc";
// CHECK THAT FILES EXIST!	
	ifstream processedFile(file);
	if (not processedFile.is_open())
	{
		cout << file << " does not exist\n"; 
		return;
	} 
	ifstream baseline1File("RT_averageBeforeTarget200.asc");
	if (not baseline1File.is_open())
	{
		cout << "RT_averageBeforeTarget200.asc does not exist\n"; 
		return;
	}
	ifstream baseline2File("RT_averageFirst200.asc");
	if (not baseline2File.is_open())
	{
		cout << "RT_averageFirst200.asc does not exist\n"; 
		return;
	}
	ifstream baseline3File("RT_average200_BeforeVisualOnset.asc");
	if (not baseline3File.is_open())
	{
		cout << "RT_average200_BeforeVisualOnset.asc does not exist\n"; 
		return;
	}	
//	
	cout << "Compute baselines one, two and three\n";
	string line;
// read and SKIP the files' headers
	getline(processedFile, line);
	getline(baseline1File, line);
	getline(baseline2File, line);
	getline(baseline3File, line);
	
	ofstream outputfile;
	outputfile.open(outputfileName);
// FILE'S HEADER
	outputfile << "pp" << '\t' << "time" << '\t' <<  "condition" 
						<< '\t' << "trial" << '\t' << "item" 
						<< '\t' << "baseline1" << '\t' << "baseline2" 
						<< '\t' << "baseline3"
						<< '\t' << "fix\n";
// PROCESS EYETRACKING FILE
	// initialize variables
	string pp, condition, item, exp;
	size_t clockTime, bin,	trial;
	double time; // time might be double in the interpolated lines
	float psize, fix, target, competitor, d1, d2;
	// all the values below are initiliazed to remove the warning: 
	// 	`b1_psize' may be used uninitialized in this function
		size_t oldTrial = 1;
		string oldPp = "";
		double b1_psize = 0;	
		double b2_psize = 0;	
		double b3_psize = 0;
	
// loop
	while (getline(processedFile, line))
	{
		istringstream linedata(line);
		linedata >> pp >> clockTime >> time >> bin >> condition >> trial >> 
			item >> exp >> psize >> target >> competitor >> d1 >> d2 >> fix; 
		
		// update baselines when eyetracking file goes from one trial to the next
		if (oldTrial != trial)
		{
			b1_psize = findMatchingBaseline(pp, trial, baseline1File);
			b3_psize = findMatchingBaseline(pp, trial, baseline3File);
		}
		oldTrial = trial; // synchronizing to  eyetracking file
		
		if (oldPp != pp)
			b2_psize = findMatchingPp(pp, baseline2File);
		oldPp = pp; // updating eyetracking file
		
		outputfile << pp  << '\t' << time  << '\t' << condition  
			<< '\t' << trial << '\t' << item  << '\t' 
			<< (((psize - b1_psize) / b1_psize) * 100) << '\t' 
			<< (((psize - b2_psize) / b2_psize) * 100) << '\t' 
			<< (((psize - b3_psize) / b3_psize) * 100) << '\t' 
			<< fix	<< '\n';
	}
	
	outputfile.close();
	processedFile.close();
	baseline3File.close();
	baseline2File.close();
	baseline1File.close();
	return;
}
