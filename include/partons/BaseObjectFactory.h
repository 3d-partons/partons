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

#include <map>
#include <string>

#include "BaseObject.h"

class BaseObjectRegistry;

class BaseObjectFactory {
public:
    /**
     * Share a unique pointer of this class
     */
    static BaseObjectFactory* getInstance();

    void delete_();

    BaseObject* newBaseObject(unsigned int classId);
    BaseObject* newBaseObject(const std::string &className);

    void removeFromStore(unsigned int baseObjectUniqueId);

private:
    /**
     * Private pointer of this class for a unique instance
     */
    static BaseObjectFactory* m_pInstance;

    /**
     * Private default constructor for a unique instance
     */
    BaseObjectFactory();

    /**
     * Default destructor
     */
    virtual ~BaseObjectFactory();

    /// Store BaseObject pointer created by the factory; used at the end of the program to delete orphan pointer.
    std::map<unsigned int, BaseObject*> m_pInstantiatedObject;

    BaseObjectRegistry* m_pBaseObjectRegistry;

    void store(BaseObject* pBaseObject);
};

#endif /* BASE_OBJECT_FACTORY_H */
