#ifndef XMLWRITER_H
#define XMLWRITER_H

#include "XMLEntity.h"
#include <stack>
#include <istream>
#include <algorithm>

class CXMLWriter{
    private:
        std::ostream &OutFile;
        std::stack <std::string> openElements;
        std::string sameElem;
        void LoadValues(const SXMLEntity &entity, const std::vector <char> &specialChar, std::vector<std::string> &values);
        void WriteStartElement(const std::vector<std::string> &values, const SXMLEntity &entity);
        void WriteCharData(const std::vector<char> &specialChar, std::vector<std::string> &values, const SXMLEntity &entity);
        void WriteCompleteElement(const std::vector<std::string> &values, const SXMLEntity &entity);
        bool WriteEndElement(const std::vector<std::string> &values, const SXMLEntity &entity);
    public:
        CXMLWriter(std::ostream &os);
        ~CXMLWriter();
        
        bool Flush();
        bool WriteEntity(const SXMLEntity &entity);
};

#endif
                                      