#include "../../../../../include/partons/modules/xi_converter/DDVCS/DDVCSXiConverterTNeglected.h"

#include "../../../../../include/partons/beans/observable/DDVCS/DDVCSObservableKinematic.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DDVCSXiConverterTNeglected::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSXiConverterTNeglected("DDVCSXiConverterTNeglected"));

DDVCSXiConverterTNeglected::DDVCSXiConverterTNeglected(
        const std::string &className) :
        DDVCSXiConverterModule(className) {
}

DDVCSXiConverterTNeglected::DDVCSXiConverterTNeglected(
        const DDVCSXiConverterTNeglected &other) :
        DDVCSXiConverterModule(other) {
}

DDVCSXiConverterTNeglected::~DDVCSXiConverterTNeglected() {
}

DDVCSXiConverterTNeglected* DDVCSXiConverterTNeglected::clone() const {
    return new DDVCSXiConverterTNeglected(*this);
}

PhysicalType<double> DDVCSXiConverterTNeglected::compute(
        const DDVCSObservableKinematic& kinematic) {

    double xB = kinematic.getXB().getValue();
    double Q2 = kinematic.getQ2().getValue();
    double Q2Prim = kinematic.getQ2Prim().getValue();
    double xi = (Q2 + Q2Prim) / (2. * Q2 / xB - Q2 - Q2Prim); //this xi is in fact eta in Belitsky2003 (skewness) eq 31 with extra minus sign so skewness is positive

    return PhysicalType<double>(xi, PhysicalUnit::NONE);
}

} /* namespace PARTONS */
