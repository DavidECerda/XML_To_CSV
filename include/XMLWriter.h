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
    public:
        CXMLWriter(std::ostream &os);
        ~CXMLWriter();
        
        bool Flush();
        bool WriteEntity(const SXMLEntity &entity);
};

#endif
                                      