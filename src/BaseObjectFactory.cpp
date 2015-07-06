#include "BaseObjectFactory.h"

#include "BaseObject.h"
#include "BaseObjectRegistry.h"

BaseObjectRegistry* BaseObjectFactory::m_pBaseObjectregistry =
        BaseObjectRegistry::getInstance();

BaseObject* BaseObjectFactory::newBaseObject(unsigned int classId) {
    return m_pBaseObjectregistry->get(classId)->clone();
}

BaseObject* BaseObjectFactory::newBaseObject(const std::string& className) {
    return m_pBaseObjectregistry->get(className)->clone();
}
