#include "Attributs.h"

#include <iostream>
#include <iterator>

#include "../../stringUtils/StringUtils.h"
//#include "../../stringUtils/StringUtils2.h"

//#include "../../stringUtils/StringUtils2.h"

//#include "../../stringUtils/StringUtils2.h"

//#include "../../stringUtils/StringUtils2.h"

//#include "../../stringUtils/StringUtils2.h"

void Attributs::insertAtt(std::pair<std::string, std::string> p) {
    if (verifInsert(p))
        sesAtt.push_back(p);
}

void Attributs::affiche() {
    if (!sesAtt.empty()) {
        for (m_it = sesAtt.begin(); m_it != sesAtt.end(); m_it++) {
            std::cout << "| " << m_it->first.c_str() << " => "
                    << m_it->second.c_str() << " ";
        }
    } else
        std::cout << "| EMPTY";
}

std::vector<std::pair<std::string, std::string> > Attributs::getAtt() {
    return sesAtt;
}

bool Attributs::verifInsert(std::pair<std::string, std::string> p) {
    std::string s = p.first;
    for (m_it = sesAtt.begin(); m_it != sesAtt.end(); m_it++) {
        if (m_it->first.compare(s) == 0)
            return false;
    }
    return true;
}

std::string Attributs::getStringValueOf(std::string key) {
    for (m_it = sesAtt.begin(); m_it != sesAtt.end(); m_it++) {
        if (m_it->first.compare(key) == 0) {
            return m_it->second;
        }
    }
    return "none";
}

int Attributs::getIntValueOf(std::string key) {
    int i = 0;
    for (m_it = sesAtt.begin(); m_it != sesAtt.end(); m_it++) {
        if (m_it->first.compare(key) == 0) {
            if (isInt(m_it->second)) {
                std::string s = m_it->second;
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

double Attributs::getDoubleValueOf(std::string key) {
    double d = 0.;
    for (m_it = sesAtt.begin(); m_it != sesAtt.end(); m_it++) {
        if (m_it->first.compare(key) == 0) {

//            d = StringUtils2::getInstance()->convertFromStringToDouble(
//                    m_it->second);

            d = StringUtils::fromStringToDouble(m_it->second);

//            std::istringstream istr(m_it->second);
//            istr >> d;

// StringUtils2::getInstance()->fromString<double>(d, std::string(m_it->second), std::dec);
        }
    }
    return d;
}

bool Attributs::isInt(std::string s) {
    if (s.find_first_of(
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ &�\"'(-�_��)=~#{[|`^@]}/*-+.!:;,?�")
            == -1) {
        return true;
    }
    return false;
}
