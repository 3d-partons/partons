/*
 * BaseObjectFactory.h
 *
 *  Created on: Jun 26, 2015
 *      Author: Bryan BERTHOU
 */

#include <map>

#include "BaseObject.h"

#ifndef BASE_OBJECT_FACTORY_H_
#define BASE_OBJECT_FACTORY_H

#include <string>

class BaseObject;
class BaseObjectRegistry;

/*
 *
 */
class BaseObjectFactory {
public:
    /**
     * Share a unique pointer of this class
     */
    static BaseObjectFactory* getInstance();

    /**
     * Default destructor
     */
    virtual ~BaseObjectFactory();

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

    /// Store BaseObject pointer created by the factory; used at the end of the program to delete orphan pointer.
    std::map<unsigned int, BaseObject*> m_pInstantiatedObject;

    BaseObjectRegistry* m_pBaseObjectRegistry;

    void store(BaseObject* pBaseObject);
};

#endif /* BASE_OBJECT_FACTORY_H */
