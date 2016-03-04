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

std::vector<double> RLVertex::Angular_Integrands(double p2,
        double q2, double k2) const {
    std::vector<double> angular_integrands(2, 0.);
    angular_integrands.at(0) = 4. / 3.
            * ((p2 + q2) / 2. + (p2 - q2) * (p2 - q2) / 2. / (k2 + 1.e-16)
                    - k2) / p2;
    angular_integrands.at(1) = 4.;
    return angular_integrands;
}

std::vector<double> RLVertex::Radial_Integrands(double p2,
        double q2, double A_p2, double A_q2, double B_p2, double B_q2,
        double sigmaV_p2, double sigmaV_q2, double sigmaS_p2,
        double sigmaS_q2) const {
    std::vector<double> radial_integrands(2, 0.);
    radial_integrands.at(0) = sigmaV_q2;
    radial_integrands.at(1) = sigmaS_q2;
    return radial_integrands;
}

std::vector<double> RLVertex::Radial_Integrands_deriv(double p2,
        double q2, double A_p2, double A_q2, double dA_p2, double dA_q2,
        double B_p2, double B_q2, double dB_p2, double dB_q2, double sigmaV_p2,
        double sigmaV_q2, double sigmaS_p2, double sigmaS_q2, double dsigmaV_p2,
        double dsigmaV_q2, double dsigmaS_p2, double dsigmaS_q2) const {
    std::vector<double> radial_integrands_deriv(2, 0.);
    radial_integrands_deriv.at(0) = dsigmaV_q2;
    radial_integrands_deriv.at(1) = dsigmaS_q2;
    return radial_integrands_deriv;
}

