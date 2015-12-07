/*
 * XMLParser.h
 *
 *  Created on: Jul 6, 2015
 *      Author: Bryan BERTHOU
 */

#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <stddef.h>
#include <string>
#include <vector>

class Attributs;

class XMLParser {
public:
    XMLParser();
    virtual ~XMLParser();

    void analyse(const std::string & filePath);

    void loop(size_t startIndex = 0);

    virtual void startElement(const std::string &elementName, Attributs attributes,
            const std::string &elementData) = 0;

    virtual void endElement(const std::string &elementName) = 0;

private:
    static const std::string EMPTY_STRING;
    static const char TAB_CHARACTER;
    static const char ATTRIBUTES_SEPARATOR_CHARACTER;
    static const char ATTRIBUTE_KEY_VALUE_SEPARATOR_CHARACTER;
    static const std::string XML_VERSION_START_TAG_NAME;
    static const std::string XML_VERSION_END_TAG_NAME;
    static const std::string XML_COMMENT_START_TAG_NAME;
    static const std::string XML_COMMENT_END_TAG_NAME;
    static const std::string XML_START_ELEMENT_TAG_NAME;
    static const std::string XML_END_ELEMENT_TAG_NAME;

    std::string m_bufferedFile;

    size_t findIndexOf(const std::string &string, size_t startIndex);

    void parseNode(size_t startIndex);

    void parseAttributesFromXMLElement(Attributs &attributs, size_t startIndex);
};

#endif /* XML_PARSER_H */
