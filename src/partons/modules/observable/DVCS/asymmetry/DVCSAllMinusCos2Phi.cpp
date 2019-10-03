#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAllMinusCos2Phi.h"

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

const unsigned int DVCSAllMinusCos2Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAllMinusCos2Phi("DVCSAllMinusCos2Phi"));

DVCSAllMinusCos2Phi::DVCSAllMinusCos2Phi(const std::string &className) :
        DVCSAllMinus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAllMinusCos2Phi::DVCSAllMinusCos2Phi(const DVCSAllMinusCos2Phi& other) :
        DVCSAllMinus(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;
    initFunctorsForIntegrations();
}

DVCSAllMinusCos2Phi::~DVCSAllMinusCos2Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAllMinusCos2Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAllMinusCos2Phi::functionToIntegrateObservable);
}

DVCSAllMinusCos2Phi* DVCSAllMinusCos2Phi::clone() const {
    return new DVCSAllMinusCos2Phi(*this);
}

void DVCSAllMinusCos2Phi::configure(const ElemUtils::Parameters &parameters) {

    DVCSAllMinus::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAllMinusCos2Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {

    DVCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setPhi(PhysicalType<double>(x, PhysicalUnit::RAD));

    return DVCSAllMinus::computeObservable(kinematic, gpdType).getValue()
            * cos(2 * x);
}

PhysicalType<double> DVCSAllMinusCos2Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(
            kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
