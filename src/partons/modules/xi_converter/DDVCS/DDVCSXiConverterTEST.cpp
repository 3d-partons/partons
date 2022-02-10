#include "../../../../../include/partons/modules/xi_converter/DDVCS/DDVCSXiConverterTEST.h"

#include <iostream>

#include "../../../../../include/partons/beans/observable/DDVCS/DDVCSObservableKinematic.h"
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

    double xB = kinematic.getXB().getValue();
    double t = kinematic.getT().getValue();
    double Q2 = kinematic.getQ2().getValue();
    double Q2Prim = kinematic.getQ2Prim().getValue();
    double xi = (Q2 + Q2Prim) / (2. * Q2 / xB - Q2 - Q2Prim + t); //this xi is in fact eta in Belitsky2003 (skewness) eq 31 with extra minus sign so skewness is positive

    return PhysicalType<double>(xi, PhysicalUnit::NONE);
}

PhysicalType<double> DDVCSXiConverterTEST::computeEta(
        const DDVCSObservableKinematic& kinematic) {

    double xB = kinematic.getXB().getValue();
    double t = kinematic.getT().getValue();
    double Q2 = kinematic.getQ2().getValue();
    double Q2Prim = kinematic.getQ2Prim().getValue();
    double eta = (Q2 + Q2Prim) / (2. * Q2 / xB - Q2 - Q2Prim + t)
            * (Q2 - Q2Prim + t / 2.) / (Q2 + Q2Prim); //this eta is in fact xi in Belitsky2003, eq 31 upon change of sign above

    return PhysicalType<double>(eta, PhysicalUnit::NONE);
}

} /* namespace PARTONS */
