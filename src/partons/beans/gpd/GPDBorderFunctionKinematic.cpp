/*
 * GPDBorderFunctionKinematic.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: Pawel Sznajder (IPNO)
 */

#include "../../../../include/partons/beans/gpd/GPDBorderFunctionKinematic.h"

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"

namespace PARTONS {


GPDBorderFunctionKinematic::GPDBorderFunctionKinematic() :
        GPDKinematic() {
}

GPDBorderFunctionKinematic::GPDBorderFunctionKinematic(
        const GPDBorderFunctionKinematic& other) :
        GPDKinematic(other) {
}

GPDBorderFunctionKinematic::GPDBorderFunctionKinematic(double xi, double t,
        double MuF2, double MuR2) :
        GPDKinematic(xi, xi, t, MuF2, MuR2) {
}

GPDBorderFunctionKinematic::GPDBorderFunctionKinematic(
        const ElemUtils::GenericType& xi, const ElemUtils::GenericType& t,
        const ElemUtils::GenericType& MuF2, const ElemUtils::GenericType& MuR2) :
        GPDKinematic(xi, xi, t, MuF2, MuR2) {
}

GPDBorderFunctionKinematic::GPDBorderFunctionKinematic(
        const ElemUtils::Parameters& parameters) :
        GPDKinematic() {

    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_XI)) {
        setX(parameters.getLastAvailable().toDouble());
        setXi(parameters.getLastAvailable().toDouble());
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_XI);
    }

    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_T)) {
        setT(parameters.getLastAvailable().toDouble());
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_T);
    }

    //TODO remove from kinematic
    setMuF2(
            parameters.get(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2).toDouble());
    setMuR2(
            parameters.get(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2).toDouble());
}

GPDBorderFunctionKinematic::~GPDBorderFunctionKinematic() {
}

std::string GPDBorderFunctionKinematic::toString() const {
    return GPDKinematic::toString();
}

void GPDBorderFunctionKinematic::updateHashSum() const {
    GPDKinematic::updateHashSum();
}

} /* namespace PARTONS */
