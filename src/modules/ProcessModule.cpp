#include "ProcessModule.h"

ProcessModule::ProcessModule(const std::string &className) :
        ModuleObject(className), m_xB(0.), m_t(0.), m_Q2(0.), m_E(0.) {

}

ProcessModule::ProcessModule(const ProcessModule &other) :
        ModuleObject(other) {
    m_xB = other.m_xB;
    m_t = other.m_t;
    m_Q2 = other.m_Q2;

    m_E = other.m_E;
}

ProcessModule::~ProcessModule() {
}
