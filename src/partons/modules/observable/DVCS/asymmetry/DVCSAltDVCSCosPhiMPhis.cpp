#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAltDVCSCosPhiMPhis.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/ProcessModule.h"

namespace PARTONS {

const unsigned int DVCSAltDVCSCosPhiMPhis::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAltDVCSCosPhiMPhis("DVCSAltDVCSCosPhiMPhis"));

DVCSAltDVCSCosPhiMPhis::DVCSAltDVCSCosPhiMPhis(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

DVCSAltDVCSCosPhiMPhis::DVCSAltDVCSCosPhiMPhis(const DVCSAltDVCSCosPhiMPhis& other) :
        Observable(other) {
}

DVCSAltDVCSCosPhiMPhis::~DVCSAltDVCSCosPhiMPhis() {
}

DVCSAltDVCSCosPhiMPhis* DVCSAltDVCSCosPhiMPhis::clone() const {
    return new DVCSAltDVCSCosPhiMPhis(*this);
}

void DVCSAltDVCSCosPhiMPhis::configure(const ElemUtils::Parameters &parameters) {
    Observable::configure(parameters);
}

double DVCSAltDVCSCosPhiMPhis::computePhiObservable(double phi) {

    double A = m_pProcessModule->computeCrossSection(+1, +1,
            NumA::Vector3D(-1., 0., 0.), phi);

    double B = m_pProcessModule->computeCrossSection(-1, +1,
            NumA::Vector3D(-1., 0., 0.), phi);

    double C = m_pProcessModule->computeCrossSection(+1, -1,
            NumA::Vector3D(-1., 0., 0.), phi);

    double D = m_pProcessModule->computeCrossSection(-1, -1,
            NumA::Vector3D(-1., 0., 0.), phi);

    double E = m_pProcessModule->computeCrossSection(+1, +1,
            NumA::Vector3D(+1, 0., 0.), phi);

    double F = m_pProcessModule->computeCrossSection(-1, +1,
            NumA::Vector3D(+1, 0., 0.), phi);

    double G = m_pProcessModule->computeCrossSection(+1, -1,
            NumA::Vector3D(+1, 0., 0.), phi);

    double H = m_pProcessModule->computeCrossSection(-1, -1,
            NumA::Vector3D(+1, 0., 0.), phi);

    if (A + B + C + D + E + F + G + H == 0.) {

        warn(__func__, "Asymmetry denominator is zero.");
        return 0.;
    }

    return ((A - B + C - D) - (E - F + G - H))
            / ((A + B + C + D) + (E + F + G + H));
}

} /* namespace PARTONS */
