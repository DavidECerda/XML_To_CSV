#ifndef CSVREADER_H
#define CSVREADER_H

#include <istream>
#include <string>
#include <vector>
#include <queue>
#include <csv.h>

class CCSVReader{
	protected:
		std::istream &InFile;
		std::vector< std::string > BuffedRow;
		static void CallbackField(void *str, size_t len, void *data); 
		static void CallbackRow(int ch, void *data);
		struct csv_parser My_Parser;
		std::queue< std::vector< std::string > > BufferOfRows;
	public:
		CCSVReader(std::istream &in);
		~CCSVReader();
		bool End() const;
		bool ReadRow(std::vector< std::string > &row);
};

#endif
