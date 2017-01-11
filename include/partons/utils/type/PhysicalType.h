#ifndef PHYSICAL_TYPE_H
#define PHYSICAL_TYPE_H

/**
 * @file PhysicalType.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date January 20, 2016
 * @version 1.0
 */

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>
#include <sstream>
#include <string>

#include "../../BaseObject.h"

/**
 * @class PhysicalType
 *
 * @brief
 */
template<class T>
class PhysicalType: public BaseObject {
public:
    static const std::string PHYSICAL_TYPE_NONE_UNIT;

    PhysicalType() :
            BaseObject("PhysicalType"), m_initialized(false), m_value(), m_unit(
                    PHYSICAL_TYPE_NONE_UNIT) {
    }
    PhysicalType(T value) :
            BaseObject("PhysicalType"), m_initialized(true), m_value(value), m_unit(
                    PHYSICAL_TYPE_NONE_UNIT) {
    }

    PhysicalType(const std::string &stringValue) :
            BaseObject("PhysicalType"), m_initialized(false), m_unit(
                    PHYSICAL_TYPE_NONE_UNIT) {
        fromStdString(stringValue);
    }

    PhysicalType(T value, const std::string &unit) :
            BaseObject("PhysicalType"), m_initialized(true), m_value(value), m_unit(
                    unit) {
    }

    PhysicalType(const std::string &stringValue, const std::string &unit) :
            BaseObject("PhysicalType"), m_initialized(false), m_unit(unit) {
        fromStdString(stringValue);
    }

    std::string toStdString() const {
        std::stringstream sstream(m_value);
        return sstream.str();
    }

    void fromStdString(const std::string &stringValue) {
        std::stringstream sstream(stringValue);

        // if conversion failed then print an exception
        if ((sstream >> m_value).fail()) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "Cast from std::string to type<T> failed !");
        }

        m_initialized = true;
    }

    std::string toString() const {
        ElemUtils::Formatter formatter;

        if (m_initialized) {
            formatter << m_value;
            if (!ElemUtils::StringUtils::equalsIgnoreCase(m_unit,
                    PHYSICAL_TYPE_NONE_UNIT)) {
                formatter << " (" << m_unit << ")";
            }
        } else {
            formatter << "not initialized";
        }

        return formatter.str();
    }

    /// Serialization

    void serialize(ElemUtils::Packet &packet) const {
        packet << m_initialized << m_value << m_unit;
    }

    void unserialize(ElemUtils::Packet &packet) {
        packet >> m_initialized;
        packet >> m_value;
        packet >> m_unit;
    }

    /// Assignment operator
    inline PhysicalType<T>& operator=(PhysicalType<T> const &rhs) {
        setValue(rhs.m_value);
        return *this;
    }

    inline PhysicalType<T>& operator=(T const &rhs) {
        setValue(rhs);
        return *this;
    }

    ///

    inline T operator()() {
        return m_value;
    }

    /// Getters & Setters

    T getValue() const {
        return m_value;
    }

    void setValue(T value) {
        m_value = value;
        m_initialized = true;
    }

    const std::string& getUnit() const {
        return m_unit;
    }

    void setUnit(const std::string& unit) {
        m_unit = unit;
    }

    bool isInitialized() const {
        return m_initialized;
    }

    void setInitialized(bool initialized) {
        m_initialized = initialized;
    }

private:

    bool m_initialized;

    T m_value;
    std::string m_unit;
};

template<class T>
const std::string PhysicalType<T>::PHYSICAL_TYPE_NONE_UNIT = "none";

template<class T>
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        PhysicalType<T>& physicalType) {
    physicalType.serialize(packet);
    return packet;
}

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

