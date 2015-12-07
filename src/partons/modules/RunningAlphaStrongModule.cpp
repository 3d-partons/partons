#include "../../../include/partons/modules/RunningAlphaStrongModule.h"

RunningAlphaStrongModule::RunningAlphaStrongModule(const std::string &className) :
        ModuleObject(className), m_Mu(0.), m_nf(0) {

}

RunningAlphaStrongModule::RunningAlphaStrongModule(
        const RunningAlphaStrongModule &other) :
        ModuleObject(other) {
    m_Mu = other.m_Mu;
    m_nf = other.m_nf;
}

RunningAlphaStrongModule::~RunningAlphaStrongModule() {
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
