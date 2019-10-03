#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAltIntCosPhiMPhisCos1Phi.h"

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

const unsigned int DVCSAltIntCosPhiMPhisCos1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAltIntCosPhiMPhisCos1Phi(
                        "DVCSAltIntCosPhiMPhisCos1Phi"));

DVCSAltIntCosPhiMPhisCos1Phi::DVCSAltIntCosPhiMPhisCos1Phi(
        const std::string &className) :
        DVCSAltIntCosPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAltIntCosPhiMPhisCos1Phi::DVCSAltIntCosPhiMPhisCos1Phi(
        const DVCSAltIntCosPhiMPhisCos1Phi& other) :
        DVCSAltIntCosPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAltIntCosPhiMPhisCos1Phi::~DVCSAltIntCosPhiMPhisCos1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAltIntCosPhiMPhisCos1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAltIntCosPhiMPhisCos1Phi::functionToIntegrateObservable);
}

DVCSAltIntCosPhiMPhisCos1Phi* DVCSAltIntCosPhiMPhisCos1Phi::clone() const {
    return new DVCSAltIntCosPhiMPhisCos1Phi(*this);
}

void DVCSAltIntCosPhiMPhisCos1Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAltIntCosPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAltIntCosPhiMPhisCos1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {

    DVCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setPhi(PhysicalType<double>(x, PhysicalUnit::RAD));

    return DVCSAltIntCosPhiMPhis::computeObservable(kinematic, gpdType).getValue() * cos(x);
}

PhysicalType<double> DVCSAltIntCosPhiMPhisCos1Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
