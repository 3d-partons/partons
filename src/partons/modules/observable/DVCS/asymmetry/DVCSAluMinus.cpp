#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAluMinus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAluMinus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAluMinus("DVCSAluMinus"));

DVCSAluMinus::DVCSAluMinus(const std::string &className) :
        DVCSObservable(className) {
}

DVCSAluMinus::DVCSAluMinus(const DVCSAluMinus& other) :
        DVCSObservable(other) {
}

DVCSAluMinus::~DVCSAluMinus() {
}

DVCSAluMinus* DVCSAluMinus::clone() const {
    return new DVCSAluMinus(*this);
}

PhysicalType<double> DVCSAluMinus::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    PhysicalType<double> A = m_pProcessModule->compute(+1, -1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> B = m_pProcessModule->compute(-1, -1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType).getValue();

    if ((A + B).getValue() == 0.) {

        warn(__func__, "Asymmetry denominator is zero");
        return PhysicalType<double>(0., PhysicalUnit::NONE);
    }

    return (A - B) / (A + B);
}

} /* namespace PARTONS */
