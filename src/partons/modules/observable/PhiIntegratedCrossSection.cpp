#include "../../../../include/partons/modules/observable/PhiIntegratedCrossSection.h"

#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int PhiIntegratedCrossSection::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new PhiIntegratedCrossSection("PhiIntegratedCrossSection"));

PhiIntegratedCrossSection::PhiIntegratedCrossSection(const std::string &className) :
        CrossSectionObservable(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);

    initFunctorsForIntegrations();
}

PhiIntegratedCrossSection::PhiIntegratedCrossSection(const PhiIntegratedCrossSection& other) :
        CrossSectionObservable(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;

    initFunctorsForIntegrations();
}

PhiIntegratedCrossSection::~PhiIntegratedCrossSection() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void PhiIntegratedCrossSection::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &PhiIntegratedCrossSection::functionToIntegrateObservable);
}

PhiIntegratedCrossSection* PhiIntegratedCrossSection::clone() const {
    return new PhiIntegratedCrossSection(*this);
}

double PhiIntegratedCrossSection::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return CrossSectionObservable::computePhiObservable(x);
}

double PhiIntegratedCrossSection::computeFourierObservable() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters);
}
