#include "../../../../include/partons/modules/observable/AutSinPhiPhiS.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/ProcessModule.h"

// Initialise [class]::classId with a unique name.
const unsigned int AutSinPhiPhiS::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(new AutSinPhiPhiS("AutSinPhiPhiS"));

AutSinPhiPhiS::AutSinPhiPhiS(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

AutSinPhiPhiS::AutSinPhiPhiS(const AutSinPhiPhiS& other) :
        Observable(other) {
}

AutSinPhiPhiS::~AutSinPhiPhiS() {
}

AutSinPhiPhiS* AutSinPhiPhiS::clone() const {
    return new AutSinPhiPhiS(*this);
}

double AutSinPhiPhiS::computePhiObservable(double phi) {

    double result = 0.;

    double A = m_pProcessModule->computeCrossSection(0, -1,
            NumA::Vector3D(0., 0., 0.), phi);

    double B = m_pProcessModule->computeCrossSection(0, -1,
            NumA::Vector3D(0., -1., 0.), phi);

    //TODO !!! division par zero !!!
   // result = (A - B) / (A + B);
    result = B/A-1;
    return result;
}
