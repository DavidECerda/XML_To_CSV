#include <gtest/gtest.h>
#include "XMLWriter.h"
#include "XMLReader.h"
#include <fstream>
#include <iostream>
#include <sstream>

TEST(XMLTest, XMLReadTest){
	std::stringstream teststream("<course SUBJ=\"ABC\" CRSE=\"114\" SEC=\"2\">500000\n\n\r\ter"
								  "<student SEAT=\"1\" SID=\"900667130\" SURNAME=\"Stein\" PREFNAME=\"Ava\" LEVEL=\"UG\" UNITS=\"2\" CLASS=\"FR\" MAJOR=\"DEF2\" GRADE=\"I\" STATUS=\"RE\" EMAIL=\"ava.s.stein@fakeu.edu\"/>gravity"
								  "</course>");
	
	CXMLReader testobj(teststream);
	SXMLEntity tempEntity;
	
	EXPECT_FALSE(testobj.End());
	EXPECT_TRUE(testobj.ReadEntity(tempEntity,true));

	EXPECT_EQ(tempEntity.DNameData, "course");
	EXPECT_TRUE(tempEntity.DType == SXMLEntity::EType::StartElement);
	EXPECT_TRUE(tempEntity.AttributeExists("SEC"));
	EXPECT_EQ(tempEntity.AttributeValue("CRSE"),"114");
	EXPECT_EQ(std::get<0>(tempEntity.DAttributes[0]),"SUBJ");
	EXPECT_EQ(std::get<1>(tempEntity.DAttributes[0]),"ABC");
	EXPECT_EQ(std::get<0>(tempEntity.DAttributes[1]),"CRSE");
	EXPECT_EQ(std::get<1>(tempEntity.DAttributes[1]),"114");
	EXPECT_EQ(std::get<0>(tempEntity.DAttributes[2]),"SEC");
	EXPECT_EQ(std::get<1>(tempEntity.DAttributes[2]),"2");

	EXPECT_TRUE(testobj.ReadEntity(tempEntity,true));
	EXPECT_EQ(tempEntity.DNameData, "student");
	EXPECT_TRUE(tempEntity.DType == SXMLEntity::EType::StartElement);
	EXPECT_TRUE(tempEntity.AttributeExists("PREFNAME"));
	EXPECT_EQ(tempEntity.AttributeValue("EMAIL"),"ava.s.stein@fakeu.edu");
	EXPECT_EQ(std::get<0>(tempEntity.DAttributes[0]),"SEAT");
	EXPECT_EQ(std::get<1>(tempEntity.DAttributes[0]),"1");
	EXPECT_EQ(std::get<0>(tempEntity.DAttributes[1]),"SID");
	EXPECT_EQ(std::get<1>(tempEntity.DAttributes[1]),"900667130");
	EXPECT_EQ(std::get<0>(tempEntity.DAttributes[2]),"SURNAME");
	EXPECT_EQ(std::get<1>(tempEntity.DAttributes[2]),"Stein");
	EXPECT_EQ(std::get<0>(tempEntity.DAttributes[3]),"PREFNAME");
	EXPECT_EQ(std::get<1>(tempEntity.DAttributes[3]),"Ava");
	EXPECT_EQ(std::get<0>(tempEntity.DAttributes[4]),"LEVEL");
	EXPECT_EQ(std::get<1>(tempEntity.DAttributes[4]),"UG");
	EXPECT_EQ(std::get<0>(tempEntity.DAttributes[5]),"UNITS");
	EXPECT_EQ(std::get<1>(tempEntity.DAttributes[5]),"2");
	EXPECT_EQ(std::get<0>(tempEntity.DAttributes[6]),"CLASS");
	EXPECT_EQ(std::get<1>(tempEntity.DAttributes[6]),"FR");
	EXPECT_EQ(std::get<0>(tempEntity.DAttributes[7]),"MAJOR");
	EXPECT_EQ(std::get<1>(tempEntity.DAttributes[7]),"DEF2");
	EXPECT_EQ(std::get<0>(tempEntity.DAttributes[8]),"GRADE");
	EXPECT_EQ(std::get<1>(tempEntity.DAttributes[8]),"I");
	EXPECT_EQ(std::get<0>(tempEntity.DAttributes[9]),"STATUS");
	EXPECT_EQ(std::get<1>(tempEntity.DAttributes[9]),"RE");
	EXPECT_EQ(std::get<0>(tempEntity.DAttributes[10]),"EMAIL");
	EXPECT_EQ(std::get<1>(tempEntity.DAttributes[10]),"ava.s.stein@fakeu.edu");

	EXPECT_TRUE(testobj.ReadEntity(tempEntity));
	EXPECT_TRUE(tempEntity.DType == SXMLEntity::EType::EndElement);
	EXPECT_EQ(tempEntity.DNameData, "student");

	EXPECT_TRUE(testobj.ReadEntity(tempEntity));
	EXPECT_TRUE(tempEntity.DType == SXMLEntity::EType::CharData);
	EXPECT_TRUE(tempEntity.DNameData == "gravity");

	EXPECT_TRUE(testobj.ReadEntity(tempEntity));
	EXPECT_TRUE(tempEntity.DType == SXMLEntity::EType::EndElement);
	EXPECT_EQ(tempEntity.DNameData, "course");

	EXPECT_TRUE(testobj.End());
	EXPECT_FALSE(testobj.ReadEntity(tempEntity));
}

