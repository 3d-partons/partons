#ifndef MODULE_OBJECT_H
#define MODULE_OBJECT_H

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
#include "beans/channel/ChannelType.h"

namespace ElemUtils {
class Parameters;
} /* namespace ElemUtils */

namespace PARTONS {

class ModuleObjectFactory;

/**
 * @class ModuleObject
 *
 * @brief Super class of all module types - A module is a class that performs a specific compute task.
 * A ModuleObject can be threaded. See services for more details.
 *
 * ex : GK11Model is a module that computes only GPDq(x, xi, t, MuF, MuR) defined by Kroll-Goloskokov in 2011. \n
 * ex : DVCSCFFModel is a module that evaluates the convolution of the GPD H with the hard scattering kernel at twist 2 necessary to the evaluation of DVCS scattering amplitudes.
 */
class ModuleObject: public BaseObject, public ElemUtils::Thread {

public:

    /**
     * Constructor.
     * See BaseObject class for more info about input parameter.
     *
     * Needn't be used directly. Use the ModuleObjectFactory to clone a module instead!
     *
     * @param className Class's name of child class.
     * @param channelType Channel type.
     */
    ModuleObject(const std::string &className, ChannelType::Type channelType);

    /**
     * Default destructor.
     */
    virtual ~ModuleObject();

    virtual ModuleObject* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();

    /**
     * Test of modules.
     *
     * @return List of results.
     */
    virtual std::vector<double> test();

    /**
     * Provides a generic method to configure all types of modules by passing a Parameters object.
     * Parameters class represents a list of couples key/value (see Parameters class documentation for more info).
     *
     * @param parameters ElemUtils::Parameters object.
     */
    virtual void configure(const ElemUtils::Parameters &parameters);

    /**
     * Method used in automation to prepare all the modules used by this current module and configure them recursively.
     * The recursion is linked to the imbrication in XML files. \n
     * Can be implemented in the child class if it needs modules not needed by the parent class. But there must be first a call to the parent method.
     * @param subModulesData Data used to retrieve the needed modules and their configuration.
     */
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    /**
     * Get reference module id.
     */
    unsigned int getReferenceModuleId() const;

    /**
     * Set reference module id.
     */
    void setReferenceModuleId(unsigned int referenceModuleId);

    /**
     * Get channel type.
     */
    ChannelType::Type getChannelType() const;

protected:

    /***
     * Copy constructor.
     * Used by the factory to clone modules, shouldn't be used directly.
     *
     * @param other Object to be copied.
     */
    ModuleObject(const ModuleObject &other);

    /**
     * Pointer tp module object factory.
     */
    ModuleObjectFactory* m_pModuleObjectFactory;

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

private:

    /**
     * Reference module id.
     */
    unsigned int m_referenceModuleId;

    /**
     * Channel type.
     */
    ChannelType::Type m_channelType;
};

} /* namespace PARTONS */

#endif /* MODULE_OBJECT_H */
