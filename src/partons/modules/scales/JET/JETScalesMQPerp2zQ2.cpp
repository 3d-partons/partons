#include "../../../../../include/partons/modules/scales/JET/JETScalesMQPerp2zQ2.h"

#include <cmath>

#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../../include/partons/beans/Scales.h"

namespace PARTONS {

const unsigned int JETScalesMQPerp2zQ2::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new JETScalesMQPerp2zQ2("JETScalesMQPerp2zQ2"));

JETScalesMQPerp2zQ2::JETScalesMQPerp2zQ2(const std::string &className) :
        JETScalesModule(className){
}

JETScalesMQPerp2zQ2::JETScalesMQPerp2zQ2(
        const JETScalesMQPerp2zQ2 &other) :
        JETScalesModule(other){
}

JETScalesMQPerp2zQ2::~JETScalesMQPerp2zQ2() {
}

JETScalesMQPerp2zQ2* JETScalesMQPerp2zQ2::clone() const {
    return new JETScalesMQPerp2zQ2(*this);
}

Scales JETScalesMQPerp2zQ2::compute(const JETObservableKinematic& kinematic) {

        double m;

        switch(kinematic.getJetType()){

                case JetType::UP:
                case JetType::DOWN:
                case JetType::STRANGE:{
                        m = 0.; break;
                }

                case JetType::CHARM:{
                        m = 1.25; break;
                }

                default:{
                        throw ElemUtils::CustomException(getClassName(), __func__, "unknown type for assignment");
                }
        }

        double qPerp2 = kinematic.getQPerp2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
        double z = kinematic.getZ().getValue();
        double Q2 = kinematic.getQ2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();

        double mu2 = pow(m, 2) + qPerp2 + z * (1. - z) * Q2;

        PhysicalType<double> scale(mu2, PhysicalUnit::GEV2);

        return Scales(scale, scale);
}

} /* namespace PARTONS */
