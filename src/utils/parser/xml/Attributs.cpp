#include "Attributs.h"

#include <sstream>
#include <stdexcept>
#include <utility>

Attributs::Attributs() {

}

Attributs::~Attributs() {

}

std::string Attributs::getStringValueOf(const std::string &key) {
    std::map<std::string, std::string>::const_iterator it;
    it = m_attributes.find(key);

    if (it != m_attributes.end()) {
        return it->second;
    } else {
        throw std::runtime_error(
                "[Attributs::getStringValueOf] Enable to find key = " + key);
    }
}

//TODO tester le cast
int Attributs::getIntValueOf(const std::string &key) {
    int i = 0;
    std::istringstream istr(getStringValueOf(key));
    istr >> i;
    return i;
}

//TODO tester le cast
double Attributs::getDoubleValueOf(const std::string &key) {
    double d = 0.;
    std::istringstream istr(getStringValueOf(key));
    istr >> d;
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

void Attributs::add(const std::string& key, const std::string& value) {
    m_attributes.insert(std::make_pair(key, value));
}

std::string Attributs::toString() {
    std::string result = "";

    for (std::map<std::string, std::string>::const_iterator it =
            m_attributes.begin(); it != m_attributes.end(); it++) {
        result += (it->first) + " = " + (it->second) + '\n';
    }

    return result;
}

bool Attributs::isAvailable(const std::string& key) const {
    bool result = false;

    std::map<std::string, std::string>::const_iterator it = m_attributes.find(
            key);
    if (it != m_attributes.end()) {
        result = true;
    }

    return result;
}

bool Attributs::getBooleanValueOf(const std::string& key) {
    bool b = false;

    std::stringstream sstream;
    sstream << getStringValueOf(key);

    // if conversion failed then print an exception
    if ((sstream >> b).fail()) {
        throw std::runtime_error(
                "[Attributs::getBooleanValueOf] cast from std::string to bool failed ! ");
    }

    return b;
}
