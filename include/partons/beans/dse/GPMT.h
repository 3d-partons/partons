/**
 * @file GPMT.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Feb 17, 2016
 * @version 1.0
 *
 * @class GPMT
 */

#ifndef GPMT_H_
#define GPMT_H_

#include <string>

#include "GluonPropagator.h"

class GPMT: public GluonPropagator {
public:
    GPMT(double w = 0.4, double I = 5.8, double LambdaQCD = 0.234,
            int Nf = 4);
    GPMT(const std::string &className, double w = 0.4,
            double I = 5.8, double LambdaQCD = 0.234, int Nf = 4);
    virtual ~GPMT();

    virtual GPMT* clone() const;

    virtual std::string toString() const;

    virtual double evaluateG(double k2) const;

protected:
    GPMT(const GPMT& other);
};

#endif /* GPMT_H_ */
