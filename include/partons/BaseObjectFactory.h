#ifndef BASE_OBJECT_FACTORY_H_
#define BASE_OBJECT_FACTORY_H

/**
 * @file BaseObjectFactory.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 June 2015
 * @version 1.0
 *
 * @class BaseObjectFactory
 *
 * @brief
 */

#include <SFML/System/Mutex.hpp>
#include <map>
#include <string>

#include "BaseObject.h"

class BaseObjectRegistry;

class BaseObjectFactory {
public:
    /**
     * Default destructor
     */
    virtual ~BaseObjectFactory();

    BaseObject* newBaseObject(unsigned int classId);
    BaseObject* newBaseObject(const std::string &className);

    void removeFromStore(unsigned int baseObjectUniqueId);

private:
    // To allow only Partons class to create a new instance of this class.
    // Used to avoid multiple singleton class and to avoid multithreading problem especially when getInstance() is called.
    // There is a bad behaviour with first instance initialization and mutex.
    friend class Partons;

    /**
     * Private default constructor to ensure the creation of a single instance of the class, managed by Parton's class.
     *
     * @param pBaseObjectRegistry
     */
    BaseObjectFactory(BaseObjectRegistry* pBaseObjectRegistry);

    sf::Mutex m_mutex;

    BaseObjectRegistry* m_pBaseObjectRegistry;

    /// Store BaseObject pointer created by the factory; used at the end of the program to delete orphan pointer.
    std::map<unsigned int, BaseObject*> m_pInstantiatedObject;

    void store(BaseObject* pBaseObject);
};

#endif /* BASE_OBJECT_FACTORY_H */
