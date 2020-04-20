#include "CSVReader.h"
#include <iostream>

#define BUFFER_SIZE 8192

CCSVReader::CCSVReader(std::istream &in) : InFile(in){
	csv_init(&My_Parser,0);
}

CCSVReader::~CCSVReader(){
	csv_free(&My_Parser);
}

void CCSVReader::CallbackField(void *str, size_t len, void *data){
	CCSVReader *Ptr = static_cast<CCSVReader*>(data);
	Ptr->BuffedRow.push_back(std::string(static_cast<const char*>(str), len));
}
        
void CCSVReader::CallbackRow(int ch, void *data){
	CCSVReader *Ptr = static_cast<CCSVReader*>(data);
	Ptr->BufferOfRows.push(Ptr->BuffedRow);
	Ptr->BuffedRow.clear();
}

bool CCSVReader::End() const{
	if (InFile.eof() && BufferOfRows.empty()){
		return true;
	}
	else{
		return false;
	}
}

bool CCSVReader::ReadRow(std::vector<std::string> &row){
	if(this->End()){
		return false;
	}else if(BufferOfRows.size() > 0){
		row = BufferOfRows.front();
		BufferOfRows.pop();
	}else{
		char buffer[BUFFER_SIZE];
		while(BufferOfRows.empty()){
			InFile.read(buffer, BUFFER_SIZE);
			unsigned int len = InFile.gcount();
			if(csv_parse(&My_Parser,buffer,len,CCSVReader::CallbackField,CCSVReader::CallbackRow,this) != len){
				std::cout<<std::string(csv_strerror(csv_error(&My_Parser)))<<std::endl;
				return false;
			}
			if(InFile.eof()){
				csv_fini(&My_Parser, CCSVReader::CallbackField, CCSVReader::CallbackRow, this);
			}
		}
		row = BufferOfRows.front();
		BufferOfRows.pop();
	}
	return true;
}
