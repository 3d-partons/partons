#ifndef PHYSICAL_TYPE_H
#define PHYSICAL_TYPE_H

/**
 * @file PhysicalType.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date January 20, 2016
 * @version 1.0
 */

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/thread/Packet.h>
#include <sstream>
#include <string>

#include "../../BaseObject.h"
#include "PhysicalUnit.h"

namespace PARTONS {

/**
 * @class PhysicalType
 *
 * @brief Value with unit.
 *
 * This class is a container for a value, unit and switch to mark if the value is initialized or not.
 */
template<class T>
class PhysicalType: public BaseObject {

public:

    /**
     * Default constructor.
     */
    PhysicalType() :
            BaseObject("PhysicalType"), m_initialized(false), m_value(), m_unit(
                    PhysicalUnit::UNDEFINED) {
    }

    /**
     * Default constructor with unit provided.
     */
    PhysicalType(PhysicalUnit::Type unit) :
            BaseObject("PhysicalType"), m_initialized(false), m_value(), m_unit(
                    unit) {
    }

//    PhysicalType(T value) :
//            BaseObject("PhysicalType"), m_initialized(true), m_value(value), m_unit(
//                    PHYSICAL_TYPE_NONE_UNIT) {
//    }

//    PhysicalType(const std::string &stringValue) :
//            BaseObject("PhysicalType"), m_initialized(false), m_unit(
//                    PHYSICAL_TYPE_NONE_UNIT) {
//        fromStdString(stringValue);
//    }

    /**
     * Assignment constructor.
     */
    PhysicalType(T value, PhysicalUnit::Type unit) :
            BaseObject("PhysicalType"), m_initialized(true), m_value(value), m_unit(
                    unit) {
    }

    /**
     * Assignment constructor.
     */
    PhysicalType(const std::string &stringValue, PhysicalUnit::Type unit) :
            BaseObject("PhysicalType"), m_initialized(true), m_unit(unit) {
        valueFromStdString(stringValue);
    }

    /**
     * Assignment constructor.
     */
    PhysicalType(const ElemUtils::GenericType& value, PhysicalUnit::Type unit) :
            BaseObject("PhysicalType"), m_initialized(true), m_unit(unit) {
        valueFromStdString(value.getString());
    }

    /**
     * Copy constructor.
     */
    PhysicalType(const PhysicalType &other) :
            BaseObject("PhysicalType"), m_initialized(other.m_initialized), m_value(
                    other.m_value), m_unit(other.m_unit) {

    }

    /**
     * Return string representing the value (only!).
     */
    std::string toStdString() const {
        std::stringstream sstream(m_value);
        return sstream.str();
    }

    /**
     * Return string representing the whole object.
     */
    std::string toString() const {

        ElemUtils::Formatter formatter;

        formatter << m_value;
        formatter << " [";
        formatter << PhysicalUnit(m_unit).toString();
        formatter << "]";

        return formatter.str();
    }

    /**
     * Serialize into given Packet.
     * @param packet Target Packet.
     */
    void serialize(ElemUtils::Packet &packet) const {
        packet << m_initialized << m_value << PhysicalUnit(m_unit);
    }

    /**
     * Retrieve data from given Packet.
     * @param packet Input Packet.
     */
    void unserialize(ElemUtils::Packet &packet) {

        packet >> m_initialized;
        packet >> m_value;

        PhysicalUnit type;
        packet >> type;
        m_unit = type;
    }

    /**
     * Assignment operator.
     */
    inline PhysicalType<T>& operator=(PhysicalType<T> const &rhs) {
        setValue(rhs.m_value);
        setUnit(rhs.m_unit);
        return *this;
    }

    /**
     * Assignment operator.
     */
    inline PhysicalType<T>& operator=(T const &rhs) {
        setValue(rhs);
        return *this;
    }

    /**
     * Operator to return value.
     */
    inline T operator()() {
        return m_value;
    }