TEST(XMLTest, XMLWriteTest){

	std::string test = "<people>Who"	
							"<nationality COUN=\"USA\">"
								"<state CITY=\"San Jose\">"
									"<college UNIV=\"UCDavis\">"
										"<course SUBJ=\"ABC\" CRSE=\"376\" SEC=\"36\">"
											"<student SEAT=\"1\" SID=\"409337710\" SURNAME=\"White\" PREFNAME=\"Victoria\" LEVEL=\"GR\" UNITS=\"12\" CLASS=\"D1\" MAJOR=\"ABCG\" GRADE=\"S\" STATUS=\"RE\" EMAIL=\"victoria.b.white@fakeu.edu\"/>"
											"<student SEAT=\"2\" SID=\"997678589\" SURNAME=\"Carroll\" PREFNAME=\"Jackson\" LEVEL=\"GR\" UNITS=\"12\" CLASS=\"D2\" MAJOR=\"ABCG\" GRADE=\"S\" STATUS=\"RE\" EMAIL=\"jackson.a.carroll@fakeu.edu\"/>"
											"<student SEAT=\"3\" SID=\"719554701\" SURNAME=\"English\" PREFNAME=\"Vincent\" LEVEL=\"GR\" UNITS=\"12\" CLASS=\"D1\" MAJOR=\"OT78\" GRADE=\"I\" STATUS=\"RE\" EMAIL=\"vincent.a.english@fakeu.edu\"/>"
											"<student SEAT=\"4\" SID=\"341825746\" SURNAME=\"Henderson\" PREFNAME=\"Caleb\" LEVEL=\"GR\" UNITS=\"8\" CLASS=\"D1\" MAJOR=\"ABCG\" GRADE=\"S\" STATUS=\"RE\" EMAIL=\"caleb.l.henderson@fakeu.edu\"/>"
										"</course>"
									"</college>"
								"</state>"
							"</nationality>"
						"</people>";
	
	std::vector<std::string> names = {"SEAT","SID","SURNAME","PREFNAME","LEVEL","UNITS","CLASS","MAJOR","GRADE","STATUS","EMAIL"};
	std::vector<std::string> values = {"1", "409337710", "White" ,"Victoria", "GR","12","D1","ABCG","S","RE","victoria.b.white@fakeu.edu",
                 "2","997678589","Carroll","Jackson","GR","12","D2","ABCG","S","RE","jackson.a.carroll@fakeu.edu",
                 "3","719554701","English","Vincent","GR","12","D1","OT78","I","RE","vincent.a.english@fakeu.edu",
                 "4","341825746","Henderson","Caleb","GR","8","D1","ABCG","S","RE","caleb.l.henderson@fakeu.edu"};
	std::stringstream testwritestream("");
	CXMLWriter testobj(testwritestream);
	SXMLEntity tempEntity;
	
	tempEntity.DType = SXMLEntity::EType::StartElement;
	tempEntity.DNameData = "people";
	EXPECT_TRUE(testobj.WriteEntity(tempEntity));

	tempEntity.DType = SXMLEntity::EType::CharData;
	tempEntity.DNameData = "Who";
	tempEntity.SetAttribute("COUN","San Jose");
	EXPECT_TRUE(testobj.WriteEntity(tempEntity));

	tempEntity.DType = SXMLEntity::EType::StartElement;
	tempEntity.DNameData = "nationality";
	tempEntity.SetAttribute("COUN","USA");
	EXPECT_TRUE(testobj.WriteEntity(tempEntity));

	tempEntity.DNameData = "state";
	tempEntity.DAttributes.clear();
	tempEntity.DAttributes.push_back(std::make_pair("CITY","San Jose"));
	EXPECT_TRUE(testobj.WriteEntity(tempEntity));

	tempEntity.DNameData = "college";
	tempEntity.DAttributes.clear();
	tempEntity.SetAttribute("UNIV","UCDavis");
	EXPECT_TRUE(testobj.WriteEntity(tempEntity));

	tempEntity.DNameData = "course";
	tempEntity.DAttributes.clear();
	tempEntity.SetAttribute("SUBJ","ABC"), tempEntity.SetAttribute("CRSE","376"), tempEntity.SetAttribute("SEC","36");
	EXPECT_TRUE(testobj.WriteEntity(tempEntity));

	tempEntity.DNameData = "student";
	tempEntity.DAttributes.clear();
	
	int students = (values.size()/names.size());
	int k;
	
	for(int i = 0; i < students-1; i++){
		tempEntity.DType = SXMLEntity::EType::CompleteElement;
		for (unsigned int j = 0; j < names.size(); j++){
			tempEntity.SetAttribute(names[j],values[k]);
			k++;
		}
		EXPECT_TRUE(testobj.WriteEntity(tempEntity));
	}
	tempEntity.DType = SXMLEntity::EType::StartElement;
	for (unsigned int j = 0; j < names.size(); j++){
			tempEntity.SetAttribute(names[j],values[k]);
			k++;
	}
	EXPECT_TRUE(testobj.WriteEntity(tempEntity));
	tempEntity.DType = SXMLEntity::EType::EndElement;
	tempEntity.DNameData = "I am a test";
	EXPECT_FALSE(testobj.WriteEntity(tempEntity));
	tempEntity.DNameData = "student";
	EXPECT_TRUE(testobj.WriteEntity(tempEntity));
	tempEntity.DNameData = "course";
	EXPECT_TRUE(testobj.WriteEntity(tempEntity));
	EXPECT_TRUE(testobj.Flush());
	EXPECT_FALSE(testobj.Flush());
	EXPECT_EQ(test,testwritestream.str());

	std::stringstream teststream(test);
	CXMLReader testobj2(teststream);
	std::stringstream writetest("");
	CXMLWriter testobj3(writetest);
	while(testobj2.ReadEntity(tempEntity)){
		testobj3.WriteEntity(tempEntity);
	}
	EXPECT_EQ(writetest.str(),testwritestream.str());
}

