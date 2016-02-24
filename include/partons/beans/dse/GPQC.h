/**
 * @file GPQC.h
 * @author Nabil Chouika (CEA Saclay / SPhN)
 * @date Feb 24 2016
 * @version 1.0
 *
 * @class GPQC
 * @brief QC Gluon Propagator (Qin-Chang)
 */

#ifndef GPQC_H_
#define GPQC_H_

#include <string>

#include "GluonPropagator.h"

class GPQC: public GluonPropagator {
public:
    GPQC(double w = 0.5, double I = 5.8, double LambdaQCD = 0.234,
            int Nf = 4);
    GPQC(const std::string &className, double w = 0.5,
            double I = 5.8, double LambdaQCD = 0.234, int Nf = 4);
    virtual ~GPQC();

    virtual GPQC* clone() const;

    virtual std::string toString() const;

    virtual double evaluateG(double k2) const;

protected:
    GPQC(const GPQC& other);
};

#endif /* GPQC_H_ */
