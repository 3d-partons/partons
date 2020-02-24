#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAltIntSinPhiMPhisSin2Phi.h"

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

const unsigned int DVCSAltIntSinPhiMPhisSin2Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAltIntSinPhiMPhisSin2Phi(
                        "DVCSAltIntSinPhiMPhisSin2Phi"));

DVCSAltIntSinPhiMPhisSin2Phi::DVCSAltIntSinPhiMPhisSin2Phi(
        const std::string &className) :
        DVCSAltIntSinPhiMPhis(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAltIntSinPhiMPhisSin2Phi::DVCSAltIntSinPhiMPhisSin2Phi(
        const DVCSAltIntSinPhiMPhisSin2Phi& other) :
        DVCSAltIntSinPhiMPhis(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAltIntSinPhiMPhisSin2Phi::~DVCSAltIntSinPhiMPhisSin2Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAltIntSinPhiMPhisSin2Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAltIntSinPhiMPhisSin2Phi::functionToIntegrateObservable);
}

DVCSAltIntSinPhiMPhisSin2Phi* DVCSAltIntSinPhiMPhisSin2Phi::clone() const {
    return new DVCSAltIntSinPhiMPhisSin2Phi(*this);
}

void DVCSAltIntSinPhiMPhisSin2Phi::configure(
        const ElemUtils::Parameters &parameters) {

    DVCSAltIntSinPhiMPhis::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAltIntSinPhiMPhisSin2Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {

    DVCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setPhi(PhysicalType<double>(x, PhysicalUnit::RAD));

    return DVCSAltIntSinPhiMPhis::computeObservable(kinematic, gpdType).getValue() * sin(2 * x);
}

PhysicalType<double> DVCSAltIntSinPhiMPhisSin2Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
