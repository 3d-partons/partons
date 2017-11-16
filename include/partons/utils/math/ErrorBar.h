#ifndef ERROR_BAR_H
#define ERROR_BAR_H

/**
 * @file ErrorBar.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 16 Semptember 2015
 * @version 1.0
 */

namespace PARTONS {

/**
 * @class ErrorBar
 *
 * @brief Class representing single asymmetrical error bar.
 *
 * This class represents a single asymmetrical error bar. It is used in particular to handle experimental data.
 */
class ErrorBar {

public:

    /**
     * Default constructor.
     */
    ErrorBar();

    /**
     * Assignment constructor.
     *
     * @param lowerBound Lower bound of the uncertainty.
     * @param upperBound Upper bound of the uncertainty.
     */
    ErrorBar(double lowerBound, double upperBound);

    /**
     * Destructor.
     */
    virtual ~ErrorBar();

    /**
     * Get lower bound of the uncertainty.
     */
    double getLowerBound() const;

    /**
     * Set lower bound of the uncertainty.
     */
    void setLowerBound(double lowerBound);

    /**
     * Get upper bound of the uncertainty.
     */
    double getUpperBound() const;

    /**
     * Set upper bound of the uncertainty.
     */
    void setUpperBound(double upperBound);

protected:

    /**
     * Lower bound of the uncertainty.
     */
    double m_lowerBound;

    /**
     * Upper bound of the uncertainty.
     */
    double m_upperBound;
};

} /* namespace PARTONS */

#endif /* ERROR_BAR_H */
