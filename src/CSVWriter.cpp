#include "CSVWriter.h"
#include <iostream>
CCSVWriter::CCSVWriter(std::ostream &ou) : OutFile(ou){
}

CCSVWriter::~CCSVWriter(){
}

bool CCSVWriter::WriteRow(const std::vector< std::string > &row){
	if(OutFile.tellp() != 0){
		OutFile << "\r\n";
	}
	for (unsigned int i = 0; i < row.size(); i++){
		std::string str = row[i];
		if(str.length() > 0){
			OutFile << "\"";
			for (auto ch:str){
				if(ch == '\"'){
					OutFile << "\"\"";
				}else{
					OutFile << ch;
				}
			}
			OutFile << "\"";
			//OutFile << '\"' + str + '\"';
		}
		if (i != row.size()-1){
			OutFile <<",";
		}
	}
	OutFile.flush();
	return true;
}
