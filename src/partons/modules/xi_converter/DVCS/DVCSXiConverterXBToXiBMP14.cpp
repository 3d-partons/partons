#include "../../../../../include/partons/modules/xi_converter/DVCS/DVCSXiConverterXBToXiBMP14.h"

#include "../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSXiConverterXBToXiBMP14::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSXiConverterXBToXiBMP14("DVCSXiConverterXBToXiBMP14"));

DVCSXiConverterXBToXiBMP14::DVCSXiConverterXBToXiBMP14(const std::string &className) :
        DVCSXiConverterModule(className) {
}

DVCSXiConverterXBToXiBMP14::DVCSXiConverterXBToXiBMP14(const DVCSXiConverterXBToXiBMP14 &other) :
        DVCSXiConverterModule(other) {
}

DVCSXiConverterXBToXiBMP14::~DVCSXiConverterXBToXiBMP14() {
}

DVCSXiConverterXBToXiBMP14* DVCSXiConverterXBToXiBMP14::clone() const {
    return new DVCSXiConverterXBToXiBMP14(*this);
}

PhysicalType<double> DVCSXiConverterXBToXiBMP14::compute(
        const DVCSObservableKinematic& kinematic) {

    double xB = kinematic.getXB().getValue();
    double t = kinematic.getT().getValue();
    double Q2 = kinematic.getQ2().getValue();

    return PhysicalType<double>(xB * (1. + t / Q2) / (2. - xB * (1. - t / Q2)),
            PhysicalUnit::NONE);
}

} /* namespace PARTONS */
