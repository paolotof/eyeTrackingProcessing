#include "main.ih"

void ERDD_withBase1And2(string& outputfileName, size_t timeBefore)
{
	string fillersOrNot = "withFillers";
	if (outputfileName.find("noFillers") != string::npos)
		fillersOrNot = "noFillers";

	string file;
	file += string("RT_") + fillersOrNot + "_" + std::to_string(timeBefore) + 
		"_ALL_medianInterp.asc";
// 	cout << file << '\n';
	
	ifstream processedFile(file);
	if (not processedFile.is_open())
	{
		cout << file << " does not exist\n"; 
		return;
	} 
	
	ifstream baseline1File("RT_average200_BeforeVisualOnset.asc");
	if (not baseline1File.is_open())
	{
		cout << "RT_average200_BeforeVisualOnset.asc does not exist\n"; 
		return;
	}	

	ifstream baseline2File("RT_averageFirst200.asc");
	if (not baseline2File.is_open())
	{
		cout << "RT_averageFirst200.asc does not exist\n"; 
		return;
	}
	
	cout << "Compute baselines one and two\n";
	string line;
// read and SKIP the files' headers
	getline(processedFile, line);
	getline(baseline1File, line);
	getline(baseline2File, line);
// read the first lines to synchronize the files
	string pp, condition, item, exp;
	size_t clockTime, bin,	trial;
	int time;
	float psize,	fix, target, competitor, d1, d2;
	getline(processedFile, line);
	istringstream linedata(line);
	linedata >> pp >> clockTime >> time >> bin >> condition >> trial >> 
		item >> exp >> psize >> target >> competitor >> d1 >> d2 >> fix; 
	size_t oldTrial = trial; // this is to update baseline1
	string oldPp = pp; // this is to update baseline2
// baselines	
	double b1_psize;	
	b1_psize = findMatchingBaseline(pp, trial, baseline1File);
	double b2_psize;	
	b2_psize = findMatchingPp(pp, baseline2File);
	
// FIRST LINE			
	ofstream outputfile;
	cout << outputfileName << "\n";
	outputfile.open(outputfileName);
	outputfile << "pp" << '\t' << "time" << '\t' <<  "condition" 
						<< '\t' << "trial" << '\t' << "item" 
						<< '\t' << "baseline1" << '\t' << "baseline2" 
						<< '\t' << "fix\n";
	
	outputfile << pp  << '\t' << time  << '\t' << condition  
						<< '\t' << trial << '\t' << item  << '\t' 
						<< (((psize - b1_psize) / b1_psize) * 100) << '\t'  
						<< (((psize - b2_psize) / b2_psize) * 100) << '\t' 
						<< fix	<< '\n';
// FOLLOWING LINES			
	while (getline(processedFile, line))
	{
		istringstream linedata(line);
		linedata >> pp >> clockTime >> time >> bin >> condition >> trial >> 
			item >> exp >> psize >> target >> competitor >> d1 >> d2 >> fix; 
		
		outputfile << pp  << '\t' << time  << '\t' << condition  
			<< '\t' << trial << '\t' << item  << '\t' 
			<< (((psize - b1_psize) / b1_psize) * 100) << '\t' 
			<< (((psize - b2_psize) / b2_psize) * 100) << '\t' 
			<< fix	<< '\n';
			
		// update baselines 
		if (oldTrial != trial)
			b1_psize = findMatchingBaseline(pp, trial, baseline1File);
		oldTrial = trial; // updating eyetracking file
		
		if (oldPp != pp)
			b2_psize = findMatchingPp(pp, baseline2File);
		oldPp = pp; // updating eyetracking file
		
	}
	
	outputfile.close();
	processedFile.close();
	baseline1File.close();
	baseline2File.close();
	return;
}
