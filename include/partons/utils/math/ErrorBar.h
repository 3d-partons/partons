#ifndef ERROR_BAR_H
#define ERROR_BAR_H

/**
 * @file ErrorBar.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 16 Semptember 2015
 * @version 1.0
 */

#include <ElementaryUtils/string_utils/Formatter.h>
#include <string>

#include "../../BaseObject.h"

namespace PARTONS {

/**
 * @class ErrorBar
 *
 * @brief Class representing single asymmetrical error bar.
 *
 * This class represents a single asymmetrical error bar. It is used in particular to handle experimental data.
 */
template<class T>
class ErrorBar: public BaseObject {

public:

    /**
     * Default constructor.
     */
    ErrorBar() :
            BaseObject("PhysicalType"), m_initialized(false), m_lowerBound(0.), m_upperBound(
                    0.) {
    }

    /**
     * Assignment constructor.
     *
     * @param lowerBound Lower bound of the uncertainty.
     * @param upperBound Upper bound of the uncertainty.
     */
    ErrorBar(T lowerBound, T upperBound) :
            BaseObject("PhysicalType"), m_initialized(true), m_lowerBound(
                    lowerBound), m_upperBound(upperBound) {
    }

    /**
     * Copy constructor.
     */
    ErrorBar(const ErrorBar& other) :
            BaseObject(other), m_initialized(other.m_initialized), m_lowerBound(
                    other.m_lowerBound), m_upperBound(other.m_upperBound) {
    }

    /**
     * Destructor.
     */
    virtual ~ErrorBar() {
    }

    virtual std::string toString() const {

        ElemUtils::Formatter formatter;

        formatter << "-" << m_lowerBound << " +" << m_upperBound;

        return formatter.str();
    }

    /**
     * Get lower bound of the uncertainty.
     */
    T getLowerBound() const {
        return m_lowerBound;
    }

    /**
     * Get upper bound of the uncertainty.
     */
    T getUpperBound() const {
        return m_upperBound;
    }

    /**
     * Set lower and upper bounds of the uncertainty.
     */
    void setBounds(T lowerBound, T upperBound) {

        m_lowerBound = lowerBound;
        m_upperBound = upperBound;

        m_initialized = true;
    }

    /**
     * Check if initialized.
     */
    bool isInitialized() const {
        return m_initialized;
    }

    /**
     * Set if initialized.
     */
    void setInitialized(bool initialized) {
        m_initialized = initialized;
    }

private:

    /**
     * True if value initialized.
     */
    bool m_initialized;

    /**
     * Lower bound of the uncertainty.
     */
    T m_lowerBound;

    /**
     * Upper bound of the uncertainty.
     */
    T m_upperBound;
};

} /* namespace PARTONS */

#endif /* ERROR_BAR_H */
