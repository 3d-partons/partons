#include "../../../../../include/partons/modules/scales/DDVCS/DDVCSScalesVirtualitiesSum.h"

#include "../../../../../include/partons/beans/observable/DDVCS/DDVCSObservableKinematic.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../../include/partons/beans/Scales.h"

namespace PARTONS {

const unsigned int DDVCSScalesVirtualitiesSum::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSScalesVirtualitiesSum("DDVCSScalesVirtualitiesSum"));

DDVCSScalesVirtualitiesSum::DDVCSScalesVirtualitiesSum(
        const std::string &className) :
        DDVCSScalesModule(className) {
}

DDVCSScalesVirtualitiesSum::DDVCSScalesVirtualitiesSum(
        const DDVCSScalesVirtualitiesSum &other) :
        DDVCSScalesModule(other) {
}

DDVCSScalesVirtualitiesSum::~DDVCSScalesVirtualitiesSum() {
}

DDVCSScalesVirtualitiesSum* DDVCSScalesVirtualitiesSum::clone() const {
    return new DDVCSScalesVirtualitiesSum(*this);
}

Scales DDVCSScalesVirtualitiesSum::compute(
        const DDVCSObservableKinematic& kinematic) {

    PhysicalType<double> scale = kinematic.getQ2() + kinematic.getQ2Prim();

    return Scales(scale, scale);
}

} /* namespace PARTONS */
