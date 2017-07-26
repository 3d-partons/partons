#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAulMinus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/ProcessModule.h"

namespace PARTONS {


const unsigned int DVCSAulMinus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAulMinus("DVCSAulMinus"));

DVCSAulMinus::DVCSAulMinus(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

DVCSAulMinus::DVCSAulMinus(const DVCSAulMinus& other) :
        Observable(other) {
}

DVCSAulMinus::~DVCSAulMinus() {
}

DVCSAulMinus* DVCSAulMinus::clone() const {
    return new DVCSAulMinus(*this);
}

void DVCSAulMinus::configure(const ElemUtils::Parameters &parameters) {
	Observable::configure(parameters);
}

double DVCSAulMinus::computePhiObservable(double phi) {

    double A = m_pProcessModule->computeCrossSection(+1, -1,
            NumA::Vector3D(0., 0., -1.), phi);

    double B = m_pProcessModule->computeCrossSection(-1, -1,
            NumA::Vector3D(0., 0., -1.), phi);

    double C = m_pProcessModule->computeCrossSection(+1, -1,
            NumA::Vector3D(0., 0., +1.), phi);

    double D = m_pProcessModule->computeCrossSection(-1, -1,
            NumA::Vector3D(0., 0., +1.), phi);

    if (A + B + C + D == 0.) {

        warn(__func__, "Asymmetry denominator is zero.");
        return 0.;
    }

    return ((A + B) - (C + D)) / ((A + B) + (C + D));
}

} /* namespace PARTONS */
