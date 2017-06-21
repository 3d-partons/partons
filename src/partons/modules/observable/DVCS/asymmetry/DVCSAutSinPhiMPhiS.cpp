#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAutSinPhiMPhiS.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/ProcessModule.h"

const unsigned int DVCSAutSinPhiMPhiS::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAutSinPhiMPhiS("DVCSAutSinPhiMPhiS"));

DVCSAutSinPhiMPhiS::DVCSAutSinPhiMPhiS(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

DVCSAutSinPhiMPhiS::DVCSAutSinPhiMPhiS(const DVCSAutSinPhiMPhiS& other) :
        Observable(other) {
}

DVCSAutSinPhiMPhiS::~DVCSAutSinPhiMPhiS() {
}

DVCSAutSinPhiMPhiS* DVCSAutSinPhiMPhiS::clone() const {
    return new DVCSAutSinPhiMPhiS(*this);
}

double DVCSAutSinPhiMPhiS::computePhiObservable(double phi) {

    double A = m_pProcessModule->computeCrossSection(+1, -1,
            NumA::Vector3D(0., -1., 0.), phi);

    double B = m_pProcessModule->computeCrossSection(-1, -1,
            NumA::Vector3D(0., -1., 0.), phi);

    double C = m_pProcessModule->computeCrossSection(+1, -1,
            NumA::Vector3D(0., +1., 0.), phi);

    double D = m_pProcessModule->computeCrossSection(-1, -1,
            NumA::Vector3D(0., +1., 0.), phi);

    if (A + B + C + D == 0.) {

        warn(__func__, "Asymmetry denominator is zero.");
        return 0.;
    }

    return ((A + B) - (C + D)) / ((A + B) + (C + D));
}
