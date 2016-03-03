#include "../../../../include/partons/beans/dse/BCSimplifiedVertex.h"

BCSimplifiedVertex::BCSimplifiedVertex() :
        RLVertex("BCSimplifiedVertex") {
}

BCSimplifiedVertex::BCSimplifiedVertex(const std::string& className) :
        RLVertex(className) {
}

BCSimplifiedVertex::BCSimplifiedVertex(const BCSimplifiedVertex& other) :
        RLVertex(other) {
}

BCSimplifiedVertex::~BCSimplifiedVertex() {
    // TODO Auto-generated destructor stub
}

BCSimplifiedVertex* BCSimplifiedVertex::clone() const {
    return new BCSimplifiedVertex(*this);
}

std::string BCSimplifiedVertex::toString() const {
    return RLVertex::toString();
}

double BCSimplifiedVertex::H_A_func(unsigned int i, double p2, double q2,
        double A_p2, double A_q2, double B_p2, double B_q2, double sigmaV_p2,
        double sigmaV_q2, double sigmaS_p2, double sigmaS_q2) const {
    switch (i) {
    case 0:
        return sigmaV_q2 * (A_p2 + A_q2) * 0.5;
        break;
    default:
        basisElementsWarning(__func__);
        return 0.;
        break;
    }
}

double BCSimplifiedVertex::H_M_func(unsigned int i, double p2, double q2,
        double A_p2, double A_q2, double B_p2, double B_q2, double sigmaV_p2,
        double sigmaV_q2, double sigmaS_p2, double sigmaS_q2) const {
    switch (i) {
    case 0:
        return sigmaS_q2 * (A_p2 + A_q2) * 0.5;
        break;
    default:
        basisElementsWarning(__func__);
        return 0.;
        break;
    }
}

double BCSimplifiedVertex::H_A_deriv(unsigned int i, double p2, double q2,
        double A_p2, double A_q2, double dA_p2, double dA_q2, double B_p2,
        double B_q2, double dB_p2, double dB_q2, double sigmaV_p2,
        double sigmaV_q2, double sigmaS_p2, double sigmaS_q2, double dsigmaV_p2,
        double dsigmaV_q2, double dsigmaS_p2, double dsigmaS_q2) const {
    switch (i) {
    case 0:
        return (dsigmaV_q2 * (A_p2 + A_q2) + sigmaV_q2 * (dA_p2 + dA_q2)) * 0.5;
        break;
    default:
        basisElementsWarning(__func__);
        return 0.;
        break;
    }
}

double BCSimplifiedVertex::H_M_deriv(unsigned int i, double p2, double q2,
        double A_p2, double A_q2, double dA_p2, double dA_q2, double B_p2,
        double B_q2, double dB_p2, double dB_q2, double sigmaV_p2,
        double sigmaV_q2, double sigmaS_p2, double sigmaS_q2, double dsigmaV_p2,
        double dsigmaV_q2, double dsigmaS_p2, double dsigmaS_q2) const {
    switch (i) {
    case 0:
        return (dsigmaS_q2 * (A_p2 + A_q2) + sigmaS_q2 * (dA_p2 + dA_q2)) * 0.5;
        break;
    default:
        basisElementsWarning(__func__);
        return 0.;
        break;
    }
}

