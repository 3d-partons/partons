#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAulPlus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAulPlus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAulPlus("DVCSAulPlus"));

DVCSAulPlus::DVCSAulPlus(const std::string &className) :
        DVCSObservable(className) {
}

DVCSAulPlus::DVCSAulPlus(const DVCSAulPlus& other) :
        DVCSObservable(other) {
}

DVCSAulPlus::~DVCSAulPlus() {
}

DVCSAulPlus* DVCSAulPlus::clone() const {
    return new DVCSAulPlus(*this);
}

PhysicalType<double> DVCSAulPlus::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    PhysicalType<double> A = m_pProcessModule->compute(+1, +1,
            NumA::Vector3D(0., 0., -1.), kinematic, gpdType).getValue();

    PhysicalType<double> B = m_pProcessModule->compute(-1, +1,
            NumA::Vector3D(0., 0., -1.), kinematic, gpdType).getValue();

    PhysicalType<double> C = m_pProcessModule->compute(+1, +1,
            NumA::Vector3D(0., 0., +1.), kinematic, gpdType).getValue();

    PhysicalType<double> D = m_pProcessModule->compute(-1, +1,
            NumA::Vector3D(0., 0., +1.), kinematic, gpdType).getValue();

    if ((A + B + C + D).getValue() == 0.) {

        warn(__func__, "Asymmetry denominator is zero");
        return PhysicalType<double>(0., PhysicalUnit::NONE);
    }

    return ((A + B) - (C + D)) / ((A + B) + (C + D));
}

} /* namespace PARTONS */
