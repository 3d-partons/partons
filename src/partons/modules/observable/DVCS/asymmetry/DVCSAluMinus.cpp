#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAluMinus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/ProcessModule.h"

namespace PARTONS {


const unsigned int DVCSAluMinus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAluMinus("DVCSAluMinus"));

DVCSAluMinus::DVCSAluMinus(const std::string &className) :
        Observable(className) {
    m_channel = ChannelType::DVCS;
}

DVCSAluMinus::DVCSAluMinus(const DVCSAluMinus& other) :
        Observable(other) {
}

DVCSAluMinus::~DVCSAluMinus() {
}

DVCSAluMinus* DVCSAluMinus::clone() const {
    return new DVCSAluMinus(*this);
}

void DVCSAluMinus::configure(const ElemUtils::Parameters &parameters) {
	Observable::configure(parameters);
}

double DVCSAluMinus::computePhiObservable(double phi) {

    double A = m_pProcessModule->computeCrossSection(+1, -1,
            NumA::Vector3D(0., 0., 0.), phi);

    double B = m_pProcessModule->computeCrossSection(-1, -1,
            NumA::Vector3D(0., 0., 0.), phi);

    if (A + B == 0.) {

        warn(__func__, "Asymmetry denominator is zero.");
        return 0.;
    }

    return (A - B) / (A + B);
}

} /* namespace PARTONS */
