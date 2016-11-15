#ifndef DEFAULT_XML_PARSER_H
#define DEFAULT_XML_PARSER_H

/**
 * @file DefaultXMLParser.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date April 01, 2016
 * @version 1.0
 */

#include <ElementaryUtils/parser/XMLParser.h>
#include <string>

#include "XMLParserI.h"

/**
 * @class DefaultXMLParser
 *
 * @brief
 */
class DefaultXMLParser: public XMLParserI, public ElemUtils::XMLParser {
public:
    DefaultXMLParser();
    virtual ~DefaultXMLParser();

    virtual Scenario* parseScenario(Scenario* pScenario);

    virtual void startElement(const std::string &elementName,
            ElemUtils::XMLAttributs attributes, const std::string &elementData);

    virtual void endElement(const std::string &elementName);

private:
    bool m_isModuleNodePreviouslyCreated;
};

#endif /* DEFAULT_XML_PARSER_H */
