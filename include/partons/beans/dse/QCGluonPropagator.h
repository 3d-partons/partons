/**
 * @file QCGluonPropagator.h
 * @author Nabil Chouika (CEA Saclay / SPhN)
 * @date Feb 24 2016
 * @version 1.0
 */

#ifndef GPQC_H_
#define GPQC_H_

#include <string>

#include "GluonPropagator.h"

namespace PARTONS {

/**
 * @class QCGluonPropagator
 * @brief QC Gluon Propagator (Qin-Chang)
 */
class QCGluonPropagator: public GluonPropagator {
public:
    QCGluonPropagator(double w = 0.5, double I = 5.8, double LambdaQCD = 0.234,
            int Nf = 4);
    virtual ~QCGluonPropagator();

    virtual QCGluonPropagator* clone() const;

    virtual std::string toString() const;

    virtual double evaluateG(double k2) const;

protected:
    QCGluonPropagator(const std::string &className, double w = 0.5,
            double I = 5.8, double LambdaQCD = 0.234, int Nf = 4);
    QCGluonPropagator(const QCGluonPropagator& other);

    virtual void updateIR();
};

} /* namespace PARTONS */

#endif /* GPQC_H_ */
