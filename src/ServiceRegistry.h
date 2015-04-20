#ifndef SERVICE_REGISTRY_H
#define SERVICE_REGISTRY_H

/**
 * @file ServiceRegistry.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 16 April 2015
 *
 * @class ServiceRegistry
 * @brief
 */

#include <map>
#include <string>

#include "services/Service.h"

class Service;

class ServiceRegistry {
public:
    /**
     * Share a unique pointer of this class
     */
    static ServiceRegistry* getInstance();

    /**
     * Default destructor
     */
    virtual ~ServiceRegistry();

    /**
     *
     *
     * @param pService
     * @return previous ID
     */
    std::string registerNewService(Service * pService);

    /**
     *
     * @param serviceName
     * @return pointer to the desired service
     */
    Service* getService(const std::string & serviceName);

private:

    /**
     * Private pointer of this class for a unique instance
     */
    static ServiceRegistry* m_pInstance;

    /**
     * Default destructor
     */
    ServiceRegistry();

    std::map<std::string, Service*> m_registry; ///< list of all available service ; a service is identified by its ID
    std::map<std::string, Service*>::iterator m_it; ///< iterator to the list of service
};

#endif /* SERVICE_REGISTRY_H */
