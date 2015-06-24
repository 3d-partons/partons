/*
 * BaseObjectManager.h
 *
 *  Created on: Jun 23, 2015
 *      Author: Bryan BERTHOU
 */

#ifndef BASEOBJECTMANAGER_H_
#define BASEOBJECTMANAGER_H_

#include <map>

#include "BaseObject.h"

class BaseObjectManager {
public:
    /**
     * Share a unique pointer of this class
     */
    static BaseObjectManager* getInstance();

    virtual ~BaseObjectManager();

    void add(BaseObject* pBaseObject);

private:
    /**
     * Private pointer of this class for a unique instance
     */
    static BaseObjectManager* m_pInstance;

    BaseObjectManager();

    std::map<unsigned int, BaseObject*> m_instantiatedObject;
    std::map<unsigned int, BaseObject*>::iterator m_it;
};

#endif /* BASEOBJECTMANAGER_H_ */
