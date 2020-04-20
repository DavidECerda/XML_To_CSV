#include <gtest/gtest.h>
//#include "XMLWriter.h"
#include "CSVWriter.h"
//#include "XMLReader.h"
#include "CSVReader.h"

#include <istream>
#include <iostream>
#include <sstream>

TEST(CSVTest, CSVReadTest){
	std::stringstream teststream("\"Name,five\",\"Height\",\"Age\"\n\"Alex\",\"Like 10\",\"93\"\n\"Carl\",\"333\",\"21\"");
	CCSVReader testobj(teststream);
	//teststream << "Name,Height,Age\nAlex,Like 10,93\nCarl,333,21";
	//std::cout << teststream.str() << std::endl;
	
	EXPECT_FALSE(testobj.End());

	std::vector< std::string > row;
	EXPECT_TRUE(testobj.ReadRow(row));

	EXPECT_EQ(row[0],"Name,five");
	EXPECT_EQ(row[1],"Height");
	EXPECT_EQ(row[2],"Age");

	EXPECT_TRUE(testobj.ReadRow(row));
	EXPECT_EQ(row[0],"Alex");
	EXPECT_EQ(row[1],"Like 10");
	EXPECT_EQ(row[2],"93");

	EXPECT_TRUE(testobj.ReadRow(row));

	EXPECT_EQ(row[0],"Carl");
	EXPECT_EQ(row[1],"333");
	EXPECT_EQ(row[2],"21");

	EXPECT_TRUE(testobj.End());

	EXPECT_FALSE(testobj.ReadRow(row));
}

TEST(CSVTest, CSVLibTest){
	std::stringstream teststream("\"\"\"a,b\"\"\",,\" \"\"\"\" \",\"\"\"\"\" \",\" \"\"\"\"\",\"\"\"\"\"\"");

	CCSVReader testobj(teststream);
	
	EXPECT_FALSE(testobj.End());

	std::vector< std::string > row;
	EXPECT_TRUE(testobj.ReadRow(row));

	EXPECT_EQ(row[0],"\"a,b\"");
	EXPECT_EQ(row[1],"");
	EXPECT_EQ(row[2]," \"\" ");
	EXPECT_EQ(row[3],"\"\" ");
	EXPECT_EQ(row[4]," \"\"");
	EXPECT_EQ(row[5],"\"\"");

	std::stringstream teststreamTwo("\",\",ffsdfsf,\",\",\"\"");
	CCSVReader testobjtwo(teststreamTwo);
	EXPECT_FALSE(testobjtwo.End());

	std::vector< std::string > rowTwo;
	EXPECT_TRUE(testobjtwo.ReadRow(rowTwo));

	EXPECT_EQ(rowTwo[0],",");
	EXPECT_EQ(rowTwo[1],"ffsdfsf");
	EXPECT_EQ(rowTwo[2],",");
	EXPECT_EQ(rowTwo[3],"");

	EXPECT_TRUE(testobjtwo.End());

	EXPECT_FALSE(testobjtwo.ReadRow(rowTwo));
}

TEST(CSVTest, CSVWriteTest){
	std::stringstream teststream("");
	CCSVWriter testobj(teststream);
	std::vector< std::string > row = {"Name, five", "Height", "Age"};
	
	EXPECT_TRUE(testobj.WriteRow(row));
	EXPECT_EQ(teststream.str(), "\"Name, five\",\"Height\",\"Age\"");

	row.clear();
	row = {"Alex","Like \"10\"","93   "};

	EXPECT_TRUE(testobj.WriteRow(row));
	EXPECT_EQ(teststream.str(), "\"Name, five\",\"Height\",\"Age\"\r\n\"Alex\",\"Like \"\"10\"\"\",\"93   \"");
}

