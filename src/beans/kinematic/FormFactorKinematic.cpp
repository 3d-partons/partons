#include "FormFactorKinematic.h"

#include "../../utils/stringUtils/Formatter.h"

FormFactorKinematic::FormFactorKinematic(double xB, double t, double Q2)
        : Kinematic(xB, t), m_binId(0), m_Q2(Q2) {
}

FormFactorKinematic::FormFactorKinematic(unsigned int binId, double xB,
        double t, double Q2)
        : Kinematic(xB, t), m_binId(binId), m_Q2(Q2) {
}

FormFactorKinematic::~FormFactorKinematic() {
}

std::string FormFactorKinematic::toString() {
    return Formatter() << Kinematic::toString() << "m_binId = " << m_binId
            << " m_Q2 = " << m_Q2;
}

// ##### GETTERS & SETTERS #####

unsigned int FormFactorKinematic::getBinId() const {
    return m_binId;
}

void FormFactorKinematic::setBinId(unsigned int binId) {
    m_binId = binId;
}

double FormFactorKinematic::getQ2() const {
    return m_Q2;
}

void FormFactorKinematic::setQ2(double q2) {
    m_Q2 = q2;
}
