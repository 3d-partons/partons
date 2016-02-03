#include "../../include/partons/ModuleObjectFactory.h"

#include "../../include/partons/BaseObjectFactory.h"
#include "../../include/partons/modules/ActiveFlavorsModule.h"
#include "../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../include/partons/modules/dse/GapEquationSolverModule.h"
#include "../../include/partons/modules/DoubleDistributionModule.h"
#include "../../include/partons/modules/evolution/GPDEvolutionModule.h"
#include "../../include/partons/modules/GPDModule.h"
#include "../../include/partons/modules/observable/Observable.h"
#include "../../include/partons/modules/process/DVCSModule.h"
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

DoubleDistributionModule* ModuleObjectFactory::newDoubleDistributionModule(
        unsigned int classId) {
    return static_cast<DoubleDistributionModule*>(m_pBaseObjectFactory->newBaseObject(
            classId));
}

DoubleDistributionModule* ModuleObjectFactory::newDoubleDistributionModule(
        const std::string& className) {
    return static_cast<DoubleDistributionModule*>(m_pBaseObjectFactory->newBaseObject(
            className));
}

GPDEvolutionModule* ModuleObjectFactory::newGPDEvolutionModule(
        unsigned int classId) {
    return static_cast<GPDEvolutionModule*>(m_pBaseObjectFactory->newBaseObject(
            classId));
}

GPDEvolutionModule* ModuleObjectFactory::newGPDEvolutionModule(
        const std::string& className) {
    return static_cast<GPDEvolutionModule*>(m_pBaseObjectFactory->newBaseObject(
            className));
}

GPDModule* ModuleObjectFactory::newGPDModule(unsigned int classId) {
    return static_cast<GPDModule*>(m_pBaseObjectFactory->newBaseObject(classId));
}

GPDModule* ModuleObjectFactory::newGPDModule(const std::string& className) {
    return static_cast<GPDModule*>(m_pBaseObjectFactory->newBaseObject(
            className));
}

DVCSConvolCoeffFunctionModule* ModuleObjectFactory::newDVCSConvolCoeffFunctionModule(
        unsigned int classId) {
    return static_cast<DVCSConvolCoeffFunctionModule*>(m_pBaseObjectFactory->newBaseObject(
            classId));
}

DVCSConvolCoeffFunctionModule* ModuleObjectFactory::newDVCSConvolCoeffFunctionModule(
        const std::string& className) {
    return static_cast<DVCSConvolCoeffFunctionModule*>(m_pBaseObjectFactory->newBaseObject(
            className));
}

ProcessModule* ModuleObjectFactory::newObservableModule(unsigned int classId) {
    return static_cast<ProcessModule*>(m_pBaseObjectFactory->newBaseObject(
            classId));
}

ProcessModule* ModuleObjectFactory::newObservableModule(
        const std::string& className) {
    return static_cast<ProcessModule*>(m_pBaseObjectFactory->newBaseObject(
            className));
}

DVCSModule* ModuleObjectFactory::newDVCSModule(unsigned int classId) {
    return static_cast<DVCSModule*>(m_pBaseObjectFactory->newBaseObject(classId));
}

DVCSModule* ModuleObjectFactory::newDVCSModule(const std::string& className) {
    return static_cast<DVCSModule*>(m_pBaseObjectFactory->newBaseObject(
            className));
}

RunningAlphaStrongModule* ModuleObjectFactory::newRunningAlphaStrongModule(
        unsigned int classId) {
    return static_cast<RunningAlphaStrongModule*>(m_pBaseObjectFactory->newBaseObject(
            classId));
}

RunningAlphaStrongModule* ModuleObjectFactory::newRunningAlphaStrongModule(
        const std::string& className) {
    return static_cast<RunningAlphaStrongModule*>(m_pBaseObjectFactory->newBaseObject(
            className));
}

ActiveFlavorsModule* ModuleObjectFactory::newActiveFlavorsModule(
        unsigned int classId) {
    return static_cast<ActiveFlavorsModule*>(m_pBaseObjectFactory->newBaseObject(
            classId));
}

ActiveFlavorsModule* ModuleObjectFactory::newActiveFlavorsModule(
        const std::string &className) {
    return static_cast<ActiveFlavorsModule*>(m_pBaseObjectFactory->newBaseObject(
            className));
}

ScaleModule* ModuleObjectFactory::newScaleModule(unsigned int classId) {
    return static_cast<ScaleModule*>(m_pBaseObjectFactory->newBaseObject(
            classId));
}

ScaleModule* ModuleObjectFactory::newScaleModule(const std::string &className) {
    return static_cast<ScaleModule*>(m_pBaseObjectFactory->newBaseObject(
            className));
}

XiConverterModule* ModuleObjectFactory::newXiConverterModule(
        unsigned int classId) {
    return static_cast<XiConverterModule*>(m_pBaseObjectFactory->newBaseObject(
            classId));
}
XiConverterModule* ModuleObjectFactory::newXiConverterModule(
        const std::string &className) {
    return static_cast<XiConverterModule*>(m_pBaseObjectFactory->newBaseObject(
            className));
}

GapEquationSolverModule* ModuleObjectFactory::newGapEquationSolverModule(
        unsigned int classId) {
    return static_cast<GapEquationSolverModule*>(m_pBaseObjectFactory->newBaseObject(
            classId));
}
GapEquationSolverModule* ModuleObjectFactory::newGapEquationSolverModule(
        const std::string &className) {
    return static_cast<GapEquationSolverModule*>(m_pBaseObjectFactory->newBaseObject(
            className));
}

Observable* ModuleObjectFactory::newObservable(unsigned int classId) {
    return static_cast<Observable*>(m_pBaseObjectFactory->newBaseObject(classId));
}

Observable* ModuleObjectFactory::newObservable(const std::string& className) {
    return static_cast<Observable*>(m_pBaseObjectFactory->newBaseObject(
            className));
}

