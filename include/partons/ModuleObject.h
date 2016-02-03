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
#include <string>

#include "utils/ParameterList.h"
#include "utils/thread/Thread.h"

class ModuleObject: public BaseObject, public Thread {
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
    virtual void configure(ParameterList parameters);

    /**
     * Return the number of current parameters used for configure this module.
     */
    //TODO Remove
    size_t getNbOfParameters();

    virtual ModuleObject* clone() const = 0;

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString() const;

    virtual void init();

protected:

    ParameterList m_parameters; ///< Parameters used for configure this module.

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
