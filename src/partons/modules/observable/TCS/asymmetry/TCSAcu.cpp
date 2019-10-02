#include "../../../../../../include/partons/modules/observable/TCS/asymmetry/TCSAcu.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/TCS/TCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int TCSAcu::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSAcu("TCSAcu"));

TCSAcu::TCSAcu(const std::string &className) :
        TCSObservable(className) {
}

TCSAcu::TCSAcu(const TCSAcu& other) :
        TCSObservable(other) {
}

TCSAcu::~TCSAcu() {
}

TCSAcu* TCSAcu::clone() const {
    return new TCSAcu(*this);
}

PhysicalType<double> TCSAcu::computeObservable(
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    //evaluate
    PhysicalType<double> A = m_pProcessModule->compute(1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> B = m_pProcessModule->compute(-1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType).getValue();

    //combine
    if ((A + B).getValue() == 0.) {

        warn(__func__, "Asymmetry denominator is zero");
        return PhysicalType<double>(0., PhysicalUnit::NONE);
    }

    //return
    return (A - B) / (A + B);
}

} /* namespace PARTONS */
