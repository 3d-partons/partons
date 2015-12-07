#include "../../include/partons/ModuleObjectFactory.h"

#include "../../include/partons/BaseObjectFactory.h"
#include "../../include/partons/modules/ActiveFlavorsModule.h"
#include "../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../include/partons/modules/DoubleDistributionModule.h"
#include "../../include/partons/modules/evolution/GPDEvolutionModule.h"
#include "../../include/partons/modules/GPDModule.h"
#include "../../include/partons/modules/process/DVCSModule.h"
#include "../../include/partons/modules/RunningAlphaStrongModule.h"
#include "../../include/partons/modules/scale/ScaleModule.h"
#include "../../include/partons/modules/xb_to_xi/XiConverterModule.h"

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

ProcessModule* ModuleObjectFactory::newObservableModule(unsigned int classId) {
    return static_cast<ProcessModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            classId));
}

ProcessModule* ModuleObjectFactory::newObservableModule(
        const std::string& className) {
    return static_cast<ProcessModule*>(BaseObjectFactory::getInstance()->newBaseObject(
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

ScaleModule* ModuleObjectFactory::newScaleModule(unsigned int classId) {
    return static_cast<ScaleModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            classId));
}

ScaleModule* ModuleObjectFactory::newScaleModule(const std::string &className) {
    return static_cast<ScaleModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            className));
}

XiConverterModule* ModuleObjectFactory::newXiConverterModule(
        unsigned int classId) {
    return static_cast<XiConverterModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            classId));
}
XiConverterModule* ModuleObjectFactory::newXiConverterModule(
        const std::string &className) {
    return static_cast<XiConverterModule*>(BaseObjectFactory::getInstance()->newBaseObject(
            className));
}
