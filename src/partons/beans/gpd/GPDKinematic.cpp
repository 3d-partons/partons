#include "../../../../include/partons/beans/gpd/GPDKinematic.h"

#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/thread/Packet.h>

#include "../../../../include/partons/beans/observable/ObservableKinematic.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"

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

//GPDKinematic::GPDKinematic(const std::string& x, const std::string& xi,
//        const std::string& t, const std::string& MuF2, const std::string& MuR2) :
//        Kinematic("GPDKinematic"), m_kinematicType(KinematicType::THEO), m_x(
//                ElemUtils::StringUtils::fromStringToDouble(x)), m_xi(
//                ElemUtils::StringUtils::fromStringToDouble(xi)), m_t(
//                ElemUtils::StringUtils::fromStringToDouble(t)), m_MuF2(
//                ElemUtils::StringUtils::fromStringToDouble(MuF2)), m_MuR2(
//                ElemUtils::StringUtils::fromStringToDouble(MuR2)) {
//}

GPDKinematic::GPDKinematic(const ElemUtils::GenericType& x,
        const ElemUtils::GenericType& xi, const ElemUtils::GenericType& t,
        const ElemUtils::GenericType& MuF2, const ElemUtils::GenericType& MuR2) :
        Kinematic("GPDKinematic"), m_kinematicType(KinematicType::THEO), m_x(
                x.toDouble()), m_xi(xi.toDouble()), m_t(t.toDouble()), m_MuF2(
                MuF2.toDouble()), m_MuR2(MuR2.toDouble()) {
}

GPDKinematic::GPDKinematic(const ElemUtils::Parameters &parameters) :
        Kinematic("GPDKinematic"), m_kinematicType(KinematicType::THEO), m_x(
                0.), m_xi(0.), m_t(0.), m_MuF2(0.), m_MuR2(0.) {

    if (parameters.isAvailable(GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_X)) {
        m_x = parameters.getLastAvailable().toDouble();
    } else {
        error(__func__,
                ElemUtils::Formatter() << "Missing parameter <"
                        << GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_X << ">");
    }
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

    //TODO remove from kinematic
    m_MuF2 =
            parameters.get(GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2).toDouble();
    m_MuR2 =
            parameters.get(GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUR2).toDouble();
}

GPDKinematic::~GPDKinematic() {
}

std::string GPDKinematic::toString() const {
    return ElemUtils::Formatter() << Kinematic::toString() << " m_x = " << m_x
            << " m_xi = " << m_xi << " m_t = " << m_t << " m_MuF2 = " << m_MuF2
            << "(Gev2) m_MuR2 = " << m_MuR2 << "(Gev2)";
}

//ComparisonReport GPDKinematic::compare(const GPDKinematic& referenceObject,
//        const NumA::Tolerances& tolerances) const {
//    ComparisonReport comparisonReport(getClassName());
//
//    ComparisonData x_comparisonData = CompareUtils::compareDouble(
//            GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_X, getX(),
//            referenceObject.getX(), tolerances);
//    comparisonReport.addComparisonData(x_comparisonData);
//
//    ComparisonData xi_comparisonData = CompareUtils::compareDouble(
//            GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI, getXi(),
//            referenceObject.getXi(), tolerances);
//    comparisonReport.addComparisonData(xi_comparisonData);
//
//    ComparisonData MuF2_comparisonData = CompareUtils::compareDouble(
//            GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2, getMuF2(),
//            referenceObject.getMuF2(), tolerances);
//    comparisonReport.addComparisonData(MuF2_comparisonData);
//
//    ComparisonData MuR2_comparisonData = CompareUtils::compareDouble(
//            GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2, getMuR2(),
//            referenceObject.getMuR2(), tolerances);
//    comparisonReport.addComparisonData(MuR2_comparisonData);
//
//    return comparisonReport;
//}

void GPDKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_x << m_xi << m_t << m_MuF2
                            << m_MuR2));
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

void GPDKinematic::serialize(ElemUtils::Packet &packet) const {
    Kinematic::serialize(packet);
    packet << m_x << m_xi << m_t << m_MuF2 << m_MuR2;
}

void GPDKinematic::unserialize(ElemUtils::Packet &packet) {
//    int tempListEntryPosition;
//    packet >> tempListEntryPosition;
//    setListEntryPosition(tempListEntryPosition);

    Kinematic::unserialize(packet);

    packet >> m_x;
    packet >> m_xi;
    packet >> m_t;
    packet >> m_MuF2;
    packet >> m_MuR2;
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        GPDKinematic& kinematic) {
    kinematic.serialize(packet);
    return packet;
}
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        GPDKinematic& kinematic) {

    kinematic.unserialize(packet);
    return packet;
}
