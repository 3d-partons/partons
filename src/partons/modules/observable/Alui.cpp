#include "../../../../include/partons/modules/observable/Alui.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/ProcessModule.h"

// Initialise [class]::classId with a unique name.
const unsigned int Alui::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(new Alui("Alui"));

Alui::Alui(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

Alui::Alui(const Alui& other) :
        Observable(other) {
}

Alui::~Alui() {
}

Alui* Alui::clone() const {
    return new Alui(*this);
}

// optimisation remplacement des multiples appels similaires par A - B / A + B
double Alui::compute(double phi) {

    double result = 0.;

    double SigmaPP = m_pProcessModule->computeCrossSection(+1, +1,
            NumA::Vector3D(0., 0., 0.), phi);

    double SigmaPM = m_pProcessModule->computeCrossSection(-1, +1,
            NumA::Vector3D(0., 0., 0.), phi);

    double SigmaMP = m_pProcessModule->computeCrossSection(+1, -1,
            NumA::Vector3D(0., 0., 0.), phi);

    double SigmaMM = m_pProcessModule->computeCrossSection(-1, -1,
            NumA::Vector3D(0., 0., 0.), phi);

    //TODO !!! division par zero !!!
    result = ((SigmaPP - SigmaPM) - (SigmaMP - SigmaMM))
            / (SigmaPP + SigmaPM + SigmaMP + SigmaMM);

    return result;
}
