#include "../../../../include/partons/beans/gpd/GPDKinematic.h"

#include "../../../../include/partons/beans/observable/ObservableKinematic.h"
#include "../../../../include/partons/utils/compare/CompareUtils.h"
#include "../../../../include/partons/utils/compare/ComparisonData.h"
#include "../../../../include/partons/utils/GenericType.h"
#include "../../../../include/partons/utils/ParameterList.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"
#include "../../../../include/partons/utils/test/ComparisonReport.h"

const std::string GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_X = "x";
const std::string GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI = "xi";
const std::string GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2 = "MuF2";
const std::string GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUR2 = "MuR2";

GPDKinematic::GPDKinematic() :
        Kinematic("GPDKinematic"), m_kinematicType(KinematicType::THEO), m_x(
                0.), m_xi(0.), m_t(0.), m_MuF2(0.), m_MuR2(0.) {
}

GPDKinematic::GPDKinematic(double x, double xi, double t, double MuF2,
        double MuR2) :
        Kinematic("GPDKinematic"), m_kinematicType(KinematicType::THEO), m_x(x), m_xi(
                xi), m_t(t), m_MuF2(MuF2), m_MuR2(MuR2) {
}

GPDKinematic::GPDKinematic(ParameterList &parameterList) :
        Kinematic("GPDKinematic"), m_kinematicType(KinematicType::THEO), m_x(
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

std::string GPDKinematic::toString() const {
    return Formatter() << Kinematic::toString() << " m_x = " << m_x
            << " m_xi = " << m_xi << " m_t = " << m_t << " m_MuF2 = " << m_MuF2
            << "(Gev2) m_MuR2 = " << m_MuR2 << "(Gev2)";
}

ComparisonReport GPDKinematic::compare(const GPDKinematic& referenceObject,
        const Tolerances& tolerances) const {
    ComparisonReport comparisonReport(getClassName());

    ComparisonData x_comparisonData = CompareUtils::compareDouble(
            GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_X, getX(),
            referenceObject.getX(), tolerances);
    comparisonReport.addComparisonData(x_comparisonData);

    ComparisonData xi_comparisonData = CompareUtils::compareDouble(
            GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI, getXi(),
            referenceObject.getXi(), tolerances);
    comparisonReport.addComparisonData(xi_comparisonData);

    ComparisonData MuF2_comparisonData = CompareUtils::compareDouble(
            GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2, getMuF2(),
            referenceObject.getMuF2(), tolerances);
    comparisonReport.addComparisonData(MuF2_comparisonData);

    ComparisonData MuR2_comparisonData = CompareUtils::compareDouble(
            GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2, getMuR2(),
            referenceObject.getMuR2(), tolerances);
    comparisonReport.addComparisonData(MuR2_comparisonData);

    return comparisonReport;
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
