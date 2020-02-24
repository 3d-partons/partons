#include "../../../../include/partons/beans/gpd/GPDKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

const std::string GPDKinematic::GPD_KNEMATIC_CLASS_NAME = "GPDKinematic";

const std::string GPDKinematic::KINEMATIC_PARAMETER_NAME_X = "x";
const std::string GPDKinematic::KINEMATIC_PARAMETER_NAME_XI = "xi";
const std::string GPDKinematic::KINEMATIC_PARAMETER_NAME_T = "t";
const std::string GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2 = "MuF2";
const std::string GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2 = "MuR2";

const std::string GPDKinematic::KINEMATIC_PARAMETER_NAME_X_UNIT = "x_unit";
const std::string GPDKinematic::KINEMATIC_PARAMETER_NAME_XI_UNIT = "xi_unit";
const std::string GPDKinematic::KINEMATIC_PARAMETER_NAME_T_UNIT = "t_unit";
const std::string GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2_UNIT = "MuF2_unit";
const std::string GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2_UNIT = "MuR2_unit";

GPDKinematic::GPDKinematic() :
        Kinematic("GPDKinematic", ChannelType::UNDEFINED), m_x(
                PhysicalType<double>(PhysicalUnit::NONE)), m_xi(
                PhysicalType<double>(PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_MuF2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(PhysicalUnit::GEV2)) {
}

GPDKinematic::GPDKinematic(double x, double xi, double t, double MuF2,
        double MuR2) :
        Kinematic("GPDKinematic", ChannelType::UNDEFINED), m_x(
                PhysicalType<double>(x, PhysicalUnit::NONE)), m_xi(
                PhysicalType<double>(xi, PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_MuF2(
                PhysicalType<double>(MuF2, PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(MuR2, PhysicalUnit::GEV2)) {
}

GPDKinematic::GPDKinematic(const PhysicalType<double> &x,
        const PhysicalType<double> &xi, const PhysicalType<double> &t,
        const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2) :
        Kinematic("GPDKinematic", ChannelType::UNDEFINED), m_x(
                PhysicalType<double>(PhysicalUnit::NONE)), m_xi(
                PhysicalType<double>(PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_MuF2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(PhysicalUnit::GEV2)) {

    m_x.checkIfSameUnitCategoryAs(x);
    m_xi.checkIfSameUnitCategoryAs(xi);
    m_t.checkIfSameUnitCategoryAs(t);
    m_MuF2.checkIfSameUnitCategoryAs(MuF2);
    m_MuR2.checkIfSameUnitCategoryAs(MuR2);

    m_x = x;
    m_xi = xi;
    m_t = t;
    m_MuF2 = MuF2;
    m_MuR2 = MuR2;
}

GPDKinematic::GPDKinematic(const ElemUtils::GenericType& x,
        const ElemUtils::GenericType& xi, const ElemUtils::GenericType& t,
        const ElemUtils::GenericType& MuF2, const ElemUtils::GenericType& MuR2) :
        Kinematic("GPDKinematic", ChannelType::UNDEFINED), m_x(
                PhysicalType<double>(x, PhysicalUnit::NONE)), m_xi(
                PhysicalType<double>(xi, PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_MuF2(
                PhysicalType<double>(MuF2, PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(MuR2, PhysicalUnit::GEV2)) {
}

GPDKinematic::GPDKinematic(const GPDKinematic& other) :
        Kinematic(other), m_x(other.m_x), m_xi(other.m_xi), m_t(other.m_t), m_MuF2(
                other.m_MuF2), m_MuR2(other.m_MuR2) {
}

GPDKinematic::~GPDKinematic() {
}

void GPDKinematic::configure(const ElemUtils::Parameters &parameters) {

    //run for mother
    Kinematic::configure(parameters);

    double value;
    PhysicalUnit::Type unit;

    //x
    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_X)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                GPDKinematic::KINEMATIC_PARAMETER_NAME_X_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setX(value, unit);
        } else {
            setX(value, PhysicalUnit::NONE);
        }
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_X);
    }

    //xi
    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_XI)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                GPDKinematic::KINEMATIC_PARAMETER_NAME_XI_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setXi(value, unit);
        } else {
            setXi(value, PhysicalUnit::NONE);
        }
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_XI);
    }

    //t
    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_T)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                GPDKinematic::KINEMATIC_PARAMETER_NAME_T_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setT(value, unit);
        } else {
            setT(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_T);
    }

    //muF2
    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setMuF2(value, unit);
        } else {
            setMuF2(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2);
    }

    //muR2
    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setMuR2(value, unit);
        } else {
            setMuR2(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2);
    }
}

