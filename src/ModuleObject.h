#ifndef BASE_MODULE_H
#define BASE_MODULE_H

/**
 * @file ModuleObject.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 06 Aout 2014
 * @version 1.0
 *
 * @class ModuleObject
 *
 * @brief Super class of all module types - A module is a class that performs a specific compute task
 *
 * ex : GK11Model is a module that computes only GPDq(x, xi, t, MuF, MuR) defined by Kroll-Goloskokov in 2011 \n
 * ex : DVCSCFFModule is a module that evaluates the convolution of the GPD H with the hard scattering kernel at twist 2 necessary to the evaluation of DVCS scattering amplitudes.
 */

#include <stddef.h>
#include <map>
#include <string>

#include "beans/GenericData.h"
#include "BaseObject.h"

class Parameters;

class MathIntegratorModule;

class ModuleObject: public BaseObject {
public:
    /**
     * Constructor.
     * See BaseObject class for more info about input parameter.
     *
     * @param className class's name of child class.
     */
    ModuleObject(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~ModuleObject();

    /**
     * Provides a generic method to configure all types of modules by passing a Parameters object.
     * Parameters class represents a list of couples key/value (see Parameters class documentation for more info)
     *
     * @param parameters
     */
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
    virtual double functionsToIntegrate(double * x, double * parameters = 0);

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString();

    unsigned int getCurrentFunctionToIntegrate() const;
    void setCurrentFunctionToIntegrate(unsigned int currentFunctionToIntegrate);

    virtual void init();

protected:
    std::map<std::string, GenericData> m_parameters; ///< Parameters used for configure this module.

    unsigned int m_currentFunctionToIntegrate; ///< Variable used by the module to know what mathematical function integrate.

    MathIntegratorModule* m_pMathIntegratorModule; ///< Pointer to MathIntegratorModule class.

    /***
     * Copy constructor
     * @param other
     */
    ModuleObject(const ModuleObject &other);

    /**
     * Pure virtual function that provides skeleton for module initialization.
     * Children must define and override it.
     */
    virtual void initModule() = 0;

    /**
     * Pure virtual function that provides skeleton to check if the module is well initialized and configured.
     * Children must define and override it.
     */
    virtual void isModuleWellConfigured() = 0;
};

#endif /* BASE_MODULE_H */
