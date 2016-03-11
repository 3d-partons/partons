/**
 * @file GPMT.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Feb 17, 2016
 * @version 1.0
 *
 * @class GPMT
 * @brief MT Gluon Propagator (Maris & Tandy).
 */

#ifndef GPMT_H_
#define GPMT_H_

#include <string>

#include "GluonPropagator.h"

class MTGluonPropagator: public GluonPropagator {
public:
    MTGluonPropagator(double w = 0.4, double I = 5.8, double LambdaQCD = 0.234,
            int Nf = 4);
    virtual ~MTGluonPropagator();

    virtual MTGluonPropagator* clone() const;

    virtual std::string toString() const;

    virtual double evaluateG(double k2) const;

protected:
    MTGluonPropagator(const std::string &className, double w = 0.4,
            double I = 5.8, double LambdaQCD = 0.234, int Nf = 4);
    MTGluonPropagator(const MTGluonPropagator& other);

    virtual void updateIR();
};

#endif /* GPMT_H_ */
