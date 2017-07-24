#include "../../../../../../include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionPhiIntegrated.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

// Initialise [class]::classId with a unique name.
const unsigned int DVCSCrossSectionPhiIntegrated::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCrossSectionPhiIntegrated("DVCSCrossSectionPhiIntegrated"));

DVCSCrossSectionPhiIntegrated::DVCSCrossSectionPhiIntegrated(const std::string &className) :
        DVCSCrossSection(className), MathIntegratorModule(), m_pFunctionToIntegrateObservable(
                0) {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);

    initFunctorsForIntegrations();
}

DVCSCrossSectionPhiIntegrated::DVCSCrossSectionPhiIntegrated(const DVCSCrossSectionPhiIntegrated& other) :
        DVCSCrossSection(other), MathIntegratorModule(other) {

    m_pFunctionToIntegrateObservable = 0;

    initFunctorsForIntegrations();
}

DVCSCrossSectionPhiIntegrated::~DVCSCrossSectionPhiIntegrated() {
    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

void DVCSCrossSectionPhiIntegrated::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &DVCSCrossSectionPhiIntegrated::functionToIntegrateObservable);
}

DVCSCrossSectionPhiIntegrated* DVCSCrossSectionPhiIntegrated::clone() const {
    return new DVCSCrossSectionPhiIntegrated(*this);
}

void DVCSCrossSectionPhiIntegrated::configure(const ElemUtils::Parameters &parameters) {

	DVCSCrossSection::configure(parameters);
	MathIntegratorModule::configureIntegrator(parameters);
}

double DVCSCrossSectionPhiIntegrated::functionToIntegrateObservable(double x,
        std::vector<double> params) {
    return DVCSCrossSection::computePhiObservable(x);
}

double DVCSCrossSectionPhiIntegrated::computeFourierObservable() {
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters);
}
