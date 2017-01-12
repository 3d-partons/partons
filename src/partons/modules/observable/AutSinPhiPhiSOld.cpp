#include "../../../../include/partons/modules/observable/AutSinPhiPhiSOld.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/ProcessModule.h"

// Initialise [class]::classId with a unique name.
const unsigned int AutSinPhiPhiSOld::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(new AutSinPhiPhiSOld("AutSinPhiPhiSOld"));

AutSinPhiPhiSOld::AutSinPhiPhiSOld(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

AutSinPhiPhiSOld::AutSinPhiPhiSOld(const AutSinPhiPhiSOld& other) :
        Observable(other) {
}

AutSinPhiPhiSOld::~AutSinPhiPhiSOld() {
}

AutSinPhiPhiSOld* AutSinPhiPhiSOld::clone() const {
    return new AutSinPhiPhiSOld(*this);
}

double AutSinPhiPhiSOld::computePhiObservable(double phi) {

    double result = 0.;

    double A = m_pProcessModule->computeCrossSection(0, -1,
            NumA::Vector3D(0., 1., 0.), phi);

    double B = m_pProcessModule->computeCrossSection(0, -1,
            NumA::Vector3D(0., -1., 0.), phi);

    //TODO !!! division par zero !!!
    result = (A - B) / (A + B);

    return result;
}
