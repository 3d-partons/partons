#include "ObservableKinematic.h"

#include "../../utils/stringUtils/Formatter.h"

ObservableKinematic::ObservableKinematic()
        : m_xB(0.), m_t(0.), m_Q2(0.), m_listOfPhi() {
}

ObservableKinematic::ObservableKinematic(double xB, double t, double Q2,
        std::vector<double> listOfPhi)
        : m_xB(xB), m_t(t), m_Q2(Q2), m_listOfPhi(listOfPhi) {
}

ObservableKinematic::~ObservableKinematic() {
}

std::string ObservableKinematic::toString() {
    Formatter formatter;

    formatter << "m_xB = " << m_xB << "m_t = " << m_t << " m_Q2 = " << m_Q2;

    for (unsigned int i = 0; i < m_listOfPhi.size(); i++) {
        formatter << "m_phi = " << m_listOfPhi[i] << "\n";
    }

    return formatter;
}

// ##### GETTERS & SETTERS #####

const std::vector<double>& ObservableKinematic::getListOfPhi() const {
    return m_listOfPhi;
}

void ObservableKinematic::setListOfPhi(const std::vector<double>& listOfPhi) {
    m_listOfPhi = listOfPhi;
}

double ObservableKinematic::getQ2() const {
    return m_Q2;
}

void ObservableKinematic::setQ2(double Q2) {
    m_Q2 = Q2;
}

double ObservableKinematic::getT() const {
    return m_t;
}

void ObservableKinematic::setT(double t) {
    m_t = t;
}

double ObservableKinematic::getXB() const {
    return m_xB;
}

void ObservableKinematic::setXB(double xB) {
    m_xB = xB;
}
