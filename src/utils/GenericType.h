/*
 * GenericType.h
 *
 *  Created on: Jul 3, 2015
 *      Author: debian
 */

#ifndef GENERICTYPE_H_
#define GENERICTYPE_H_

#include <sstream>
#include <string>

/*
 *
 */
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
