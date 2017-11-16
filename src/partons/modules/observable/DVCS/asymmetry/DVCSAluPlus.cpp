#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAluPlus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/ProcessModule.h"

namespace PARTONS {


const unsigned int DVCSAluPlus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAluPlus("DVCSAluPlus"));

DVCSAluPlus::DVCSAluPlus(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

DVCSAluPlus::DVCSAluPlus(const DVCSAluPlus& other) :
        Observable(other) {
}

DVCSAluPlus::~DVCSAluPlus() {
}

DVCSAluPlus* DVCSAluPlus::clone() const {
    return new DVCSAluPlus(*this);
}

void DVCSAluPlus::configure(const ElemUtils::Parameters &parameters) {
	Observable::configure(parameters);
}

double DVCSAluPlus::computePhiObservable(double phi) {

    double A = m_pProcessModule->computeCrossSection(+1, +1,
            NumA::Vector3D(0., 0., 0.), phi);

    double B = m_pProcessModule->computeCrossSection(-1, +1,
            NumA::Vector3D(0., 0., 0.), phi);

    if (A + B == 0.) {

        warn(__func__, "Asymmetry denominator is zero.");
        return 0.;
    }

    return (A - B) / (A + B);
}

} /* namespace PARTONS */
