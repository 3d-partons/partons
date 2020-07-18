#include "../../../../include/partons/beans/parton_distribution/CollinearDistributionKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

const std::string CollinearDistributionKinematic::COLLINEAR_DISTRIBUTION_KNEMATIC_CLASS_NAME = "CollinearDistributionKinematic";

const std::string CollinearDistributionKinematic::KINEMATIC_PARAMETER_NAME_X = "x";
const std::string CollinearDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUF2 = "MuF2";
const std::string CollinearDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUR2 = "MuR2";

const std::string CollinearDistributionKinematic::KINEMATIC_PARAMETER_NAME_X_UNIT = "x_unit";
const std::string CollinearDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUF2_UNIT = "MuF2_unit";
const std::string CollinearDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUR2_UNIT = "MuR2_unit";

CollinearDistributionKinematic::CollinearDistributionKinematic() :
        Kinematic("CollinearDistributionKinematic", ChannelType::UNDEFINED), m_x(
                PhysicalType<double>(PhysicalUnit::NONE)), m_MuF2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(PhysicalUnit::GEV2)) {
}

CollinearDistributionKinematic::CollinearDistributionKinematic(double x, double MuF2, double MuR2) :
        Kinematic("CollinearDistributionKinematic", ChannelType::UNDEFINED), m_x(
                PhysicalType<double>(x, PhysicalUnit::NONE)), m_MuF2(
                PhysicalType<double>(MuF2, PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(MuR2, PhysicalUnit::GEV2)) {
}

CollinearDistributionKinematic::CollinearDistributionKinematic(const PhysicalType<double> &x,
        const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2) :
        Kinematic("CollinearDistributionKinematic", ChannelType::UNDEFINED), m_x(
                PhysicalType<double>(PhysicalUnit::NONE)), m_MuF2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(PhysicalUnit::GEV2)) {

    m_x.checkIfSameUnitCategoryAs(x);
    m_MuF2.checkIfSameUnitCategoryAs(MuF2);
    m_MuR2.checkIfSameUnitCategoryAs(MuR2);

    m_x = x;
    m_MuF2 = MuF2;
    m_MuR2 = MuR2;
}

