#include "XMLReader.h"
#include <expat.h>
#include <iostream>
#define BUFFER_SIZE 8192

CXMLReader::CXMLReader(std::istream &is) : InFile(is){
	MyParser = XML_ParserCreate(NULL);
	XML_SetUserData(MyParser, this);
	XML_SetElementHandler(MyParser,CXMLReader::start_element,CXMLReader::end_element);
	XML_SetCharacterDataHandler(MyParser,CXMLReader::handle_data);
}

void CXMLReader::start_element(void *data, const char* elem, const char** att){ //called on a start element and adds elems to entity
	CXMLReader *Ptr = static_cast<CXMLReader*>(data);
	struct SXMLEntity MyEntity;
	MyEntity.DType = SXMLEntity::EType::StartElement;
	MyEntity.DNameData = elem;
	for (int i = 0; att[i]; i += 2){  //https://stackoverflow.com/questions/609376/geting-xml-data-using-xml-parser-expat
		
		MyEntity.SetAttribute(std::string(att[i]), std::string(att[i+1]));
	}
	Ptr->ManyEntity.push(MyEntity);
}

void CXMLReader::end_element(void *data, const char* elem){
	CXMLReader *Ptr = static_cast<CXMLReader*>(data);
	struct SXMLEntity MyEntity;

	MyEntity.DType = SXMLEntity::EType::EndElement;
	MyEntity.DNameData = elem;
	Ptr->ManyEntity.push(MyEntity);
}

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

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
	
	//bool gotEntity = true;
	bool gotEntity = false;
	if(!End()){
		if (!ManyEntity.empty()){
			if (skipcdata){
				while (!ManyEntity.empty() && ManyEntity.front().DType == SXMLEntity::EType::CharData){
					ManyEntity.pop();
				}
				if(!ManyEntity.empty()){
					entity = ManyEntity.front();
					ManyEntity.pop();
					gotEntity = true;
				}
				/*
				else{
					 return(ReadEntity(entity,true));
					 } */
			}
				
			else{
				entity = ManyEntity.front();
				ManyEntity.pop();
				gotEntity = true;
			}
			
		}
		if(!gotEntity && ManyEntity.empty()){
			char buffer[BUFFER_SIZE];

			while(ManyEntity.empty()){
				
				InFile.read(buffer, BUFFER_SIZE);

				int isfinal; //Equivalent to csv_fini
				if (InFile.eof()){
					isfinal = 1;
				}
				else{
					isfinal = 0;
				}
				if(!XML_Parse(MyParser,buffer,(InFile.gcount()),isfinal) == XML_STATUS_ERROR){
					
					if (skipcdata){
						while (!ManyEntity.empty() && ManyEntity.front().DType == SXMLEntity::EType::CharData){
							ManyEntity.pop();
						}
						if(!ManyEntity.empty()){
							entity = ManyEntity.front();
							ManyEntity.pop();
							gotEntity = true;
						}
					}
					else{
						if (!ManyEntity.empty()){
							entity = ManyEntity.front();
							ManyEntity.pop();
							gotEntity = true;
						}
					}
				}
				else{
					XML_Error MyError = XML_GetErrorCode(MyParser);
					
					std::cout<<"Issue With Parsing || Error Type: "<<XML_ErrorString(MyError)<<std::endl;
					break;
				}
			}
		}
	}
	return gotEntity;
}
	
