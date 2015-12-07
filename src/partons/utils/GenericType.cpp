#include "../../../include/partons/utils/GenericType.h"

#include <stdexcept>

GenericType::GenericType(const GenericType &other) {
    // clear current stream then copy it from other object
    m_stream.clear();
    m_stream << other.m_stream.str();
}

GenericType::~GenericType() {
}

double GenericType::toDouble() {
    //create a temporary object to avoid empty m_stream after conversion with >> operator ; for multiple use of the same GenericType object
    std::stringstream sstream;
    sstream << m_stream.rdbuf();
    double d = 0.;

    // if conversion failed then print an exception
    if ((sstream >> d).fail()) {
        throw std::runtime_error(
                "[GenericType::toDouble] cast from std::string to double failed ! ");
    }
    return d;
}

int GenericType::toInt() {
    //create a temporary object to avoid empty m_stream after conversion with >> operator ; for multiple use of the same GenericType object
    std::stringstream sstream;
    sstream << m_stream.rdbuf();
    int i = 0;

    // if conversion failed then print an exception
    if ((sstream >> i).fail()) {
        throw std::runtime_error(
                "[GenericType::toInt] cast from std::string to int failed ! ");
    }
    return i;
}

unsigned int GenericType::toUInt() {
    //create a temporary object to avoid empty m_stream after conversion with >> operator ; for multiple use of the same GenericType object
    std::stringstream sstream;
    sstream << m_stream.rdbuf();
    unsigned int ui = 0;

    // if conversion failed then print an exception
    if ((sstream >> ui).fail()) {
        throw std::runtime_error(
                "[GenericType::toUInt] cast from std::string to unsigned int failed ! ");
    }
    return ui;
}

std::string GenericType::toString() const {
    return m_stream.str();
}

bool GenericType::toBoolean() {
    //create a temporary object to avoid empty m_stream after conversion with >> operator ; for multiple use of the same GenericType object
    std::stringstream sstream;
    sstream << m_stream.rdbuf();
    bool b = false;

    // if conversion failed then print an exception
    if ((sstream >> b).fail()) {
        throw std::runtime_error(
                "[GenericType::toBoolean] cast from std::string to bool failed ! ");
    }
    return b;
}
