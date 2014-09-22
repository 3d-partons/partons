#include "IniFileParser.h"

#include <stddef.h>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

#include "../fileUtils/FileUtils.h"
#include "../stringUtils/Formatter.h"
#include "../stringUtils/StringUtils.h"

std::string IniFileParser::SECTION_KEY_CONCATENATOR = "=";

IniFileParser::IniFileParser(const std::string & configFilePath) :
        m_configFilePath(configFilePath) {

}

IniFileParser::~IniFileParser() {

}

void IniFileParser::parse() {
    if (FileUtils::isReadable(m_configFilePath)) {
        // retrieve each line of the file
        std::vector<std::string> fileLines = FileUtils::readByLine(
                m_configFilePath);

        // create an empty section name
        std::string sectionName = StringUtils::EMPTY;

        // analyse each line of the file
        for (unsigned int i = 0; i != fileLines.size(); i++) {
            analyse(fileLines[i], sectionName, i + 1);
        }
    } else {
        // if the input file is unreadable then throw an exception
        throw std::runtime_error(
                "[IniFileParser] Unable to open config file: "
                        + m_configFilePath);
    }
}

void IniFileParser::analyse(std::string & fileLine, std::string & sectionName,
        const unsigned int fileLineNumber) {

    // remove useless whitespace
    StringUtils::trim(fileLine);

    // line is empty
    if (fileLine.length() == 0) {
        // do nothing
    }
    // start with comment
    else if (fileLine.find_first_of(';') == 0
            || fileLine.find_first_of('#') == 0) {
        // do nothing
    }
    // start with open square bracket (section)
    else if (fileLine.find_first_of('[') == 0) {
        analyseSection(fileLine, sectionName, fileLineNumber);
    }
    // contains equal string
    else if (StringUtils::contains(fileLine, "=")) {
        analyseValue(fileLine, sectionName);
    }
    // else the equal character is missing
    else {
        std::cerr
                << "[IniFileParser::analyse] ERROR in the ini file '=' character is missing, line : "
                << fileLineNumber << std::endl;
    }
}

void IniFileParser::analyseSection(const std::string & fileLine,
        std::string & sectionName, const unsigned int fileLineNumber) {

    // try to retrieve the position in the string of the close square bracket character
    size_t closeSquareBracketCharPos = fileLine.find_last_of(']');

    // if there is no close square bracket character then print an error
    if (closeSquareBracketCharPos == std::string::npos) {
        std::cerr
                << "[IniFileParser::analyseSection] ERROR in the ini file ']' character is missing, line : "
                << fileLineNumber << std::endl;
    }
    // if close square bracket character is at the end of the string then retrieve the section name
    else if (closeSquareBracketCharPos != fileLine.length() - 2) {
        sectionName = fileLine.substr(1, closeSquareBracketCharPos - 2);
    }
    // else it's misplaced and print an error
    else {
        std::cerr
                << "[IniFileParser::analyseSection] ERROR in the ini file ']' character is at wrong place, line : "
                << fileLineNumber << std::endl;
    }
}

void IniFileParser::analyseValue(const std::string & fileLine,
        const std::string & sectionName) {

    // retrieve equal character position in the string line
    size_t equalCharPos = fileLine.find_first_of('=');
    // extract left part for the key
    std::string key = fileLine.substr(0, equalCharPos);
    // extract right part for the value
    std::string value = fileLine.substr(equalCharPos + 1, fileLine.length());

    // remove surrounding whitespace
    StringUtils::trim(key);
    StringUtils::trim(value);

    // add key and value in the map
    m_values.insert(
            std::pair<std::string, std::string>(makeKey(sectionName, key),
                    value));
}

std::string IniFileParser::makeKey(const std::string & sectionName,
        const std::string & key) {
    // if the section's name is empty then returns only the key
    // else returns the section name concatenated with the value of the key
    return (sectionName == StringUtils::EMPTY) ?
            key : Formatter() << sectionName << SECTION_KEY_CONCATENATOR << key;
}

//TODO voir à ajouter en parametre la valeur de retour par défaut dans le cas ou EMPTY est une valeur possible
std::string IniFileParser::getString(const std::string & section,
        const std::string & key) {
    m_it = m_values.find(makeKey(section, key));
    return (m_it != m_values.end()) ? m_it->second : StringUtils::EMPTY;
}

std::map<std::string, std::string> IniFileParser::getValues() {
    return m_values;
}
