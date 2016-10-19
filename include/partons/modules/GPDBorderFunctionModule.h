/*
 * GPDBorderFunctionModule.h
 *
 *  Created on: Oct 17, 2016
 *      Author: Pawel Sznjader (IPNO)
 */

#ifndef GPDBORDERFUNCTIONMODULE_H_
#define GPDBORDERFUNCTIONMODULE_H_

#include <string>

#include "../beans/gpd/GPDBorderFunctionKinematic.h"
#include "../beans/gpd/GPDType.h"
#include "GPDModule.h"

/**
 * Mother class to create models of GPD border function
 */
class GPDBorderFunctionModule: public GPDModule {

public:

    /**
     * Default constructor
     */
    GPDBorderFunctionModule(const std::string &className);

    /**
     * Destructor
     */
    virtual ~GPDBorderFunctionModule();

    virtual GPDModule* clone() const = 0;

    virtual void run();

    void virtual configure(const ElemUtils::Parameters &parameters);

    virtual PartonDistribution compute(const GPDBorderFunctionKinematic &kinematic,
            GPDType gpdType, bool evolution = false);

    virtual PartonDistribution compute(double xi, double t,
            double MuF2, double MuR2, GPDType::Type gpdType, bool evolution =
                    false);

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeE();
    virtual PartonDistribution computeHt();
    virtual PartonDistribution computeEt();

    virtual std::string toString();

protected:

    /**
     * Copy constructor
     *
     * @param other Object to be copied
     */
    GPDBorderFunctionModule(const GPDBorderFunctionModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

#endif /* GPDBORDERFUNCTIONMODULE_H_ */
