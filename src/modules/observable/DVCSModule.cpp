#include "DVCSModule.h"

#include "../../beans/observable/Observable.h"

DVCSModule::DVCSModule(std::string _ID)
        : ObservableModule(_ID) {

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
    }

    m_cffOutputData = other.m_cffOutputData;
}

//TODO implement
double DVCSModule::somme() {
    double result = 0.;
    return result;
}

//TODO implement
double DVCSModule::difference() {
    double result = 0.;
    return result;
}
