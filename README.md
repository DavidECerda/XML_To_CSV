# XML to CSV Reader and Writer 
> #### A C++ Project
> > ###### By: David Cerda & Alexander Dunn 


### Goal 
Create wrappers for XML and CSV parser libraries, expat and csv, in order read and write XML and CSV files. 

### Implementation
The main classes are XML/CSV reader and writer. The XML and CSV reader pass handler functions to the parser to deal with different scenarios encountered during the parsing. To hold the parsed information from the fil, XML reader has a queue of a special struct called XMLEntity that holds entity information and CSV reader has a queue of vectors of strings to represent a row. XML writer can be given a XMLEntity to write and CSV writer can be given a vector of strings

> Sources: 
>>Code here and there from Nitta, tests from Nitta, one loop in CXMLReader::start_elemnt from stackoverflow (url in the comment)
