#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAutDVCSSinPhiMPhis.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAutDVCSSinPhiMPhis::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAutDVCSSinPhiMPhis("DVCSAutDVCSSinPhiMPhis"));

DVCSAutDVCSSinPhiMPhis::DVCSAutDVCSSinPhiMPhis(const std::string &className) :
        DVCSObservable(className) {
}

DVCSAutDVCSSinPhiMPhis::DVCSAutDVCSSinPhiMPhis(
        const DVCSAutDVCSSinPhiMPhis& other) :
        DVCSObservable(other) {
}

DVCSAutDVCSSinPhiMPhis::~DVCSAutDVCSSinPhiMPhis() {
}

DVCSAutDVCSSinPhiMPhis* DVCSAutDVCSSinPhiMPhis::clone() const {
    return new DVCSAutDVCSSinPhiMPhis(*this);
}

PhysicalType<double> DVCSAutDVCSSinPhiMPhis::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    PhysicalType<double> A = m_pProcessModule->compute(+1, +1,
            NumA::Vector3D(0., -1., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> B = m_pProcessModule->compute(-1, +1,
            NumA::Vector3D(0., -1., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> C = m_pProcessModule->compute(+1, -1,
            NumA::Vector3D(0., -1., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> D = m_pProcessModule->compute(-1, -1,
            NumA::Vector3D(0., -1., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> E = m_pProcessModule->compute(+1, +1,
            NumA::Vector3D(0., +1., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> F = m_pProcessModule->compute(-1, +1,
            NumA::Vector3D(0., +1., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> G = m_pProcessModule->compute(+1, -1,
            NumA::Vector3D(0., +1., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> H = m_pProcessModule->compute(-1, -1,
            NumA::Vector3D(0., +1., 0.), kinematic, gpdType).getValue();

    if ((A + B + C + D + E + F + G + H).getValue() == 0.) {

        warn(__func__, "Asymmetry denominator is zero");
        return PhysicalType<double>(0., PhysicalUnit::NONE);
    }

    return ((A + B + C + D) - (E + F + G + H))
            / ((A + B + C + D) + (E + F + G + H));
}

} /* namespace PARTONS */
