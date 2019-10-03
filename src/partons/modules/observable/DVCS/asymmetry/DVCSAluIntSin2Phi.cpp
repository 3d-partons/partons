#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAluIntSin2Phi.h"

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

const unsigned int DVCSAluIntSin2Phi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAluIntSin2Phi("DVCSAluIntSin2Phi"));

DVCSAluIntSin2Phi::DVCSAluIntSin2Phi(const std::string &className) :
        DVCSAluInt(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

DVCSAluIntSin2Phi::DVCSAluIntSin2Phi(const DVCSAluIntSin2Phi& other) :
        DVCSAluInt(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

DVCSAluIntSin2Phi::~DVCSAluIntSin2Phi() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSAluIntSin2Phi::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSAluIntSin2Phi::functionToIntegrateObservable);
}

DVCSAluIntSin2Phi* DVCSAluIntSin2Phi::clone() const {
    return new DVCSAluIntSin2Phi(*this);
}

void DVCSAluIntSin2Phi::configure(const ElemUtils::Parameters &parameters) {

    DVCSAluInt::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSAluIntSin2Phi::functionToIntegrateObservable(double x,
        std::vector<double> params) {

    DVCSObservableKinematic kinematic;
    List<GPDType> gpdType;

    unserializeKinematicsAndGPDTypesFromStdVector(params, kinematic, gpdType);

    kinematic.setPhi(PhysicalType<double>(x, PhysicalUnit::RAD));

    return DVCSAluInt::computeObservable(kinematic, gpdType).getValue() * sin(2 * x);
}

PhysicalType<double> DVCSAluIntSin2Phi::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    std::vector<double> params = serializeKinematicsAndGPDTypesIntoStdVector(kinematic, gpdType);

    return PhysicalType<double>(
            integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
                    params) / (Constant::PI), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
