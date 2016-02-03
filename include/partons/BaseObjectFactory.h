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
    friend class Partons;

    virtual ~BaseObjectFactory();

    BaseObject* newBaseObject(unsigned int classId);
    BaseObject* newBaseObject(const std::string &className);

    void removeFromStore(unsigned int baseObjectUniqueId);

private:
    sf::Mutex m_mutex;

    /**
     * Private default constructor for a unique instance
     */
    BaseObjectFactory(BaseObjectRegistry* pBaseObjectRegistry);

    /// Store BaseObject pointer created by the factory; used at the end of the program to delete orphan pointer.
    std::map<unsigned int, BaseObject*> m_pInstantiatedObject;

    /// Pointer to unique instance of BaseObjectRegistry
    BaseObjectRegistry* m_pBaseObjectRegistry;

    void store(BaseObject* pBaseObject);
};

#endif /* BASE_OBJECT_FACTORY_H */
