#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAutSinPhiMPhis.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/ProcessModule.h"

namespace PARTONS {


const unsigned int DVCSAutSinPhiMPhis::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAutSinPhiMPhis("DVCSAutSinPhiMPhis"));

DVCSAutSinPhiMPhis::DVCSAutSinPhiMPhis(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

DVCSAutSinPhiMPhis::DVCSAutSinPhiMPhis(const DVCSAutSinPhiMPhis& other) :
        Observable(other) {
}

DVCSAutSinPhiMPhis::~DVCSAutSinPhiMPhis() {
}

DVCSAutSinPhiMPhis* DVCSAutSinPhiMPhis::clone() const {
    return new DVCSAutSinPhiMPhis(*this);
}

void DVCSAutSinPhiMPhis::configure(const ElemUtils::Parameters &parameters) {
	Observable::configure(parameters);
}

double DVCSAutSinPhiMPhis::computePhiObservable(double phi) {

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
