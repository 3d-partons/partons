#include "../../../../include/partons/modules/double_distribution/DoubleDistributionExample.h"

#include <NumA/integration/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <map>
#include <utility>

#include "../../../../include/partons/beans/double_distribution/DoubleDistributionType.h"
#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"

// Initialise [class]::classId with a unique name and selfregister this module into the global registry.
const unsigned int DoubleDistributionExample::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DoubleDistributionExample("DoubleDistributionExample"));

DoubleDistributionExample::DoubleDistributionExample(
        const std::string &className) :
        DoubleDistributionModule(className), MathIntegratorModule() {

    // Tell to parent class which dual distribution compute type are available for this model.
    // If there is just computeF() and computeG() available just remove computeK() line.

    m_listOfAvailableDualDistributionFunctions.insert(
            std::make_pair(DoubleDistributionType::F,
                    &DoubleDistributionModule::computeF));
    m_listOfAvailableDualDistributionFunctions.insert(
            std::make_pair(DoubleDistributionType::G,
                    &DoubleDistributionModule::computeG));
    m_listOfAvailableDualDistributionFunctions.insert(
            std::make_pair(DoubleDistributionType::K,
                    &DoubleDistributionModule::computeK));

}

void DoubleDistributionExample::initFunctorsForIntegrations() {
    m_pIntegrateExample = NumA::Integrator1D::newIntegrationFunctor(this,
            &DoubleDistributionExample::integrateExample);
}

DoubleDistributionExample::~DoubleDistributionExample() {
    if (m_pIntegrateExample) {
        delete m_pIntegrateExample;
        m_pIntegrateExample = 0;
    }
}

DoubleDistributionExample::DoubleDistributionExample(
        const DoubleDistributionExample& other) :
        DoubleDistributionModule(other), MathIntegratorModule(other) {

    // Copy each class variables ; see copy constructor from parent class "DualDistributionModule"

    initFunctorsForIntegrations();
}

DoubleDistributionExample* DoubleDistributionExample::clone() const {
    return new DoubleDistributionExample(*this);
}

void DoubleDistributionExample::initModule() {
    // Before call parent init.
    DoubleDistributionModule::initModule();

    m_mathIntegrator = NumA::Integrator1D::newIntegrator(
            NumA::IntegratorType1D::GK21_ADAPTIVE);

    // Compute some variables depend from kinematic value before use this specific module.
    // See "src/modules/convolCoeffFunction/DVCSCFFModel::initModule()" for example.
}

void DoubleDistributionExample::isModuleWellConfigured() {
    // Before check if parent class is well configured
    DoubleDistributionModule::isModuleWellConfigured();

    //TODO check variables range of this module and other stuff
    // See "src/modules/EvolQCDModule::isModuleWellConfigured()" for example.
}

PartonDistribution DoubleDistributionExample::computeF() {
    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    //TODO implement

    std::vector<double> parameters;

    m_mathIntegrator->integrate(m_pIntegrateExample, -1., 1., parameters);

    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

PartonDistribution DoubleDistributionExample::computeG() {
    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    // TODO implement

    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

PartonDistribution DoubleDistributionExample::computeK() {
    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    // TODO implement

    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

double DoubleDistributionExample::integrateExample(double x,
        std::vector<double>& parameters) {
    return 1.;
}
