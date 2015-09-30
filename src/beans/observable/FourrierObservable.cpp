#include "FourrierObservable.h"

#include <NumA/MathIntegrator.h>

#include "../../FundamentalPhysicalConstants.h"
#include "ObservableType.h"

FourrierObservable::FourrierObservable(const std::string &className) :
        Observable(className) {
    m_observableType = ObservableType::FOURRIER;
    m_mathIntegrator.setIntegrationMode(NumA::MathIntegrator::ROOT);
}

FourrierObservable::~FourrierObservable() {
    // TODO Auto-generated destructor stub
}

//TODO check
double FourrierObservable::compute() {
    return m_mathIntegrator.integrateWithROOT(this,
            &FourrierObservable::functionToIntegrate, 0., (2 * PI));
}
