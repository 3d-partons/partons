#include "ParserIniFile.h"

#include <stddef.h>
#include <cstdlib>
#include <fstream>
#include <iostream>

std::string ParserIniFile::EMPTY = "";

ParserIniFile::ParserIniFile() {
    commentsFound = false;
    sectionFound = false;
}

vectorIni ParserIniFile::parse_ini_file(const std::string & file) {
    return parserCore(file, EMPTY);
}

vectorIni ParserIniFile::parse_ini_file_where_section(const std::string & file,
        const std::string section) {
    return parserCore(file, section);
}

vectorIni ParserIniFile::parserCore(const std::string & file,
        const std::string section) {
    vectorIni parse;
    std::ifstream iniFile(file.c_str(), std::ios::in);
    if (iniFile) {
        std::string line;
        int numLine = 1;
        while (std::getline(iniFile, line)) {
            analyse(line, &parse, numLine, section);
            numLine++;
        }
        iniFile.close();
    } else {
        std::cout << "Erreur dans l'ouverture du fichier\n";
        //system("pause");
        exit(1);
    }
    return parse;
}

//TODO verifier la suppression des espaces avant et après la chaine
void ParserIniFile::analyse(std::string line, vectorIni* parse, int numLine,
        std::string section) {
    if (!line.empty()) {
        size_t sectionPos = line.find("[");
        size_t startChar = line.find_first_not_of(" ");
        size_t length = line.length();
        size_t commentsPos = line.find("#");
        size_t comments2Pos = line.find(";");
        size_t comments3Pos = line.find("//");

        if (commentsPos == startChar || comments2Pos == startChar
                || comments3Pos == startChar)
            commentsFound = true;
        else
            commentsFound = false;

        if (sectionPos == startChar && section != EMPTY) {
            size_t sectionEndPos = line.find("]");
            std::string sectionName = line.substr(sectionPos + 1,
                    sectionEndPos - sectionPos - 1);
            if (sectionName == section)
                sectionFound = true;
            else
                sectionFound = false;

        }
        if ((sectionPos == -1 && sectionFound && !commentsFound)
                || (sectionPos == -1 && section == EMPTY && !commentsFound)) {
            size_t delimitor = line.find("=");
            size_t delimitor2 = line.find(":");
            if (delimitor != -1) {
                std::string key = line.substr(startChar, delimitor - startChar);
                std::string value = line.substr(delimitor + 1,
                        length - delimitor);
                trim(key);
                trim(value);
                parse->push_back(std::make_pair(key, value));
            } else if (delimitor2 != -1) {
                std::string key = line.substr(startChar,
                        startChar - delimitor2 - 1);
                std::string value = line.substr(delimitor2 + 1,
                        length - delimitor2);
                parse->push_back(std::make_pair(key, value));
            } else
                std::cout << "Erreur ligne : " << numLine
                        << ", signe '=' manquant" << std::endl;
            //system("pause");
        }
    }
}

bool ParserIniFile::is_emptyString(std::string* chaine) {
    return (chaine == NULL || *chaine == EMPTY) ? true : false;
}

void ParserIniFile::trim(std::string& str, const std::string& whitespace) {
    size_t strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos) {
        str = ""; // no content
    } else {
        size_t strEnd = str.find_last_not_of(whitespace);
        size_t strRange = strEnd - strBegin + 1;

        str = str.substr(strBegin, strRange);
    }
}
