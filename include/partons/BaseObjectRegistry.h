#ifndef BASE_OBJECT_REGISTRY_H
#define BASE_OBJECT_REGISTRY_H

/**
 * @file BaseObjectRegistry.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 25 June 2015
 * @version 1.0
 *
 * @class BaseObjectRegistry
 *
 * @brief The Registry is the analog of a phonebook, which lists all available modules identified by a unique id or by a unique string for translation. And only one species of each.
 *
 * From the point of view of software engineering, it corresponds to the singleton design pattern which ensures that it is unique.
 * When a new module is created, the first thing to do is to call this unique instance, and to register the new module with a name provided by the developer of the module.
 * In turn the Registry gives a unique identifier encoded in a int variable for performance purposes.
 * Registry stores pointers to all modules in a generic way, i.e. whatever their nature are: pointers to GPDModule, to RunningAlphaStrongModule, to DoubleDistributionModule, etc.
 * This is achieved by requiring all modules to derive from a single parent class named BaseObject.
 */

#include <stddef.h>
#include <SFML/System/Mutex.hpp>
#include <map>
#include <string>

#include "BaseObject.h"

class BaseObjectRegistry {
public:
    friend class Partons;

    /**
     * Default destructor
     */
    virtual ~BaseObjectRegistry();

    /**
     * Store a unique instance of a module identified by a unique string character key.
     * @param pBaseObject: an instance of the module built by its default constructor.
     * @return A unique system identifier ; unsigned int.
     */
    unsigned int registerBaseObject(BaseObject * pBaseObject);

    /**
     * We need to perform this task after self registered BaseObject to resolve dependencies between pointed objects.
     */
    void initBaseObject();

    BaseObject* get(unsigned int classId);
    BaseObject* get(const std::string &className);

    std::string toString();

    size_t size();

private:
    sf::Mutex m_mutex;

    std::map<unsigned int, BaseObject*> m_baseObjectList;
    std::map<unsigned int, BaseObject*>::iterator m_itBaseObjectList;

    std::map<std::string, BaseObject*> m_translate;
    std::map<std::string, BaseObject*>::iterator m_itTranslate;

    static unsigned int m_uniqueClassIdCounter;

    /**
     * Private default constructor for a unique instance
     */
    BaseObjectRegistry();

    unsigned int getUniqueClassId();

    bool isAvailable(const std::string &className);

    // Stop the compiler generating methods of copy the object
    BaseObjectRegistry(BaseObjectRegistry const& other); // Not Implemented
    BaseObjectRegistry& operator=(BaseObjectRegistry const& other); // Not Implemented
};

#endif /* BASE_OBJECT_REGISTRY_H */
