#include "../../../../../include/partons/modules/xi_converter/DDVCS/DDVCSXiConverterTEST.h"

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DDVCSXiConverterTEST::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSXiConverterTEST("DDVCSXiConverterTEST"));

DDVCSXiConverterTEST::DDVCSXiConverterTEST(const std::string &className) :
        DDVCSXiConverterModule(className) {
}

DDVCSXiConverterTEST::DDVCSXiConverterTEST(const DDVCSXiConverterTEST &other) :
        DDVCSXiConverterModule(other) {
}

DDVCSXiConverterTEST::~DDVCSXiConverterTEST() {
}

DDVCSXiConverterTEST* DDVCSXiConverterTEST::clone() const {
    return new DDVCSXiConverterTEST(*this);
}

PhysicalType<double> DDVCSXiConverterTEST::compute(
        const DDVCSObservableKinematic& kinematic) {
    return PhysicalType<double>(0.5, PhysicalUnit::NONE);
}

} /* namespace PARTONS */
