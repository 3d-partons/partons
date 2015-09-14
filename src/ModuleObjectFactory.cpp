#include "ModuleObjectFactory.h"

#include "BaseObjectFactory.h"
#include "modules/ActiveFlavorsModule.h"
#include "modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "modules/DoubleDistributionModule.h"
#include "modules/evolution/GPDEvolutionModule.h"
#include "modules/GPDModule.h"
#include "modules/observable/DVCSModule.h"
#include "modules/RunningAlphaStrongModule.h"

DoubleDistributionModule* ModuleObjectFactory::newDoubleDistributionModule(
        unsigned int classId) {
    return static_cast<DoubleDistributionModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            classId));
}

DoubleDistributionModule* ModuleObjectFactory::newDoubleDistributionModule(
        const std::string& className) {
    return static_cast<DoubleDistributionModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            className));
}

GPDEvolutionModule* ModuleObjectFactory::newGPDEvolutionModule(
        unsigned int classId) {
    return static_cast<GPDEvolutionModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            classId));
}

GPDEvolutionModule* ModuleObjectFactory::newGPDEvolutionModule(
        const std::string& className) {
    return static_cast<GPDEvolutionModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            className));
}

GPDModule* ModuleObjectFactory::newGPDModule(unsigned int classId) {
    return static_cast<GPDModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            classId));
}

GPDModule* ModuleObjectFactory::newGPDModule(const std::string& className) {
    return static_cast<GPDModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            className));
}

DVCSConvolCoeffFunctionModule* ModuleObjectFactory::newDVCSConvolCoeffFunctionModule(
        unsigned int classId) {
    return static_cast<DVCSConvolCoeffFunctionModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            classId));
}

DVCSConvolCoeffFunctionModule* ModuleObjectFactory::newDVCSConvolCoeffFunctionModule(
        const std::string& className) {
    return static_cast<DVCSConvolCoeffFunctionModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            className));
}

ObservableModule* ModuleObjectFactory::newObservableModule(
        unsigned int classId) {
    return static_cast<ObservableModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            classId));
}

ObservableModule* ModuleObjectFactory::newObservableModule(
        const std::string& className) {
    return static_cast<ObservableModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            className));
}

DVCSModule* ModuleObjectFactory::newDVCSModule(unsigned int classId) {
    return static_cast<DVCSModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            classId));
}

DVCSModule* ModuleObjectFactory::newDVCSModule(const std::string& className) {
    return static_cast<DVCSModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            className));
}

RunningAlphaStrongModule* ModuleObjectFactory::newRunningAlphaStrongModule(
        unsigned int classId) {
    return static_cast<RunningAlphaStrongModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            classId));
}

RunningAlphaStrongModule* ModuleObjectFactory::newRunningAlphaStrongModule(
        const std::string& className) {
    return static_cast<RunningAlphaStrongModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            className));
}

ActiveFlavorsModule* ModuleObjectFactory::newActiveFlavorsModule(
        unsigned int classId) {
    return static_cast<ActiveFlavorsModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            classId));
}

ActiveFlavorsModule* ModuleObjectFactory::newActiveFlavorsModule(
        const std::string &className) {
    return static_cast<ActiveFlavorsModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            className));
}
