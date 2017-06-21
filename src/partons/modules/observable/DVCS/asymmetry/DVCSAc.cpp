#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAc.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/ProcessModule.h"

const unsigned int DVCSAc::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAc("DVCSAc"));

DVCSAc::DVCSAc(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

DVCSAc::DVCSAc(const DVCSAc& other) :
        Observable(other) {
}

DVCSAc::~DVCSAc() {
}

DVCSAc* DVCSAc::clone() const {
    return new DVCSAc(*this);
}

double DVCSAc::computePhiObservable(double phi) {

    double A = m_pProcessModule->computeCrossSection(+1, +1,
            NumA::Vector3D(0., 0., 0.), phi);

    double B = m_pProcessModule->computeCrossSection(-1, +1,
            NumA::Vector3D(0., 0., 0.), phi);

    double C = m_pProcessModule->computeCrossSection(+1, -1,
            NumA::Vector3D(0., 0., 0.), phi);

    double D = m_pProcessModule->computeCrossSection(-1, -1,
            NumA::Vector3D(0., 0., 0.), phi);

    if(A + B + C + D == 0.){

        warn(__func__, "Asymmetry denominator is zero.");
        return 0.;
    }

    return ((A + B) - (C + D)) / ((A + B) + (C + D));
}
