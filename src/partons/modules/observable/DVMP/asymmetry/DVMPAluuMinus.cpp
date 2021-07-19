#include "../../../../../../include/partons/modules/observable/DVMP/asymmetry/DVMPAluuMinus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/MesonPolarization.h"
#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DVMP/DVMPProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVMPAluuMinus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPAluuMinus("DVMPAluuMinus"));

DVMPAluuMinus::DVMPAluuMinus(const std::string &className) :
        DVMPObservable(className) {
}

DVMPAluuMinus::DVMPAluuMinus(const DVMPAluuMinus& other) :
        DVMPObservable(other) {
}

DVMPAluuMinus::~DVMPAluuMinus() {
}

DVMPAluuMinus* DVMPAluuMinus::clone() const {
    return new DVMPAluuMinus(*this);
}

PhysicalType<double> DVMPAluuMinus::computeObservable(
        const DVMPObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //TODO VECTOR MESON POLARISATION

    PhysicalType<double> A = m_pProcessModule->compute(+1, -1,
            NumA::Vector3D(0., 0., 0.), MesonPolarization::UNDEFINED, kinematic,
            gpdType).getValue();

    PhysicalType<double> B = m_pProcessModule->compute(-1, -1,
            NumA::Vector3D(0., 0., 0.), MesonPolarization::UNDEFINED, kinematic,
            gpdType).getValue();

    if ((A + B).getValue() == 0.) {

        warn(__func__, "Asymmetry denominator is zero");
        return PhysicalType<double>(0., PhysicalUnit::NONE);
    }

    return (A - B) / (A + B);
}

} /* namespace PARTONS */
