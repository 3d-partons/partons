#include "../../../../../../include/partons/modules/observable/DVMP/asymmetry/DVMPAuluMinusSin1Phi.h"

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

const unsigned int DVMPAuluMinusSin1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPAuluMinusSin1Phi("DVMPAuluMinusSin1Phi"));

DVMPAuluMinusSin1Phi::DVMPAuluMinusSin1Phi(const std::string &className) :
        DVMPAuluMinus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVMPAuluMinusSin1Phi::DVMPAuluMinusSin1Phi(const DVMPAuluMinusSin1Phi& other) :
        DVMPAuluMinus(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

DVMPAuluMinusSin1Phi::~DVMPAuluMinusSin1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVMPAuluMinusSin1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVMPAuluMinusSin1Phi::functionToIntegrateObservable);
}

DVMPAuluMinusSin1Phi* DVMPAuluMinusSin1Phi::clone() const {
    return new DVMPAuluMinusSin1Phi(*this);
}

void DVMPAuluMinusSin1Phi::configure(const ElemUtils::Parameters &parameters) {

    DVMPAuluMinus::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVMPAuluMinusSin1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {

    DVMPObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setPhi(PhysicalType<double>(x, PhysicalUnit::RAD));

    return DVMPAuluMinus::computeObservable(kinematic, gpdType).getValue() * sin(x);
}

PhysicalType<double> DVMPAuluMinusSin1Phi::computeObservable(
        const DVMPObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
