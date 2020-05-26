#include <NumA/linear_algebra/vector/Vector3D.h>
#include <cmath>
#include <string>

#include "../../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../../include/partons/beans/List.h"
#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/observable/TCS/asymmetry/TCSAfb.h"
#include "../../../../../../include/partons/modules/process/TCS/TCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int TCSAfb::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSAfb("TCSAfb"));

TCSAfb::TCSAfb(const std::string &className) :
        TCSObservable(className) {
}

TCSAfb::TCSAfb(const TCSAfb& other) :
        TCSObservable(other) {
}

TCSAfb::~TCSAfb() {
}

TCSAfb* TCSAfb::clone() const {
    return new TCSAfb(*this);
}

PhysicalType<double> TCSAfb::computeObservable(
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    //backward kinematics
    TCSObservableKinematic kinematicBackward = kinematic;

    kinematicBackward.setPhi(
    M_PI + kinematic.getPhi().makeSameUnitAs(PhysicalUnit::RAD).getValue(),
            PhysicalUnit::RAD);
    kinematicBackward.setTheta(
    M_PI - kinematic.getTheta().makeSameUnitAs(PhysicalUnit::RAD).getValue(),
            PhysicalUnit::RAD);

    //evaluate
    PhysicalType<double> A = m_pProcessModule->compute(1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> B = m_pProcessModule->compute(-1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType).getValue();

    PhysicalType<double> C = m_pProcessModule->compute(1,
            NumA::Vector3D(0., 0., 0.), kinematicBackward, gpdType).getValue();

    PhysicalType<double> D = m_pProcessModule->compute(-1,
            NumA::Vector3D(0., 0., 0.), kinematicBackward, gpdType).getValue();

    //combine
    if ((A + B + C + D).getValue() == 0.) {

        warn(__func__, "Asymmetry denominator is zero");
        return PhysicalType<double>(0., PhysicalUnit::NONE);
    }

    //return
    return ((A + B) - (C + D)) / (A + B + C + D);
}

} /* namespace PARTONS */
