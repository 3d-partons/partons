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

class ConvolCoeffFunctionModule: public ModuleObject {
public:
    ConvolCoeffFunctionModule(const std::string &className);
    virtual ~ConvolCoeffFunctionModule();

protected:
    /**
     * Copy constructor
     */
    ConvolCoeffFunctionModule(const ConvolCoeffFunctionModule &other);

};

#endif /* CONVOL_COEFF_FUNCTION_MODULE_H */
