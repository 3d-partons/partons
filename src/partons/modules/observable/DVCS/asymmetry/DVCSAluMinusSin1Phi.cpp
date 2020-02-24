#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAluMinusSin1Phi.h"

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

const unsigned int DVCSAluMinusSin1Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAluMinusSin1Phi("DVCSAluMinusSin1Phi"));

DVCSAluMinusSin1Phi::DVCSAluMinusSin1Phi(const std::string &className) :
        DVCSAluMinus(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAluMinusSin1Phi::DVCSAluMinusSin1Phi(const DVCSAluMinusSin1Phi& other) :
        DVCSAluMinus(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

DVCSAluMinusSin1Phi::~DVCSAluMinusSin1Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }

}

void DVCSAluMinusSin1Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAluMinusSin1Phi::functionToIntegrateObservable);
}

DVCSAluMinusSin1Phi* DVCSAluMinusSin1Phi::clone() const {
    return new DVCSAluMinusSin1Phi(*this);
}

void DVCSAluMinusSin1Phi::configure(const ElemUtils::Parameters &parameters) {

    DVCSAluMinus::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAluMinusSin1Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {

    DVCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setPhi(PhysicalType<double>(x, PhysicalUnit::RAD));

    return DVCSAluMinus::computeObservable(kinematic, gpdType).getValue() * sin(x);
}

PhysicalType<double> DVCSAluMinusSin1Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
