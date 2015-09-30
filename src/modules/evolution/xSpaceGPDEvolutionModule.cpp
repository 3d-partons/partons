#include "xSpaceGPDEvolutionModule.h"

#include <NumA/MathIntegrator.h>

#include "../../beans/active_flavors/NfInterval.h"

xSpaceGPDEvolutionModule::xSpaceGPDEvolutionModule(const std::string &className) :
        GPDEvolutionModule(className), MathIntegratorModule() {
    // TODO Auto-generated constructor stub

}

xSpaceGPDEvolutionModule::~xSpaceGPDEvolutionModule() {
    // TODO Auto-generated destructor stub
}

xSpaceGPDEvolutionModule::xSpaceGPDEvolutionModule(
        const xSpaceGPDEvolutionModule &other) :
        GPDEvolutionModule(other), MathIntegratorModule(other) {
}

void xSpaceGPDEvolutionModule::initModule() {
    m_mathIntegrator.setIntegrationMode(NumA::MathIntegrator::ROOT);
}

void xSpaceGPDEvolutionModule::isModuleWellConfigured() {
}

double xSpaceGPDEvolutionModule::nonSingletMuFDerivative(
        const NfInterval &nfInterval) {

    return m_mathIntegrator.integrateWithROOT(this,
            &xSpaceGPDEvolutionModule::integrateNonSinglet,
            nfInterval.getLowerBound(), nfInterval.getUpperBound());
}

double xSpaceGPDEvolutionModule::integrateNonSinglet(double* variables,
        double* parameters) {
    // variables[0] = y
    // variables[0] = MuF2

    // virtuality : kernelNonSinglet evaluated by child
    return kernelNonSinglet(variables[0])
            * nonSingletGPD(m_currentNonSingletIndex, m_currentNf, variables[0],
                    variables[1]);
}

double xSpaceGPDEvolutionModule::singletMuFDerivative(
        const NfInterval &nfInterval) {

    return m_mathIntegrator.integrateWithROOT(this,
            &xSpaceGPDEvolutionModule::integrateSinglet,
            nfInterval.getLowerBound(), nfInterval.getUpperBound());
}

double xSpaceGPDEvolutionModule::integrateSinglet(double *variables,
        double *parameters) {
    // variables[0] = y
    // m_partonDistributionEvolutionBase[0] = FG (previous value)
    // m_partonDistributionEvolutionBase[1] = FS (previous value)

    // virtuality : Vqq & Vqg evaluated by child
    return kernelSingletqq(variables[0]) * m_partonDistributionEvolutionBase[1]
            + kernelSingletqg(variables[0])
                    * m_partonDistributionEvolutionBase[0];
}

double xSpaceGPDEvolutionModule::gluonMuFDerivative(
        const NfInterval &nfInterval) {

    return m_mathIntegrator.integrateWithROOT(this,
            &xSpaceGPDEvolutionModule::integrateGluon,
            nfInterval.getLowerBound(), nfInterval.getUpperBound());
}

double xSpaceGPDEvolutionModule::integrateGluon(double *variables,
        double *parameters) {
    // variables[0] = y
    // m_partonDistributionEvolutionBase[0] = FG (previous value)
    // m_partonDistributionEvolutionBase[1] = FS (previous value)

    // virtuality : Vgq & Vgg evaluated by child
    return kernekSingletgq(variables[0]) * m_partonDistributionEvolutionBase[1]
            + kernelSingletgg(variables[0])
                    * m_partonDistributionEvolutionBase[0];
}
