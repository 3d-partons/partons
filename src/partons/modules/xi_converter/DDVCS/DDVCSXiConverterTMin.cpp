#include "../../../../../include/partons/modules/xi_converter/DDVCS/DDVCSXiConverterTMin.h"

#include <iostream>

#include "../../../../../include/partons/beans/observable/DDVCS/DDVCSObservableKinematic.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {

const unsigned int DDVCSXiConverterTMin::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSXiConverterTMin("DDVCSXiConverterTMin"));

DDVCSXiConverterTMin::DDVCSXiConverterTMin(const std::string &className) :
        DDVCSXiConverterModule(className) {
}

DDVCSXiConverterTMin::DDVCSXiConverterTMin(const DDVCSXiConverterTMin &other) :
        DDVCSXiConverterModule(other) {
}

DDVCSXiConverterTMin::~DDVCSXiConverterTMin() {
}

DDVCSXiConverterTMin* DDVCSXiConverterTMin::clone() const {
    return new DDVCSXiConverterTMin(*this);
}

PhysicalType<double> DDVCSXiConverterTMin::compute(
        const DDVCSObservableKinematic& kinematic) {

    double xB = kinematic.getXB().getValue();
    double t = kinematic.getT().getValue();
    double Q2 = kinematic.getQ2().getValue();
    double Q2Prim = kinematic.getQ2Prim().getValue();

//    double eps2 = pow(2. * xB * Constant::PROTON_MASS, 2.) / Q2;
//
//    double tMin = -1. / (4. * xB * (1. - xB) + eps2);
//    tMin *= (2. * ((1. - xB) * Q2 - xB * Q2Prim) + eps2 * (Q2 - Q2Prim)
//            - 2. * sqrt(1. + eps2)
//                    * sqrt(
//                            pow((1. - xB) * Q2 - xB * Q2Prim, 2.)
//                                    - eps2 * Q2 * Q2Prim));

    double tForLT = 0.;

    double xi = (Q2 + Q2Prim) / (2. * Q2 / xB - Q2 - Q2Prim + tForLT); // skewness at t = 0 (which is the skewness dropping t/Q2 and similar terms). This xi is in fact eta in Belitsky2003 eq 31 with extra minus sign so it is positive

    return PhysicalType<double>(xi, PhysicalUnit::NONE);
}

} /* namespace PARTONS */
