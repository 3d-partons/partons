#include "GenericType.h"

GenericType::GenericType(const GenericType &other) {
    m_stream.clear();
    m_stream << other.m_stream.str();
}

GenericType::~GenericType() {
    // TODO Auto-generated destructor stub
}

GenericType::GenericType(const double value) {
    m_stream << value;
}

GenericType::GenericType(const int value) {
    m_stream << value;
}

GenericType::GenericType(const std::string &value) {
    m_stream << value;
}

double GenericType::getDouble() {
    //To avoid empty m_stream after conversion with >> operator
    std::stringstream sstream;
    sstream << m_stream.rdbuf();
    double d = 0.;

    if ((sstream >> d).fail()) {
        //TODO throw exception
    }
    return d;
}

int GenericType::getInt() {
    //To avoid empty m_stream after conversion with >> operator
    std::stringstream sstream;
    sstream << m_stream.rdbuf();
    int i = 0;
    if ((sstream >> i).fail()) {
        //TODO throw exception
    }
    return i;
}

std::string GenericType::getString() {
    return m_stream.str();
}
