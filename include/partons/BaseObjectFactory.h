#ifndef BASE_OBJECT_FACTORY_H_
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

class BaseObjectRegistry;

/**
 * @class BaseObjectFactory
 *
 * @brief Cette classe n'est instenciable que par la classe Partons pour éviter des erreur à l'initialisation du programme.
 * Fournis un clone de l'objet (sous la forme d'un pointer BaseObject), identifié par son nom de classe, stocké dans la BaseObjectResgistry.
 * De plus, un pointer vers chaque objet instancié est mémorisé pour sa destruction ultérieur si l'objet se retrouve orphelin.
 */
class BaseObjectFactory {
public:
    /**
     * Default destructor
     */
    virtual ~BaseObjectFactory();

    /**
     * Provide a clone of the desired object class stored in the registry identified by its integer identifier.
     *
     * @param classId
     * @return
     */
    BaseObject* newBaseObject(unsigned int classId);

    /**
     * Provide a clone of the desired object class stored in the registry identified by its string class name.
     *
     * @param classId
     * @return
     */
    BaseObject* newBaseObject(const std::string &className);

    /**
     *
     *
     * @param baseObjectUniqueId
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

#endif /* BASE_OBJECT_FACTORY_H */
