#ifndef SERVICE_OBJECT_REGISTRY_H
#define SERVICE_OBJECT_REGISTRY_H

/**
 * @file ServiceObjectRegistry.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 01 July 2015
 * @version 1.0
 *
 * @class ServiceObjectRegistry
 *
 * @brief
 */

#include <string>

class BaseObjectRegistry;
class DoubleDistributionService;
class DVCSConvolCoeffFunctionService;
class GPDService;
class ObservableService;
class ServiceObject;
class VizualisationService;

class ServiceObjectRegistry {
public:
    virtual ~ServiceObjectRegistry();

    ServiceObject* get(unsigned int classId) const;
    ServiceObject* get(const std::string &className) const;

    GPDService* getGPDService() const;
    DVCSConvolCoeffFunctionService* getConvolCoeffFunctionService() const;
    ObservableService* getObservableService() const;
    VizualisationService* getVizualisationService() const;
    DoubleDistributionService* getDoubleDistributionService() const;

private:
    friend class Partons;

    BaseObjectRegistry* m_pBaseObjectRegistry;

    ServiceObjectRegistry(BaseObjectRegistry* m_pBaseObjectRegistry);

    void checkBaseObjectRegistryNullPointer() const;
};

#endif /* SERVICE_OBJECT_REGISTRY_H */
