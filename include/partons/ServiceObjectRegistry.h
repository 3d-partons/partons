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

    static ServiceObject* get(unsigned int classId);
    static ServiceObject* get(const std::string &className);

    static GPDService* getGPDService();
    static DVCSConvolCoeffFunctionService* getConvolCoeffFunctionService();
    static ObservableService* getObservableService();
    static VizualisationService* getVizualisationService();
    static DoubleDistributionService* getDoubleDistributionService();

private:

    static BaseObjectRegistry* m_pBaseObjectRegistry;

    static void checkBaseObjectRegistryNullPointer();
};

#endif /* SERVICE_OBJECT_REGISTRY_H */
