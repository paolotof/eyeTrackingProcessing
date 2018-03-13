#include "main.ih"

double matchBaselines(string& pp, size_t trial, string baseline2read)
{
	ifstream baselineFile(baseline2read);
	if (not baselineFile.is_open())
	{
		cout << "matchBaselines:: "<< baseline2read << " does not exist\n"; 
		return 0;
	}
	string line;
	getline(baselineFile, line); // SKIP the files' headers
	
	string b_pp;
	size_t b_trial;
	double b_psize;
	
	while (getline(baselineFile, line))
	{
		istringstream lineBase(line);
		lineBase >> b_pp >> b_trial >> b_psize;
// base 3 has only three values, so we assign b_x to b_psize if lineBase fails
// 		if (!lineBase) // lineBase is false when it cannot be read anymore
// 		if (lineBase)
		if (!lineBase.eof())
		{
			lineBase >> b_psize >> b_psize;// does it overwrite automatically?
			b_pp = b_pp.erase(b_pp.find(".asc"), 4);
		}
			
		if ((b_trial == trial) and (b_pp.compare(pp) == 0))
			break;
	}
	baselineFile.close();
	return(b_psize);
}
