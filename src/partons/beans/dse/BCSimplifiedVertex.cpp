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

std::vector<double> BCSimplifiedVertex::Radial_Integrands(double p2, double q2,
        double A_p2, double A_q2, double B_p2, double B_q2, double sigmaV_p2,
        double sigmaV_q2, double sigmaS_p2, double sigmaS_q2) const {
    std::vector<double> radial_integrands(2, 0.);
    radial_integrands.at(0) = sigmaV_q2 * (A_p2 + A_q2) * 0.5;
    radial_integrands.at(1) = sigmaS_q2 * (A_p2 + A_q2) * 0.5;
    return radial_integrands;
}

std::vector<double> BCSimplifiedVertex::Radial_Integrands_deriv(double p2, double q2,
        double A_p2, double A_q2, double dA_p2, double dA_q2, double B_p2,
        double B_q2, double dB_p2, double dB_q2, double sigmaV_p2,
        double sigmaV_q2, double sigmaS_p2, double sigmaS_q2, double dsigmaV_p2,
        double dsigmaV_q2, double dsigmaS_p2, double dsigmaS_q2) const {
    std::vector<double> radial_integrands_deriv(2, 0.);
    radial_integrands_deriv.at(0) = (dsigmaV_q2 * (A_p2 + A_q2) + sigmaV_q2 * (dA_p2 + dA_q2)) * 0.5;
    radial_integrands_deriv.at(1) = (dsigmaS_q2 * (A_p2 + A_q2) + sigmaS_q2 * (dA_p2 + dA_q2)) * 0.5;
    return radial_integrands_deriv;
}
