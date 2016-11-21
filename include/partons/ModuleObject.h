#ifndef BASE_MODULE_H
#define BASE_MODULE_H

/**
 * @file ModuleObject.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date Aout 06, 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/thread/Thread.h>
#include <map>
#include <string>

#include "beans/automation/BaseObjectData.h"

namespace ElemUtils {
class Parameters;
} /* namespace ElemUtils */

/**
 * @class ModuleObject
 *
 * @brief Super class of all module types - A module is a class that performs a specific compute task
 * A ModuleObject can be threaded. See services for more details.
 *
 * ex : GK11Model is a module that computes only GPDq(x, xi, t, MuF, MuR) defined by Kroll-Goloskokov in 2011 \n
 * ex : DVCSCFFModule is a module that evaluates the convolution of the GPD H with the hard scattering kernel at twist 2 necessary to the evaluation of DVCS scattering amplitudes.
 */
class ModuleObject: public BaseObject, public ElemUtils::Thread {
public:
    static const std::string CLASS_NAME;

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
    virtual void configure(const ElemUtils::Parameters &parameters);

    /**
     * See documentation about this method in BaseObject class for more details.
     */
    virtual ModuleObject* clone() const = 0;

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString() const;

    /**
     * See documentation about this method in BaseObject class for more details.
     */
    virtual void resolveObjectDependencies();

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

protected:
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
