#include "ServiceObjectRegistry.h"

#include <stdexcept>

#include "BaseObjectRegistry.h"
#include "services/DoubleDistributionService.h"
#include "services/DVCSConvolCoeffFunctionService.h"
#include "services/GPDService.h"
#include "services/ObservableService.h"
#include "services/VizualisationService.h"

BaseObjectRegistry* ServiceObjectRegistry::m_pBaseObjectRegistry =
        BaseObjectRegistry::getInstance();

ServiceObject* ServiceObjectRegistry::get(unsigned int classId) {
    // Check if m_pBaseObjectRegistry is NULL pointer before trying to get related classId object from it.
    ServiceObjectRegistry::checkBaseObjectRegistryNullPointer();
    return static_cast<ServiceObject*>(ServiceObjectRegistry::m_pBaseObjectRegistry->get(
            classId));
}

ServiceObject* ServiceObjectRegistry::get(const std::string &className) {
    // Check if m_pBaseObjectRegistry is NULL pointer before trying to get related classId object from it.
    ServiceObjectRegistry::checkBaseObjectRegistryNullPointer();
    return static_cast<ServiceObject*>(ServiceObjectRegistry::m_pBaseObjectRegistry->get(
            className));
}

GPDService* ServiceObjectRegistry::getGPDService() {
    return static_cast<GPDService*>(get(GPDService::classId));
}

void ServiceObjectRegistry::checkBaseObjectRegistryNullPointer() {
    if (ServiceObjectRegistry::m_pBaseObjectRegistry == 0) {
        throw std::runtime_error(
                "[ServiceObjectRegistry::checkBaseObjectRegistryNullPointer] m_pBaseObjectRegistry is NULL pointer, cannot run application !");
    }
}

//TODO tester le cast BaseObject -> ServoceObject -> CoefficientFunctionObject
DVCSConvolCoeffFunctionService* ServiceObjectRegistry::getConvolCoeffFunctionService() {
    return static_cast<DVCSConvolCoeffFunctionService*>(get(
            DVCSConvolCoeffFunctionService::classId));
}

ObservableService* ServiceObjectRegistry::getObservableService() {
    return static_cast<ObservableService*>(get(ObservableService::classId));
}

VizualisationService* ServiceObjectRegistry::getVizualisationService() {
    return static_cast<VizualisationService*>(get(VizualisationService::classId));
}

DoubleDistributionService* ServiceObjectRegistry::getDoubleDistributionService() {
    return static_cast<DoubleDistributionService*>(get(
            DoubleDistributionService::classId));
}
