#ifndef BASE_OBJECT_REGISTRY_H
#define BASE_OBJECT_REGISTRY_H

/**
 * @file BaseObjectRegistry.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date June 25, 2015
 * @version 1.0
 */

#include <stddef.h>
#include <SFML/System/Mutex.hpp>
#include <map>
#include <string>

#include "BaseObject.h"

/** @class BaseObjectRegistry
 *
 * @brief The Registry is the analog of a phonebook, which lists all available objects (modules or services most of the time) identified by a unique integer identifier or by a unique string (class name) for translation. And only one species of each.
 *
 * From the point of view of software engineering, the registry corresponds to the singleton design pattern which ensures that it is unique.
 * When a new module or services are created, the first thing to do is to call this unique instance, and to register the new module or services with a name provided by the developer the class.
 * In turn the Registry gives a unique identifier encoded in a integer variable for performance purposes.
 * Registry stores pointers to all objects in a generic way, i.e. whatever their nature are: pointers to GPDModule, to RunningAlphaStrongModule, to AutomationService, etc.
 * This is achieved by requiring all objects to derive from a single parent class named BaseObject.
 */
class BaseObjectRegistry {
public:
    /**
     * Static function to be able to retrieve a unique instance pointer of this class anywhere in the code.
     *
     * @return a unique instance of this class
     */
    static BaseObjectRegistry* getInstance();

    /**
     * Default destructor
     */
    virtual ~BaseObjectRegistry();

    /**
     * Store a unique instance of a module identified by a unique string character key.
     * @param pBaseObject: an instance of the module built by its default constructor.
     * @return A unique identifier by class
     */
    unsigned int registerBaseObject(BaseObject * pBaseObject);

    /**
     * Some objects like modules depend on each other.
     * But it is impossible to guarantee the order in which they are added to the registry will be made upon the resolution of statics variables (mechanism of self-registration)
     * So we need to perform this task just before the use of PARTONS software to resolve dependencies between all kind of registered objects.
     */
    void resolveBaseObjectDependencies();

    /**
     * Try to find stored object identified by its unique class identifier.
     *
     * @param classId
     * @return reference to object in memory or throw an exception
     */
    BaseObject* get(unsigned int classId) const;

    /**
     * Try to find stored object identified by its class name.
     *
     * @param className
     * @return reference to object in memory or throw an exception
     */
    BaseObject* get(const std::string &className) const;

    /**
     * Print information about current state of the registry (size of the registry, which objects are stored, ...)
     *
     * @return string
     */
    virtual std::string toString() const;

    /**
     * Return the size of the registry.
     * How many objects are stored.
     *
     * @return registry size
     */
    size_t size() const;

private:
    /**
     * Private pointer of this class for a unique instance
     */
    static BaseObjectRegistry* m_pInstance;

    /**
     * Private default constructor for a unique instance
     */
    BaseObjectRegistry();

    /**
     * Because of PARTONS is a program using threads we must ensure that only one object manipulates the registry at the same time to avoid memory access violation.
     * It's performed by a mechanism called mutex or semaphore.
     */
    mutable sf::Mutex m_mutex;

    std::map<unsigned int, BaseObject*> m_baseObjectList; ///< list of registered objects identified by their unique integer identifier
    std::map<std::string, BaseObject*> m_translateList; ///< list of registered objects identified by their class name.

    static unsigned int m_uniqueClassIdCounter; ///< Increment unique class identifier

    /**
     * To performance purpose it's faster to compare two integers than two string when we search for an object in the registry.
     * This method return a unique integer identifier when a new class object his added to the registry.
     * It's just an integer incremented one by one.
     *
     * @return unique identifier by class
     */
    unsigned int getUniqueClassId();

    /**
     * Try to find an object into the registry by its class name
     *
     * @param className
     * @return pointer to object if class name found into registry, NULL else.
     */
    BaseObject* isAvailable(const std::string &className) const;

    /**
     * Try to find an object into the registry by its class identifier
     *
     * @param classId
     * @return pointer to object if class name found into registry, NULL else.
     */
    BaseObject* isAvailable(const unsigned int classId) const;

    /**
     * Return last referenced object by the iterator on m_translateList.
     *
     * @return reference to the object in memory
     */
    BaseObject* getLastAvailableObjectIdentifiedByClassName() const;

    /**
     * Return last referenced object by the iterator on m_baseObjectList.
     *
     * @return reference to the object in memory
     */
    BaseObject* getLastAvailableObjectIdentifiedByClassId() const;

    // Stop the compiler generating methods of copy the object
    BaseObjectRegistry(BaseObjectRegistry const& other); // Not Implemented
    BaseObjectRegistry& operator=(BaseObjectRegistry const& other); // Not Implemented
};

#endif /* BASE_OBJECT_REGISTRY_H */
