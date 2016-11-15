#include "../../include/partons/ModuleObjectFactory.h"

#include "../../include/partons/BaseObjectFactory.h"
#include "../../include/partons/modules/ActiveFlavorsModule.h"
#include "../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../include/partons/modules/dse/GapEquationSolverModule.h"
#include "../../include/partons/modules/DoubleDistributionModule.h"
#include "../../include/partons/modules/evolution/GPDEvolutionModule.h"
#include "../../include/partons/modules/GPDBorderFunctionModule.h"
#include "../../include/partons/modules/GPDSubtractionConstantModule.h"
#include "../../include/partons/modules/observable/Observable.h"
#include "../../include/partons/modules/overlap/IncompleteGPDModule.h"
#include "../../include/partons/modules/process/DVCSModule.h"
#include "../../include/partons/modules/radon_inverse/RadonInverseModule.h"
#include "../../include/partons/modules/RunningAlphaStrongModule.h"
#include "../../include/partons/modules/scale/ScaleModule.h"
#include "../../include/partons/modules/xb_to_xi/XiConverterModule.h"

ModuleObjectFactory::ModuleObjectFactory(BaseObjectFactory* pBaseObjectFactory) :
        m_pBaseObjectFactory(pBaseObjectFactory) {
}

ModuleObjectFactory::~ModuleObjectFactory() {
    // Nothing to destroy
    // m_pBaseObjectFactory pointer will be deleted by the Partons class
}

ModuleObject* ModuleObjectFactory::newModuleObject(unsigned int classId) {
    return static_cast<ModuleObject*>(m_pBaseObjectFactory->newBaseObject(
            classId));
}

ModuleObject* ModuleObjectFactory::newModuleObject(
        const std::string& className) {
    return static_cast<ModuleObject*>(m_pBaseObjectFactory->newBaseObject(
            className));
}

DoubleDistributionModule* ModuleObjectFactory::newDoubleDistributionModule(
        unsigned int classId) {
    return static_cast<DoubleDistributionModule*>(newModuleObject(classId));
}

DoubleDistributionModule* ModuleObjectFactory::newDoubleDistributionModule(
        const std::string& className) {
    return static_cast<DoubleDistributionModule*>(newModuleObject(className));
}

GPDEvolutionModule* ModuleObjectFactory::newGPDEvolutionModule(
        unsigned int classId) {
    return static_cast<GPDEvolutionModule*>(newModuleObject(classId));
}

GPDEvolutionModule* ModuleObjectFactory::newGPDEvolutionModule(
        const std::string& className) {
    return static_cast<GPDEvolutionModule*>(newModuleObject(className));
}

GPDModule* ModuleObjectFactory::newGPDModule(unsigned int classId) {
    return static_cast<GPDModule*>(newModuleObject(classId));
}

GPDModule* ModuleObjectFactory::newGPDModule(const std::string& className) {
    return static_cast<GPDModule*>(newModuleObject(className));
}

GPDBorderFunctionModule* ModuleObjectFactory::newGPDBorderFunctionModule(
        unsigned int classId) {
    return static_cast<GPDBorderFunctionModule*>(newModuleObject(classId));
}

GPDBorderFunctionModule* ModuleObjectFactory::newGPDBorderFunctionModule(
        const std::string& className) {
    return static_cast<GPDBorderFunctionModule*>(newModuleObject(className));
}

GPDSubtractionConstantModule* ModuleObjectFactory::newGPDSubtractionConstantModule(
        unsigned int classId) {
    return static_cast<GPDSubtractionConstantModule*>(newModuleObject(classId));
}

GPDSubtractionConstantModule* ModuleObjectFactory::newGPDSubtractionConstantModule(
        const std::string& className) {
    return static_cast<GPDSubtractionConstantModule*>(newModuleObject(className));
}

DVCSConvolCoeffFunctionModule* ModuleObjectFactory::newDVCSConvolCoeffFunctionModule(
        unsigned int classId) {
    return static_cast<DVCSConvolCoeffFunctionModule*>(newModuleObject(classId));
}