CollinearDistributionKinematic::CollinearDistributionKinematic(const ElemUtils::GenericType& x,
        const ElemUtils::GenericType& MuF2, const ElemUtils::GenericType& MuR2) :
        Kinematic("CollinearDistributionKinematic", ChannelType::UNDEFINED), m_x(
                PhysicalType<double>(x, PhysicalUnit::NONE)), m_MuF2(
                PhysicalType<double>(MuF2, PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(MuR2, PhysicalUnit::GEV2)) {
}

CollinearDistributionKinematic::CollinearDistributionKinematic(const CollinearDistributionKinematic& other) :
        Kinematic(other), m_x(other.m_x), m_MuF2(other.m_MuF2), m_MuR2(other.m_MuR2) {
}

CollinearDistributionKinematic::~CollinearDistributionKinematic() {
}

void CollinearDistributionKinematic::configure(const ElemUtils::Parameters &parameters) {

    //run for mother
    Kinematic::configure(parameters);

    double value;
    PhysicalUnit::Type unit;

    //x
    if (parameters.isAvailable(CollinearDistributionKinematic::KINEMATIC_PARAMETER_NAME_X)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                CollinearDistributionKinematic::KINEMATIC_PARAMETER_NAME_X_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setX(value, unit);
        } else {
            setX(value, PhysicalUnit::NONE);
        }
    } else {
        errorMissingParameter(CollinearDistributionKinematic::KINEMATIC_PARAMETER_NAME_X);
    }

    //muF2
    if (parameters.isAvailable(CollinearDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUF2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                CollinearDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUF2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setMuF2(value, unit);
        } else {
            setMuF2(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(CollinearDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUF2);
    }

    //muR2
    if (parameters.isAvailable(CollinearDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUR2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                CollinearDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUR2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setMuR2(value, unit);
        } else {
            setMuR2(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(CollinearDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUR2);
    }
}

std::string CollinearDistributionKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << Kinematic::toString() << '\n';

    if (m_x.isInitialized())
        formatter << "x: " << m_x.toString() << ' ';
    if (m_MuF2.isInitialized())
        formatter << "muF2: " << m_MuF2.toString() << ' ';
    if (m_MuR2.isInitialized())
        formatter << "muR2: " << m_MuR2.toString() << ' ';

    return formatter.str();
}

void CollinearDistributionKinematic::serialize(ElemUtils::Packet &packet) const {

    Kinematic::serialize(packet);

    packet << m_x << m_MuF2 << m_MuR2;
}

void CollinearDistributionKinematic::unserialize(ElemUtils::Packet &packet) {

    Kinematic::unserialize(packet);

    packet >> m_x;
    packet >> m_MuF2;
    packet >> m_MuR2;

    updateHashSum();
}

void CollinearDistributionKinematic::serializeIntoStdVector(std::vector<double>& vec) const {

    Kinematic::serializeIntoStdVector(vec);

    m_x.serializeIntoStdVector(vec);
    m_MuF2.serializeIntoStdVector(vec);
    m_MuR2.serializeIntoStdVector(vec);
}

void CollinearDistributionKinematic::unserializeFromStdVector(
        std::vector<double>::const_iterator& it,
        const std::vector<double>::const_iterator& end) {

    Kinematic::unserializeFromStdVector(it, end);

    m_x.unserializeFromStdVector(it, end);
    m_MuF2.unserializeFromStdVector(it, end);
    m_MuR2.unserializeFromStdVector(it, end);

    updateHashSum();
}

bool CollinearDistributionKinematic::operator ==(const CollinearDistributionKinematic& other) const {
    return m_x == other.getX() && m_MuF2 == other.getMuF2() && m_MuR2 == other.getMuR2();
}

bool CollinearDistributionKinematic::operator !=(const CollinearDistributionKinematic& other) const {
    return !((*this) == other);
}

void CollinearDistributionKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_x.toStdString()
                            << m_MuF2.toStdString() << m_MuR2.toStdString()));
}

const PhysicalType<double>& CollinearDistributionKinematic::getX() const {
    return m_x;
}

const PhysicalType<double>& CollinearDistributionKinematic::getMuF2() const {
    return m_MuF2;
}

const PhysicalType<double>& CollinearDistributionKinematic::getMuR2() const {
    return m_MuR2;
}

void CollinearDistributionKinematic::setX(const PhysicalType<double>& x) {

    m_x.checkIfSameUnitCategoryAs(x);
    m_x = x;
    updateHashSum();
}

void CollinearDistributionKinematic::setMuF2(const PhysicalType<double>& muF2) {

    m_MuF2.checkIfSameUnitCategoryAs(muF2);
    m_MuF2 = muF2;
    updateHashSum();
}

void CollinearDistributionKinematic::setMuR2(const PhysicalType<double>& muR2) {

    m_MuR2.checkIfSameUnitCategoryAs(muR2);
    m_MuR2 = muR2;
    updateHashSum();
}

void CollinearDistributionKinematic::setX(double x, PhysicalUnit::Type unit) {
    setX(PhysicalType<double>(x, unit));
}

void CollinearDistributionKinematic::setMuF2(double muF2, PhysicalUnit::Type unit) {
    setMuF2(PhysicalType<double>(muF2, unit));
}

void CollinearDistributionKinematic::setMuR2(double muR2, PhysicalUnit::Type unit) {
    setMuR2(PhysicalType<double>(muR2, unit));
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        CollinearDistributionKinematic& kinematic) {

    kinematic.serialize(packet);
    return packet;
}
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        CollinearDistributionKinematic& kinematic) {

    kinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
