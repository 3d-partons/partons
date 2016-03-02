/**
 * @file GluonPropagator.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Jan 28, 2016
 * @version 1.0
 *
 * @class GluonPropagator
 */

#ifndef GLUONPROPAGATOR_H_
#define GLUONPROPAGATOR_H_

#include <string>

#include "../../BaseObject.h"

class GluonPropagator: public BaseObject {
public:
    GluonPropagator(const std::string &className, double w = 0.4,
            double I = 5.8, double LambdaQCD = 0.234, int Nf = 4);
    virtual ~GluonPropagator();

    virtual GluonPropagator* clone() const = 0;

    virtual std::string toString() const;

    virtual double evaluateAlpha(double k2) const;
    virtual double evaluateG(double k2) const = 0;

    // TODO Use configure(ParameterList) instead because those parameters depend on the model (daughter class)
    double getC() const;
    void setC(double w, double c);
    double getD() const;
    void setD(double w, double d);
    double getI() const;
    void setI(double w, double i);
    double getLambdaQCD() const;
    void setLambdaQCD(double lambdaQCD);
    int getNf() const;
    void setNf(int nf);
    double getW() const;

protected:
    GluonPropagator(const GluonPropagator& other);

    // TODO Move those attributes to the MT model (daughter class) and use configure(ParameterList) to define them
    double m_w; ///<
    double m_D; ///<
    double m_c; ///<
    double m_I;
    double m_LambdaQCD; ///<
    int m_Nf; ///<

};

#endif /* GLUONPROPAGATOR_H_ */
