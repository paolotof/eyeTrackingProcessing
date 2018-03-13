#include "main.ih"

/* in this function timeBefore is only used to define the filename but it is never used 
 to define which time interval is actually used. */ 

void ERDD_withBase123(string& outputfileName, size_t timeBefore, 
											size_t baselineLength, string& filePrefix)
{
	string fillersOrNot = "withFillers";
	if (outputfileName.find("noFillers") != string::npos)
		fillersOrNot = "noFillers";

	string file;
// 	file += string("RT_") + fillersOrNot + "_" + std::to_string(timeBefore) + 
// 		"_ALL_medianInterp.asc";
	file += filePrefix + fillersOrNot + "_" + std::to_string(timeBefore) + 
	"_ALL_medianInterp.asc";
// CHECK THAT FILES EXIST!	
	ifstream processedFile(file);
	if (not processedFile.is_open())
	{
		cout << "ERDD_withBase123:: " << file << " does not exist\n"; 
		return;
	} 
//	
	cout << "Compute baselines one, two and three\n";
	string line;
	getline(processedFile, line);// read and SKIP the files' headers
	
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
// 	size_t clockTime, bin,	trial;
	size_t clockTime,	trial;
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
	while (getline(processedFile, line)) {
		istringstream linedata(line);
// 		linedata >> pp >> clockTime >> time >> bin >> condition >> trial >> 
// 			item >> exp >> psize >> target >> competitor >> d1 >> d2 >> fix; 
		linedata >> pp >> clockTime >> time >> condition >> trial >> item 
			>> exp >> psize >> target >> competitor >> d1 >> d2 >> fix; 
		
		// update baselines when eyetracking file goes from one trial to the next
		if (oldTrial != trial) {
			if (filePrefix.find("word") != string::npos)
				b1_psize = matchBaselines(pp, trial, (filePrefix + "averageBeforeTarget"
				+ std::to_string(baselineLength) + ".asc"));
			b3_psize = matchBaselines(pp, trial, (filePrefix + "average" 
			+ std::to_string(baselineLength) + "_BeforeVisualOnset.asc"));
		}														
		oldTrial = trial; // synchronizing to  eyetracking file
		if (oldPp != pp)
			b2_psize = matchBaselines(pp, 1, (filePrefix + "averageFirst" 
			+ std::to_string(baselineLength) + ".asc"));
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
	return;
}
