#include "../../../../include/partons/beans/dse/DBVertex.h"

DBVertex::DBVertex() :
        BCVertex("DBVertex", 5) {
}

DBVertex::DBVertex(const std::string& className,
        unsigned int numberOfBasisElements) :
        BCVertex(className, numberOfBasisElements) {
}

DBVertex::DBVertex(const DBVertex& other) :
        BCVertex(other) {
}

DBVertex::~DBVertex() {
    // TODO Auto-generated destructor stub
}

DBVertex* DBVertex::clone() const {
    return new DBVertex(*this);
}

std::string DBVertex::toString() const {
    return QuarkGluonVertex::toString();
}

//TODO Deal with divisions by zero! (case p=q)
std::vector<double> DBVertex::Angular_Integrands(double p2, double q2,
        double k2) const {
    std::vector<double> angular_integrands = BCVertex::Angular_Integrands(p2,
            q2, k2);
    angular_integrands.at(3) = -2. / 3.
            * ((k2 * k2) - 2 * k2 * (p2 + q2) + (p2 - q2) * (p2 - q2)) / p2
            / (p2 - q2);
    angular_integrands.at(4) = -2. * (k2 + p2 - q2) / p2 / (p2 - q2);
    angular_integrands.at(m_N + 3) = 0.;
    angular_integrands.at(m_N + 4) = -2. * (k2 - p2 + q2) / (p2 - q2);
    return angular_integrands;
}

std::vector<double> DBVertex::Radial_Integrands(double p2, double q2,
        double A_p2, double A_q2, double B_p2, double B_q2, double sigmaV_p2,
        double sigmaV_q2, double sigmaS_p2, double sigmaS_q2) const {
    std::vector<double> radial_integrands = BCVertex::Radial_Integrands(p2, q2,
            A_p2, A_q2, B_p2, B_q2, sigmaV_p2, sigmaV_q2, sigmaS_p2, sigmaS_q2);

//    double M_q2 = B_q2 / A_q2;
//    double M_p2 = B_p2 / A_p2;
//    double eta = 0.65;
//    double tau5 = eta * (B_p2 - B_q2);
//    double tau4 = tau5 * 4. * (M_p2 + M_q2)
//            / (p2 + M_p2 * M_p2 + q2 + M_q2 * M_q2);
//
//    radial_integrands.at(3) = sigmaV_q2 * tau4;
//    radial_integrands.at(4) = sigmaS_q2 * tau5;
//    radial_integrands.at(m_N + 3) = 0.;
//    radial_integrands.at(m_N + 4) = sigmaV_q2 * tau5;

    radial_integrands.at(3) = sigmaV_q2 * 4. * (A_p2 - A_q2);
    radial_integrands.at(4) = sigmaS_q2 * (B_p2 - B_q2);
    radial_integrands.at(m_N + 3) = 0.;
    radial_integrands.at(m_N + 4) = sigmaV_q2 * (B_p2 - B_q2);
    return radial_integrands;
}

std::vector<double> DBVertex::Radial_Integrands_deriv(double p2, double q2,
        double A_p2, double A_q2, double dA_p2, double dA_q2, double B_p2,
        double B_q2, double dB_p2, double dB_q2, double sigmaV_p2,
        double sigmaV_q2, double sigmaS_p2, double sigmaS_q2, double dsigmaV_p2,
        double dsigmaV_q2, double dsigmaS_p2, double dsigmaS_q2) const {
    std::vector<double> radial_integrands_deriv =
            BCVertex::Radial_Integrands_deriv(p2, q2, A_p2, A_q2, dA_p2, dA_q2,
                    B_p2, B_q2, dB_p2, dB_q2, sigmaV_p2, sigmaV_q2, sigmaS_p2,
                    sigmaS_q2, dsigmaV_p2, dsigmaV_q2, dsigmaS_p2, dsigmaS_q2);

//    double M_q2 = B_q2 / A_q2;
//    double M_p2 = B_p2 / A_p2;
//    double eta = 0.65;
//    double tau5 = eta * (B_p2 - B_q2);
//    double M_pq = (p2 + M_p2 * M_p2 + q2 + M_q2 * M_q2);
//    double M_inv_pq = 4. * (M_p2 + M_q2) / M_pq;
//    double tau4 = tau5 * M_inv_pq;
//    double dM_q2 = (dB_q2 * A_q2 - B_q2 * dA_q2) / (A_q2 * A_q2);
//    double dM_p2 = (dB_p2 * A_p2 - B_p2 * dA_p2) / (A_p2 * A_p2);
//    double dtau5 = eta * (dB_p2 - dB_q2);
//    double dM_pq = 2. * (M_p2 * dM_p2 + M_q2 * dM_q2);
//    double dM_inv_pq = 4. * ((dM_p2 + dM_q2) * M_pq - (M_p2 + M_q2) * dM_pq)
//            / (M_pq * M_pq);
//    double dtau4 = dtau5 * M_inv_pq + tau5 * dM_inv_pq;
//    radial_integrands_deriv.at(3) = dsigmaV_q2 * tau4 + sigmaV_q2 * dtau4;
//    radial_integrands_deriv.at(4) = dsigmaS_q2 * tau5 + sigmaS_q2 * dtau5;
//    radial_integrands_deriv.at(m_N + 3) = 0.;
//    radial_integrands_deriv.at(m_N + 4) = dsigmaV_q2 * tau5 + sigmaV_q2 * dtau5;

    radial_integrands_deriv.at(3) = 4.
            * (dsigmaV_q2 * (A_p2 - A_q2) + sigmaV_q2 * (dA_p2 - dA_q2));
    radial_integrands_deriv.at(4) = dsigmaS_q2 * (B_p2 - B_q2) + sigmaS_q2 * (dB_p2 - dB_q2);
    radial_integrands_deriv.at(m_N + 3) = 0.;
    radial_integrands_deriv.at(m_N + 4) = dsigmaV_q2 * (B_p2 - B_q2) + sigmaV_q2 * (dB_p2 - dB_q2);
    return radial_integrands_deriv;
}
