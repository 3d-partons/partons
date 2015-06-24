/*
 * BaseObjectRegistry.h
 *
 *  Created on: Jun 23, 2015
 *      Author: Bryan BERTHOU
 */

#ifndef BASE_OBJECT_REGISTRY_H
#define BASE_OBJECT_REGISTRY_H

#include <map>
#include <string>

#include "BaseObject.h"

class BaseObjectRegistry {
public:
    /**
     * Share a unique pointer of this class
     */
    static BaseObjectRegistry* getInstance();

    /**
     * Default destructor
     */
    virtual ~BaseObjectRegistry();

    void add(BaseObject* baseObject);
    BaseObject* get(const std::string & baseObjectName);

private:
    /**
     * Private pointer of this class for a unique instance
     */
    static BaseObjectRegistry* m_pInstance;

    /**
     * Default constructor
     */
    BaseObjectRegistry();

    std::map<std::string, BaseObject*> m_registry; ///< list of all available object ; an object is identified by its ID
    std::map<std::string, BaseObject*>::iterator m_it; ///< iterator to the list of object
};

#endif /* BASE_OBJECT_REGISTRY_H */
