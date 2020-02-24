#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAltIntCosPhiMPhis.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAltIntCosPhiMPhis::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAltIntCosPhiMPhis("DVCSAltIntCosPhiMPhis"));

DVCSAltIntCosPhiMPhis::DVCSAltIntCosPhiMPhis(const std::string &className) :
        DVCSObservable(className) {
}

DVCSAltIntCosPhiMPhis::DVCSAltIntCosPhiMPhis(const DVCSAltIntCosPhiMPhis& other) :
        DVCSObservable(other) {
}

DVCSAltIntCosPhiMPhis::~DVCSAltIntCosPhiMPhis() {
}

DVCSAltIntCosPhiMPhis* DVCSAltIntCosPhiMPhis::clone() const {
    return new DVCSAltIntCosPhiMPhis(*this);
}

PhysicalType<double> DVCSAltIntCosPhiMPhis::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    PhysicalType<double> A = m_pProcessModule->compute(+1, +1,
            NumA::Vector3D(-1., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> B = m_pProcessModule->compute(-1, +1,
            NumA::Vector3D(-1., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> C = m_pProcessModule->compute(+1, -1,
            NumA::Vector3D(-1., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> D = m_pProcessModule->compute(-1, -1,
            NumA::Vector3D(-1., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> E = m_pProcessModule->compute(+1, +1,
            NumA::Vector3D(+1., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> F = m_pProcessModule->compute(-1, +1,
            NumA::Vector3D(+1., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> G = m_pProcessModule->compute(+1, -1,
            NumA::Vector3D(+1., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> H = m_pProcessModule->compute(-1, -1,
            NumA::Vector3D(+1., 0., 0.), kinematic, gpdType).getValue();

    if ((A + B + C + D + E + F + G + H).getValue() == 0.) {

        warn(__func__, "Asymmetry denominator is zero");
        return PhysicalType<double>(0., PhysicalUnit::NONE);
    }

    return ((A - B - C + D) - (E - F - G + H))
            / ((A + B + C + D) + (E + F + G + H));
}

} /* namespace PARTONS */
