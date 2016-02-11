#include "../../../../include/partons/modules/dse/BCSimplifiedModel.h"

#include "../../../../include/partons/beans/dse/QuarkPropagator.h"
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

double BCSimplifiedModel::F_A_func(double p2, double q2, double k2) const {
    return 2. / 3.
            * ((p2 + q2) / 2. + (p2 - q2) * (p2 - q2) / 2. / (k2 + 1.e-16) - k2)
            / (p2 + 1.e-16);
}

double BCSimplifiedModel::F_M_func(double p2, double q2, double k2) const {
    return 2.;
}

double BCSimplifiedModel::H_A_func(double p2, double q2) const {
    return getQuarkPropagator()->evaluateSigmaV(q2)
            * (getQuarkPropagator()->evaluateA(p2)
                    + getQuarkPropagator()->evaluateA(q2));
}

double BCSimplifiedModel::H_M_func(double p2, double q2) const {
    return getQuarkPropagator()->evaluateSigmaS(q2)
            * (getQuarkPropagator()->evaluateA(p2)
                    + getQuarkPropagator()->evaluateA(q2));
}

double BCSimplifiedModel::H_A_deriv_a(double p2, double q2, unsigned int j) const {
    return getQuarkPropagator()->differentiateSigmaV_a(q2, j)
            * (getQuarkPropagator()->evaluateA(p2)
                    + getQuarkPropagator()->evaluateA(q2))
            + getQuarkPropagator()->evaluateSigmaV(q2)
                    * (getQuarkPropagator()->differentiateA(p2, j)
                            + getQuarkPropagator()->differentiateA(q2, j));
}

double BCSimplifiedModel::H_M_deriv_a(double p2, double q2, unsigned int j) const {
    return getQuarkPropagator()->differentiateSigmaS_a(q2, j)
            * (getQuarkPropagator()->evaluateA(p2)
                    + getQuarkPropagator()->evaluateA(q2))
            + getQuarkPropagator()->evaluateSigmaS(q2)
                    * (getQuarkPropagator()->differentiateA(p2, j)
                            + getQuarkPropagator()->differentiateA(q2, j));
}

double BCSimplifiedModel::H_A_deriv_b(double p2, double q2, unsigned int j) const {
    return getQuarkPropagator()->differentiateSigmaV_b(q2, j)
            * (getQuarkPropagator()->evaluateA(p2)
                    + getQuarkPropagator()->evaluateA(q2));
}

double BCSimplifiedModel::H_M_deriv_b(double p2, double q2, unsigned int j) const {
    return getQuarkPropagator()->differentiateSigmaS_b(q2, j)
            * (getQuarkPropagator()->evaluateA(p2)
                    + getQuarkPropagator()->evaluateA(q2));
}
