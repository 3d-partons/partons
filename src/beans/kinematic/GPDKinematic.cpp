#include "GPDKinematic.h"

#include "../../utils/stringUtils/Formatter.h"

const std::string GPDKinematic::GPD_KINEMATIC_DB_COLUMN_NAME_X = "x";
const std::string GPDKinematic::GPD_KINEMATIC_DB_COLUMN_NAME_XI = "xi";
const std::string GPDKinematic::GPD_KINEMATIC_DB_COLUMN_NAME_T = "t";
const std::string GPDKinematic::GPD_KINEMATIC_DB_COLUMN_NAME_MUF = "MuF";
const std::string GPDKinematic::GPD_KINEMATIC_DB_COLUMN_NAME_MUR = "MuR";

GPDKinematic::GPDKinematic()
        : m_kinematicType(KinematicType::THEO), m_x(0.), m_xi(0.), m_t(0.), m_MuF(
                0.), m_MuR(0.) {
}

GPDKinematic::GPDKinematic(double x, double xi, double t, double MuF,
        double MuR)
        : m_kinematicType(KinematicType::THEO), m_x(x), m_xi(xi), m_t(t), m_MuF(
                MuF), m_MuR(MuR) {
}

GPDKinematic::~GPDKinematic() {
}

std::string GPDKinematic::toString() {
    return Formatter() << "m_x = " << m_x << " m_xi = " << m_xi << "m_t = "
            << m_t << " m_MuF = " << m_MuF << " m_MuR = " << m_MuR;
}

// ##### GETTERS & SETTERS #####

double GPDKinematic::getMuF() const {
    return m_MuF;
}

void GPDKinematic::setMuF(double muF) {
    m_MuF = muF;
}

double GPDKinematic::getMuR() const {
    return m_MuR;
}

void GPDKinematic::setMuR(double muR) {
    m_MuR = muR;
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
