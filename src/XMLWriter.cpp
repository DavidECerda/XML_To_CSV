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
	std::string NameorData = entity.DNameData;
	std::vector <std::string> values;

	for (auto att:entity.DAttributes){
		std::string value = std::get<1>(att);

		for (unsigned int i = 0; i < value.length(); i++) {
			auto it = std::find(specialChar.begin(),specialChar.end(),value[i]);
			if(*it != '\0'){
				switch(std::distance(specialChar.begin(),it)){
					case 0 : value.replace(i,1,"&quot;");
						break;
					case 1 : value.replace(i,1,"&amp;");
						break;
					case 2 : value.replace(i,1,"&lt;");
						break;
					case 3 : value.replace(i,1,"&gt;");
						break;
					case 4 : value.replace(i,1,"&apos;");
						break;
					default : break;
				}
			}
		}
		values.push_back(value);
	}
	//Used a switch loop since it makes life easier
	switch(entity.DType){
		case SXMLEntity::EType::StartElement :
			OutFile << "<" + NameorData;
			if(entity.DAttributes.empty()){
				OutFile << ">";
			}
			else{
				OutFile << " ";
				for (unsigned int i = 0; i<entity.DAttributes.size(); i++/*auto att:entity.DAttributes*/){
					OutFile << std::get<0>(entity.DAttributes[i]) + "=" + "\"" + values[i] + "\"";
						if ((entity.DAttributes.size() - 1) != i){
							OutFile <<" ";
						}
					}
			OutFile << ">";
			}
			openElements.push(NameorData);
			sameElem = NameorData;
			wrote=true;
			break;

		case SXMLEntity::EType::EndElement :

			if(!openElements.empty()){
				if (NameorData == openElements.top()){
					if(!sameElem.empty() && openElements.top() == sameElem){
						long pos = OutFile.tellp();
						OutFile.seekp(pos-1);
						OutFile << "/>";
						openElements.pop();
						wrote = true;
					}
					else{
						OutFile << "</" + NameorData + ">";
						openElements.pop();
						wrote = true;
					}
				}
				else{
					wrote = false;
				}
				if(wrote){
				sameElem.clear();
				}
			}
			break;

		case SXMLEntity::EType::CharData :
			
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
			wrote = true;
			sameElem.clear();
			break;

		case SXMLEntity::EType::CompleteElement :
			OutFile << "<" + entity.DNameData + " ";
			for (unsigned int i = 0; i<entity.DAttributes.size(); i++/*auto att:entity.DAttributes*/){
				OutFile << std::get<0>(entity.DAttributes[i]) + "=" + "\"" + values[i] + "\"";
				if ((entity.DAttributes.size() -1) != i){
					OutFile <<" ";
				}
			}
			OutFile << "/>";
			sameElem.clear();
			wrote = true;
			break;

		default : wrote = false;
			break;
	}
	OutFile.flush();
	return wrote;
}

