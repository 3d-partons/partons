#include "../../../../../../include/partons/modules/observable/DVMP/asymmetry/DVMPAluuMinusSin1Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/Observable.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVMPAluuMinusSin1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPAluuMinusSin1Phi("DVMPAluuMinusSin1Phi"));

DVMPAluuMinusSin1Phi::DVMPAluuMinusSin1Phi(const std::string &className) :
        DVMPAluuMinus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVMPAluuMinusSin1Phi::DVMPAluuMinusSin1Phi(const DVMPAluuMinusSin1Phi& other) :
        DVMPAluuMinus(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

DVMPAluuMinusSin1Phi::~DVMPAluuMinusSin1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVMPAluuMinusSin1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVMPAluuMinusSin1Phi::functionToIntegrateObservable);
}

DVMPAluuMinusSin1Phi* DVMPAluuMinusSin1Phi::clone() const {
    return new DVMPAluuMinusSin1Phi(*this);
}

void DVMPAluuMinusSin1Phi::configure(const ElemUtils::Parameters &parameters) {

    DVMPAluuMinus::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVMPAluuMinusSin1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {

    DVMPObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setPhi(PhysicalType<double>(x, PhysicalUnit::RAD));

    return DVMPAluuMinus::computeObservable(kinematic, gpdType).getValue() * sin(x);
}

PhysicalType<double> DVMPAluuMinusSin1Phi::computeObservable(
        const DVMPObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
