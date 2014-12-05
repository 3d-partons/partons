#include "ObservableModule.h"

ObservableModule::ObservableModule(std::string _ID)
        : ModuleObject(_ID), m_xB(0.), m_t(0.), m_Q2(0.) {

}

ObservableModule::ObservableModule(const ObservableModule &other)
        : ModuleObject(other) {
    m_xB = other.m_xB;
    m_t = other.m_t;
    m_Q2 = other.m_Q2;
}

ObservableModule::~ObservableModule() {
}
