#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAutMinusSinPhiMPhis.h"
#include "../../../../../../include/partons/modules/process/ProcessModule.h"

namespace PARTONS {

const unsigned int DVCSAutMinusSinPhiMPhis::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAutMinusSinPhiMPhis("DVCSAutMinusSinPhiMPhis"));

DVCSAutMinusSinPhiMPhis::DVCSAutMinusSinPhiMPhis(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

DVCSAutMinusSinPhiMPhis::DVCSAutMinusSinPhiMPhis(
        const DVCSAutMinusSinPhiMPhis& other) :
        Observable(other) {
}

DVCSAutMinusSinPhiMPhis::~DVCSAutMinusSinPhiMPhis() {
}

DVCSAutMinusSinPhiMPhis* DVCSAutMinusSinPhiMPhis::clone() const {
    return new DVCSAutMinusSinPhiMPhis(*this);
}

void DVCSAutMinusSinPhiMPhis::configure(
        const ElemUtils::Parameters &parameters) {
    Observable::configure(parameters);
}

double DVCSAutMinusSinPhiMPhis::computePhiObservable(double phi) {

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

} /* namespace PARTONS */