    // ******************************************
    // **** GETTERS AND SETTERS *****************
    // ******************************************

    /**
     * Get value.
     */
    T getValue() const {
        return m_value;
    }

    /**
     * Set value. This function makes the object marked as initialized.
     */
    void setValue(T value) {
        m_value = value;
        m_initialized = true;
    }

    /**
     * Get unit.
     */
    PhysicalUnit::Type getUnit() const {
        return m_unit;
    }

    /**
     * Set unit.
     */
    void setUnit(PhysicalUnit::Type unit) {
        m_unit = unit;
    }

    /**
     * Check if initialized.
     */
    bool isInitialized() const {
        return m_initialized;
    }

    /**
     * Set as initialized.
     */
    void setInitialized(bool initialized) {
        m_initialized = initialized;
    }

private:

    /**
     * Set value from std::string.
     */
    void valueFromStdString(const std::string &stringValue) {

        //string stream
        std::stringstream sstream(stringValue);

        // if conversion failed then print an exception
        if ((sstream >> m_value).fail()) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "Cast from std::string to type<T> failed !");
        }
    }

    /**
     * True if value initialized.
     */
    bool m_initialized;

    /**
     * Value.
     */
    T m_value;

    /**
     * Unit.
     */
    PhysicalUnit::Type m_unit;
};

/**
 * Stream operator to serialize class into Packet. See also Kinematic::serialize().
 */
template<class T>
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        PhysicalType<T>& physicalType) {
    physicalType.serialize(packet);
    return packet;
}

/**
 * Stream operator to serialize class into Packet. See also Kinematic::serialize().
 */
template<class T>
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        const PhysicalType<T>& physicalType) {
    physicalType.serialize(packet);
    return packet;
}

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
template<class T>
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        PhysicalType<T>& physicalType) {
    physicalType.unserialize(packet);
    return packet;
}

/// Comparison operators

template<class T>
inline bool operator==(PhysicalType<T> const &lhs, PhysicalType<T> const &rhs) {
    return lhs.getValue() == rhs.getValue();
}
template<class T>
inline bool operator!=(PhysicalType<T> const &lhs, PhysicalType<T> const &rhs) {
    return lhs.getValue() != rhs.getValue();
}
template<class T>
inline bool operator<(PhysicalType<T> const &lhs, PhysicalType<T> const &rhs) {
    return lhs.getValue() < rhs.getValue();
}
template<class T>
inline bool operator>(PhysicalType<T> const &lhs, PhysicalType<T> const &rhs) {
    return lhs.getValue() > rhs.getValue();
}
template<class T>
inline bool operator<=(PhysicalType<T> const &lhs, PhysicalType<T> const &rhs) {
    return lhs.getValue() <= rhs.getValue();
}
template<class T>
inline bool operator>=(PhysicalType<T> const &lhs, PhysicalType<T> const &rhs) {
    return lhs.getValue() >= rhs.getValue();
}
//
//template<class T>
//inline bool operator==(PhysicalType<T> const &lhs, T const &rhs) {
//    return lhs.getValue() == rhs;
//}
//template<class T>
//inline bool operator!=(PhysicalType<T> const &lhs, T const &rhs) {
//    return lhs.getValue() != rhs;
//}
//template<class T>
//inline bool operator<(PhysicalType<T> const &lhs, T const &rhs) {
//    return lhs.getValue() < rhs;
//}
//template<class T>
//inline bool operator>(PhysicalType<T> const &lhs, T const &rhs) {
//    return lhs.getValue() > rhs;
//}
//template<class T>
//inline bool operator<=(PhysicalType<T> const &lhs, T const &rhs) {
//    return lhs.getValue() <= rhs;
//}
//template<class T>
//inline bool operator>=(PhysicalType<T> const &lhs, T const &rhs) {
//    return lhs.getValue() >= rhs;
//}
//
//template<class T>
//inline bool operator==(T const &lhs, PhysicalType<T> const &rhs) {
//    return lhs == rhs.getValue();
//}
//template<class T>
//inline bool operator!=(T const &lhs, PhysicalType<T> const &rhs) {
//    return lhs != rhs.getValue();
//}
//template<class T>
//inline bool operator<(T const &lhs, PhysicalType<T> const &rhs) {
//    return lhs < rhs.getValue();
//}
//template<class T>
//inline bool operator>(T const &lhs, PhysicalType<T> const &rhs) {
//    return lhs > rhs.getValue();
//}
//template<class T>
//inline bool operator<=(T const &lhs, PhysicalType<T> const &rhs) {
//    return lhs <= rhs.getValue();
//}
//template<class T>
//inline bool operator>=(T const &lhs, PhysicalType<T> const &rhs) {
//    return lhs >= rhs.getValue();
//}

