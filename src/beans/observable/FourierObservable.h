/*
 * FourrierObservable.h
 *
 *  Created on: Sep 28, 2015
 *      Author: debian
 */

#ifndef FOURIEROBSERVABLE_H_
#define FOURIEROBSERVABLE_H_

#include <string>

#include "../../modules/MathIntegratorModule.h"
#include "Observable.h"

class FourierObservable: public Observable, public MathIntegratorModule {
public:
    FourierObservable(const std::string &className);
    virtual ~FourierObservable();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual Observable* clone() const = 0;

    double compute();

protected:
    virtual double functionToIntegrate(double *x, double *params) = 0;
};

#endif /* FOURRIEROBSERVABLE_H_ */
