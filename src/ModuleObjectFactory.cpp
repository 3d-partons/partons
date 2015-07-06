#include "ModuleObjectFactory.h"

#include "BaseObjectFactory.h"
#include "modules/CoefficientFunctionModule.h"
#include "modules/EvolQCDModule.h"
#include "modules/GPDModule.h"
#include "modules/MathIntegratorModule.h"
#include "modules/observable/DVCSModule.h"
#include "modules/RunningAlphaStrongModule.h"

GPDModule* ModuleObjectFactory::newGPDModule(unsigned int classId) {
    return static_cast<GPDModule*>(BaseObjectFactory::newBaseObject(classId));
}

GPDModule* ModuleObjectFactory::newGPDModule(const std::string& className) {
    return static_cast<GPDModule*>(BaseObjectFactory::newBaseObject(className));
}

CoefficientFunctionModule* ModuleObjectFactory::newCoefficientFunctionModule(
        unsigned int classId) {
    return static_cast<CoefficientFunctionModule*>(BaseObjectFactory::newBaseObject(
            classId));
}

CoefficientFunctionModule* ModuleObjectFactory::newCoefficientFunctionModule(
        const std::string& className) {
    return static_cast<CoefficientFunctionModule*>(BaseObjectFactory::newBaseObject(
            className));
}

ObservableModule* ModuleObjectFactory::newObservableModule(
        unsigned int classId) {
    return static_cast<ObservableModule*>(BaseObjectFactory::newBaseObject(
            classId));
}

ObservableModule* ModuleObjectFactory::newObservableModule(
        const std::string& className) {
    return static_cast<ObservableModule*>(BaseObjectFactory::newBaseObject(
            className));
}

EvolQCDModule* ModuleObjectFactory::newEvolQCDModule(unsigned int classId) {
    return static_cast<EvolQCDModule*>(BaseObjectFactory::newBaseObject(classId));
}

EvolQCDModule* ModuleObjectFactory::newEvolQCDModule(
        const std::string& className) {
    return static_cast<EvolQCDModule*>(BaseObjectFactory::newBaseObject(
            className));
}

DVCSModule* ModuleObjectFactory::newDVCSModule(unsigned int classId) {
    return static_cast<DVCSModule*>(BaseObjectFactory::newBaseObject(classId));
}

DVCSModule* ModuleObjectFactory::newDVCSModule(const std::string& className) {
    return static_cast<DVCSModule*>(BaseObjectFactory::newBaseObject(className));
}

MathIntegratorModule* ModuleObjectFactory::newMathIntegratorModule(
        unsigned int classId) {
    return static_cast<MathIntegratorModule*>(BaseObjectFactory::newBaseObject(
            classId));
}

MathIntegratorModule* ModuleObjectFactory::newMathIntegratorModule(
        const std::string& className) {
    return static_cast<MathIntegratorModule*>(BaseObjectFactory::newBaseObject(
            className));
}

RunningAlphaStrongModule* ModuleObjectFactory::newRunningAlphaStrongModule(
        unsigned int classId) {
    return static_cast<RunningAlphaStrongModule*>(BaseObjectFactory::newBaseObject(
            classId));
}

RunningAlphaStrongModule* ModuleObjectFactory::newRunningAlphaStrongModule(
        const std::string& className) {
    return static_cast<RunningAlphaStrongModule*>(BaseObjectFactory::newBaseObject(
            className));
}
