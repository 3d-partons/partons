/*
 * @file Tolerances.h
 * @author: Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date 3 July 2015
 * @version 1.0
 *
 * @class Tolerances
 *
 * @brief Define absolute and relative tolerance for comparison of real numbers (double) and check they are positive.
 */

#ifndef TOLERANCES_H_
#define TOLERANCES_H_

class Tolerances {
public:
    Tolerances();
    Tolerances(double absoluteTolerance, double relativeTolerance);
    virtual ~Tolerances();

private:
    double m_absoluteTolerance;
    double m_relativeTolerance;
};

#endif /* TOLERANCES_H_ */
