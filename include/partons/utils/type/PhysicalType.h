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
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "../../BaseObject.h"
#include "PhysicalUnit.h"

namespace PARTONS {

/**
 * @class PhysicalType
 *
 * @brief Value with unit.
 *
 * This class is a container for a value, unit and switch to mark if the value is initialized or not. Setting a value (either via setter or one of constructors) make the object "initialized".
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
     * Assignment constructor.
     */
    PhysicalType(PhysicalUnit::Type unit) :
            BaseObject("PhysicalType"), m_initialized(false), m_value(), m_unit(
                    unit) {
    }

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
    PhysicalType(const PhysicalType& other) :
            BaseObject(other), m_initialized(other.m_initialized), m_value(
                    other.m_value), m_unit(other.m_unit) {

    }

    /**
     * Return string representing the whole object.
     */
    std::string toString() const {

        ElemUtils::Formatter formatter;

        formatter << m_value;
        formatter << " [";
        formatter << PhysicalUnit(m_unit).getShortName();
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
     * Serialize into std::vector<double>
     */
    void serializeIntoStdVector(std::vector<double>& vec) const {

        vec.push_back(static_cast<double>(m_initialized));
        vec.push_back(static_cast<double>(m_value));
        vec.push_back(static_cast<double>(m_unit));
    }

    /**
     * Uneserialize from std::vector<double>
     */
    void unserializeFromStdVector(std::vector<double>::const_iterator& it,
            const std::vector<double>::const_iterator& end) {

        if (it >= end) {
            throw ElemUtils::CustomException(this->getClassName(), __func__,
                    "Input vector is too short");
        }

        m_initialized = static_cast<bool>(*it);
        it++;

        if (it >= end) {
            throw ElemUtils::CustomException(this->getClassName(), __func__,
                    "Input vector is too short");
        }

        m_value = static_cast<T>(*it);
        it++;

        if (it >= end) {
            throw ElemUtils::CustomException(this->getClassName(), __func__,
                    "Input vector is too short");
        }

        m_unit = static_cast<PhysicalUnit::Type>(*it);
        it++;
    }

    /**
     * Return string representing value and unit. To be used mainly in the generation of hash sums.
     */
    std::string toStdString() const {

        std::stringstream sstream;
        sstream << m_value << " " << m_unit;
        return sstream.str();
    }

    /**
     * Assignment operator.
     */
    inline PhysicalType<T>& operator=(PhysicalType<T> const &rhs) {

        setValue(rhs.m_value);
        setUnit(rhs.m_unit);
        setInitialized(rhs.m_initialized);

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

    /**
     * Check if the same unit category. If units are different make the conversion.
     */
    PhysicalType<T> makeSameUnitAs(PhysicalUnit::Type other) const {

        //check if different units
        if (m_unit != other) {

            //check unit category
            checkIfSameUnitCategoryAs(other);

            //check if not PhysicalUnit::UNDEFINED
            if (other == PhysicalUnit::UNDEFINED) {
                throw ElemUtils::CustomException(this->getClassName(), __func__,
                        "Can not make conversion from PhysicalUnit::UNDEFINED");
            }

            //make conversion
            return PhysicalType<T>(
                    m_value * PhysicalUnit(m_unit).getConversionFactor()
                            / PhysicalUnit(other).getConversionFactor(), other);
        }

        //return
        return PhysicalType<T>(*this);
    }

    /**
     * Check if the same unit category. If units are different make the conversion.
     */
    PhysicalType<T> makeSameUnitAs(const PhysicalType<T>& other) const {
        return makeSameUnitAs(other.getUnit());
    }

    /**
     * Check if the same unit. If units are different throw exception.
     */
    void checkIfSameUnitAs(PhysicalUnit::Type other) const {
        if (m_unit != other) {
            throw ElemUtils::CustomException(this->getClassName(), __func__,
                    ElemUtils::Formatter() << "Units are different, this unit: "
                            << PhysicalUnit(m_unit).toString()
                            << " other unit: "
                            << PhysicalUnit(other).toString());
        }
    }

    /**
     * Check if the same unit. If units are different throw exception.
     */
    void checkIfSameUnitAs(const PhysicalType<T>& other) const {
        checkIfSameUnitAs(other.getUnit());
    }

    /**
     * Check if the same unit category. If units are different throw exception.
     */
    void checkIfSameUnitCategoryAs(PhysicalUnit::Type other) const {
        if (PhysicalUnit(m_unit).getUnitCategory()
                != PhysicalUnit(other).getUnitCategory()) {
            throw ElemUtils::CustomException(this->getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Units have different categories, this unit: "
                            << PhysicalUnit(m_unit).toString()
                            << " other unit: "
                            << PhysicalUnit(other).toString());
        }
    }

    /**
     * Check if the same unit category. If units are different throw exception.
     */
    void checkIfSameUnitCategoryAs(const PhysicalType<T>& other) const {
        checkIfSameUnitCategoryAs(other.getUnit());
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
    return (lhs.getValue() == rhs.makeSameUnitAs(lhs).getValue());
}

template<class T>
inline bool operator!=(PhysicalType<T> const &lhs, PhysicalType<T> const &rhs) {
    return (lhs.getValue() != rhs.makeSameUnitAs(lhs).getValue());
}

template<class T>
inline bool operator<(PhysicalType<T> const &lhs, PhysicalType<T> const &rhs) {
    return (lhs.getValue() < rhs.makeSameUnitAs(lhs).getValue());
}

template<class T>
inline bool operator>(PhysicalType<T> const &lhs, PhysicalType<T> const &rhs) {
    return (lhs.getValue() > rhs.makeSameUnitAs(lhs).getValue());
}

template<class T>
inline bool operator<=(PhysicalType<T> const &lhs, PhysicalType<T> const &rhs) {
    return (lhs.getValue() <= rhs.makeSameUnitAs(lhs).getValue());
}

template<class T>
inline bool operator>=(PhysicalType<T> const &lhs, PhysicalType<T> const &rhs) {
    return (lhs.getValue() >= rhs.makeSameUnitAs(lhs).getValue());
}

/// Arithmetic operators

template<class T>
inline PhysicalType<T> operator+(PhysicalType<T> const &lhs,
        PhysicalType<T> const &rhs) {

    lhs.checkIfSameUnitAs(rhs);
    return PhysicalType<T>(lhs.getValue() + rhs.getValue(), lhs.getUnit());
}

template<class T>
inline PhysicalType<T>& operator+=(PhysicalType<T> &lhs,
        PhysicalType<T> const &rhs) {
    lhs = lhs + rhs;
    return lhs;
}

template<class T>
inline PhysicalType<T> operator-(PhysicalType<T> const &lhs,
        PhysicalType<T> const &rhs) {

    lhs.checkIfSameUnitAs(rhs);
    return PhysicalType<T>(lhs.getValue() - rhs.getValue(), lhs.getUnit());
}

template<class T>
inline PhysicalType<T>& operator-=(PhysicalType<T> &lhs,
        PhysicalType<T> const &rhs) {
    lhs = lhs - rhs;
    return lhs;
}

template<class T>
inline PhysicalType<T> operator/(PhysicalType<T> const &lhs,
        PhysicalType<T> const &rhs) {

    lhs.checkIfSameUnitAs(rhs);
    return PhysicalType<T>(lhs.getValue() / rhs.getValue(), PhysicalUnit::NONE);
}

template<class T>
inline PhysicalType<T>& operator/=(PhysicalType<T> &lhs,
        PhysicalType<T> const &rhs) {
    lhs = lhs / rhs;
    return lhs;
}

/// Arithmetic operators

template<class T>
inline PhysicalType<T> operator*(PhysicalType<T> const &lhs, T const &rhs) {
    return PhysicalType<T>(lhs.getValue() * rhs, lhs.getUnit());
}

template<class T>
inline PhysicalType<T>& operator*=(PhysicalType<T> &lhs, T const &rhs) {
    lhs = lhs * rhs;
    return lhs;
}

template<class T>
inline PhysicalType<T> operator*(T const &lhs, PhysicalType<T> const &rhs) {
    return PhysicalType<T>(lhs * rhs.getValue(), rhs.getUnit());
}

template<class T>
inline PhysicalType<T> operator/(PhysicalType<T> const &lhs, T const &rhs) {
    return PhysicalType<T>(lhs.getValue() / rhs, lhs.getUnit());
}

template<class T>
inline PhysicalType<T>& operator/=(PhysicalType<T> &lhs, T const &rhs) {
    lhs = lhs / rhs;
    return lhs;
}

} /* namespace PARTONS */

#endif /* PHYSICAL_TYPE_H */
