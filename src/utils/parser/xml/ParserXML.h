#ifndef PARSER_XML_H
#define PARSER_XML_H

/**
 * @file ParserXML.h
 * @author Adrien KIELB, Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 30 January 2014
 * @version 1.0
 *
 * @class ParserXML
 * @brief
 */

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Attributs.h"

class ParserXML {

protected:

    std::multimap<std::string, std::pair<Attributs, std::string> > fileBuffer;
    std::string m_total;

    void loop(std::string);
    void catchparam(Attributs *, std::string *);

public:

    ParserXML();

    virtual ~ParserXML();

    virtual void startElement(std::string nomBalise, Attributs att,
            std::string value) = 0;

    std::multimap<std::string, std::pair<Attributs, std::string> > analyse(
            const std::string & file);

    void printMap();

    std::multimap<std::string, std::pair<Attributs, std::string> > getMap();

    std::vector<std::pair<std::string, std::string> > getAttOfAll(std::string);
    std::vector<Attributs> getAttOf(std::string);
    Attributs getAttOf(std::string, int);
    std::vector<std::string> getValueOf(std::string);
    std::vector<std::string> getNameWhichContains(std::string);
    std::string getStringValueOf(std::string, int);
    std::string getStringValueOf(std::string);
    int getIntValueOf(std::string);
    double getDoubleValueOf(std::string);

    bool isInt(std::string);

};

#endif /* PARSER_XML_H */
