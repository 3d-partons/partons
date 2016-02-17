#include "../../../../include/partons/modules/dse/RLModel.h"

#include "../../../../include/partons/beans/dse/QuarkPropagator.h"
#include "../../../../include/partons/BaseObjectRegistry.h"

// Initialise [class]::classId with a unique name.
const unsigned int RLModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new RLModel("RLModel"));

RLModel::RLModel(const std::string &className) :
        GapEquationSolverModule(className) {

}
RLModel::~RLModel() {
    // TODO Auto-generated destructor stub
}

RLModel::RLModel(const RLModel& other) :
        GapEquationSolverModule(other) {
}

RLModel* RLModel::clone() const {
    return new RLModel(*this);
}

void RLModel::initModule() {
    GapEquationSolverModule::initModule();
}

void RLModel::isModuleWellConfigured() {
    GapEquationSolverModule::isModuleWellConfigured();
}

double RLModel::F_A_func(double p2, double q2, double k2) const {
    return 4. / 3.
            * ((p2 + q2) / 2. + (p2 - q2) * (p2 - q2) / 2. / (k2 + 1.e-16) - k2)
            / (p2 + 1.e-16);
}

double RLModel::F_M_func(double p2, double q2, double k2) const {
    return 4.;
}

double RLModel::H_A_func(double p2, double q2) const {
    return getQuarkPropagator()->evaluateSigmaV(q2);
}

double RLModel::H_M_func(double p2, double q2) const {
    return getQuarkPropagator()->evaluateSigmaS(q2);
}

double RLModel::H_A_deriv_a(double p2, double q2, unsigned int j) const {
    return getQuarkPropagator()->differentiateSigmaV_a(q2,j);
}

double RLModel::H_M_deriv_a(double p2, double q2, unsigned int j) const {
    return getQuarkPropagator()->differentiateSigmaS_a(q2,j);
}

double RLModel::H_A_deriv_b(double p2, double q2, unsigned int j) const {
    return getQuarkPropagator()->differentiateSigmaV_b(q2,j);
}

double RLModel::H_M_deriv_b(double p2, double q2, unsigned int j) const {
    return getQuarkPropagator()->differentiateSigmaS_b(q2,j);
}

double RLModel::H_A_func(double A_p2, double A_q2, double B_p2, double B_q2,
        double sigmaV_p2, double sigmaV_q2, double sigmaS_p2,
        double sigmaS_q2) const {
    return sigmaV_q2;
}

double RLModel::H_M_func(double A_p2, double A_q2, double B_p2, double B_q2,
        double sigmaV_p2, double sigmaV_q2, double sigmaS_p2,
        double sigmaS_q2) const {
    return sigmaS_q2;
}

double RLModel::H_A_deriv_a(double A_p2, double A_q2, double dA_p2,
        double dA_q2, double B_p2, double B_q2, double sigmaV_p2,
        double sigmaV_q2, double sigmaS_p2, double sigmaS_q2,
        double dsigmaV_a_p2, double dsigmaV_b_p2, double dsigmaV_a_q2,
        double dsigmaV_b_q2, double dsigmaS_a_p2, double dsigmaS_b_p2,
        double dsigmaS_a_q2, double dsigmaS_b_q2) const {
    return dsigmaV_a_q2;
}

double RLModel::H_M_deriv_a(double A_p2, double A_q2, double dA_p2,
        double dA_q2, double B_p2, double B_q2, double sigmaV_p2,
        double sigmaV_q2, double sigmaS_p2, double sigmaS_q2,
        double dsigmaV_a_p2, double dsigmaV_b_p2, double dsigmaV_a_q2,
        double dsigmaV_b_q2, double dsigmaS_a_p2, double dsigmaS_b_p2,
        double dsigmaS_a_q2, double dsigmaS_b_q2) const {
    return dsigmaS_a_q2;
}

double RLModel::H_A_deriv_b(double A_p2, double A_q2, double B_p2, double B_q2,
        double dB_p2, double dB_q2, double sigmaV_p2, double sigmaV_q2,
        double sigmaS_p2, double sigmaS_q2, double dsigmaV_a_p2,
        double dsigmaV_b_p2, double dsigmaV_a_q2, double dsigmaV_b_q2,
        double dsigmaS_a_p2, double dsigmaS_b_p2, double dsigmaS_a_q2,
        double dsigmaS_b_q2) const {
    return dsigmaV_b_q2;
}

double RLModel::H_M_deriv_b(double A_p2, double A_q2, double B_p2, double B_q2,
        double dB_p2, double dB_q2, double sigmaV_p2, double sigmaV_q2,
        double sigmaS_p2, double sigmaS_q2, double dsigmaV_a_p2,
        double dsigmaV_b_p2, double dsigmaV_a_q2, double dsigmaV_b_q2,
        double dsigmaS_a_p2, double dsigmaS_b_p2, double dsigmaS_a_q2,
        double dsigmaS_b_q2) const {
    return dsigmaS_b_q2;
}