TEST(XMLTest, XMLRWConversionTest){
	std::string test = "<course SUBJ=\"ABC\" CRSE=\"204\" SEC=\"2\">fafas"
    "<student SEAT=\"1\" SID=\"679358754\" SURNAME=\"Rose\" PREFNAME=\"Sarah\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O268\" GRADE=\"C+\" STATUS=\"RE\" EMAIL=\"sarah.o.rose@fakeu.edu\"/>dsa&quot;dsd"
    "<student SEAT=\"2\" SID=\"493845023\" SURNAME=\"Freeman\" PREFNAME=\"Alexander\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O239\" GRADE=\"A+\" STATUS=\"RE\" EMAIL=\"alexander.b.freeman@fakeu.edu\"/>fsaa&amp;&amp;das"
    "<student SEAT=\"3\" SID=\"992212797\" SURNAME=\"Keller\" PREFNAME=\"Olivia\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"OT35\" GRADE=\"C+\" STATUS=\"RE\" EMAIL=\"olivia.v.keller@fakeu.edu\"/>da&gt;sd"
    "<student SEAT=\"4\" SID=\"540708735\" SURNAME=\"Daniel\" PREFNAME=\"Noah\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"ABC1\" GRADE=\"B-\" STATUS=\"RE\" EMAIL=\"noah.a.daniel@fakeu.edu\"/>ads&lt;ada"
 	"<student SEAT=\"5\" SID=\"411409681\" SURNAME=\"O&apos;brien\" PREFNAME=\"Mason\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O224\" GRADE=\"C+\" STATUS=\"RE\" EMAIL=\"mason.z.o&apos;brien@fakeu.edu\"/>as\tsa\ndas"
	"<teams><pair STUDENTONE=\"Alex D.\" STUDENTTWO=\"David C.\"/><pair STUDENTONE=\"Ryan P.\" STUDENTTWO=\"Harry Z.\"/><pair STUDENTONE=\"Faust M.\" STUDENTTWO=\"William S.\"/><pair STUDENTONE=\"Chris N.\" STUDENTTWO=\"Nick T.\"/></teams>"
    "<student SEAT=\"11\" SID=\"793868457\" SURNAME=\"Robertson\" PREFNAME=\"Benjamin\" LEVEL=\"GR\" UNITS=\"4\" CLASS=\"GM\" MAJOR=\"O272\" GRADE=\"B\" STATUS=\"RE\" EMAIL=\"benjamin.b.robertson@fakeu.edu\"/>adasda\n"
    "<student SEAT=\"12\" SID=\"976734390\" SURNAME=\"Peters\" PREFNAME=\"Hailey\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O224\" GRADE=\"B\" STATUS=\"RE\" EMAIL=\"hailey.v.peters@fakeu.edu\"/>"
    "<student SEAT=\"13\" SID=\"515177515\" SURNAME=\"Boyd\" PREFNAME=\"Mateo\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"JR\" MAJOR=\"O224\" GRADE=\"A-\" STATUS=\"RE\" EMAIL=\"mateo.k.boyd@fakeu.edu\"/>\t"
    "<student SEAT=\"21\" SID=\"746877527\" SURNAME=\"Cook\" PREFNAME=\"Brandon\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"ABC1\" GRADE=\"B+\" STATUS=\"RE\" EMAIL=\"brandon.j.cook@fakeu.edu\"/>"
    "<student SEAT=\"22\" SID=\"379128764\" SURNAME=\"Whitney\" PREFNAME=\"Mason\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O160\" GRADE=\"B+\" STATUS=\"RE\" EMAIL=\"mason.j.whitney@fakeu.edu\"/>asddsa"
	"</course>";



	std::stringstream teststream(test);

	CXMLReader testobj(teststream);
	SXMLEntity tempEntity;
	std::stringstream testwritestream("");
	CXMLWriter testobj2(testwritestream);
	
	EXPECT_FALSE(testobj.End());
	while(testobj.ReadEntity(tempEntity)){
		EXPECT_TRUE(testobj2.WriteEntity(tempEntity));
	}
	EXPECT_TRUE(testobj.End());
	EXPECT_EQ(teststream.str(),testwritestream.str());
	
	std::stringstream teststream2(test);

	std::stringstream testwritestream2("");
	CXMLReader testobj3(teststream2);
	CXMLWriter testobj4(testwritestream2);
	SXMLEntity tempEntity2;

	EXPECT_FALSE(testobj3.End());
	while(testobj3.ReadEntity(tempEntity2,true)){
		EXPECT_TRUE(testobj4.WriteEntity(tempEntity2));
	}
	EXPECT_TRUE(testobj.End());
	EXPECT_FALSE(teststream2.str()==testwritestream2.str());
	EXPECT_EQ("<course SUBJ=\"ABC\" CRSE=\"204\" SEC=\"2\"><student SEAT=\"1\" SID=\"679358754\" SURNAME=\"Rose\" PREFNAME=\"Sarah\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O268\" GRADE=\"C+\" STATUS=\"RE\" EMAIL=\"sarah.o.rose@fakeu.edu\"/>"
	"<student SEAT=\"2\" SID=\"493845023\" SURNAME=\"Freeman\" PREFNAME=\"Alexander\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O239\" GRADE=\"A+\" STATUS=\"RE\" EMAIL=\"alexander.b.freeman@fakeu.edu\"/>"
	"<student SEAT=\"3\" SID=\"992212797\" SURNAME=\"Keller\" PREFNAME=\"Olivia\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"OT35\" GRADE=\"C+\" STATUS=\"RE\" EMAIL=\"olivia.v.keller@fakeu.edu\"/>"
	"<student SEAT=\"4\" SID=\"540708735\" SURNAME=\"Daniel\" PREFNAME=\"Noah\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"ABC1\" GRADE=\"B-\" STATUS=\"RE\" EMAIL=\"noah.a.daniel@fakeu.edu\"/>"
	"<student SEAT=\"5\" SID=\"411409681\" SURNAME=\"O&apos;brien\" PREFNAME=\"Mason\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O224\" GRADE=\"C+\" STATUS=\"RE\" EMAIL=\"mason.z.o&apos;brien@fakeu.edu\"/>"
	"<teams><pair STUDENTONE=\"Alex D.\" STUDENTTWO=\"David C.\"/><pair STUDENTONE=\"Ryan P.\" STUDENTTWO=\"Harry Z.\"/><pair STUDENTONE=\"Faust M.\" STUDENTTWO=\"William S.\"/><pair STUDENTONE=\"Chris N.\" STUDENTTWO=\"Nick T.\"/></teams>"
	"<student SEAT=\"11\" SID=\"793868457\" SURNAME=\"Robertson\" PREFNAME=\"Benjamin\" LEVEL=\"GR\" UNITS=\"4\" CLASS=\"GM\" MAJOR=\"O272\" GRADE=\"B\" STATUS=\"RE\" EMAIL=\"benjamin.b.robertson@fakeu.edu\"/>"
    "<student SEAT=\"12\" SID=\"976734390\" SURNAME=\"Peters\" PREFNAME=\"Hailey\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O224\" GRADE=\"B\" STATUS=\"RE\" EMAIL=\"hailey.v.peters@fakeu.edu\"/>"
    "<student SEAT=\"13\" SID=\"515177515\" SURNAME=\"Boyd\" PREFNAME=\"Mateo\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"JR\" MAJOR=\"O224\" GRADE=\"A-\" STATUS=\"RE\" EMAIL=\"mateo.k.boyd@fakeu.edu\"/>"
    "<student SEAT=\"21\" SID=\"746877527\" SURNAME=\"Cook\" PREFNAME=\"Brandon\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"ABC1\" GRADE=\"B+\" STATUS=\"RE\" EMAIL=\"brandon.j.cook@fakeu.edu\"/>"
    "<student SEAT=\"22\" SID=\"379128764\" SURNAME=\"Whitney\" PREFNAME=\"Mason\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O160\" GRADE=\"B+\" STATUS=\"RE\" EMAIL=\"mason.j.whitney@fakeu.edu\"/>"
	"</course>",testwritestream2.str());
	
}

