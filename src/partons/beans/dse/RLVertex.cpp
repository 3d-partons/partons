#include "../../../../include/partons/beans/dse/RLVertex.h"

#include <vector>

RLVertex::RLVertex() :
        QuarkGluonVertex("RLVertex", 1) {
}

RLVertex::RLVertex(const std::string& className) :
        QuarkGluonVertex(className, 1) {
}

RLVertex::RLVertex(const RLVertex& other) :
        QuarkGluonVertex(other) {
}

RLVertex::~RLVertex() {
    // TODO Auto-generated destructor stub
}

RLVertex* RLVertex::clone() const {
    return new RLVertex(*this);
}

std::string RLVertex::toString() const {
    return QuarkGluonVertex::toString();
}

double RLVertex::F_A_func(unsigned int i, double p2, double q2,
        double k2) const {
    switch (i) {
    case 0:
        return 4. / 3.
                * ((p2 + q2) / 2. + (p2 - q2) * (p2 - q2) / 2. / (k2 + 1.e-16)
                        - k2) / p2;
        break;
    default:
        basisElementsWarning(__func__);
        return 0.;
        break;
    }
}

double RLVertex::F_M_func(unsigned int i, double p2, double q2,
        double k2) const {
    switch (i) {
    case 0:
        return 4.;
        break;
    default:
        basisElementsWarning(__func__);
        return 0.;
        break;
    }
}

double RLVertex::H_A_func(unsigned int i, double p2, double q2, double A_p2,
        double A_q2, double B_p2, double B_q2, double sigmaV_p2,
        double sigmaV_q2, double sigmaS_p2, double sigmaS_q2) const {
    switch (i) {
    case 0:
        return sigmaV_q2;
        break;
    default:
        basisElementsWarning(__func__);
        return 0.;
        break;
    }
}

double RLVertex::H_M_func(unsigned int i, double p2, double q2, double A_p2,
        double A_q2, double B_p2, double B_q2, double sigmaV_p2,
        double sigmaV_q2, double sigmaS_p2, double sigmaS_q2) const {
    switch (i) {
    case 0:
        return sigmaS_q2;
        break;
    default:
        basisElementsWarning(__func__);
        return 0.;
        break;
    }
}

double RLVertex::H_A_deriv(unsigned int i, double p2, double q2, double A_p2,
        double A_q2, double dA_p2, double dA_q2, double B_p2, double B_q2,
        double dB_p2, double dB_q2, double sigmaV_p2, double sigmaV_q2,
        double sigmaS_p2, double sigmaS_q2, double dsigmaV_p2,
        double dsigmaV_q2, double dsigmaS_p2, double dsigmaS_q2) const {
    switch (i) {
    case 0:
        return dsigmaV_q2;
        break;
    default:
        basisElementsWarning(__func__);
        return 0.;
        break;
    }
}

double RLVertex::H_M_deriv(unsigned int i, double p2, double q2, double A_p2,
        double A_q2, double dA_p2, double dA_q2, double B_p2, double B_q2,
        double dB_p2, double dB_q2, double sigmaV_p2, double sigmaV_q2,
        double sigmaS_p2, double sigmaS_q2, double dsigmaV_p2,
        double dsigmaV_q2, double dsigmaS_p2, double dsigmaS_q2) const {
    switch (i) {
    case 0:
        return dsigmaS_q2;
        break;
    default:
        basisElementsWarning(__func__);
        return 0.;
        break;
    }
}

