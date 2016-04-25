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

class AutomationService;
class BaseObjectRegistry;
class ComparisonService;
class CryptographicHashService;
class DoubleDistributionService;
class ConvolCoeffFunctionService;
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
    ConvolCoeffFunctionService* getConvolCoeffFunctionService() const;
    ObservableService* getObservableService() const;
    VizualisationService* getVizualisationService() const;
    DoubleDistributionService* getDoubleDistributionService() const;
    ComparisonService* getComparisonService() const;
    AutomationService* getAutomationService() const;
    CryptographicHashService* getCryptographicHashService() const;

private:
    // To allow only Partons class to create a new instance of this class.
    // Used to avoid multiple singleton class and to avoid multithreading problem especially when getInstance() is called.
    // There is a bad behaviour with first instance initialization and mutex.
    friend class Partons;

    /**
     * Private default constructor to ensure the creation of a single instance of the class, managed by Parton's class.
     *
     * @param m_pBaseObjectRegistry
     */
    ServiceObjectRegistry(BaseObjectRegistry* m_pBaseObjectRegistry);

    BaseObjectRegistry* m_pBaseObjectRegistry;

    void checkBaseObjectRegistryNullPointer() const;
};

#endif /* SERVICE_OBJECT_REGISTRY_H */