TEST(XMLTest, XMLWRConversionTest){
	std::string test ="<state CITY=\"San Jose\">"
						"<college UNIV=\"UCDavis\">"
							"<course SUBJ=\"ABC\" CRSE=\"376\" SEC=\"36\">"
								"<student SEAT=\"1\" SID=\"409337710\" SURNAME=\"White\" PREFNAME=\"Victoria\" LEVEL=\"GR\" UNITS=\"12\" CLASS=\"D1\" MAJOR=\"ABCG\" GRADE=\"S\" STATUS=\"RE\" EMAIL=\"victoria.b.white@fakeu.edu\"/>"
								"<student SEAT=\"2\" SID=\"997678589\" SURNAME=\"Carroll\" PREFNAME=\"Jackson\" LEVEL=\"GR\" UNITS=\"12\" CLASS=\"D2\" MAJOR=\"ABCG\" GRADE=\"S\" STATUS=\"RE\" EMAIL=\"jackson.a.carroll@fakeu.edu\"/>"
								"<student SEAT=\"3\" SID=\"719554701\" SURNAME=\"English\" PREFNAME=\"Vincent\" LEVEL=\"GR\" UNITS=\"12\" CLASS=\"D1\" MAJOR=\"OT78\" GRADE=\"I\" STATUS=\"RE\" EMAIL=\"vincent.a.english@fakeu.edu\"/>"
								"<student SEAT=\"4\" SID=\"341825746\" SURNAME=\"Henderson\" PREFNAME=\"Caleb\" LEVEL=\"GR\" UNITS=\"8\" CLASS=\"D1\" MAJOR=\"ABCG\" GRADE=\"S\" STATUS=\"RE\" EMAIL=\"caleb.l.henderson@fakeu.edu\"/>"
							"</course>"
						"</college>"
					"</state>";
	
	std::vector<std::string> names = {"SEAT","SID","SURNAME","PREFNAME","LEVEL","UNITS","CLASS","MAJOR","GRADE","STATUS","EMAIL"};
	std::vector<std::string> values = {"1", "409337710", "White" ,"Victoria", "GR","12","D1","ABCG","S","RE","victoria.b.white@fakeu.edu",
                 "2","997678589","Carroll","Jackson","GR","12","D2","ABCG","S","RE","jackson.a.carroll@fakeu.edu",
                 "3","719554701","English","Vincent","GR","12","D1","OT78","I","RE","vincent.a.english@fakeu.edu",
                 "4","341825746","Henderson","Caleb","GR","8","D1","ABCG","S","RE","caleb.l.henderson@fakeu.edu"};
	std::stringstream testwritestream("");
	CXMLWriter testobj(testwritestream);
	SXMLEntity tempEntity;

	tempEntity.DType = SXMLEntity::EType::StartElement;
	tempEntity.DNameData = "state";
	tempEntity.DAttributes.clear();
	tempEntity.SetAttribute("CITY","San Jose");
	EXPECT_TRUE(testobj.WriteEntity(tempEntity));

	tempEntity.DNameData = "college";
	tempEntity.DAttributes.clear();
	tempEntity.SetAttribute("UNIV","UCDavis");
	EXPECT_TRUE(testobj.WriteEntity(tempEntity));

	tempEntity.DNameData = "course";
	tempEntity.DAttributes.clear();
	tempEntity.SetAttribute("SUBJ","ABC"), tempEntity.SetAttribute("CRSE","376"), tempEntity.SetAttribute("SEC","36");
	EXPECT_TRUE(testobj.WriteEntity(tempEntity));

	tempEntity.DNameData = "student";
	tempEntity.DAttributes.clear();

	int students = (values.size()/names.size());
	int k;

	for(int i = 0; i < students; i++){
		tempEntity.DType = SXMLEntity::EType::CompleteElement;
		for (unsigned int j = 0; j < names.size(); j++){
			tempEntity.SetAttribute(names[j],values[k]);
			k++;
		}
		EXPECT_TRUE(testobj.WriteEntity(tempEntity));
	}

	EXPECT_TRUE(testobj.Flush());
	EXPECT_EQ(test,testwritestream.str());

	CXMLReader testobj2(testwritestream);

	EXPECT_TRUE(testobj2.ReadEntity(tempEntity,true));
	EXPECT_TRUE(tempEntity.AttributeExists("CITY"));
	EXPECT_EQ(tempEntity.AttributeValue("CITY"),"San Jose");
	EXPECT_TRUE(testobj2.ReadEntity(tempEntity,true));
	EXPECT_TRUE(testobj2.ReadEntity(tempEntity,true));
	EXPECT_TRUE(testobj2.ReadEntity(tempEntity,true));
	EXPECT_TRUE(tempEntity.DType == SXMLEntity::EType::StartElement);
	EXPECT_TRUE(tempEntity.AttributeExists("PREFNAME"));
	EXPECT_EQ(tempEntity.AttributeValue("PREFNAME"),"Victoria");

	while(testobj2.ReadEntity(tempEntity,true));
	EXPECT_TRUE(tempEntity.DType == SXMLEntity::EType::EndElement);
	EXPECT_EQ(tempEntity.DNameData, "state");
}

