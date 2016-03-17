#ifndef CONVOL_COEFF_FUNCTION_MODULE_H
#define CONVOL_COEFF_FUNCTION_MODULE_H

/**
 * @file ConvolCoeffFunctionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 22 July 2015
 * @version 1.0
 *
 * @class ConvolCoeffFunctionModule
 *
 * @brief
 */

#include <string>

#include "../ModuleObject.h"
#include "MathIntegratorModule.h"

class ConvolCoeffFunctionModule: public ModuleObject,
        public MathIntegratorModule {
public:
    ConvolCoeffFunctionModule(const std::string &className);
    virtual ~ConvolCoeffFunctionModule();

    virtual void configure(const ElemUtils::Parameters &parameters);

protected:
    /**
     * Copy constructor
     */
    ConvolCoeffFunctionModule(const ConvolCoeffFunctionModule &other);

};

#endif /* CONVOL_COEFF_FUNCTION_MODULE_H */
