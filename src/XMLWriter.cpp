#include "XMLWriter.h"
#include <iostream>
#include <utility>
#include <algorithm>


CXMLWriter::CXMLWriter(std::ostream &os) : OutFile(os){}

CXMLWriter::~CXMLWriter(){}

bool CXMLWriter::Flush(){
	bool wrote = false;
	while (!openElements.empty()){
		OutFile << "</" + openElements.top() + ">";
		openElements.pop();
		wrote = true;
	}
	return wrote;
}

bool CXMLWriter::WriteEntity(const SXMLEntity &entity){
	bool wrote = false;

	std::vector<char> specialChar = {'"','&','<','>','\'','\0'};
	std::vector <std::string> values;

	LoadValues(entity, specialChar, values);
	
	//Used a switch loop since it makes life easier
	switch(entity.DType){
		case SXMLEntity::EType::StartElement :
			WriteStartElement(values, entity);
			wrote = true;

		case SXMLEntity::EType::EndElement :
			wrote = WriteEndElement(values, entity);

		case SXMLEntity::EType::CharData :
			WriteCharData(specialChar, values, entity);
			wrote = true;

		case SXMLEntity::EType::CompleteElement :
			WriteCompleteElement(values, entity);
			wrote = true;

		default : wrote = false;
	}
	OutFile.flush();
	return wrote;
}

void CXMLWriter::WriteStartElement(const std::vector <std::string> &values, const SXMLEntity &entity){
	std::string NameorData = entity.DNameData;
	OutFile << "<" + NameorData;
	if(entity.DAttributes.empty()){
		OutFile << ">";
	}
	else{
		OutFile << " ";
		for (unsigned int i = 0; i<entity.DAttributes.size(); i++){
			OutFile << std::get<0>(entity.DAttributes[i]) + "=" + "\"" + values[i] + "\"";
				if ((entity.DAttributes.size() - 1) != i){
					OutFile <<" ";
				}
			}
	OutFile << ">";
	}
	openElements.push(NameorData);
	sameElem = NameorData;
}

bool CXMLWriter::WriteEndElement(const std::vector <std::string> &values, const SXMLEntity &entity){
	std::string NameorData = entity.DNameData;
	if(!openElements.empty()){
		if (NameorData == openElements.top()){
			if(!sameElem.empty() && openElements.top() == sameElem){
				long pos = OutFile.tellp();
				OutFile.seekp(pos-1);
				OutFile << "/>";
				openElements.pop();
			}
			else{
				OutFile << "</" + NameorData + ">";
				openElements.pop();
			}

			sameElem.clear();
			return true;
		}
	}
	return false;
}
void CXMLWriter::WriteCharData(const std::vector<char> &specialChar, const std::vector <std::string> &values, const SXMLEntity &entity){
	std::string NameorData = entity.DNameData;
	for (unsigned int i = 0; i < NameorData.length(); i++) {
		auto it = std::find(specialChar.begin(),specialChar.end(),NameorData[i]);
		if(*it != '\0'){
			switch(std::distance(specialChar.begin(),it)){
				case 0 : NameorData.replace(i,1,"&quot;");
					break;
				case 1 : NameorData.replace(i,1,"&amp;");
					break;
				case 2 : NameorData.replace(i,1,"&lt;");
					break;
				case 3 : NameorData.replace(i,1,"&gt;");
					break;
				case 4 : NameorData.replace(i,1,"&apos;");
					break;
				default : break;
			}
		}
	}
	OutFile << NameorData;
	sameElem.clear();
	return; 
}

void CXMLWriter::WriteCompleteElement(const std::vector<std::string> &values, const SXMLEntity &entity){
	OutFile << "<" + entity.DNameData + " ";

	for (unsigned int i = 0; i<entity.DAttributes.size(); i++/*auto att:entity.DAttributes*/){
		OutFile << std::get<0>(entity.DAttributes[i]) + "=" + "\"" + values[i] + "\"";
		if ((entity.DAttributes.size() -1) != i){
			OutFile <<" ";
		}
	}

	OutFile << "/>";
	sameElem.clear();
	return;
}