TEST(XMLTest,SideTest){
	std::string test = "<breakfast_menu>\n"
							"\t<food>\n"
								"\t<name>Belgian Waffles</name>\n"
								"\t<price>$5.95</price>\n"
								"\t<description>Two of our famous Belgian Waffles with plenty of real maple syrup</description>\n"
								"\t<calories>650</calories>\n"
							"\t</food>\n"
							"\t<food>\n"
								"\t<name>Strawberry Belgian Waffles</name>\n"
								"\t<price>$7.95</price>\n"
								"\t<description>Light Belgian waffles covered with strawberries and whipped cream</description>\n"
								"\t<calories>900</calories>\n"
							"\t</food>\n"
							"\t<food>\n"
								"\t<name>Berry-Berry Belgian Waffles</name>\n"
								"\t<price>$8.95</price>\n"
								"\t<description>Light Belgian waffles covered with an assortment of fresh berries and whipped cream</description>\n"
								"\t<calories>900</calories>\n"
							"\t</food>\n"
							"\t<food>\n"
								"\t<name>French Toast</name>\n"
								"\t<price>$4.50</price>\n"
								"\t<description>Thick slices made from our homemade sourdough bread</description>\n"
								"\t<calories>600</calories>\n"
							"\t</food>\n"
								"\t<food>\n"
								"\t<name>Homestyle Breakfast</name>\n"
								"\t<price>$6.95</price>\n"
								"\t<description>Two eggs, bacon or sausage, toast, and our ever-popular hash browns</description>\n"
								"\t<calories>950</calories>\n"
							"\t</food>\n"
						"\t</breakfast_menu>";

	std::stringstream teststream(test);

	std::stringstream testwritestream("");
	CXMLReader testobj(teststream);
	CXMLWriter testobj2(testwritestream);
	SXMLEntity tempEntity;
	EXPECT_FALSE(testobj.End());
	while(!testobj.End()){
		testobj.ReadEntity(tempEntity);
		testobj2.WriteEntity(tempEntity);
	}
	EXPECT_TRUE(testobj.End());
	EXPECT_EQ(test,testwritestream.str());
}

