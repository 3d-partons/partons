#ifndef BASE_MODULE_H
#define BASE_MODULE_H

/**
 * @file BaseModule.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 06 Aout 2014
 * @version 1.0
 *
 * Last update : 22 September 2014
 *
 * @class BaseModule
 * @brief Super class of all module types - A module is a class that performs a specific compute task
 *
 * ex : GK11Model is a module that computes only GPDq(x, xi, t, MuF, MuR) defined by Kroll-Goloskokov in 2011 \n
 * ex : DVCSCFFModule is a module that evaluates the convolution of the GPD H with the hard scattering kernel at twist 2 necessary to the evaluation of DVCS scattering amplitudes.
 */

#include <stddef.h>
#include <map>
#include <string>

#include "../beans/GenericData.h"
#include "../BaseObject.h"

class Parameters;

class MathIntegratorModule;

class ModuleObject: public BaseObject {
public:
    ModuleObject(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~ModuleObject();

    virtual void configure(Parameters parameters);

    /**
     * Return the number of current parameters used for configure this module.
     */
    size_t getNbOfParameters();

    virtual ModuleObject* clone() const = 0;

    // Callbacks are tremendously useful in object-oriented design when one needs to decouple two classes but let them be connected via a single function call.
    // Wouldn't it be nice if C++ supported callbacks directly?
    // Unfortunately, C++ does not offer anything like this. myObject.foo is a combination of the object pointer &myObject and the member function pointer foo. C++ does not have a pointer type that combines these two pointers.
    // One technique for implementing callbacks is to use an interface class. An interface class contains a member function to be overridden by a deriver. This is the callback function.
    // cf : http://tedfelix.com/software/c++-callbacks.html
    virtual double functionsToIntegrate(const double * x,
            const double * parameters = 0);

    virtual std::string toString();

    unsigned int getCurrentFunctionToIntegrate() const;
    void setCurrentFunctionToIntegrate(unsigned int currentFunctionToIntegrate);

protected:
    std::map<std::string, GenericData> m_parameters; ///< parameters used for configure this module.
    unsigned int m_currentFunctionToIntegrate;

    MathIntegratorModule* m_pMathIntegratorModule;

    /***
     * Copy constructor
     * @param other
     */
    ModuleObject(const ModuleObject &other);

    virtual void initModule() = 0;
    virtual void isModuleWellConfigured() = 0;

    GenericData getValue();
};

#endif /* BASE_MODULE_H */
