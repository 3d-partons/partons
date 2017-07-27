#ifndef DEFAULT_XML_PARSER_H
#define DEFAULT_XML_PARSER_H

/**
 * @file DefaultXMLParser.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date April 01, 2016
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/parser/XMLParser.h>
#include <string>

#include "XMLParserI.h"

namespace PARTONS {

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
            ElemUtils::Parameters attributes, const std::string &elementData);

    virtual void emptyStartElement(const std::string &elementName,
            ElemUtils::Parameters attributes);

    virtual void endElement(const std::string &elementName);

private:
    bool m_isModuleNodePreviouslyCreated;
};

} /* namespace PARTONS */

#endif /* DEFAULT_XML_PARSER_H */
