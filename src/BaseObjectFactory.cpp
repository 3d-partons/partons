#include "BaseObjectFactory.h"

#include "BaseObjectRegistry.h"
#include "modules/CoefficientFunctionModule.h"
#include "modules/EvolQCDModule.h"
#include "modules/GPDModule.h"
#include "modules/MathIntegratorModule.h"
#include "modules/observable/DVCSModule.h"
#include "modules/RunningAlphaStrongModule.h"

// Global static pointer used to ensure a single instance of the class.
BaseObjectFactory* BaseObjectFactory::m_pInstance = 0;

BaseObjectFactory::BaseObjectFactory() :
        m_pBaseObjectregistry(BaseObjectRegistry::getInstance()) {

}

BaseObjectFactory::~BaseObjectFactory() {
    if (m_pInstance != 0) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

BaseObjectFactory* BaseObjectFactory::getInstance() {
    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new BaseObjectFactory();
    }

    return m_pInstance;
}

BaseObject* BaseObjectFactory::newBaseObject(unsigned int classId) {
    return m_pBaseObjectregistry->get(classId)->clone();
}

BaseObject* BaseObjectFactory::newBaseObject(const std::string& className) {
    return m_pBaseObjectregistry->get(className)->clone();
}
