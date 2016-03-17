#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/beans/observable/ObservableKinematic.h"

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic() :
        Kinematic("DVCSConvolCoeffFunctionKinematic"), m_binId(0), m_xi(0.), m_t(
                0.), m_Q2(0.), m_MuF2(0.), m_MuR2(0.) {
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(
        const ElemUtils::Parameters &parameters) :
        Kinematic("DVCSConvolCoeffFunctionKinematic"), m_binId(0), m_xi(0.), m_t(
                0.), m_Q2(0.), m_MuF2(0.), m_MuR2(0.) {
    if (parameters.isAvailable(GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI)) {
        m_xi = parameters.getLastAvailable().toDouble();
    } else {
        error(__func__,
                ElemUtils::Formatter() << "Missing parameter <"
                        << GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI
                        << ">");
    }
    if (parameters.isAvailable(ObservableKinematic::PARAMETER_NAME_T)) {
        m_t = parameters.getLastAvailable().toDouble();
    } else {
        error(__func__,
                ElemUtils::Formatter() << "Missing parameter <"
                        << ObservableKinematic::PARAMETER_NAME_T << ">");
    }
    if (parameters.isAvailable(ObservableKinematic::PARAMETER_NAME_Q2)) {
        m_Q2 = parameters.getLastAvailable().toDouble();
    } else {
        error(__func__,
                ElemUtils::Formatter() << "Missing parameter <"
                        << ObservableKinematic::PARAMETER_NAME_Q2 << ">");
    }

    //TODO remove from kinematic
    m_MuF2 =
            parameters.get(GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2).toDouble();
    m_MuR2 =
            parameters.get(GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUR2).toDouble();
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(double xi,
        double t, double Q2, double MuF2, double MuR2) :
        Kinematic("DVCSConvolCoeffFunctionKinematic"), m_binId(0), m_xi(xi), m_t(
                t), m_Q2(Q2), m_MuF2(MuF2), m_MuR2(MuR2) {
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(
        unsigned int binId, double xi, double t, double Q2, double MuF2,
        double MuR2) :
        Kinematic("DVCSConvolCoeffFunctionKinematic"), m_binId(binId), m_xi(xi), m_t(
                t), m_Q2(Q2), m_MuF2(MuF2), m_MuR2(MuR2) {
}

DVCSConvolCoeffFunctionKinematic::~DVCSConvolCoeffFunctionKinematic() {
}

std::string DVCSConvolCoeffFunctionKinematic::toString() {
    return ElemUtils::Formatter() << "m_binId = " << m_binId << " m_xi = "
            << m_xi << " m_t = " << m_t << " m_Q2 = " << m_Q2 << " m_MuF2 = "
            << m_MuF2 << " m_MuR2 = " << m_MuR2;
}

unsigned int DVCSConvolCoeffFunctionKinematic::getBinId() const {
    return m_binId;
}

double DVCSConvolCoeffFunctionKinematic::getMuF2() const {
    return m_MuF2;
}

double DVCSConvolCoeffFunctionKinematic::getMuR2() const {
    return m_MuR2;
}

double DVCSConvolCoeffFunctionKinematic::getQ2() const {
    return m_Q2;
}

double DVCSConvolCoeffFunctionKinematic::getT() const {
    return m_t;
}

double DVCSConvolCoeffFunctionKinematic::getXi() const {
    return m_xi;
}
