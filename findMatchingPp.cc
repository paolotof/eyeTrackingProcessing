#include "main.ih"

double findMatchingPp(string& pp, ifstream& baselineFile)
{
	string line;
	string b_pp;
	size_t b_trial;
	double b_psize, b_x, b_y;
	while(getline(baselineFile, line))
	{
		istringstream lineBase(line);
		lineBase >> b_pp >> b_trial >> b_x >> b_y >> b_psize;
		b_pp = b_pp.erase(b_pp.find(".asc"), 4);

		if (b_pp.compare(pp) == 0)
			break;
	}
	return(b_psize);
}
