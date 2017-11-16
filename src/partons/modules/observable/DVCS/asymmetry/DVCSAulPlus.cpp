#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAulPlus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/ProcessModule.h"

namespace PARTONS {

const unsigned int DVCSAulPlus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAulPlus("DVCSAulPlus"));

DVCSAulPlus::DVCSAulPlus(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

DVCSAulPlus::DVCSAulPlus(const DVCSAulPlus& other) :
        Observable(other) {
}

DVCSAulPlus::~DVCSAulPlus() {
}

DVCSAulPlus* DVCSAulPlus::clone() const {
    return new DVCSAulPlus(*this);
}

void DVCSAulPlus::configure(const ElemUtils::Parameters &parameters) {
    Observable::configure(parameters);
}

double DVCSAulPlus::computePhiObservable(double phi) {

    double A = m_pProcessModule->computeCrossSection(+1, +1,
            NumA::Vector3D(0., 0., -1.), phi);

    double B = m_pProcessModule->computeCrossSection(-1, +1,
            NumA::Vector3D(0., 0., -1.), phi);

    double C = m_pProcessModule->computeCrossSection(+1, +1,
            NumA::Vector3D(0., 0., +1.), phi);

    double D = m_pProcessModule->computeCrossSection(-1, +1,
            NumA::Vector3D(0., 0., +1.), phi);

    if (A + B + C + D == 0.) {

        warn(__func__, "Asymmetry denominator is zero.");
        return 0.;
    }

    return ((A + B) - (C + D)) / ((A + B) + (C + D));
}

} /* namespace PARTONS */
