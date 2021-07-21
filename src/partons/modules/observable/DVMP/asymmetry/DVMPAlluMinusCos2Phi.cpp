#include "../../../../../../include/partons/modules/observable/DVMP/asymmetry/DVMPAlluMinusCos2Phi.h"

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

const unsigned int DVMPAlluMinusCos2Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPAlluMinusCos2Phi("DVMPAlluMinusCos2Phi"));

DVMPAlluMinusCos2Phi::DVMPAlluMinusCos2Phi(const std::string &className) :
        DVMPAlluMinus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVMPAlluMinusCos2Phi::DVMPAlluMinusCos2Phi(const DVMPAlluMinusCos2Phi& other) :
        DVMPAlluMinus(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

DVMPAlluMinusCos2Phi::~DVMPAlluMinusCos2Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVMPAlluMinusCos2Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVMPAlluMinusCos2Phi::functionToIntegrateObservable);
}

DVMPAlluMinusCos2Phi* DVMPAlluMinusCos2Phi::clone() const {
    return new DVMPAlluMinusCos2Phi(*this);
}

void DVMPAlluMinusCos2Phi::configure(const ElemUtils::Parameters &parameters) {

    DVMPAlluMinus::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVMPAlluMinusCos2Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {

    DVMPObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setPhi(PhysicalType<double>(x, PhysicalUnit::RAD));

    return DVMPAlluMinus::computeObservable(kinematic, gpdType).getValue()
            * cos(2 * x);
}

PhysicalType<double> DVMPAlluMinusCos2Phi::computeObservable(
        const DVMPObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(
            kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
