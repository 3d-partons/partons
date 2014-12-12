#include "DVCSModule.h"

#include "../../beans/observable/Observable.h"

DVCSModule::DVCSModule(const std::string &className)
        : ObservableModule(className), m_xB(0.), m_t(0.), m_Q2(0.), m_phi(0.), m_phiS(
                0.), m_phie(0.), m_E(0.), m_phaseSpace(0.), m_pObservable(0) {

}

DVCSModule::~DVCSModule() {

}

DVCSModule::DVCSModule(const DVCSModule& other)
        : ObservableModule(other) {
    m_xB = other.m_xB;
    m_t = other.m_t;
    m_Q2 = other.m_Q2;

    m_phi = other.m_phi;
    m_phiS = other.m_phiS;
    m_phie = other.m_phie;

    m_E = other.m_E;
    m_phaseSpace = other.m_phaseSpace;

    if (other.m_pObservable != 0) {
        m_pObservable = other.m_pObservable->clone();
    } else {
        m_pObservable = 0;
    }

    m_cffOutputData = other.m_cffOutputData;
}
