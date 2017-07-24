#ifndef ACTIVE_FLAVORS_THRESHOLDS_H
#define ACTIVE_FLAVORS_THRESHOLDS_H

/**
 * @file ActiveFlavorsThresholds.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 07 August 2015
 * @version 1.0
 */

#include <string>

/**
 * @class ActiveFlavorsThresholds
 *
 * @brief Interval of factorization scale with fixed number of flavors.
 *
 * This class is used to indicate an interval of factorization scale where a fixed number of quark flavors is active.
 */
class ActiveFlavorsThresholds {

public:

    /**
     * Default constructor.
     */
    ActiveFlavorsThresholds();

    /**
     * Assignment constructor.
     * @param nf Number of quark flavor being active in the interval.
     * @param lowerBound Lower bound of the interval.
     * @param upperBound Upper bound of the interval.
     * @param index Index of interval.
     */
    ActiveFlavorsThresholds(unsigned short nf, double lowerBound, double upperBound,
            unsigned int index);

    /**
     * Destructor.
     */
    virtual ~ActiveFlavorsThresholds();

    /**
     * Relation operator that checks if the value of left operand is less than the value of right operand (in this case returned is this->m_nf < other.m_nf).
     * Used by std::sort function.
     * @param other Right hand value.
     * @return True if the value of left operand is less than the value of right operand, otherwise false.
     */
    bool operator <(const ActiveFlavorsThresholds &other) const;

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get number of quark flavor being active in the interval.
     */
    unsigned short getNf() const;

    /**
     * Set number of quark flavor being active in the interval.
     */
    void setNf(unsigned short nf);

    /**
     * Get lower bound of the interval.
     */
    double getLowerBound() const;

    /**
     * Set lower bound of the interval.
     */
    void setLowerBound(double lowerBound);

    /**
     * Get upper bound of the interval.
     */
    double getUpperBound() const;

    /**
     * Set upper bound of the interval.
     */
    void setUpperBound(double upperBound);

    /**
     * Get index of interval.
     */
    unsigned int getIndex() const;

    /**
     * Set index of interval.
     */
    void setIndex(unsigned int index);

private:

    /**
     * Number of quark flavor being active in the interval.
     */
    unsigned short m_nf;

    /**
     * Lower bound of the interval.
     */
    double m_lowerBound;

    /**
     * Upper bound of the interval.
     */
    double m_upperBound;

    /**
     * Index of interval.
     */
    unsigned int m_index;
};

#endif /* ACTIVE_FLAVORS_THRESHOLDS_H */
