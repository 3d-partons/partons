#include "GPDKinematic.h"

#include "../../utils/GenericType.h"
#include "../../utils/ParameterList.h"
#include "../../utils/stringUtils/Formatter.h"
#include "../observable/ObservableKinematic.h"

const std::string GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_X = "x";
const std::string GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI = "xi";
const std::string GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2 = "MuF2";
const std::string GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUR2 = "MuR2";

GPDKinematic::GPDKinematic() :
        BaseObject("GPDKinematic"), m_kinematicType(KinematicType::THEO), m_x(
                0.), m_xi(0.), m_t(0.), m_MuF2(0.), m_MuR2(0.) {
}

GPDKinematic::GPDKinematic(double x, double xi, double t, double MuF2,
        double MuR2) :
        BaseObject("GPDKinematic"), m_kinematicType(KinematicType::THEO), m_x(
                x), m_xi(xi), m_t(t), m_MuF2(MuF2), m_MuR2(MuR2) {
}

GPDKinematic::GPDKinematic(ParameterList &parameterList) :
        BaseObject("GPDKinematic"), m_kinematicType(KinematicType::THEO), m_x(
                0.), m_xi(0.), m_t(0.), m_MuF2(0.), m_MuR2(0.) {

    if (parameterList.isAvailable(
            GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_X)) {
        m_x = parameterList.getLastAvailable().toDouble();
    } else {
        throwException(__func__,
                Formatter() << "Missing parameter <"
                        << GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_X << ">");
    }
    if (parameterList.isAvailable(
            GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI)) {
        m_xi = parameterList.getLastAvailable().toDouble();
    } else {
        throwException(__func__,
                Formatter() << "Missing parameter <"
                        << GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI
                        << ">");
    }
    if (parameterList.isAvailable(ObservableKinematic::PARAMETER_NAME_T)) {
        m_t = parameterList.getLastAvailable().toDouble();
    } else {
        throwException(__func__,
                Formatter() << "Missing parameter <"
                        << ObservableKinematic::PARAMETER_NAME_T << ">");
    }

    //TODO remove from kinematic
    m_MuF2 =
            parameterList.get(GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2).toDouble();
    m_MuR2 =
            parameterList.get(GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUR2).toDouble();
}

GPDKinematic::~GPDKinematic() {
}

std::string GPDKinematic::toString() {
    return Formatter() << "m_x = " << m_x << " m_xi = " << m_xi << "m_t = "
            << m_t << " m_MuF = " << m_MuF2 << " m_MuR = " << m_MuR2;
}

// ##### GETTERS & SETTERS #####

double GPDKinematic::getMuF2() const {
    return m_MuF2;
}

void GPDKinematic::setMuF2(double muF2) {
    m_MuF2 = muF2;
}

double GPDKinematic::getMuR2() const {
    return m_MuR2;
}

void GPDKinematic::setMuR2(double muR2) {
    m_MuR2 = muR2;
}

double GPDKinematic::getX() const {
    return m_x;
}

void GPDKinematic::setX(double x) {
    m_x = x;
}

double GPDKinematic::getXi() const {
    return m_xi;
}

void GPDKinematic::setXi(double xi) {
    m_xi = xi;
}

double GPDKinematic::getT() const {
    return m_t;
}

void GPDKinematic::setT(double t) {
    m_t = t;
}

KinematicType::Type GPDKinematic::getKinematicType() const {
    return m_kinematicType;
}

void GPDKinematic::setKinematicType(KinematicType::Type kinematicType) {
    m_kinematicType = kinematicType;
}
//sf::Packet& operator <<(sf::Packet& packet, const GPDKinematic& object) {
//    return packet << object.m_x << object.m_xi << object.m_t << object.m_MuF
//            << object.m_MuR;
//}
//
//sf::Packet& operator >>(sf::Packet& packet, GPDKinematic& object) {
//    return packet >> object.m_x >> object.m_xi >> object.m_t >> object.m_MuF
//            >> object.m_MuR;
//}
