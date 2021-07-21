#include "../../../../../include/partons/modules/xi_converter/DVMP/DVMPXiConverterXBToXi.h"

#include "../../../../../include/partons/beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVMPXiConverterXBToXi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPXiConverterXBToXi("DVMPXiConverterXBToXi"));

DVMPXiConverterXBToXi::DVMPXiConverterXBToXi(const std::string &className) :
        DVMPXiConverterModule(className) {
}

DVMPXiConverterXBToXi::DVMPXiConverterXBToXi(const DVMPXiConverterXBToXi &other) :
        DVMPXiConverterModule(other) {
}

DVMPXiConverterXBToXi::~DVMPXiConverterXBToXi() {
}

DVMPXiConverterXBToXi* DVMPXiConverterXBToXi::clone() const {
    return new DVMPXiConverterXBToXi(*this);
}

PhysicalType<double> DVMPXiConverterXBToXi::compute(
        const DVMPObservableKinematic& kinematic) {
    return PhysicalType<double>(
            kinematic.getXB().getValue() / (2. - kinematic.getXB().getValue()),
            PhysicalUnit::NONE);
}

} /* namespace PARTONS */