/// Arithmetic operators

//TODO upgrade select unit
//TODO use setters for avoid problem with m_initialized value
//
//template<class T>
//inline PhysicalType<T> operator+(PhysicalType<T> const &lhs,
//        PhysicalType<T> const &rhs) {
//    return PhysicalType<T>(lhs.getValue() + rhs.getValue(), lhs.getUnit());
//}
//template<class T>
//inline PhysicalType<T> operator-(PhysicalType<T> const &lhs,
//        PhysicalType<T> const &rhs) {
//    return PhysicalType<T>(lhs.getValue() - rhs.getValue(), lhs.getUnit());
//}
//template<class T>
//inline PhysicalType<T> operator*(PhysicalType<T> const &lhs,
//        PhysicalType<T> const &rhs) {
//    return PhysicalType<T>(lhs.getValue() * rhs.getValue(), lhs.getUnit());
//}
//template<class T>
//inline PhysicalType<T> operator/(PhysicalType<T> const &lhs,
//        PhysicalType<T> const &rhs) {
//    return PhysicalType<T>(lhs.getValue() / rhs.getValue(), lhs.getUnit());
//}
//
//template<class T>
//inline PhysicalType<T> operator+(PhysicalType<T> const &lhs, T const &rhs) {
//    return PhysicalType<T>(lhs.getValue() + rhs, lhs.getUnit());
//}
//template<class T>
//inline PhysicalType<T> operator-(PhysicalType<T> const &lhs, T const &rhs) {
//    return PhysicalType<T>(lhs.getValue() - rhs, lhs.getUnit());
//}
//template<class T>
//inline PhysicalType<T> operator*(PhysicalType<T> const &lhs, T const &rhs) {
//    return PhysicalType<T>(lhs.getValue() * rhs, lhs.getUnit()); //WRONG! TODO
//}
//template<class T>
//inline PhysicalType<T> operator/(PhysicalType<T> const &lhs, T const &rhs) {
//    return PhysicalType<T>(lhs.getValue() / rhs, lhs.getUnit()); //WRONG! TODO
//}
//
//template<class T>
//inline PhysicalType<T> operator+(T const &lhs, PhysicalType<T> const &rhs) {
//    return PhysicalType<T>(lhs + rhs.getValue(), rhs.getUnit());
//}
//template<class T>
//inline PhysicalType<T> operator-(T const &lhs, PhysicalType<T> const &rhs) {
//    return PhysicalType<T>(lhs - rhs.getValue(), rhs.getUnit());
//}
//template<class T>
//inline PhysicalType<T> operator*(T const &lhs, PhysicalType<T> const &rhs) {
//    return PhysicalType<T>(lhs * rhs.getValue(), rhs.getUnit()); //WRONG! TODO
//}
//template<class T>
//inline PhysicalType<T> operator/(T const &lhs, PhysicalType<T> const &rhs) {
//    return PhysicalType<T>(lhs / rhs.getValue(), rhs.getUnit()); //WRONG! TODO
//}

} /* namespace PARTONS */

#endif /* PHYSICAL_TYPE_H */
