#ifndef GENERIC_TYPE_H
#define GENERIC_TYPE_H

/**
 * @file GenericType.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 03 July 2014
 * @version 1.0
 *
 * @class GenericType
 *
 * @brief
 */

#include <sstream>
#include <string>

class GenericType {
public:
    /**
     * Copy constructor
     * @param other
     */
    GenericType(const GenericType &other);

    GenericType(const double value);
    GenericType(const int value);
    GenericType(const std::string &value);

    virtual ~GenericType();

    double getDouble();
    int getInt();
    std::string getString();

private:
    std::stringstream m_stream;

    ////////////////////////////////////////////////////////////
    /// Disallow comparisons between GenericTypes
    ///
    ////////////////////////////////////////////////////////////
    bool operator ==(const GenericType& right) const;
    bool operator !=(const GenericType& right) const;
};

#endif /* GENERICTYPE_H_ */
