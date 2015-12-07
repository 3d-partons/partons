#include "../../../../../include/partons/utils/parser/xml/XMLParser.h"

#include <algorithm>
#include <fstream> // needed
#include <iostream>
#include <iterator>
#include <stdexcept>

#include "../../../../../include/partons/utils/parser/xml/Attributs.h"

const std::string XMLParser::EMPTY_STRING = "";
const char XMLParser::TAB_CHARACTER = '\t';
const char XMLParser::ATTRIBUTE_KEY_VALUE_SEPARATOR_CHARACTER = '=';
const char XMLParser::ATTRIBUTES_SEPARATOR_CHARACTER = ' ';

const std::string XMLParser::XML_VERSION_START_TAG_NAME = "<?";
const std::string XMLParser::XML_VERSION_END_TAG_NAME = "?>";
const std::string XMLParser::XML_COMMENT_START_TAG_NAME = "<!--";
const std::string XMLParser::XML_COMMENT_END_TAG_NAME = "-->";
const std::string XMLParser::XML_START_ELEMENT_TAG_NAME = "<";
const std::string XMLParser::XML_END_ELEMENT_TAG_NAME = ">";

XMLParser::XMLParser() :
        m_bufferedFile(XMLParser::EMPTY_STRING) {
}

XMLParser::~XMLParser() {
}

void XMLParser::analyse(const std::string& filePath) {
    // try to open file
    std::ifstream file(filePath.c_str());
    // if success
    if (file) {
        // retrieve each file lines and concat it into one string
        std::string line;
        while (std::getline(file, line)) {
            m_bufferedFile += line;
        }

        // erase some extra string like tabs
        m_bufferedFile.erase(
                std::remove(m_bufferedFile.begin(), m_bufferedFile.end(),
                        XMLParser::TAB_CHARACTER), m_bufferedFile.end());

        // loop over previous buffered file to parse it
        loop();
    } else {
        // else cannot open file; throw exception
        throw std::runtime_error(
                "[XMLParser::analyse] Enable to open file : " + filePath);
    }

    // clear m_bufferedFile
    m_bufferedFile = EMPTY_STRING;
}

void XMLParser::loop(size_t startIndex) {
    //check if there is some xml version tag
    size_t xmlVersionStartIndex = m_bufferedFile.find(
            XML_VERSION_START_TAG_NAME, startIndex);

    if (xmlVersionStartIndex != std::string::npos) {
        // skip xml version node
        loop(findIndexOf(XML_VERSION_END_TAG_NAME, xmlVersionStartIndex));
    } else {
        //then check if there is some comments tag
        size_t xmlCommentStartIndex = m_bufferedFile.find(
                XML_COMMENT_START_TAG_NAME, startIndex);
        if (xmlCommentStartIndex != std::string::npos) {
            // skip comment node
            loop(findIndexOf(XML_COMMENT_END_TAG_NAME, xmlCommentStartIndex));
        } else {
            //then check if there is a node tag
            size_t xmlSartTagIndex = m_bufferedFile.find(
                    XML_START_ELEMENT_TAG_NAME, startIndex);
            if (xmlSartTagIndex != std::string::npos) {
                // parse node
                parseNode(xmlSartTagIndex);
            }
        }
    }
    //File without xml node
}

size_t XMLParser::findIndexOf(const std::string &string, size_t startIndex) {
    return m_bufferedFile.find(string, startIndex);
}

void XMLParser::parseNode(size_t startIndex) {

    if (m_bufferedFile.at(startIndex + 1) == '/') {
        // End element reach
        // retrieve index of the end element
        size_t xmlEndElementIndex = m_bufferedFile.find(
                XMLParser::XML_END_ELEMENT_TAG_NAME, startIndex);
        if (xmlEndElementIndex != std::string::npos) {
            // extract end element name
            endElement(
                    m_bufferedFile.substr(startIndex + 2,
                            xmlEndElementIndex - (startIndex + 2)));
        } else {
            throw std::runtime_error(
                    "[XMLParser::parseNode] XML bad file format; missing \">\"");
        }
    } else {
        // else it's a start element; check it

        size_t xmlEndTagIndex = m_bufferedFile.find(XML_END_ELEMENT_TAG_NAME,
                startIndex);

        if (xmlEndTagIndex != std::string::npos) {
            // It's a start element; by default with empty data;
            std::string nodeValue = XMLParser::EMPTY_STRING;

            if (m_bufferedFile.at(xmlEndTagIndex - 1) != '/') {
                // it's a element node with data
                //TODO retrieve data like <node> some data </node>
                // nodeValue = some data;
            }

            // retrieve element name
            size_t elementNameEndIndex =
                    m_bufferedFile.find_first_not_of(
                            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_",
                            startIndex + 1);
            // retrieve its attributes
            Attributs attributes;

            parseAttributesFromXMLElement(attributes, elementNameEndIndex);

            startElement(
                    m_bufferedFile.substr(startIndex + 1,
                            elementNameEndIndex - (startIndex + 1)), attributes,
                    nodeValue);

        } else {
            throw std::runtime_error(
                    "[XMLParser::parseNode] XML bad file format; missing \">\"");
        }
    }

    loop(startIndex + 1);
}

void XMLParser::parseAttributesFromXMLElement(Attributs &attributs,
        size_t startIndex) {
// retrieve first character position of key attribut
    size_t keyStringStartIndex = m_bufferedFile.find_first_not_of(
            XMLParser::ATTRIBUTES_SEPARATOR_CHARACTER, startIndex);
// if this character not represents an end element
    if (m_bufferedFile.at(keyStringStartIndex) != '/'
            && m_bufferedFile.at(keyStringStartIndex) != '>') {
        // try find the equal char separator
        size_t equalCharIndex = m_bufferedFile.find_first_of(
                XMLParser::ATTRIBUTE_KEY_VALUE_SEPARATOR_CHARACTER,
                keyStringStartIndex);
        if (equalCharIndex != std::string::npos) {
            // if found;
            // try to retrieve start index of value
            size_t valueStringStartIndex = m_bufferedFile.find_first_of('"',
                    equalCharIndex) + 1;
            // try to retrieve end index of value
            size_t valueStringEndIndex = m_bufferedFile.find_first_of('"',
                    valueStringStartIndex);

            if (valueStringStartIndex != std::string::npos
                    && valueStringEndIndex != std::string::npos) {
                // if value retrieve then extract key and value to add them to attributs
                attributs.add(
                        m_bufferedFile.substr(keyStringStartIndex,
                                equalCharIndex - keyStringStartIndex),
                        m_bufferedFile.substr(valueStringStartIndex,
                                valueStringEndIndex - valueStringStartIndex));

                // parse remaining attributes recursively
                parseAttributesFromXMLElement(attributs,
                        valueStringEndIndex + 1);
            }
        }
    }
}
