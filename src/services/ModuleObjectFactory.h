#ifndef BASE_MODULE_FACTORY_H
#define BASE_MODULE_FACTORY_H

/**
 * @file ModuleObjectFactory.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 06 Aout 2014
 * @version 1.0
 *
 * Last update : 24 September 2014
 *
 * @class ModuleObjectFactory
 * @brief \<singleton\> Use for register all available module, create/configure and share them for a later use.
 *
 * Use map of pointer because you can't instantiate object from abstract class, you can only share pointer.
 */

#include <map>
#include <string>

#include "../modules/ModuleObject.h"

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
     * @return Not used. It's for enable registerModule() to be executed before the main() function and auto-register all available module.
     */
    bool registerModule(ModuleObject * _pModuleObject);

    ModuleObject* getModule(const std::string & ID);
    GPDModule* getGPDModule(const std::string & ID);
    CFFModule* getCFFModule(const std::string & ID);
    EvolQCDModule* getEvolQCDModule(const std::string & ID);
    DVCSModule* getDVCSModule(const std::string & ID);

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
