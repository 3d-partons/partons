#include "../../../../../../include/partons/modules/observable/TCS/asymmetry/TCSAutCosPhiMPhis.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/TCS/TCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int TCSAutCosPhiMPhis::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSAutCosPhiMPhis("TCSAutCosPhiMPhis"));

TCSAutCosPhiMPhis::TCSAutCosPhiMPhis(const std::string &className) :
        TCSObservable(className) {
}

TCSAutCosPhiMPhis::TCSAutCosPhiMPhis(const TCSAutCosPhiMPhis& other) :
        TCSObservable(other) {
}

TCSAutCosPhiMPhis::~TCSAutCosPhiMPhis() {
}

TCSAutCosPhiMPhis* TCSAutCosPhiMPhis::clone() const {
    return new TCSAutCosPhiMPhis(*this);
}

PhysicalType<double> TCSAutCosPhiMPhis::computeObservable(
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    //evaluate
    PhysicalType<double> A = m_pProcessModule->compute(1,
            NumA::Vector3D(1., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> B = m_pProcessModule->compute(-1,
            NumA::Vector3D(-1., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> C = m_pProcessModule->compute(1,
            NumA::Vector3D(1., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> D = m_pProcessModule->compute(-1,
            NumA::Vector3D(-1., 0., 0.), kinematic, gpdType).getValue();

    //combine
    if ((A + B + C + D).getValue() == 0.) {

        warn(__func__, "Asymmetry denominator is zero");
        return PhysicalType<double>(0., PhysicalUnit::NONE);
    }

    //return
    return ((A + B) - (C + D)) / (A + B + C + D);
}

} /* namespace PARTONS */
