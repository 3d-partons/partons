#include "../../../../../../include/partons/modules/observable/DVMP/asymmetry/DVMPAlluMinus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/MesonPolarization.h"
#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DVMP/DVMPProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVMPAlluMinus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPAlluMinus("DVMPAlluMinus"));

DVMPAlluMinus::DVMPAlluMinus(const std::string &className) :
        DVMPObservable(className) {
}

DVMPAlluMinus::DVMPAlluMinus(const DVMPAlluMinus& other) :
        DVMPObservable(other) {
}

DVMPAlluMinus::~DVMPAlluMinus() {
}

DVMPAlluMinus* DVMPAlluMinus::clone() const {
    return new DVMPAlluMinus(*this);
}

PhysicalType<double> DVMPAlluMinus::computeObservable(
        const DVMPObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //TODO VECTOR MESON POLARISATION

    PhysicalType<double> A = m_pProcessModule->compute(+1, -1,
            NumA::Vector3D(0., 0., -1.), MesonPolarization::UNDEFINED,
            kinematic, gpdType).getValue();

    PhysicalType<double> B = m_pProcessModule->compute(-1, -1,
            NumA::Vector3D(0., 0., +1.), MesonPolarization::UNDEFINED,
            kinematic, gpdType).getValue();

    PhysicalType<double> C = m_pProcessModule->compute(+1, -1,
            NumA::Vector3D(0., 0., +1.), MesonPolarization::UNDEFINED,
            kinematic, gpdType).getValue();

    PhysicalType<double> D = m_pProcessModule->compute(-1, -1,
            NumA::Vector3D(0., 0., -1.), MesonPolarization::UNDEFINED,
            kinematic, gpdType).getValue();

    if ((A + B + C + D).getValue() == 0.) {

        warn(__func__, "Asymmetry denominator is zero");
        return PhysicalType<double>(0., PhysicalUnit::NONE);
    }

    return ((A + B) - (C + D)) / ((A + B) + (C + D));
}

} /* namespace PARTONS */
