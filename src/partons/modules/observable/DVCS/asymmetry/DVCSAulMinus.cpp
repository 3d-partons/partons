#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAulMinus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAulMinus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAulMinus("DVCSAulMinus"));

DVCSAulMinus::DVCSAulMinus(const std::string &className) :
        DVCSObservable(className) {
}

DVCSAulMinus::DVCSAulMinus(const DVCSAulMinus& other) :
        DVCSObservable(other) {
}

DVCSAulMinus::~DVCSAulMinus() {
}

DVCSAulMinus* DVCSAulMinus::clone() const {
    return new DVCSAulMinus(*this);
}

PhysicalType<double> DVCSAulMinus::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    PhysicalType<double> A = m_pProcessModule->compute(+1, -1,
            NumA::Vector3D(0., 0., -1.), kinematic, gpdType).getValue();

    PhysicalType<double> B = m_pProcessModule->compute(-1, -1,
            NumA::Vector3D(0., 0., -1.), kinematic, gpdType).getValue();

    PhysicalType<double> C = m_pProcessModule->compute(+1, -1,
            NumA::Vector3D(0., 0., +1.), kinematic, gpdType).getValue();

    PhysicalType<double> D = m_pProcessModule->compute(-1, -1,
            NumA::Vector3D(0., 0., +1.), kinematic, gpdType).getValue();

    if ((A + B + C + D).getValue() == 0.) {

        warn(__func__, "Asymmetry denominator is zero");
        return PhysicalType<double>(0., PhysicalUnit::NONE);
    }

    return ((A + B) - (C + D)) / ((A + B) + (C + D));
}

} /* namespace PARTONS */
