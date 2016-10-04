#ifndef INCLUDED_INTERPDATA_
#define INCLUDED_INTERPDATA_

#include <string>

class Interpdata
{
	// data members
	std::string d_subname = "none"; // default for when no interpolation file is available
	size_t d_nTrial = 0;	// maybe this should also have a default value (check function interpolateblinks)
//     size_t d_beginInterp;
//     size_t d_endInterp;	
	double d_beginInterp;
	double d_endInterp;	
	double d_begX;
	double d_endX;
	double d_begY;
	double d_endY;
	double d_begP;
	double d_endP;
	size_t d_lines2interp;
	bool   d_doInterpolation = false;
	
	public:
		Interpdata();
	
		void setSub(std::string sub);
		void setNtrial(size_t ntrial);
	// 	void setBegin(size_t beginInterp);
	// 	void setEnd(size_t endInterp);
		void setBegin(double beginInterp);
		void setEnd(double endInterp);
		void setBegX(double begx);
		void setEndX(double endx);
		void setBegY(double begy);
		void setEndY(double endy);
		void setBegP(double begp);
		void setEndP(double endp);
		void setLines2interp(size_t nlines);
		void setDoInterpolation(bool interpolateIt);
		
		std::string sub() const;
		size_t nTrial() const;
	// 	size_t iBegin() const;
	// 	size_t iEnd() const;
		double iBegin() const;
		double iEnd() const;
		double begX() const;
		double endX() const;
		double begY() const;
		double endY() const;
		double begP() const;
		double endP() const;
		size_t lines2interp() const;
		bool interpolate() const;

		Interpdata extractInterpData(std::string& lineBegin, std::string& lineEnd);

	private:
        // Interpdata extractInterpData(std::string& line); // gives the error that this is private... not sure what it means
};
        
#endif
