#include "../../../../include/partons/beans/dse/BCVertex.h"

BCVertex::BCVertex() :
        QuarkGluonVertex("BCVertex", 3) {
}

BCVertex::BCVertex(const std::string& className) :
        QuarkGluonVertex(className, 3) {
}

BCVertex::BCVertex(const BCVertex& other) :
        QuarkGluonVertex(other) {
}

BCVertex::~BCVertex() {
    // TODO Auto-generated destructor stub
}

BCVertex* BCVertex::clone() const {
    return new BCVertex(*this);
}

std::string BCVertex::toString() const {
    return QuarkGluonVertex::toString();
}

//TODO Deal with divisions by zero! (case p=q)
std::vector<double> BCVertex::Angular_Integrands(double p2, double q2,
        double k2) const {
    std::vector<double> angular_integrands(6, 0.);
    angular_integrands.at(0) = 4. / 3.
            * ((p2 + q2) / 2. + (p2 - q2) * (p2 - q2) / 2. / (k2 + 1.e-16) - k2)
            / p2;
    angular_integrands.at(1) = 4. / 3.
            * (((p2 + q2) * (p2 - q2)) / (4 * k2)
                    + k2 * (p2 + q2) * 0.25 / (p2 - q2)
                    - 0.5 * (p2 + q2) * (p2 + q2) / (p2 - q2)) / p2;
    angular_integrands.at(2) = 4. / 3.
            * ((p2 - q2) / (2 * k2) + k2 / (2. * (p2 - q2))
                    - (p2 + q2) / (p2 - q2)) / p2;
    angular_integrands.at(3) = 4.;
    angular_integrands.at(4) = 4. / 3.
            * (-((p2 - q2) / (2 * k2)) - k2 / (2 * (p2 - q2))
                    + (p2 + q2) / (p2 - q2));
    angular_integrands.at(5) = -angular_integrands.at(4);
    return angular_integrands;
}

std::vector<double> BCVertex::Radial_Integrands(double p2, double q2,
        double A_p2, double A_q2, double B_p2, double B_q2, double sigmaV_p2,
        double sigmaV_q2, double sigmaS_p2, double sigmaS_q2) const {
    std::vector<double> radial_integrands(6, 0.);
    radial_integrands.at(0) = sigmaV_q2 * (A_p2 + A_q2) * 0.5;
    radial_integrands.at(1) = sigmaV_q2 * (A_p2 - A_q2);
    radial_integrands.at(2) = sigmaS_q2 * (B_p2 - B_q2);
    radial_integrands.at(3) = sigmaS_q2 * (A_p2 + A_q2) * 0.5;
    radial_integrands.at(4) = sigmaS_q2 * (A_p2 - A_q2);
    radial_integrands.at(5) = sigmaV_q2 * (B_p2 - B_q2);
    return radial_integrands;
}

std::vector<double> BCVertex::Radial_Integrands_deriv(double p2, double q2,
        double A_p2, double A_q2, double dA_p2, double dA_q2, double B_p2,
        double B_q2, double dB_p2, double dB_q2, double sigmaV_p2,
        double sigmaV_q2, double sigmaS_p2, double sigmaS_q2, double dsigmaV_p2,
        double dsigmaV_q2, double dsigmaS_p2, double dsigmaS_q2) const {
    std::vector<double> radial_integrands_deriv(6, 0.);
    radial_integrands_deriv.at(0) = (dsigmaV_q2 * (A_p2 + A_q2) + sigmaV_q2 * (dA_p2 + dA_q2)) * 0.5;
    radial_integrands_deriv.at(1) = dsigmaV_q2 * (A_p2 - A_q2) + sigmaV_q2 * (dA_p2 - dA_q2);
    radial_integrands_deriv.at(2) = dsigmaS_q2 * (B_p2 - B_q2) + sigmaS_q2 * (dB_p2 - dB_q2);
    radial_integrands_deriv.at(3) = (dsigmaS_q2 * (A_p2 + A_q2) + sigmaS_q2 * (dA_p2 + dA_q2)) * 0.5;
    radial_integrands_deriv.at(4) = dsigmaS_q2 * (A_p2 - A_q2) + sigmaS_q2 * (dA_p2 - dA_q2);
    radial_integrands_deriv.at(5) = dsigmaV_q2 * (B_p2 - B_q2) + sigmaV_q2 * (dB_p2 - dB_q2);
    return radial_integrands_deriv;
}
