#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAllPlus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/ProcessModule.h"

namespace PARTONS {


const unsigned int DVCSAllPlus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAllPlus("DVCSAllPlus"));

DVCSAllPlus::DVCSAllPlus(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

DVCSAllPlus::DVCSAllPlus(const DVCSAllPlus& other) :
        Observable(other) {
}

DVCSAllPlus::~DVCSAllPlus() {
}

DVCSAllPlus* DVCSAllPlus::clone() const {
    return new DVCSAllPlus(*this);
}

void DVCSAllPlus::configure(const ElemUtils::Parameters &parameters) {
	Observable::configure(parameters);
}

double DVCSAllPlus::computePhiObservable(double phi) {

    double A = m_pProcessModule->computeCrossSection(+1, +1,
            NumA::Vector3D(0., 0., -1.), phi);

    double B = m_pProcessModule->computeCrossSection(-1, +1,
            NumA::Vector3D(0., 0., +1.), phi);

    double C = m_pProcessModule->computeCrossSection(+1, +1,
            NumA::Vector3D(0., 0., +1.), phi);

    double D = m_pProcessModule->computeCrossSection(-1, +1,
            NumA::Vector3D(0., 0., -1.), phi);

    if (A + B + C + D == 0.) {

        warn(__func__, "Asymmetry denominator is zero.");
        return 0.;
    }

    return ((A + B) - (C + D)) / ((A + B) + (C + D));
}

} /* namespace PARTONS */
