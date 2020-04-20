#include "XMLWriter.h"
#include "CSVWriter.h"
#include "XMLReader.h"
#include "CSVReader.h"
#include "Path.h"
#include "StringUtils.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>


int main(int argc, char* argv[]){
	std::vector<std::string> file;
	std::vector<std::string> fileTwo;
	if (argc == 1){
		std::cout<<"Syntax Error: studconv file1 [file2 ...]"<<std::endl;
	}
	else{
		int count = 1;
		while(count < argc){
//			std::vector<std::string> file = StringUtils::Split(std::string(argv[count]),".");
			CPath file(std::string(argv[count]));
//			file = file.AbsolutePath();
//			file = std::string(file.NormalizePath());

			std::ifstream inny;
			inny.open(std::string(file), std::ifstream::in);

			std::vector< std::string > values = StringUtils::Split(std::string(file.BaseName()), "_");

			if(inny && (file.Extension() == ".csv") && values.size() == 3){
				//csv to xml;
				CCSVReader incsv(inny);
				std::ofstream outty(std::string(file.Directory()) + "/" + file.BaseName() + ".xml");
				CXMLWriter outxml(outty);

				//class info for top of the xml file
				//std::vector< std::string > values = StringUtils::Split(std::string(file.BaseName()), "_");
				std::vector< std::string > names = {"SUBJ","CRSE","SEC"};
				SXMLEntity outBuff;
				outBuff.DType = SXMLEntity::EType::StartElement;
				outBuff.DNameData = "course";
				
				for(unsigned int i = 0; i < names.size(); i++){
					outBuff.SetAttribute(names[i],values[i]);
				}
				//.... use XMLWriter to put these into the first line of xml
				outxml.WriteEntity(outBuff);
				outBuff.DAttributes.clear();

				outBuff.DType = SXMLEntity::EType::CompleteElement;
				outBuff.DNameData = "student";

				//gets the names of the differnt areas
				std::vector< std::string > types;
				incsv.ReadRow(types);
				
				//gets all the student data
				std::vector< std::string > buffy;
				while(incsv.ReadRow(buffy)){
					//check for bad line
					if(types.size()!=buffy.size()){
						continue;
					}
					else{
						outty << "\n    ";
						outty.flush();
						for(unsigned int i = 0; i < types.size(); i++){
							outBuff.SetAttribute(types[i],buffy[i]);
						}
						outxml.WriteEntity(outBuff);
						buffy.clear();
					}
					//....loop through all the parts in buff and types and add them as fields to outbuff
					
				}
				//close course
				outty << '\n';
				outty.flush();
				outxml.Flush();

				if(incsv.End()){
					std::cout << "Successfully converted " + std::string(argv[count]) << std::endl;
				}else{
					std::cout << "WARNING! Something may have gone wrong when converting " + std::string(argv[count]) << std::endl;
				}
			}else if (inny && (file.Extension() ==".xml")/* && values.size() == 3*/){
				//xml to csv
				CXMLReader inxml(inny);

				//gets the heads of the collumns of the csv
				SXMLEntity buffy;
				//get first row and check if for the right attributes
				if(!inxml.ReadEntity(buffy,true)){
					std::cout<<"Could not convert " + std::string(argv[count]) << std::endl;
					break;
				};
				
				std::vector<std::string> nameElems {"SUBJ","CRSE","SEC"};

				bool validFile = true;
				for (unsigned int i = 0; i < nameElems.size(); i++){
					if(buffy.AttributeExists(nameElems[i])){
						nameElems[i]=buffy.AttributeValue(nameElems[i]);					}
					else{
						validFile = false;
					}
				}

				std::string filename;
				if (validFile){
					filename = StringUtils::Join("_",nameElems);
				}
				else{
					std::cout << "Could not convert " + std::string(argv[count]) << std::endl;
					break;
				}
				std::ofstream outty(std::string(file.Directory())+ "/" + filename + ".csv");
				
				CCSVWriter outcsv(outty);
				std::vector< std::string > outbuff;
				
				inxml.ReadEntity(buffy,true);
				
				for (auto tup : buffy.DAttributes){
					outbuff.push_back(std::get<0>(tup));
				}
				outcsv.WriteRow(outbuff);
				outbuff.clear();
				bool badRead = false;
				do{
					//csvwriter somehow take in buffy
					if(buffy.DType == SXMLEntity::EType::StartElement){	//...make sure buffy is start or complete elem, else skip
						for (auto tup : buffy.DAttributes){
							outbuff.push_back(std::get<1>(tup));
						}
						//...set outbuff to be the different attributes
						outcsv.WriteRow(outbuff);
						outbuff.clear();
					}
					if(!inxml.ReadEntity(buffy, true)){
						badRead = true;
					}
					}while(!inxml.End());
				outty<<"\r\n"; //since his files have it for some reason
				outty.flush();
				
				if(!badRead && inxml.End()){
					std::cout << "Successfully converted " + std::string(argv[count]) << std::endl;
				}else{
					std::cout << "WARNING! Something may have gone wrong when converting " + std::string(argv[count]) << std::endl;
				}

			}else{
				std::cout << "Could not convert " + std::string(argv[count]) << std::endl;
				break;
			}
			count++;
		}
	}
	return EXIT_SUCCESS;
}
