#ifndef GENERIC_DATA_H
#define GENERIC_DATA_H

/**
 * @file GenericData.h
 * @author: Bryan BERTHOU (CEA Saclay)
 * @date 08 December 2014
 * @version 1.0
 */

#include <sstream>
#include <string>

class GenericData {
public:
    GenericData(const GenericData &other) {
        m_stream.clear();
        m_stream << other.m_stream.str();
    }

    GenericData(const double value) {
        m_stream << value;
    }

    GenericData(const int value) {
        m_stream << value;
    }

    double getDouble() {
        double d = 0.;
        m_stream >> d;
        return d;
    }

    int getInt() {
        int i = 0;
        m_stream >> i;
        return i;
    }

private:
    std::stringstream m_stream;
};

#endif /* GENERIC_DATA_H */
