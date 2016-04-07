#ifndef FOURIER_OBSERVABLE_H
#define FOURIER_OBSERVABLE_H

/**
 * @file FourierObservable.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date September 28, 2015
 * @version 1.0
 *
 * @class FourierObservable
 *
 * @brief
 */

#include <string>

#include "../MathIntegratorModule.h"
#include "Observable.h"

namespace NumA {
class FunctionType1D;
} /* namespace NumA */

class FourierObservable: public Observable, public MathIntegratorModule {
public:
    FourierObservable(const std::string &className);
    virtual ~FourierObservable();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual Observable* clone() const = 0;

    virtual double compute() = 0;

protected:
    FourierObservable(const FourierObservable &other);
};

#endif /* FOURIER_OBSERVABLE_H */
