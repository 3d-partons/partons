#include "../../../../../../include/partons/modules/observable/DVCS/asymmetry/DVCSAluDVCS.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSAluDVCS::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSAluDVCS("DVCSAluDVCS"));

DVCSAluDVCS::DVCSAluDVCS(const std::string &className) :
        DVCSObservable(className) {
}

DVCSAluDVCS::DVCSAluDVCS(const DVCSAluDVCS& other) :
        DVCSObservable(other) {
}

DVCSAluDVCS::~DVCSAluDVCS() {
}

DVCSAluDVCS* DVCSAluDVCS::clone() const {
    return new DVCSAluDVCS(*this);
}

PhysicalType<double> DVCSAluDVCS::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    PhysicalType<double> A = m_pProcessModule->compute(+1, +1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> B = m_pProcessModule->compute(+1, -1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> C = m_pProcessModule->compute(-1, +1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> D = m_pProcessModule->compute(-1, -1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType).getValue();

    if ((A + B + C + D).getValue() == 0.) {

        warn(__func__, "Asymmetry denominator is zero");
        return PhysicalType<double>(0., PhysicalUnit::NONE);
    }

    return ((A + B) - (C + D)) / ((A + B) + (C + D));
}

} /* namespace PARTONS */
