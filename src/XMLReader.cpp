#include "XMLReader.h"
#include <expat.h>
#include <iostream>


CXMLReader::CXMLReader(std::istream &is) : InFile(is){
	MyParser = XML_ParserCreate(NULL);
	XML_SetUserData(MyParser, this);
	XML_SetElementHandler(MyParser,CXMLReader::start_element,CXMLReader::end_element);
	XML_SetCharacterDataHandler(MyParser,CXMLReader::handle_data);
}

//------------------------------------------------//
/// @brief Handles a start element 
/// @param data Pointer assigned to a CXMLReader
/// @param elem The data of an element
/// @param att The attributes of an element
//------------------------------------------------//
void CXMLReader::start_element(void *data, const char* elem, const char** att){
	CXMLReader *Ptr = static_cast<CXMLReader*>(data);
	struct SXMLEntity MyEntity;
	MyEntity.DType = SXMLEntity::EType::StartElement;
	MyEntity.DNameData = elem;
	for (int i = 0; att[i]; i += 2){  //https://stackoverflow.com/questions/609376/geting-xml-data-using-xml-parser-expat
		MyEntity.SetAttribute(std::string(att[i]), std::string(att[i+1]));
	}
	Ptr->ManyEntity.push(MyEntity);
}

//------------------------------------------------//
/// @brief Handles the an end element 
/// @param data Pointer assigned to a CXMLReader
/// @param elem The data of an element
//------------------------------------------------//
void CXMLReader::end_element(void *data, const char* elem){
	CXMLReader *Ptr = static_cast<CXMLReader*>(data);
	struct SXMLEntity MyEntity;

	MyEntity.DType = SXMLEntity::EType::EndElement;
	MyEntity.DNameData = elem;
	Ptr->ManyEntity.push(MyEntity);
}

//------------------------------------------------//
/// @brief Handles the data of an element 
/// @param data Pointer assigned to a CXMLReader
/// @param stuff The data of an element
/// @param length Size of the data
//------------------------------------------------//
void CXMLReader::handle_data(void *data, const char* stuff, int len){
	CXMLReader *Ptr = static_cast<CXMLReader*>(data);
	struct SXMLEntity MyEntity;

	MyEntity.DType = SXMLEntity::EType::CharData;
	MyEntity.DNameData = std::string(stuff,len);
	Ptr->ManyEntity.push(MyEntity);
}

CXMLReader::~CXMLReader(){
	XML_ParserFree(MyParser);
}

bool CXMLReader::End(){
	if (InFile.eof() && ManyEntity.empty()){
		return true;
	}
	else{
		return false;
	}
}

//------------------------------------------------//
/// @brief Reads an entity of the XML file and loads buffer if needed
/// @param entity Entity to store an element in
/// @param skipcdata Flag to skip over data
//------------------------------------------------//
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
	
	bool gotEntity;
	if(!End()){
		if (!ManyEntity.empty()){
			gotEntity = ReadSingleEntity(entity, skipcdata);
		}

		if(!gotEntity && ManyEntity.empty()){
			char buffer[BUFFER_SIZE];

			while(ManyEntity.empty()){

				int isfinal = ParseFile(buffer);

				if(!XML_Parse(MyParser,buffer,(InFile.gcount()),isfinal) == XML_STATUS_ERROR){
					gotEntity = ReadSingleEntity(entity, skipcdata);
				}
				else{
					ThrowError();
					break;
				}
			}
		}
	}
	return gotEntity;
}


int CXMLReader::ParseFile(char buffer[BUFFER_SIZE]){
	InFile.read(buffer, BUFFER_SIZE);

	int isfinal; //Equivalent to csv_fini
	if (InFile.eof()){
		isfinal = 1;
	}
	else{
		isfinal = 0;
	}
	return isfinal;
}

//------------------------------------------------//
/// @brief Reads a single element of XMLFile
/// @param entity Entity to store an element in
/// @param skipcdata Flag to skip over data
//------------------------------------------------//
bool CXMLReader::ReadSingleEntity(SXMLEntity &entity, bool skipcdata){
	if (skipcdata){
		while (!ManyEntity.empty() && ManyEntity.front().DType == SXMLEntity::EType::CharData){
			ManyEntity.pop();
		}
		if(!ManyEntity.empty()){
			entity = ManyEntity.front();
			ManyEntity.pop();
		}
		else return false;
	}
	else{
		if (!ManyEntity.empty()){
			entity = ManyEntity.front();
			ManyEntity.pop();
		}
		else return false;
	}
	return true;
} 

void CXMLReader::ThrowError(){
	XML_Error MyError = XML_GetErrorCode(MyParser);
	std::cerr<<"Issue With Parsing || Error Type: "<<XML_ErrorString(MyError)<<std::endl;
	return;
}
