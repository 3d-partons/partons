#include "ServiceRegistry.h"

#include <stdexcept>
#include <utility>

#include "modules/ModuleObject.h"

// Global static pointer used to ensure a single instance of the class.
ServiceRegistry* ServiceRegistry::m_pInstance = 0;

ServiceRegistry::ServiceRegistry() {
}

ServiceRegistry* ServiceRegistry::getInstance() {
    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new ServiceRegistry();
    }

    return m_pInstance;
}

ServiceRegistry::~ServiceRegistry() {

    // m_registry never delete'ed. (exist until program termination)
    // because we can't guarantee correct destruction order
    for (m_it = m_registry.begin(); m_it != m_registry.end(); m_it++) {
        delete (m_it->second);
        (m_it->second) = 0;
    }

    if (m_pInstance != 0) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

std::string ServiceRegistry::registerNewService(Service * pService) {
    m_it = m_registry.find(pService->getClassName());
    if (m_it == m_registry.end()) {
        m_registry.insert(
                std::pair<std::string, Service*>(
                        pService->getClassName(), pService));
    } else {
        throw std::runtime_error(
                "[ServiceRegistry::registerNewService] Service className is already in the registry - Please check your id = "
                        + pService->getClassName());
    }

    return pService->getClassName();
}

Service* ServiceRegistry::getService(const std::string & serviceName) {
    return m_registry[serviceName];
}
