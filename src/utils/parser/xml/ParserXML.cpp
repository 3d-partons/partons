#include "ParserXML.h"

#include <stddef.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>

ParserXML::ParserXML() {
}

ParserXML::~ParserXML() {

}

std::multimap<std::string, std::pair<Attributs, std::string> > ParserXML::analyse(
        const std::string & file) {
    std::ifstream fichierLog(file.c_str());
    if (fichierLog) {
        std::string line;
        while (std::getline(fichierLog, line)) {
            m_total += " " + line;
        }
        loop(m_total);
    } else {
        std::cerr << "Erreur dans l'ouverture du fichier = " + file
                << std::endl;
        exit(1);
    }
    return fileBuffer;
}

void ParserXML::loop(std::string total) {
    size_t indexPrepro = total.find("<?");
    size_t indexComment = total.find("<!");
    if (indexPrepro == -1 && indexComment == -1) {
        size_t index = total.find_first_of("<");
        if (index != -1) {
            size_t finBalise = total.find_first_of(">");
            bool vide = false;
            if (total.at(finBalise - 1) == '/')
                vide = true;
            size_t index2 =
                    total.find_first_not_of(
                            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_",
                            index + 1);
            Attributs att;
            if (finBalise != index2 && index2 != -1) {
                std::string subString = total.substr(index2 + 1,
                        finBalise - index2 - 1);
                catchparam(&att, &subString);
            }
            if (index2 != -1) {
                std::string sub = total.substr(index + 1, index2 - index - 1);
                if (!vide) {
                    std::string baliseO = "<"
                            + total.substr(index + 1, finBalise - index - 1)
                            + ">";
                    std::string balise = "</" + sub + ">";
                    size_t pos = total.find(balise);
                    if (pos != -1) {
                        std::string value = total.substr(finBalise + 1,
                                pos - finBalise - 1);
                        if (value.empty())
                            value = "EMPTY";
                        startElement(sub, att, value);
                        fileBuffer.insert(
                                std::make_pair(sub,
                                        std::make_pair(att, value)));
                        value = total.replace(total.find(baliseO),
                                baliseO.length(), "");
                        value = value.replace(value.find(balise),
                                balise.length(), "");
                        loop(value);
                    }
                } else {
                    std::string baliseO = "<"
                            + total.substr(index + 1, finBalise - index - 1)
                            + "/>";
                    std::string value = "EMPTY";
                    fileBuffer.insert(
                            std::make_pair(sub, std::make_pair(att, value)));
                    value = total.replace(index, baliseO.length(), "");
                    loop(value);
                }
            }
        }
    } else {
        size_t OSEF;
        bool prepro = false;
        if (indexPrepro != -1) {
            OSEF = indexPrepro;
            prepro = true;
        } else
            OSEF = indexComment;
        size_t finOSEF = total.find_first_of(">", OSEF);
        if (prepro) {
            size_t finxmlname = total.find_first_of(" ", OSEF);
            size_t debutxmlname = total.find_first_not_of("?", OSEF + 1);
            std::string xmlname = total.substr(debutxmlname,
                    finxmlname - debutxmlname);
            Attributs att;
            std::string line = total.substr(finxmlname,
                    finOSEF - finxmlname - 1);
            catchparam(&att, &line);
            std::string value = "EMPTY";
            fileBuffer.insert(
                    std::make_pair(xmlname, std::make_pair(att, value)));
        } else {
            std::string name = "COMMENT";
            std::string value = total.substr(OSEF + 4, finOSEF - OSEF - 6);
            Attributs att;
            fileBuffer.insert(std::make_pair(name, std::make_pair(att, value)));
        }
        std::string valueRetour = total.replace(OSEF, finOSEF - OSEF + 1, "");
        loop(valueRetour);
    }
}

void ParserXML::catchparam(Attributs * att, std::string * line) {
    size_t index1 = line->find_first_not_of(" ");
    if (index1 != -1 && line->at(index1) != '/') {
        size_t index1copy = index1;
        size_t index2 = line->find_first_of("=");
        std::string key = line->substr(index1, index2 - index1);
        index1 = line->find_first_of("\"");
        index2 = line->find_first_of("\"", index1 + 1);
        std::string value = line->substr(index1 + 1, index2 - index1 - 1);
        att->insertAtt(std::make_pair(key, value));
        line->replace(index1copy, index2 + 1, "");
        catchparam(att, line);
    }
}

void ParserXML::printMap() {
    std::multimap<std::string, std::pair<Attributs, std::string> >::iterator it;
    for (it = fileBuffer.begin(); it != fileBuffer.end(); it++) {
        std::cout << it->first + " | " + it->second.second + " ";
        it->second.first.affiche();
        std::cout << std::endl;
    }
}

