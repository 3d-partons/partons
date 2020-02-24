#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAltDVCSCosPhiMPhisCos1Phi.h"

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

const unsigned int DVCSAltDVCSCosPhiMPhisCos1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAltDVCSCosPhiMPhisCos1Phi(
                        "DVCSAltDVCSCosPhiMPhisCos1Phi"));

DVCSAltDVCSCosPhiMPhisCos1Phi::DVCSAltDVCSCosPhiMPhisCos1Phi(
        const std::string &className) :
        DVCSAltDVCSCosPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAltDVCSCosPhiMPhisCos1Phi::DVCSAltDVCSCosPhiMPhisCos1Phi(
        const DVCSAltDVCSCosPhiMPhisCos1Phi& other) :
        DVCSAltDVCSCosPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAltDVCSCosPhiMPhisCos1Phi::~DVCSAltDVCSCosPhiMPhisCos1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAltDVCSCosPhiMPhisCos1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAltDVCSCosPhiMPhisCos1Phi::functionToIntegrateObservable);
}

DVCSAltDVCSCosPhiMPhisCos1Phi* DVCSAltDVCSCosPhiMPhisCos1Phi::clone() const {
    return new DVCSAltDVCSCosPhiMPhisCos1Phi(*this);
}

void DVCSAltDVCSCosPhiMPhisCos1Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAltDVCSCosPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAltDVCSCosPhiMPhisCos1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {

    DVCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setPhi(PhysicalType<double>(x, PhysicalUnit::RAD));

    return DVCSAltDVCSCosPhiMPhis::computeObservable(kinematic, gpdType).getValue()
            * cos(x);
}

PhysicalType<double> DVCSAltDVCSCosPhiMPhisCos1Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(
            kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
