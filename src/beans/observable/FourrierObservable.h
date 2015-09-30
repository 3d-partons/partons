/*
 * FourrierObservable.h
 *
 *  Created on: Sep 28, 2015
 *      Author: debian
 */

#ifndef FOURRIEROBSERVABLE_H_
#define FOURRIEROBSERVABLE_H_

#include <string>

#include "../../modules/MathIntegratorModule.h"
#include "Observable.h"

class FourrierObservable: public Observable, public MathIntegratorModule {
public:
    FourrierObservable(const std::string &className);
    virtual ~FourrierObservable();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual Observable* clone() const = 0;

    virtual double compute();

protected:
    virtual double functionToIntegrate(double *x, double *params) = 0;
};

#endif /* FOURRIEROBSERVABLE_H_ */
