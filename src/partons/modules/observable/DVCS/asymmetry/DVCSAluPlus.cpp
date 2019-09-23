#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAluPlus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAluPlus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAluPlus("DVCSAluPlus"));

DVCSAluPlus::DVCSAluPlus(const std::string &className) :
        DVCSObservable(className) {
}

DVCSAluPlus::DVCSAluPlus(const DVCSAluPlus& other) :
        DVCSObservable(other) {
}

DVCSAluPlus::~DVCSAluPlus() {
}

DVCSAluPlus* DVCSAluPlus::clone() const {
    return new DVCSAluPlus(*this);
}

PhysicalType<double> DVCSAluPlus::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    PhysicalType<double> A = m_pProcessModule->compute(+1, +1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> B = m_pProcessModule->compute(-1, +1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType).getValue();

    if ((A + B).getValue() == 0.) {

        warn(__func__, "Asymmetry denominator is zero");
        return PhysicalType<double>(0., PhysicalUnit::NONE);
    }

    return (A - B) / (A + B);
}

} /* namespace PARTONS */
