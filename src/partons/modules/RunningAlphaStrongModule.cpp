#include "../../../include/partons/modules/RunningAlphaStrongModule.h"

#include <cmath>

const std::string RunningAlphaStrongModule::RUNNING_ALPHA_STRONG_MODULE_CLASS_NAME =
        "RunningAlphaStrongModule";

RunningAlphaStrongModule::RunningAlphaStrongModule(const std::string &className) :
        ModuleObject(className), m_Mu2(0.), m_Mu(0.), m_nf(0), m_alphaS(0.) {

}

RunningAlphaStrongModule::RunningAlphaStrongModule(
        const RunningAlphaStrongModule &other) :
        ModuleObject(other) {
    m_Mu2 = other.m_Mu2;
    m_Mu = other.m_Mu;
    m_nf = other.m_nf;
    m_alphaS = other.m_alphaS;
}

RunningAlphaStrongModule::~RunningAlphaStrongModule() {
}

void RunningAlphaStrongModule::preCompute(const double Mu2) {
    m_Mu2 = Mu2;
    m_Mu = sqrt(m_Mu2);
}

//TODO implement
void RunningAlphaStrongModule::initModule() {

}

//TODO implement
void RunningAlphaStrongModule::isModuleWellConfigured() {

}

unsigned int RunningAlphaStrongModule::getNf() const {
    return m_nf;
}

void RunningAlphaStrongModule::setNf(unsigned int nf) {
    m_nf = nf;
}

bool RunningAlphaStrongModule::isPreviousKinematicsDifferent(
        const double MuF2) const {
    return (MuF2 != m_Mu2) ? true : false;
}

double RunningAlphaStrongModule::compute(double Mu2) {

    if (isPreviousKinematicsDifferent(Mu2)) {

        preCompute(Mu2);

        initModule();
        isModuleWellConfigured();

        m_alphaS = compute();
    }

    return m_alphaS;
}

void RunningAlphaStrongModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    ModuleObject::prepareSubModules(subModulesData);

    // Nothing to do.
}
