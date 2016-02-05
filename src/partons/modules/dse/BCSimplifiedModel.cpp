#include "../../../../include/partons/modules/dse/BCSimplifiedModel.h"

#include <cmath>

#include "../../../../include/partons/beans/dse/GluonPropagator.h"
#include "../../../../include/partons/BaseObjectRegistry.h"

// Initialise [class]::classId with a unique name.
const unsigned int BCSimplifiedModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new BCSimplifiedModel("BCSimplifiedModel"));

BCSimplifiedModel::BCSimplifiedModel(const std::string &className) :
        GapEquationSolverModule(className) {

}
BCSimplifiedModel::~BCSimplifiedModel() {
    // TODO Auto-generated destructor stub
}

BCSimplifiedModel::BCSimplifiedModel(const BCSimplifiedModel& other) :
        GapEquationSolverModule(other) {
}

BCSimplifiedModel* BCSimplifiedModel::clone() const {
    return new BCSimplifiedModel(*this);
}

void BCSimplifiedModel::initModule() {
    GapEquationSolverModule::initModule();
}

void BCSimplifiedModel::isModuleWellConfigured() {
    GapEquationSolverModule::isModuleWellConfigured();
}

double BCSimplifiedModel::ThetaA_func(std::vector<double> z,
        std::vector<double> parameters) {
    double p2 = parameters.at(0);
    double q2 = parameters.at(1);

    return sqrt(1 - z.at(0) * z.at(0))
            * getGluonPropagator()->evaluateG(k2_func(p2, q2, z.at(0)))
            * F1_func(p2, q2, k2_func(p2, q2, z.at(0)));
}

double BCSimplifiedModel::ThetaM_func(std::vector<double> z,
        std::vector<double> parameters) {
    double p2 = parameters.at(0);
    double q2 = parameters.at(1);

    return sqrt(1 - z.at(0) * z.at(0))
            * getGluonPropagator()->evaluateG(k2_func(p2, q2, z.at(0)))
            * F2_func(p2, q2, k2_func(p2, q2, z.at(0)));
}

double BCSimplifiedModel::F1_func(double p2, double q2, double k2) {
    return 4. / 3.
            * ((p2 + q2) / 2. + (p2 - q2) * (p2 - q2) / 2. / (k2 + 1.e-16) - k2)
            / (p2 + 1.e-16);
}

double BCSimplifiedModel::F2_func(double p2, double q2, double k2) {
    return 4.;
}
