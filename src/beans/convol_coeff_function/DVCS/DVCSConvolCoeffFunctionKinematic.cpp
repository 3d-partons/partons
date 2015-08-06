#include "DVCSConvolCoeffFunctionKinematic.h"

#include "../../../utils/stringUtils/Formatter.h"

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic() :
        BaseObject("DVCSConvolCoeffFunctionKinematic"), m_binId(0), m_xi(0.), m_t(
                0.), m_Q2(0.), m_MuF2(0.), m_MuR2(0.) {
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(double xi,
        double t, double Q2, double MuF2, double MuR2) :
        BaseObject("DVCSConvolCoeffFunctionKinematic"), m_binId(0), m_xi(xi), m_t(
                t), m_Q2(Q2), m_MuF2(MuF2), m_MuR2(MuR2) {
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(
        unsigned int binId, double xi, double t, double Q2, double MuF2,
        double MuR2) :
        BaseObject("DVCSConvolCoeffFunctionKinematic"), m_binId(binId), m_xi(
                xi), m_t(t), m_Q2(Q2), m_MuF2(MuF2), m_MuR2(MuR2) {
}

DVCSConvolCoeffFunctionKinematic::~DVCSConvolCoeffFunctionKinematic() {
    // TODO Auto-generated destructor stub
}

std::string DVCSConvolCoeffFunctionKinematic::toString() {
    return Formatter() << "m_binId = " << m_binId << " m_xi = " << m_xi
            << " m_t = " << m_t << " m_Q2 = " << m_Q2 << " m_MuF2 = " << m_MuF2
            << " m_MuR2 = " << m_MuR2;
}

unsigned int DVCSConvolCoeffFunctionKinematic::getBinId() const {
    return m_binId;
}

double DVCSConvolCoeffFunctionKinematic::getMuF2() const {
    return m_MuF2;
}

double DVCSConvolCoeffFunctionKinematic::getMuR2() const {
    return m_MuR2;
}

double DVCSConvolCoeffFunctionKinematic::getQ2() const {
    return m_Q2;
}

double DVCSConvolCoeffFunctionKinematic::getT() const {
    return m_t;
}

double DVCSConvolCoeffFunctionKinematic::getXi() const {
    return m_xi;
}
