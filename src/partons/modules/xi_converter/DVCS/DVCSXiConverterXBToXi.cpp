#include "../../../../../include/partons/modules/xi_converter/DVCS/DVCSXiConverterXBToXi.h"

#include "../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSXiConverterXBToXi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSXiConverterXBToXi("DVCSXiConverterXBToXi"));

DVCSXiConverterXBToXi::DVCSXiConverterXBToXi(const std::string &className) :
        DVCSXiConverterModule(className) {
}

DVCSXiConverterXBToXi::DVCSXiConverterXBToXi(const DVCSXiConverterXBToXi &other) :
        DVCSXiConverterModule(other) {
}

DVCSXiConverterXBToXi::~DVCSXiConverterXBToXi() {
}

DVCSXiConverterXBToXi* DVCSXiConverterXBToXi::clone() const {
    return new DVCSXiConverterXBToXi(*this);
}

PhysicalType<double> DVCSXiConverterXBToXi::compute(
        const DVCSObservableKinematic& kinematic) {

    return PhysicalType<double>(
            kinematic.getXB().getValue() / (2. - kinematic.getXB().getValue()),
            PhysicalUnit::NONE);
}

} /* namespace PARTONS */
