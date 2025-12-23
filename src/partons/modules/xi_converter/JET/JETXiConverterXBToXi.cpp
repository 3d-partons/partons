#include "../../../../../include/partons/modules/xi_converter/JET/JETXiConverterXBToXi.h"

#include "../../../../../include/partons/beans/observable/JET/JETObservableKinematic.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int JETXiConverterXBToXi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new JETXiConverterXBToXi("JETXiConverterXBToXi"));

JETXiConverterXBToXi::JETXiConverterXBToXi(const std::string &className) :
        JETXiConverterModule(className) {
}

JETXiConverterXBToXi::JETXiConverterXBToXi(const JETXiConverterXBToXi &other) :
        JETXiConverterModule(other) {
}

JETXiConverterXBToXi::~JETXiConverterXBToXi() {
}

JETXiConverterXBToXi* JETXiConverterXBToXi::clone() const {
    return new JETXiConverterXBToXi(*this);
}

PhysicalType<double> JETXiConverterXBToXi::compute(
        const JETObservableKinematic& kinematic) {
    return PhysicalType<double>(
            kinematic.getXB().getValue() / (2. - kinematic.getXB().getValue()),
            PhysicalUnit::NONE);
}

} /* namespace PARTONS */
