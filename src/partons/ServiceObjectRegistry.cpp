#include "../../include/partons/ServiceObjectRegistry.h"

#include <ElementaryUtils/logger/CustomException.h>
//#include <stdexcept>

#include "../../include/partons/BaseObjectRegistry.h"
#include "../../include/partons/services/automation/AutomationService.h"
#include "../../include/partons/services/ComparisonService.h"
#include "../../include/partons/services/ConvolCoeffFunctionService.h"
#include "../../include/partons/services/DoubleDistributionService.h"
#include "../../include/partons/services/GPDService.h"
#include "../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../include/partons/services/ObservableService.h"
#include "../../include/partons/services/VizualisationService.h"

ServiceObjectRegistry::ServiceObjectRegistry(
        BaseObjectRegistry* m_pBaseObjectRegistry) :
        m_pBaseObjectRegistry(m_pBaseObjectRegistry) {
}

ServiceObjectRegistry::~ServiceObjectRegistry() {
    // Nothing to destroy
    // m_pBaseObjectRegistry pointer will be deleted by the Partons class
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

void ServiceObjectRegistry::checkBaseObjectRegistryNullPointer() const {
    if (!m_pBaseObjectRegistry) {
        throw ElemUtils::CustomException("ServiceObjectRegistry", __func__,
                "m_pBaseObjectRegistry is NULL pointer, cannot run application !");
    }
}

GPDService* ServiceObjectRegistry::getGPDService() const {
    return static_cast<GPDService*>(get(GPDService::classId));
}

ConvolCoeffFunctionService* ServiceObjectRegistry::getConvolCoeffFunctionService() const {
    return static_cast<ConvolCoeffFunctionService*>(get(
            ConvolCoeffFunctionService::classId));
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

ComparisonService* ServiceObjectRegistry::getComparisonService() const {
    return static_cast<ComparisonService*>(get(ComparisonService::classId));
}

AutomationService* ServiceObjectRegistry::getAutomationService() const {
    return static_cast<AutomationService*>(m_pBaseObjectRegistry->get(
            AutomationService::classId));
}

CryptographicHashService* ServiceObjectRegistry::getCryptographicHashService() const {
    return static_cast<CryptographicHashService*>(m_pBaseObjectRegistry->get(
            CryptographicHashService::classId));
}
