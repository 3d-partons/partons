#ifndef FOURIER_OBSERVABLE_H
#define FOURIER_OBSERVABLE_H

/**
 * @file FourierObservable.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 28 September 2015
 * @version 1.0
 *
 * @class FourierObservable
 *
 * @brief
 */

#include <string>
#include <vector>

#include "../MathIntegratorModule.h"
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
    FourierObservable(const FourierObservable &other);

    virtual double functionToIntegrate(std::vector<double> x,
            std::vector<double> params) = 0;
};

#endif /* FOURIER_OBSERVABLE_H */
