#include "GPDKinematic.h"

#include <stdexcept>

#include "../../utils/GenericType.h"
#include "../../utils/ParameterList.h"
#include "../../utils/stringUtils/Formatter.h"

const std::string GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_X = "x";
const std::string GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI = "xi";
const std::string GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_T = "t";
const std::string GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2 = "MuF2";
const std::string GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUR2 = "MuR2";

GPDKinematic::GPDKinematic() :
        m_kinematicType(KinematicType::THEO), m_x(0.), m_xi(0.), m_t(0.), m_MuF2(
                0.), m_MuR2(0.) {
}

GPDKinematic::GPDKinematic(double x, double xi, double t, double MuF2,
        double MuR2) :
        m_kinematicType(KinematicType::THEO), m_x(x), m_xi(xi), m_t(t), m_MuF2(
                MuF2), m_MuR2(MuR2) {
}

GPDKinematic::GPDKinematic(const ParameterList &parameterList) :
        m_kinematicType(KinematicType::THEO) {

    try {
        m_x =
                (parameterList.get(GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_X)).toDouble();
        m_xi =
                parameterList.get(GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI).toDouble();
        m_t =
                parameterList.get(GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_T).toDouble();
        m_MuF2 = parameterList.get(
                GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2).toDouble();
        m_MuR2 = parameterList.get(
                GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUR2).toDouble();
    } catch (std::exception &e) {

        //TODO refactoring throw exception from BaseObject class

        //catch exception and add current class name to follow exception
        std::string errorMsg =
                "[GPDKinematic::GPDKinematic(const ParameterList &parameterList)]"
                        + std::string(e.what());
        throw std::runtime_error(errorMsg);
    }
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
