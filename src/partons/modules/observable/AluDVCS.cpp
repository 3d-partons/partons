#include "../../../../include/partons/modules/observable/AluDVCS.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/ProcessModule.h"

// Initialise [class]::classId with a unique name.
const unsigned int AluDVCS::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(new AluDVCS("AluDVCS"));

AluDVCS::AluDVCS(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

AluDVCS::AluDVCS(const AluDVCS& other) :
        Observable(other) {
}

AluDVCS::~AluDVCS() {
}

AluDVCS* AluDVCS::clone() const {
    return new AluDVCS(*this);
}

// optimisation remplacement des multiples appels similaires par A - B / A + B
double AluDVCS::computePhiObservable(double phi) {

    double result = 0.;

    double SigmaPP = m_pProcessModule->computeCrossSection(+1, +1,
            NumA::Vector3D(0., 0., 0.), phi);

    double SigmaPM = m_pProcessModule->computeCrossSection(-1, +1,
            NumA::Vector3D(0., 0., 0.), phi);

    double SigmaMP = m_pProcessModule->computeCrossSection(+1, -1,
            NumA::Vector3D(0., 0., 0.), phi);

    double SigmaMM = m_pProcessModule->computeCrossSection(-1, -1,
            NumA::Vector3D(0., 0., 0.), phi);

    double SigmaUU = m_pProcessModule->computeCrossSection(0., 0,
            NumA::Vector3D(0., 0., 0.), phi);

    //TODO !!! division par zero !!!
    result = ((SigmaPP + SigmaPM) - (SigmaMP + SigmaMM)) / (4 * SigmaUU);

    return result;
}
