#include "../../../../../include/partons/modules/xi_converter/DVMP/DVMPXiConverterZetaToXi.h"

#include <cmath>

#include "../../../../../include/partons/beans/MesonType.h"
#include "../../../../../include/partons/beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVMPXiConverterZetaToXi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPXiConverterZetaToXi("DVMPXiConverterZetaToXi"));

DVMPXiConverterZetaToXi::DVMPXiConverterZetaToXi(const std::string &className) :
        DVMPXiConverterModule(className) {
}

DVMPXiConverterZetaToXi::DVMPXiConverterZetaToXi(const DVMPXiConverterZetaToXi &other) :
        DVMPXiConverterModule(other) {
}

DVMPXiConverterZetaToXi::~DVMPXiConverterZetaToXi() {
}

DVMPXiConverterZetaToXi* DVMPXiConverterZetaToXi::clone() const {
    return new DVMPXiConverterZetaToXi(*this);
}

PhysicalType<double> DVMPXiConverterZetaToXi::compute(
        const DVMPObservableKinematic& kinematic) {

    double E = kinematic.getE().getValue();
    double W2 = pow(E + Constant::PROTON_MASS, 2) - pow(E, 2);
    double M = MesonType(kinematic.getMesonType()).getMass();
    double zeta = pow(M, 2) / W2;

    return PhysicalType<double>(
            zeta / (2. - zeta),
            PhysicalUnit::NONE);
}

} /* namespace PARTONS */
