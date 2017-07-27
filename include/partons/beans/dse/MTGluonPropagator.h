/**
 * @file MTGluonPropagator.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Feb 17, 2016
 * @version 1.0
 */

#ifndef GPMT_H_
#define GPMT_H_

#include <string>

#include "GluonPropagator.h"

namespace PARTONS {

/**
 * @class MTGluonPropagator
 * @brief MT Gluon Propagator (Maris & Tandy).
 */
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

} /* namespace PARTONS */

#endif /* GPMT_H_ */
