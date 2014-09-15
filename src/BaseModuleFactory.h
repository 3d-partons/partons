#ifndef BASE_MODULE_FACTORY_H
#define BASE_MODULE_FACTORY_H

/**
 * @file BaseModuleFactory.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 06 Aout 2014
 * @version 1.0
 *
 * Last update : 20 Aout 2014
 *
 * @class BaseModuleFactory
 * @brief \<singleton\> Use for register all available module, create/configure and share them for a later use.
 */

#include <map>
#include <string>

#include "BaseModule.h"

class CFFModule;
class GPDModule;

class BaseModuleFactory {
    /**
     * Private pointer of this class for a unique instance
     */
    static BaseModuleFactory* pInstance;

    /**
     * Private default constructor for a unique instance
     */
    BaseModuleFactory();

    std::map<std::string, BaseModule*> modules; ///< list of all available modules

public:

    /**
     * Share a unique pointer of this class
     */
    static BaseModuleFactory* getInstance();

    /**
     * Default destructor
     */
    virtual ~BaseModuleFactory();

//    /**
//     * Store a unique instance of a module identified by a unique string character key.
//     *
//     * @param _ID : a unique string identifier that represents the name of the module.
//     * @param _pBaseModule : an instance of the module built by its default constructor.
//     * @return Not used. It's for enable registerModule() to be executed before the main() function and auto-register all available module.
//     */
//    bool registerModule(const std::string &_ID, BaseModule* _pBaseModule);

    /**
     * Store a unique instance of a module identified by a unique string character key.
     * @param _pBaseModule: an instance of the module built by its default constructor.
     * @return Not used. It's for enable registerModule() to be executed before the main() function and auto-register all available module.
     */
    bool registerModule(BaseModule* _pBaseModule);

    BaseModule* getModule(std::string _ID);
    GPDModule* getGPDModule(std::string _ID);
    CFFModule* getCFFModule(std::string _ID);
};

#endif /* BASE_MODULE_FACTORY_H */
