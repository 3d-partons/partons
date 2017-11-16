#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAltDVCSSinPhiMPhis.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/ProcessModule.h"

namespace PARTONS {

const unsigned int DVCSAltDVCSSinPhiMPhis::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAltDVCSSinPhiMPhis("DVCSAltDVCSSinPhiMPhis"));

DVCSAltDVCSSinPhiMPhis::DVCSAltDVCSSinPhiMPhis(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

DVCSAltDVCSSinPhiMPhis::DVCSAltDVCSSinPhiMPhis(const DVCSAltDVCSSinPhiMPhis& other) :
        Observable(other) {
}

DVCSAltDVCSSinPhiMPhis::~DVCSAltDVCSSinPhiMPhis() {
}

DVCSAltDVCSSinPhiMPhis* DVCSAltDVCSSinPhiMPhis::clone() const {
    return new DVCSAltDVCSSinPhiMPhis(*this);
}

void DVCSAltDVCSSinPhiMPhis::configure(const ElemUtils::Parameters &parameters) {
    Observable::configure(parameters);
}

double DVCSAltDVCSSinPhiMPhis::computePhiObservable(double phi) {

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

    return ((A - B + C - D) - (E - F + G - H))
            / ((A + B + C + D) + (E + F + G + H));
}

} /* namespace PARTONS */
