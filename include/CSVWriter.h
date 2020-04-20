#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <ostream>
//#include<fstream>
#include <string>
#include <vector>
#include <csv.h>

class CCSVWriter{
	protected:
		std::ostream &OutFile;

	public:
		CCSVWriter(std::ostream &ou);
		~CCSVWriter();
		bool WriteRow(const std::vector< std::string > &row);
};

#endif