TEST(CSVTest, libreadtests){
	std::vector< std::string > buff;
	std::vector< std::string > correct;

	std::stringstream test01_data(" 1,2 ,  3         ,4,5\x0d\x0a");
	CCSVReader test1(test01_data);
	EXPECT_TRUE(test1.ReadRow(buff));
	correct = {"1","2","3","4","5"};
	EXPECT_EQ(buff, correct);
	buff.clear();
	correct.clear();


	std::stringstream test02_data(",,,,,\x0a");
	CCSVReader test2(test02_data);
	EXPECT_TRUE(test2.ReadRow(buff));
	correct = {"","","","","",""};
	EXPECT_EQ(buff, correct);
	buff.clear();
	correct.clear();

	std::stringstream test03_data("\",\",\",\",\"\"");
	CCSVReader test3(test03_data);
	EXPECT_TRUE(test3.ReadRow(buff));
	correct = {",",",",""};
	EXPECT_EQ(buff, correct);
	buff.clear();
	correct.clear();


	std::stringstream test04_data("\"I call our world Flatland,\x0a"
                       "not because we call it so,\x0a"
                       "but to make its nature clearer\x0a"
                       "to you, my happy readers,\x0a"
                       "who are privileged to live in Space.\"");
	CCSVReader test4(test04_data);
	EXPECT_TRUE(test4.ReadRow(buff));
	correct = {"I call our world Flatland,\x0a"
                       "not because we call it so,\x0a"
                       "but to make its nature clearer\x0a"
                       "to you, my happy readers,\x0a"
                       "who are privileged to live in Space."};
	EXPECT_EQ(buff, correct);
	buff.clear();
	correct.clear();


	std::stringstream test05_data("\"\"\"a,b\"\"\",,\" \"\"\"\" \",\"\"\"\"\" \",\" \"\"\"\"\",\"\"\"\"\"\"");
	CCSVReader test5(test05_data);
	EXPECT_TRUE(test5.ReadRow(buff));
	correct = {"\"a,b\"",""," \"\" ","\"\" "," \"\"","\"\""};
	EXPECT_EQ(buff, correct);
	buff.clear();
	correct.clear();


	std::stringstream test06_data("\" a, b ,c \", a b  c,");
	CCSVReader test6(test06_data);
	EXPECT_TRUE(test6.ReadRow(buff));
	correct = {" a, b ,c ","a b  c",""};
	EXPECT_EQ(buff, correct);
	buff.clear();
	correct.clear();

	std::stringstream test07_data("\" \"\" \" \" \"\" \"");
	CCSVReader test7(test07_data);
	EXPECT_TRUE(test7.ReadRow(buff));
	correct = {" \" \" \" \" "};
	EXPECT_EQ(buff, correct);
	buff.clear();
	correct.clear();
/*
	std::stringstream test08_data("\" abc\"                                             "
                       "                                                     "
                       "                                                     "
                       "                                                     "
                       "                                                     "
                       "                                                     "
                       "                                                     "
                       "                                                     "
                       "                                          \", \"123\"");
	std::stringstream test09_data("");
	std::stringstream test10_data("a\x0a");
	std::stringstream test11_data("1,2 ,3,4\x0a");
	std::stringstream test12_data("\x0a\x0a\x0a\x0a");
	std::stringstream test12b_data("\x0a\x0a\x0a\x0a");
	std::stringstream test13_data("\"abc\"");
	std::stringstream test14_data("1, 2, 3,\x0a\x0d\x0a  \"4\", \x0d,");
	std::stringstream test15_data("1, 2, 3,\x0a\x0d\x0a  \"4\", \x0d\"\"");
	std::stringstream test16_data("\"1\",\"2\",\" 3 ");
	std::stringstream test16b_data("\"1\",\"2\",\" 3 ");
	std::stringstream test17_data(" a\0b\0c ");
	std::stringstream test18_data("12345678901234567890123456789012");
	std::stringstream test19_data("  , \" ,");

	std::stringstream test20_data("'''a;b''';;' '''' ';''''' ';' ''''';''''''");
	CCSVReader test20(test20_data);
	EXPECT_TRUE(test20.ReadRow(buff));
	correct = {"'a;b'",""," '' ","'' "," ''","''"};
	EXPECT_EQ(buff, correct);
	buff.clear();
	correct.clear();
*/
}

