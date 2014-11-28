#include "ObservableKinematic.h"

ObservableKinematic::ObservableKinematic(double xB, double t, double Q2,
        std::vector<double> listOfPhi)
        : FormFactorKinematic(xB, t, Q2), m_listOfPhi(listOfPhi) {
}

ObservableKinematic::~ObservableKinematic() {
}

// ##### GETTERS & SETTERS #####

const std::vector<double>& ObservableKinematic::getListOfPhi() const {
    return m_listOfPhi;
}

void ObservableKinematic::setListOfPhi(const std::vector<double>& listOfPhi) {
    m_listOfPhi = listOfPhi;
}
