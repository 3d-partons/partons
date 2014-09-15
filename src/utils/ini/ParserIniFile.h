#ifndef PARSER_INI_FILE_H
#define PARSER_INI_FILE_H

/**
 * @author Adrien KIELB
 * @author <contributor> Bryan BERTHOU
 * Last update : 10 September 2014
 */

#include <map>
#include <string>
#include <utility>
#include <vector>

typedef std::vector<std::pair<std::string, std::string> > vectorIni;
typedef std::map<std::string, std::string> mapIni;

class ParserIniFile {
    std::map<std::string, std::string> lol;

public:

    ParserIniFile();
    vectorIni parse_ini_file(const std::string & file);
    vectorIni parse_ini_file_where_section(const std::string & file,
            const std::string section);

private:
    static std::string EMPTY;

    bool sectionFound;
    bool commentsFound;

    vectorIni parserCore(const std::string & file, const std::string section);

    void analyse(std::string line, vectorIni * parse, int numLine,
            std::string section);

    bool is_emptyString(std::string* chaine);

    // Remove leading and trailing spaces from a string
    void trim(std::string& str, const std::string& whitespace = " \t");
};

#endif /* PARSER_INI_FILE_H */
