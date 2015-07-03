
#include "ModuleObjectFactory.h"

#include "BaseObjectFactory.h"
#include "modules/CoefficientFunctionModule.h"
#include "modules/EvolQCDModule.h"
#include "modules/GPDModule.h"
#include "modules/MathIntegratorModule.h"
#include "modules/observable/DVCSModule.h"
#include "modules/RunningAlphaStrongModule.h"

BaseObjectFactory* ModuleObjectFactory::m_pBaseObjectFactory =
        BaseObjectFactory::getInstance();

GPDModule* ModuleObjectFactory::newGPDModule(unsigned int classId) {
    return static_cast<GPDModule*>(ModuleObjectFactory::m_pBaseObjectFactory->newBaseObject(
            classId));
}

GPDModule* ModuleObjectFactory::newGPDModule(const std::string& className) {
    return static_cast<GPDModule*>(ModuleObjectFactory::m_pBaseObjectFactory->newBaseObject(
            className));
}

CoefficientFunctionModule* ModuleObjectFactory::newCoefficientFunctionModule(
        unsigned int classId) {
    return static_cast<CoefficientFunctionModule*>(ModuleObjectFactory::m_pBaseObjectFactory->newBaseObject(
            classId));
}

CoefficientFunctionModule* ModuleObjectFactory::newCoefficientFunctionModule(
        const std::string& className) {
    return static_cast<CoefficientFunctionModule*>(ModuleObjectFactory::m_pBaseObjectFactory->newBaseObject(
            className));
}

ObservableModule* ModuleObjectFactory::newObservableModule(
        unsigned int classId) {
    return static_cast<ObservableModule*>(ModuleObjectFactory::m_pBaseObjectFactory->newBaseObject(
            classId));
}

ObservableModule* ModuleObjectFactory::newObservableModule(
        const std::string& className) {
    return static_cast<ObservableModule*>(ModuleObjectFactory::m_pBaseObjectFactory->newBaseObject(
            className));
}

EvolQCDModule* ModuleObjectFactory::newEvolQCDModule(unsigned int classId) {
    return static_cast<EvolQCDModule*>(ModuleObjectFactory::m_pBaseObjectFactory->newBaseObject(
            classId));
}

EvolQCDModule* ModuleObjectFactory::newEvolQCDModule(
        const std::string& className) {
    return static_cast<EvolQCDModule*>(ModuleObjectFactory::m_pBaseObjectFactory->newBaseObject(
            className));
}

DVCSModule* ModuleObjectFactory::newDVCSModule(unsigned int classId) {
    return static_cast<DVCSModule*>(ModuleObjectFactory::m_pBaseObjectFactory->newBaseObject(
            classId));
}

DVCSModule* ModuleObjectFactory::newDVCSModule(const std::string& className) {
    return static_cast<DVCSModule*>(ModuleObjectFactory::m_pBaseObjectFactory->newBaseObject(
            className));
}

MathIntegratorModule* ModuleObjectFactory::newMathIntegratorModule(
        unsigned int classId) {
    return static_cast<MathIntegratorModule*>(ModuleObjectFactory::m_pBaseObjectFactory->newBaseObject(
            classId));
}

MathIntegratorModule* ModuleObjectFactory::newMathIntegratorModule(
        const std::string& className) {
    return static_cast<MathIntegratorModule*>(ModuleObjectFactory::m_pBaseObjectFactory->newBaseObject(
            className));
}

RunningAlphaStrongModule* ModuleObjectFactory::newRunningAlphaStrongModule(
        unsigned int classId) {
    return static_cast<RunningAlphaStrongModule*>(ModuleObjectFactory::m_pBaseObjectFactory->newBaseObject(
            classId));
}

RunningAlphaStrongModule* ModuleObjectFactory::newRunningAlphaStrongModule(
        const std::string& className) {
    return static_cast<RunningAlphaStrongModule*>(ModuleObjectFactory::m_pBaseObjectFactory->newBaseObject(
            className));
}
