#include "../../../../../include/partons/modules/scales/DVMP/DVMPScalesGK.h"

#include "../../../../../include/partons/beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"
#include "../../../../../include/partons/beans/Scales.h"

namespace PARTONS {

const unsigned int DVMPScalesGK::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPScalesGK("DVMPScalesGK"));

DVMPScalesGK::DVMPScalesGK(const std::string &className) :
        DVMPScalesModule(className) {
}

DVMPScalesGK::DVMPScalesGK(const DVMPScalesGK &other) :
        DVMPScalesModule(other) {
}

DVMPScalesGK::~DVMPScalesGK() {
}

DVMPScalesGK* DVMPScalesGK::clone() const {
    return new DVMPScalesGK(*this);
}

Scales DVMPScalesGK::compute(const DVMPObservableKinematic& kinematic) {

    PhysicalType<double> scale();

    return Scales(kinematic.getQ2(),
            PhysicalType<double>(0., PhysicalUnit::GEV2));
}

} /* namespace PARTONS */
