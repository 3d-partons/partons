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
 * @brief A complex object to handle different simple type (like string, int, double, bool ...) into a unique stream.
 * It used to provide a way to handle generic type.
 * Then there is a method for each simple type that you want to get back.
 */

#include <sstream>
#include <string>

//TODO add missing type converter
class GenericType {
public:

    /**
     * Construct a GenericType object from any simple type
     * @param value
     */
    template<class T>
    GenericType(T value) {
        m_stream << value;
    }

    /**
     * Default destructor
     */
    virtual ~GenericType();

    /**
     * Copy constructor
     * @param other
     */
    GenericType(const GenericType &other);

    /**
     * Convert stream to double
     * @return double
     */
    double toDouble();

    /**
     * Concert stream to integer
     * @return int
     */
    int toInt();

    /**
     * Convert stream to unsigned integer
     * @return unsigned int
     */
    unsigned int toUInt();

    /**
     * Convert stream to boolean
     *
     * @return bool
     */
    bool toBoolean();

    /**
     * Convert stream to string
     * @return std::string
     */
    std::string toString() const;

private:
    std::stringstream m_stream; ///< stream to store serialized simple type

    ////////////////////////////////////////////////////////////
    /// Disallow comparisons between GenericTypes
    ////////////////////////////////////////////////////////////
    bool operator ==(const GenericType& right) const;
    bool operator !=(const GenericType& right) const;
};

#endif /* GENERICTYPE_H_ */
