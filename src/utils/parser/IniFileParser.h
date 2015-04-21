#ifndef INI_FILE_PARSER
#define INI_FILE_PARSER

/**
 * @file IniFileParser.h
 * @version 2.0
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 18 September 2014
 * Last update : 19 September 2014
 *
 * @class IniFileParser
 * @brief
 */

#include <map>
#include <string>

class IniFileParser {
public:
    static std::string SECTION_KEY_CONCATENATOR;

    /**
     * Constructor
     *
     * @param configFilePath
     */
    IniFileParser();

    /**
     * Default destructor
     */
    virtual ~IniFileParser();

    void parse(const std::string & configFilePath);

    std::map<std::string, std::string> getValues();
    std::string getString(const std::string & key);
    std::string getString(const std::string & section, const std::string & key);

private:
    std::map<std::string, std::string> m_values;
    std::map<std::string, std::string>::iterator m_it;

    void analyse(std::string & fileLine, std::string & sectionName,
            const unsigned int fileLineNumber);
    void analyseSection(const std::string & fileLine, std::string & sectionName,
            const unsigned int fileLineNumber);
    void analyseValue(const std::string & fileLine,
            const std::string & sectionName);
    std::string makeKey(const std::string & sectionName,
            const std::string & key);
};

#endif /* INI_FILE_PARSER */
