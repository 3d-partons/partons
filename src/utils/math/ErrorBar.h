#ifndef ERROR_BAR_H
#define ERROR_BAR_H

/**
 * @file ErrorBar.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 16 Semptember 2015
 * @version 1.0
 *
 * @class ErrorBar
 *
 * @brief Object to represent in mathematics an asymetrical error bar.
 */

class ErrorBar {
public:
    /**
     * Default constructor all members init to 0.
     */
    ErrorBar();

    /**
     * Parameterized constructor
     *
     * @param upperBound
     * @param lowerBound
     */
    ErrorBar(double upperBound, double lowerBound);

    /**
     * Default destructor
     */
    virtual ~ErrorBar();

    // #################
    // Getters & Setters
    // #################

    double getLowerBound() const;
    void setLowerBound(double lowerBound);
    double getUpperBound() const;
    void setUpperBound(double upperBound);

protected:
    //TODO more comments
    double m_upperBound; ///< vertical error bar
    double m_lowerBound; ///< vertical error bar
};

#endif /* ERROR_BAR_H */
