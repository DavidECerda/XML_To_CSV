#ifndef XMLREADER_H
#define XMLREADER_H

#include "XMLEntity.h"
#include <expat.h>
#include <istream>
#include <queue>
#define BUFFER_SIZE 8192




class CXMLReader{
    private:
        std::istream &InFile;
        std::queue<struct SXMLEntity> ManyEntity;
        XML_Parser MyParser;
        void ThrowError();
        bool ReadSingleEntity(SXMLEntity &entity, bool skipcdata = false);
        int ParseFile(char buffer[BUFFER_SIZE]);
    protected:
        static void start_element(void *data, const char* elem, const char** att);
        static void end_element(void *data, const char* elem);
        static void handle_data(void *data, const char* stuff, int len);
    public:
        CXMLReader(std::istream &is);
        ~CXMLReader();
        
        bool End();
        bool ReadEntity(SXMLEntity &entity, bool skipcdata = false);
};

#endif
