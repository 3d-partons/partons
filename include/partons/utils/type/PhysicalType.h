#ifndef PHYSICAL_TYPE_H
#define PHYSICAL_TYPE_H

/**
 * @file PhysicalType.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 20 January 2016
 * @version 1.0
 *
 * @class PhysicalType
 *
 * @brief
 */

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/thread/Packet.h>
#include <sstream>
#include <stdexcept>
#include <string>

template<class T>
class PhysicalType {
public:
    PhysicalType() :
            m_initialized(false), m_value(), m_unit("none") {
    }
    PhysicalType(T value) :
            m_initialized(false), m_value(value), m_unit("none") {
    }

    PhysicalType(const std::string &stringValue) :
            m_initialized(false), m_unit("none") {
        fromStdString(stringValue);
    }

    PhysicalType(T value, const std::string &unit) :
            m_initialized(false), m_value(value), m_unit(unit) {
    }

    PhysicalType(const std::string &stringValue, const std::string &unit) :
            m_initialized(false), m_unit(unit) {
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
            throw std::runtime_error(
                    "[PhysicalBaseType::fromString] cast from std::string to T failed ! ");
        }

        m_initialized = true;
    }

    std::string toString() const {
        ElemUtils::Formatter formatter;

        if (m_initialized) {
            formatter << m_value << "(" << m_unit << ")";
        } else {
            formatter << "nan";
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
