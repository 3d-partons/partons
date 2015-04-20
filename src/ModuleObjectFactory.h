#ifndef BASE_MODULE_FACTORY_H
#define BASE_MODULE_FACTORY_H

/**
 * @file ModuleObjectFactory.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 06 Aout 2014
 * @version 1.0
 *
 * @class ModuleObjectFactory
 *
 * @brief \<singleton\> Use for register all available module, create/configure and share them for a later use.
 *
 * Use map of pointer because you can't instantiate object from abstract class, you can only share pointer.
 */

#include <map>
#include <string>

#include "modules/ModuleObject.h"

class RunningAlphaStrongModule;

class DVCSModule;

class CFFModule;
class EvolQCDModule;
class GPDModule;

class ModuleObjectFactory {
public:

    /**
     * Share a unique pointer of this class
     */
    static ModuleObjectFactory* getInstance();

    /**
     * Default destructor
     */
    virtual ~ModuleObjectFactory();

    /**
     * Store a unique instance of a module identified by a unique string character key.
     * @param _pBaseModule: an instance of the module built by its default constructor.
     * @return Classname of the module
     */
    std::string registerModule(ModuleObject * _pModuleObject);

    void init();

    ModuleObject* newModule(const std::string & ID);
    GPDModule* newGPDModule(const std::string & ID);
    CFFModule* newCFFModule(const std::string & ID);
    EvolQCDModule* newEvolQCDModule(const std::string & ID);
    DVCSModule* newDVCSModule(const std::string & ID);
    MathIntegratorModule* newMathIntegratorModule(const std::string & ID);
    RunningAlphaStrongModule* newRunningAlphaStrongModule(
            const std::string & ID);

    std::string toString();

private:
    /**
     * Private pointer of this class for a unique instance
     */
    static ModuleObjectFactory* m_pInstance;

    /**
     * Private default constructor for a unique instance
     */
    ModuleObjectFactory();

    std::map<std::string, ModuleObject*> m_moduleRegistry; ///< list of all available modules
    std::map<std::string, ModuleObject*>::iterator m_it;

};

#endif /* BASE_MODULE_FACTORY_H */
