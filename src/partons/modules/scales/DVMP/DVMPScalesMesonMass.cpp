#include "../../../../../include/partons/modules/scales/DVMP/DVMPScalesMesonMass.h"

#include <cmath>

#include "../../../../../include/partons/beans/MesonType.h"
#include "../../../../../include/partons/beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/beans/Scales.h"

namespace PARTONS {

const unsigned int DVMPScalesMesonMass::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPScalesMesonMass("DVMPScalesMesonMass"));

DVMPScalesMesonMass::DVMPScalesMesonMass(const std::string &className) :
        DVMPScalesModule(className) {
}

DVMPScalesMesonMass::DVMPScalesMesonMass(const DVMPScalesMesonMass &other) :
        DVMPScalesModule(other) {
}

DVMPScalesMesonMass::~DVMPScalesMesonMass() {
}

DVMPScalesMesonMass* DVMPScalesMesonMass::clone() const {
    return new DVMPScalesMesonMass(*this);
}

Scales DVMPScalesMesonMass::compute(const DVMPObservableKinematic& kinematic) {

    double scale = pow(MesonType(kinematic.getMesonType()).getMass() * 10., 2);

    return Scales(scale, scale);
}

} /* namespace PARTONS */