std::string GPDKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << Kinematic::toString() << '\n';

    if (m_x.isInitialized())
        formatter << "x: " << m_x.toString() << ' ';
    if (m_xi.isInitialized())
        formatter << "xi: " << m_xi.toString() << ' ';
    if (m_t.isInitialized())
        formatter << "t: " << m_t.toString() << ' ';
    if (m_MuF2.isInitialized())
        formatter << "muF2: " << m_MuF2.toString() << ' ';
    if (m_MuR2.isInitialized())
        formatter << "muR2: " << m_MuR2.toString() << ' ';

    return formatter.str();
}

void GPDKinematic::serialize(ElemUtils::Packet &packet) const {

    Kinematic::serialize(packet);

    packet << m_x << m_xi << m_t << m_MuF2 << m_MuR2;
}

void GPDKinematic::unserialize(ElemUtils::Packet &packet) {

    Kinematic::unserialize(packet);

    packet >> m_x;
    packet >> m_xi;
    packet >> m_t;
    packet >> m_MuF2;
    packet >> m_MuR2;

    updateHashSum();
}

void GPDKinematic::serializeIntoStdVector(std::vector<double>& vec) const {

    Kinematic::serializeIntoStdVector(vec);

    m_x.serializeIntoStdVector(vec);
    m_xi.serializeIntoStdVector(vec);
    m_t.serializeIntoStdVector(vec);
    m_MuF2.serializeIntoStdVector(vec);
    m_MuR2.serializeIntoStdVector(vec);
}

void GPDKinematic::unserializeFromStdVector(
        std::vector<double>::const_iterator& it,
        const std::vector<double>::const_iterator& end) {

    Kinematic::unserializeFromStdVector(it, end);

    m_x.unserializeFromStdVector(it, end);
    m_xi.unserializeFromStdVector(it, end);
    m_t.unserializeFromStdVector(it, end);
    m_MuF2.unserializeFromStdVector(it, end);
    m_MuR2.unserializeFromStdVector(it, end);

    updateHashSum();
}

bool GPDKinematic::operator ==(const GPDKinematic& other) const {
    return m_x == other.getX() && m_xi == other.getXi() && m_t == other.getT()
            && m_MuF2 == other.getMuF2() && m_MuR2 == other.getMuR2();
}

bool GPDKinematic::operator !=(const GPDKinematic& other) const {
    return !((*this) == other);
}

void GPDKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_x.toStdString()
                            << m_xi.toStdString() << m_t.toStdString()
                            << m_MuF2.toStdString() << m_MuR2.toStdString()));
}

const PhysicalType<double>& GPDKinematic::getX() const {
    return m_x;
}

const PhysicalType<double>& GPDKinematic::getXi() const {
    return m_xi;
}

const PhysicalType<double>& GPDKinematic::getT() const {
    return m_t;
}

const PhysicalType<double>& GPDKinematic::getMuF2() const {
    return m_MuF2;
}

const PhysicalType<double>& GPDKinematic::getMuR2() const {
    return m_MuR2;
}

void GPDKinematic::setX(const PhysicalType<double>& x) {

    m_x.checkIfSameUnitCategoryAs(x);
    m_x = x;
    updateHashSum();
}

void GPDKinematic::setXi(const PhysicalType<double>& xi) {

    m_xi.checkIfSameUnitCategoryAs(xi);
    m_xi = xi;
    updateHashSum();
}

void GPDKinematic::setT(const PhysicalType<double>& t) {

    m_t.checkIfSameUnitCategoryAs(t);
    m_t = t;
    updateHashSum();
}

void GPDKinematic::setMuF2(const PhysicalType<double>& muF2) {

    m_MuF2.checkIfSameUnitCategoryAs(muF2);
    m_MuF2 = muF2;
    updateHashSum();
}

void GPDKinematic::setMuR2(const PhysicalType<double>& muR2) {

    m_MuR2.checkIfSameUnitCategoryAs(muR2);
    m_MuR2 = muR2;
    updateHashSum();
}

void GPDKinematic::setX(double x, PhysicalUnit::Type unit) {
    setX(PhysicalType<double>(x, unit));
}

void GPDKinematic::setXi(double xi, PhysicalUnit::Type unit) {
    setXi(PhysicalType<double>(xi, unit));
}

void GPDKinematic::setT(double t, PhysicalUnit::Type unit) {
    setT(PhysicalType<double>(t, unit));
}

void GPDKinematic::setMuF2(double muF2, PhysicalUnit::Type unit) {
    setMuF2(PhysicalType<double>(muF2, unit));
}

void GPDKinematic::setMuR2(double muR2, PhysicalUnit::Type unit) {
    setMuR2(PhysicalType<double>(muR2, unit));
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

} /* namespace PARTONS */
