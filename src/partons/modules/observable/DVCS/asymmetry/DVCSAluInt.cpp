#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAluInt.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/ProcessModule.h"

namespace PARTONS {


const unsigned int DVCSAluInt::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAluInt("DVCSAluInt"));

DVCSAluInt::DVCSAluInt(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

DVCSAluInt::DVCSAluInt(const DVCSAluInt& other) :
        Observable(other) {
}

DVCSAluInt::~DVCSAluInt() {
}

DVCSAluInt* DVCSAluInt::clone() const {
    return new DVCSAluInt(*this);
}

void DVCSAluInt::configure(const ElemUtils::Parameters &parameters) {
	Observable::configure(parameters);
}

double DVCSAluInt::computePhiObservable(double phi) {

    double A = m_pProcessModule->computeCrossSection(+1, +1,
            NumA::Vector3D(0., 0., 0.), phi);

    double B = m_pProcessModule->computeCrossSection(+1, -1,
            NumA::Vector3D(0., 0., 0.), phi);

    double C = m_pProcessModule->computeCrossSection(-1, +1,
            NumA::Vector3D(0., 0., 0.), phi);

    double D = m_pProcessModule->computeCrossSection(-1, -1,
            NumA::Vector3D(0., 0., 0.), phi);

    if (A + B + C + D == 0.) {

        warn(__func__, "Asymmetry denominator is zero.");
        return 0.;
    }

    return ((A - B) - (C - D)) / ((A + B) + (C + D));
}

} /* namespace PARTONS */
