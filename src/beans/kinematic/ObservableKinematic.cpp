#include "ObservableKinematic.h"

#include "../../utils/stringUtils/Formatter.h"

class Formatter;

ObservableKinematic::ObservableKinematic(double xB, double t, double Q2,
        std::vector<double> listOfPhi)
        : FormFactorKinematic(xB, t, Q2), m_listOfPhi(listOfPhi) {
}

ObservableKinematic::~ObservableKinematic() {
}

std::string ObservableKinematic::toString() {
    Formatter formatter;

    formatter << FormFactorKinematic::toString();

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
