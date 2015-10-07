#include "FourierObservable.h"

#include <NumA/MathIntegrator.h>

#include "../../FundamentalPhysicalConstants.h"
#include "ObservableType.h"

FourierObservable::FourierObservable(const std::string &className) :
        Observable(className) {
    m_observableType = ObservableType::FOURIER;
    m_mathIntegrator.setIntegrationMode(NumA::MathIntegrator::ROOT);
}

FourierObservable::~FourierObservable() {
    // TODO Auto-generated destructor stub
}

//TODO check
double FourierObservable::compute() {
    return m_mathIntegrator.integrateWithROOT(this,
            &FourierObservable::functionToIntegrate, 0., (2 * PI));
}
