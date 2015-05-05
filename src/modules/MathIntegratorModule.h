#ifndef MATH_INTEGRATOR_MODULE_H
#define MATH_INTEGRATOR_MODULE_H

/**
 * @file MathIntegratorModule
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 09 december 2014
 * @version 1.0
 *
 * @class MathIntegratorModule
 *
 * @brief
 */

#include <string>

#include "ModuleObject.h"

//TODO ajouter une liste de parametres pour configurer les differents integrators
class MathIntegratorModule: public ModuleObject {
public:
    MathIntegratorModule(const std::string &className);
    virtual ~MathIntegratorModule();

    virtual double compute(unsigned int functionName,
            ModuleObject* pModuleObject, double xMin, double xMax) = 0;

    virtual MathIntegratorModule* clone() const = 0;

protected:
    /***
     * Copy constructor
     * @param other
     */
    MathIntegratorModule(const MathIntegratorModule &other);

    void preCompute(unsigned int functionName, ModuleObject* pModuleObject);

    double (ModuleObject::*m_pFunction)(double *, double *);

    virtual void isModuleWellConfigured() = 0;
    virtual void initModule() = 0;
};

#endif /* MATH_INTEGRATOR_MODULE_H */
