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
 * @brief
 */

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

    /**
     * Store a unique instance of a module identified by a unique string character key.
     * @param _pBaseModule: an instance of the module built by its default constructor.
     * @return Classname of the module
     */
    unsigned int registerBaseObject(BaseObject * pBaseObject);

    // We need to perform this task after self registered BaseObject to resolve dependencies between pointed objects.
    void initBaseObject();

    BaseObject* get(unsigned int classId);
    BaseObject* get(const std::string &className);

    std::string toString();

private:
    std::map<unsigned int, BaseObject*> m_baseObjectList;
    std::map<unsigned int, BaseObject*>::iterator m_itBaseObjectList;

    std::map<std::string, BaseObject*> m_translate;
    std::map<std::string, BaseObject*>::iterator m_itTranslate;

    static unsigned int m_uniqueClassIdCounter;

    unsigned int getUniqueClassId();

    /**
     * Private pointer of this class for a unique instance
     */
    static BaseObjectRegistry* m_pInstance;

    /**
     * Private default constructor for a unique instance
     */
    BaseObjectRegistry();

    bool isAvailable(const std::string &className);

    // Stop the compiler generating methods of copy the object
    BaseObjectRegistry(BaseObjectRegistry const& other); // Not Implemented
    BaseObjectRegistry& operator=(BaseObjectRegistry const& other); // Not Implemented
};

#endif /* BASE_OBJECT_REGISTRY_H */
