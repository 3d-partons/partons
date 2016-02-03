#include "../../include/partons/ServiceObjectRegistry.h"

#include <stdexcept>

#include "../../include/partons/BaseObjectRegistry.h"
#include "../../include/partons/services/DoubleDistributionService.h"
#include "../../include/partons/services/DVCSConvolCoeffFunctionService.h"
#include "../../include/partons/services/GPDService.h"
#include "../../include/partons/services/ObservableService.h"
#include "../../include/partons/services/VizualisationService.h"

ServiceObjectRegistry::ServiceObjectRegistry(
        BaseObjectRegistry* m_pBaseObjectRegistry) :
        m_pBaseObjectRegistry(m_pBaseObjectRegistry) {
}

ServiceObjectRegistry::~ServiceObjectRegistry() {
}

ServiceObject* ServiceObjectRegistry::get(unsigned int classId) const {
    // Check if m_pBaseObjectRegistry is NULL pointer before trying to get related classId object from it.
    checkBaseObjectRegistryNullPointer();
    return static_cast<ServiceObject*>(ServiceObjectRegistry::m_pBaseObjectRegistry->get(
            classId));
}

ServiceObject* ServiceObjectRegistry::get(const std::string &className) const {
    // Check if m_pBaseObjectRegistry is NULL pointer before trying to get related classId object from it.
    checkBaseObjectRegistryNullPointer();
    return static_cast<ServiceObject*>(ServiceObjectRegistry::m_pBaseObjectRegistry->get(
            className));
}

GPDService* ServiceObjectRegistry::getGPDService() const {
    return static_cast<GPDService*>(get(GPDService::classId));
}

void ServiceObjectRegistry::checkBaseObjectRegistryNullPointer() const {
    if (ServiceObjectRegistry::m_pBaseObjectRegistry == 0) {
        throw std::runtime_error(
                "[ServiceObjectRegistry::checkBaseObjectRegistryNullPointer] m_pBaseObjectRegistry is NULL pointer, cannot run application !");
    }
}

//TODO tester le cast BaseObject -> ServoceObject -> CoefficientFunctionObject
DVCSConvolCoeffFunctionService* ServiceObjectRegistry::getConvolCoeffFunctionService() const {
    return static_cast<DVCSConvolCoeffFunctionService*>(get(
            DVCSConvolCoeffFunctionService::classId));
}

ObservableService* ServiceObjectRegistry::getObservableService() const {
    return static_cast<ObservableService*>(get(ObservableService::classId));
}

VizualisationService* ServiceObjectRegistry::getVizualisationService() const {
    return static_cast<VizualisationService*>(get(VizualisationService::classId));
}

DoubleDistributionService* ServiceObjectRegistry::getDoubleDistributionService() const {
    return static_cast<DoubleDistributionService*>(get(
            DoubleDistributionService::classId));
}