std::multimap<std::string, std::pair<Attributs, std::string> > ParserXML::getMap() {
    return fileBuffer;
}

std::vector<std::pair<std::string, std::string> > ParserXML::getAttOfAll(
        std::string strSearch) {
    std::vector<std::pair<std::string, std::string> > v;
    std::multimap<std::string, std::pair<Attributs, std::string> >::iterator it;
    for (it = fileBuffer.begin(); it != fileBuffer.end(); it++) {
        if (it->first.compare(strSearch) == 0
                && it->first.length() == strSearch.length()) {
            std::vector<std::pair<std::string, std::string> >::iterator it2;
            std::vector<std::pair<std::string, std::string> > att =
                    it->second.first.getAtt();
            for (it2 = att.begin(); it2 != att.end(); it2++) {
                v.push_back(std::make_pair(it2->first, it2->second));
            }
        }
    }
    return v;
}

std::vector<Attributs> ParserXML::getAttOf(std::string strSearch) {
    std::vector<Attributs> v;
    std::multimap<std::string, std::pair<Attributs, std::string> >::iterator it;
    for (it = fileBuffer.begin(); it != fileBuffer.end(); it++) {
        if (it->first.compare(strSearch) == 0
                && it->first.length() == strSearch.length()) {
            v.push_back(it->second.first);
        }
    }
    return v;
}

Attributs ParserXML::getAttOf(std::string strSearch, int stop) {
    Attributs v;
    std::multimap<std::string, std::pair<Attributs, std::string> >::iterator it;
    for (it = fileBuffer.begin(); it != fileBuffer.end(); it++) {
        if (it->first.compare(strSearch) == 0
                && it->first.length() == strSearch.length()) {
            int loop = stop - 1;
            for (int i = 0; i < loop; i++)
                it++;
            if (it->first.compare(strSearch) == 0
                    && it->first.length() == strSearch.length()) {
                v = it->second.first;
                return v;
            }
        }
    }
    return v;
}

std::vector<std::string> ParserXML::getValueOf(std::string strSearch) {
    std::vector<std::string> v;
    std::multimap<std::string, std::pair<Attributs, std::string> >::iterator it;
    for (it = fileBuffer.begin(); it != fileBuffer.end(); it++) {
        if (it->first.compare(strSearch) == 0
                && it->first.length() == strSearch.length()) {
            v.push_back(it->second.second);
        }
    }
    return v;
}

std::string ParserXML::getStringValueOf(std::string strSearch, int stop) {
    std::string v;
    std::multimap<std::string, std::pair<Attributs, std::string> >::iterator it;
    for (it = fileBuffer.begin(); it != fileBuffer.end(); it++) {
        if (it->first.compare(strSearch) != -1
                && it->first.length() == strSearch.length()) {
            int loop = stop - 1;
            for (int i = 0; i < loop; i++)
                it++;
            if (it->first.compare(strSearch) == 0
                    && it->first.length() == strSearch.length()) {
                v = it->second.second;
                return v;
            }
        }
    }
    return v;
}

std::vector<std::string> ParserXML::getNameWhichContains(
        std::string strSearch) {
    std::vector<std::string> v;
    std::multimap<std::string, std::pair<Attributs, std::string> >::iterator it;
    for (it = fileBuffer.begin(); it != fileBuffer.end(); it++) {
        if (it->first.find(strSearch) != -1) {
            v.push_back(it->first);
        }
    }
    return v;
}

int ParserXML::getIntValueOf(std::string key) {
    int i = 0;
    std::multimap<std::string, std::pair<Attributs, std::string> >::iterator it;
    for (it = fileBuffer.begin(); it != fileBuffer.end(); it++) {
        if (it->first.compare(key) == 0) {
            if (isInt(it->second.second)) {
                std::string s = it->second.second;
                int loop = 1;
                for (int pos = (int) s.size() - 1; pos >= 0;
                        pos--, loop *= 10) {
                    i += ((char) s[pos] - 48) * loop;
                }
                return i;
            }
        }
    }
    return i;
}

double ParserXML::getDoubleValueOf(std::string key) {
    double i = 0.;
    std::multimap<std::string, std::pair<Attributs, std::string> >::iterator it;
    for (it = fileBuffer.begin(); it != fileBuffer.end(); it++) {
        if (it->first.compare(key) == 0) {
            if (isInt(it->second.second)) {
                std::string s = it->second.second;
                int loop = 1;
                for (int pos = (int) s.size() - 1; pos >= 0;
                        pos--, loop *= 10) {
                    i += ((char) s[pos] - 48) * loop;
                }
                return i;
            }
        }
    }
    return i;
}

bool ParserXML::isInt(std::string s) {
    if (s.find_first_of(
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ &�\"'(-�_��)=~#{[|`^@]}/*-+.!:;,?�")
            == -1) {
        return true;
    }
    return false;
}
