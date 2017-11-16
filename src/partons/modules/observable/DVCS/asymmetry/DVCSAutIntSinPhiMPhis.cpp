#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAutIntSinPhiMPhis.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/ProcessModule.h"

namespace PARTONS {

const unsigned int DVCSAutIntSinPhiMPhis::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAutIntSinPhiMPhis("DVCSAutIntSinPhiMPhis"));

DVCSAutIntSinPhiMPhis::DVCSAutIntSinPhiMPhis(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

DVCSAutIntSinPhiMPhis::DVCSAutIntSinPhiMPhis(const DVCSAutIntSinPhiMPhis& other) :
        Observable(other) {
}

DVCSAutIntSinPhiMPhis::~DVCSAutIntSinPhiMPhis() {
}

DVCSAutIntSinPhiMPhis* DVCSAutIntSinPhiMPhis::clone() const {
    return new DVCSAutIntSinPhiMPhis(*this);
}

void DVCSAutIntSinPhiMPhis::configure(const ElemUtils::Parameters &parameters) {
    Observable::configure(parameters);
}

double DVCSAutIntSinPhiMPhis::computePhiObservable(double phi) {

    double A = m_pProcessModule->computeCrossSection(+1, +1,
            NumA::Vector3D(0., -1., 0.), phi);

    double B = m_pProcessModule->computeCrossSection(-1, +1,
            NumA::Vector3D(0., -1., 0.), phi);

    double C = m_pProcessModule->computeCrossSection(+1, -1,
            NumA::Vector3D(0., -1., 0.), phi);

    double D = m_pProcessModule->computeCrossSection(-1, -1,
            NumA::Vector3D(0., -1., 0.), phi);

    double E = m_pProcessModule->computeCrossSection(+1, +1,
            NumA::Vector3D(0., +1., 0.), phi);

    double F = m_pProcessModule->computeCrossSection(-1, +1,
            NumA::Vector3D(0., +1., 0.), phi);

    double G = m_pProcessModule->computeCrossSection(+1, -1,
            NumA::Vector3D(0., +1., 0.), phi);

    double H = m_pProcessModule->computeCrossSection(-1, -1,
            NumA::Vector3D(0., +1., 0.), phi);

    if (A + B + C + D + E + F + G + H == 0.) {

        warn(__func__, "Asymmetry denominator is zero.");
        return 0.;
    }

    return ((A + B - C - D) - (E + F - G - H))
            / ((A + B + C + D) + (E + F + G + H));
}

} /* namespace PARTONS */
