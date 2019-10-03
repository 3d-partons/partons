#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAllPlusCos1Phi.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/observable/Observable.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAllPlusCos1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAllPlusCos1Phi("DVCSAllPlusCos1Phi"));

DVCSAllPlusCos1Phi::DVCSAllPlusCos1Phi(const std::string &className) :
        DVCSAllPlus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAllPlusCos1Phi::DVCSAllPlusCos1Phi(const DVCSAllPlusCos1Phi& other) :
        DVCSAllPlus(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAllPlusCos1Phi::~DVCSAllPlusCos1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAllPlusCos1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAllPlusCos1Phi::functionToIntegrateObservable);
}

DVCSAllPlusCos1Phi* DVCSAllPlusCos1Phi::clone() const {
    return new DVCSAllPlusCos1Phi(*this);
}

void DVCSAllPlusCos1Phi::configure(const ElemUtils::Parameters &parameters) {

    DVCSAllPlus::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAllPlusCos1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {

    DVCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setPhi(PhysicalType<double>(x, PhysicalUnit::RAD));

    return DVCSAllPlus::computeObservable(kinematic, gpdType).getValue()
            * cos(x);
}

PhysicalType<double> DVCSAllPlusCos1Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(
            kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