DVCSConvolCoeffFunctionModule* ModuleObjectFactory::newDVCSConvolCoeffFunctionModule(
        const std::string& className) {
    return static_cast<DVCSConvolCoeffFunctionModule*>(newModuleObject(
            className));
}

ProcessModule* ModuleObjectFactory::newObservableModule(unsigned int classId) {
    return static_cast<ProcessModule*>(newModuleObject(classId));
}

ProcessModule* ModuleObjectFactory::newObservableModule(
        const std::string& className) {
    return static_cast<ProcessModule*>(newModuleObject(className));
}

DVCSModule* ModuleObjectFactory::newProcessModule(unsigned int classId) {
    return static_cast<DVCSModule*>(newModuleObject(classId));
}

DVCSModule* ModuleObjectFactory::newProcessModule(
        const std::string& className) {
    return static_cast<DVCSModule*>(newModuleObject(className));
}

RunningAlphaStrongModule* ModuleObjectFactory::newRunningAlphaStrongModule(
        unsigned int classId) {
    return static_cast<RunningAlphaStrongModule*>(newModuleObject(classId));
}

RunningAlphaStrongModule* ModuleObjectFactory::newRunningAlphaStrongModule(
        const std::string& className) {
    return static_cast<RunningAlphaStrongModule*>(newModuleObject(className));
}

ActiveFlavorsModule* ModuleObjectFactory::newActiveFlavorsModule(
        unsigned int classId) {
    return static_cast<ActiveFlavorsModule*>(newModuleObject(classId));
}

ActiveFlavorsModule* ModuleObjectFactory::newActiveFlavorsModule(
        const std::string &className) {
    return static_cast<ActiveFlavorsModule*>(newModuleObject(className));
}

ScaleModule* ModuleObjectFactory::newScaleModule(unsigned int classId) {
    return static_cast<ScaleModule*>(newModuleObject(classId));
}

ScaleModule* ModuleObjectFactory::newScaleModule(const std::string &className) {
    return static_cast<ScaleModule*>(newModuleObject(className));
}

XiConverterModule* ModuleObjectFactory::newXiConverterModule(
        unsigned int classId) {
    return static_cast<XiConverterModule*>(newModuleObject(classId));
}
XiConverterModule* ModuleObjectFactory::newXiConverterModule(
        const std::string &className) {
    return static_cast<XiConverterModule*>(newModuleObject(className));
}

GapEquationSolverModule* ModuleObjectFactory::newGapEquationSolverModule(
        unsigned int classId) {
    return static_cast<GapEquationSolverModule*>(newModuleObject(classId));
}
GapEquationSolverModule* ModuleObjectFactory::newGapEquationSolverModule(
        const std::string &className) {
    return static_cast<GapEquationSolverModule*>(newModuleObject(className));
}

Observable* ModuleObjectFactory::newObservable(unsigned int classId) {
    return static_cast<Observable*>(newModuleObject(classId));
}

Observable* ModuleObjectFactory::newObservable(const std::string& className) {
    return static_cast<Observable*>(newModuleObject(className));
}

IncompleteGPDModule* ModuleObjectFactory::newIncompleteGPDModule(
        unsigned int classId) {
    return static_cast<IncompleteGPDModule*>(newModuleObject(classId));
}

IncompleteGPDModule* ModuleObjectFactory::newIncompleteGPDModule(
        const std::string& className) {
    return static_cast<IncompleteGPDModule*>(newModuleObject(className));
}

RadonInverseModule* ModuleObjectFactory::newRadonMatrixModule(
        unsigned int classId) {
    return static_cast<RadonInverseModule*>(newModuleObject(classId));
}

RadonInverseModule* ModuleObjectFactory::newRadonMatrixModule(
        const std::string& className) {
    return static_cast<RadonInverseModule*>(newModuleObject(className));
}