TEST(CSVTest, libwritetests){
	std::vector< std::string > buff;

	std::stringstream test01_data("");
	CCSVWriter test1(test01_data);
	buff = {"1","2","3","4","5"};
	EXPECT_TRUE(test1.WriteRow(buff));
	EXPECT_EQ(test01_data.str(), "\"1\",\"2\",\"3\",\"4\",\"5\"");
	buff.clear();

	std::stringstream test02_data("");
	CCSVWriter test2(test02_data);
	buff = {"","","","","",""};
	EXPECT_TRUE(test2.WriteRow(buff));
	EXPECT_EQ(test02_data.str(), ",,,,,");
	buff.clear();

	std::stringstream test03_data("");
	CCSVWriter test3(test03_data);
	buff = {",",",",""};
	EXPECT_TRUE(test3.WriteRow(buff));
	EXPECT_EQ(test03_data.str(), "\",\",\",\",");
	buff.clear();


	std::stringstream test04_data("");
	CCSVWriter test4(test04_data);
	buff = {"I call our world Flatland,\x0a"
                       "not because we call it so,\x0a"
                       "but to make its nature clearer\x0a"
                       "to you, my happy readers,\x0a"
                       "who are privileged to live in Space."};
	EXPECT_TRUE(test4.WriteRow(buff));
	EXPECT_EQ(test04_data.str(), "\"I call our world Flatland,\x0a"
                       "not because we call it so,\x0a"
                       "but to make its nature clearer\x0a"
                       "to you, my happy readers,\x0a"
                       "who are privileged to live in Space.\"");
	buff.clear();


	std::stringstream test05_data("");
	CCSVWriter test5(test05_data);
	buff = {"\"a,b\"",""," \"\" ","\"\" "," \"\"","\"\""};
	EXPECT_TRUE(test5.WriteRow(buff));
	EXPECT_EQ(test05_data.str(), "\"\"\"a,b\"\"\",,\" \"\"\"\" \",\"\"\"\"\" \",\" \"\"\"\"\",\"\"\"\"\"\"");
	buff.clear();


	std::stringstream test06_data("");
	CCSVWriter test6(test06_data);
	buff = {" a, b ,c ","a b  c",""};
	EXPECT_TRUE(test6.WriteRow(buff));
	EXPECT_EQ(test06_data.str(), "\" a, b ,c \",\"a b  c\",");
	buff.clear();

	std::stringstream test07_data("");
	CCSVWriter test7(test07_data);
	buff = {" \" \" \" \" "};
	EXPECT_TRUE(test7.WriteRow(buff));
	EXPECT_EQ(test07_data.str(), "\" \"\" \"\" \"\" \"\" \"");
	buff.clear();
}

TEST(CSVTest, CSVRWConversionTest){
	std::stringstream testreadstream("\"Name, five\",\" Height\",\"Age\"\n\"Alex\",\"Like 10\",\"93\"");
	CCSVReader testobj1(testreadstream);
	std::vector< std::string > cow;
	EXPECT_TRUE(testobj1.ReadRow(cow));

	std::stringstream testwritestream("");
	CCSVWriter testobj2(testwritestream);
	EXPECT_TRUE(testobj2.WriteRow(cow));
	EXPECT_EQ(testwritestream.str(), "\"Name, five\",\" Height\",\"Age\"");
}

TEST(CSVTest, CSVWRConversionTest){
	std::stringstream testwritestream("");
	CCSVWriter testobj2(testwritestream);
	std::vector< std::string > row = {"Name, five", "Height", "Age"};
	EXPECT_TRUE(testobj2.WriteRow(row));

	CCSVReader testobj1(testwritestream);
	std::vector< std::string > cow;
	EXPECT_TRUE(testobj1.ReadRow(cow));

	EXPECT_EQ(cow,row);
}


