#ifndef BASE_OBJECT_FACTORY_H
#define BASE_OBJECT_FACTORY_H

/**
 * @file BaseObjectFactory.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date June 26, 2015
 * @version 1.0
 */

#include <SFML/System/Mutex.hpp>
#include <map>
#include <string>

#include "BaseObject.h"

namespace PARTONS {

class BaseObjectRegistry;

/**
 * @class BaseObjectFactory
 *
 * @brief Provides a clone (returned as a BaseObject pointer) of an object identified by its class name and previously stored in the BaseObjectRegistry.
 * Furthermore, a pointer to each clone is registered in order to destroy it later on if it becomes orphan.\n
 * This class is only instantiable by the class Partons to avoid any error at the initialization of the program.
 */
class BaseObjectFactory {
public:
    /**
     * Default destructor.
     */
    virtual ~BaseObjectFactory();

    /**
     * Provides a clone of the desired object class stored in the registry identified by its integer identifier.
     *
     * @param classId
     * @return
     */
    BaseObject* newBaseObject(unsigned int classId);

    /**
     * Provides a clone of the desired object class stored in the registry identified by its string class name.
     *
     * @param classId
     * @return
     */
    BaseObject* newBaseObject(const std::string &className);

    /**
     * Remove an object from the factory.
     *
     * @param baseObjectUniqueId Unique id identifying each object.
     */
    void removeFromStore(unsigned int baseObjectUniqueId);

private:
    // To allow only Partons class to create a unique of this class.
    // Used to avoid multiple singleton class and to avoid multithreading problem especially when getInstance() is called.
    // There is a bad behaviour with first instance initialization and mutex.
    friend class Partons;

    sf::Mutex m_mutex; ///< Mutex to secure concurrent access to the map of instanciated obejcts.

    BaseObjectRegistry* m_pBaseObjectRegistry; ///< Pointer to ask BaseObjectRegistry pointer associated to a specific object class.

    std::map<unsigned int, BaseObject*> m_pInstantiatedObject; ///< Store BaseObject pointer created by the factory; used at the end of the program to delete orphan pointer.

    /**
     * Private default constructor to ensure the creation of a single instance of the class, managed by Parton's class.
     *
     * @param pBaseObjectRegistry
     */
    BaseObjectFactory(BaseObjectRegistry* pBaseObjectRegistry);

    /**
     * Store cloned object pointer to its instantiated object map.
     *
     * @param pBaseObject
     */
    void store(BaseObject* pBaseObject);
};

} /* namespace PARTONS */

#endif /* BASE_OBJECT_FACTORY_H */
