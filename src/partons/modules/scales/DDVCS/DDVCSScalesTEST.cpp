#include "../../../../../include/partons/modules/scales/DDVCS/DDVCSScalesTEST.h"

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../../include/partons/beans/Scales.h"

namespace PARTONS {

const unsigned int DDVCSScalesTEST::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSScalesTEST("DDVCSScalesTEST"));

DDVCSScalesTEST::DDVCSScalesTEST(const std::string &className) :
        DDVCSScalesModule(className) {
}

DDVCSScalesTEST::DDVCSScalesTEST(const DDVCSScalesTEST &other) :
        DDVCSScalesModule(other) {
}

DDVCSScalesTEST::~DDVCSScalesTEST() {
}

DDVCSScalesTEST* DDVCSScalesTEST::clone() const {
    return new DDVCSScalesTEST(*this);
}

void DDVCSScalesTEST::configure(const ElemUtils::Parameters &parameters) {
    DDVCSScalesModule::configure(parameters);
}

Scales DDVCSScalesTEST::compute(const DDVCSObservableKinematic& kinematic) {

    PhysicalType<double> scale(1., PhysicalUnit::GEV2);

    return Scales(scale, scale);
}

} /* namespace PARTONS */