template<class T>
inline bool operator==(PhysicalType<T> const &lhs, T const &rhs) {
    return lhs.getValue() == rhs;
}
template<class T>
inline bool operator!=(PhysicalType<T> const &lhs, T const &rhs) {
    return lhs.getValue() != rhs;
}
template<class T>
inline bool operator<(PhysicalType<T> const &lhs, T const &rhs) {
    return lhs.getValue() < rhs;
}
template<class T>
inline bool operator>(PhysicalType<T> const &lhs, T const &rhs) {
    return lhs.getValue() > rhs;
}
template<class T>
inline bool operator<=(PhysicalType<T> const &lhs, T const &rhs) {
    return lhs.getValue() <= rhs;
}
template<class T>
inline bool operator>=(PhysicalType<T> const &lhs, T const &rhs) {
    return lhs.getValue() >= rhs;
}

template<class T>
inline bool operator==(T const &lhs, PhysicalType<T> const &rhs) {
    return lhs == rhs.getValue();
}
template<class T>
inline bool operator!=(T const &lhs, PhysicalType<T> const &rhs) {
    return lhs != rhs.getValue();
}
template<class T>
inline bool operator<(T const &lhs, PhysicalType<T> const &rhs) {
    return lhs < rhs.getValue();
}
template<class T>
inline bool operator>(T const &lhs, PhysicalType<T> const &rhs) {
    return lhs > rhs.getValue();
}
template<class T>
inline bool operator<=(T const &lhs, PhysicalType<T> const &rhs) {
    return lhs <= rhs.getValue();
}
template<class T>
inline bool operator>=(T const &lhs, PhysicalType<T> const &rhs) {
    return lhs >= rhs.getValue();
}

/// Arithmetic operators

//TODO upgrade select unit
//TODO use setters for avoid problem with m_initialized value

template<class T>
inline PhysicalType<T> operator+(PhysicalType<T> const &lhs,
        PhysicalType<T> const &rhs) {
    return PhysicalType<T>(lhs.getValue() + rhs.getValue(), lhs.getUnit());
}
template<class T>
inline PhysicalType<T> operator-(PhysicalType<T> const &lhs,
        PhysicalType<T> const &rhs) {
    return PhysicalType<T>(lhs.getValue() - rhs.getValue(), lhs.getUnit());
}
template<class T>
inline PhysicalType<T> operator*(PhysicalType<T> const &lhs,
        PhysicalType<T> const &rhs) {
    return PhysicalType<T>(lhs.getValue() * rhs.getValue(), lhs.getUnit());
}
template<class T>
inline PhysicalType<T> operator/(PhysicalType<T> const &lhs,
        PhysicalType<T> const &rhs) {
    return PhysicalType<T>(lhs.getValue() / rhs.getValue(), lhs.getUnit());
}

template<class T>
inline PhysicalType<T> operator+(PhysicalType<T> const &lhs, T const &rhs) {
    return PhysicalType<T>(lhs.getValue() + rhs, lhs.getUnit());
}
template<class T>
inline PhysicalType<T> operator-(PhysicalType<T> const &lhs, T const &rhs) {
    return PhysicalType<T>(lhs.getValue() - rhs, lhs.getUnit());
}
template<class T>
inline PhysicalType<T> operator*(PhysicalType<T> const &lhs, T const &rhs) {
    return PhysicalType<T>(lhs.getValue() * rhs, lhs.getUnit());
}
template<class T>
inline PhysicalType<T> operator/(PhysicalType<T> const &lhs, T const &rhs) {
    return PhysicalType<T>(lhs.getValue() / rhs, lhs.getUnit());
}

template<class T>
inline PhysicalType<T> operator+(T const &lhs, PhysicalType<T> const &rhs) {
    return PhysicalType<T>(lhs + rhs.getValue(), rhs.getUnit());
}
template<class T>
inline PhysicalType<T> operator-(T const &lhs, PhysicalType<T> const &rhs) {
    return PhysicalType<T>(lhs - rhs.getValue(), rhs.getUnit());
}
template<class T>
inline PhysicalType<T> operator*(T const &lhs, PhysicalType<T> const &rhs) {
    return PhysicalType<T>(lhs * rhs.getValue(), rhs.getUnit());
}
template<class T>
inline PhysicalType<T> operator/(T const &lhs, PhysicalType<T> const &rhs) {
    return PhysicalType<T>(lhs / rhs.getValue(), rhs.getUnit());
}

#endif /* PHYSICAL_TYPE_H */
