#include "../../../../include/partons/beans/parton_distribution/PartonDistributionKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

const std::string PartonDistributionKinematic::PARTON_DISTRIBUTION_KNEMATIC_CLASS_NAME = "PartonDistributionKinematic";

const std::string PartonDistributionKinematic::KINEMATIC_PARAMETER_NAME_X = "x";
const std::string PartonDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUF2 = "MuF2";
const std::string PartonDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUR2 = "MuR2";

const std::string PartonDistributionKinematic::KINEMATIC_PARAMETER_NAME_X_UNIT = "x_unit";
const std::string PartonDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUF2_UNIT = "MuF2_unit";
const std::string PartonDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUR2_UNIT = "MuR2_unit";

PartonDistributionKinematic::PartonDistributionKinematic() :
        Kinematic("PartonDistributionKinematic", ChannelType::UNDEFINED), m_x(
                PhysicalType<double>(PhysicalUnit::NONE)), m_MuF2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(PhysicalUnit::GEV2)) {
}

PartonDistributionKinematic::PartonDistributionKinematic(double x, double MuF2, double MuR2) :
        Kinematic("PartonDistributionKinematic", ChannelType::UNDEFINED), m_x(
                PhysicalType<double>(x, PhysicalUnit::NONE)), m_MuF2(
                PhysicalType<double>(MuF2, PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(MuR2, PhysicalUnit::GEV2)) {
}

PartonDistributionKinematic::PartonDistributionKinematic(const PhysicalType<double> &x,
        const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2) :
        Kinematic("PartonDistributionKinematic", ChannelType::UNDEFINED), m_x(
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

PartonDistributionKinematic::PartonDistributionKinematic(const ElemUtils::GenericType& x,
        const ElemUtils::GenericType& MuF2, const ElemUtils::GenericType& MuR2) :
        Kinematic("PartonDistributionKinematic", ChannelType::UNDEFINED), m_x(
                PhysicalType<double>(x, PhysicalUnit::NONE)), m_MuF2(
                PhysicalType<double>(MuF2, PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(MuR2, PhysicalUnit::GEV2)) {
}

PartonDistributionKinematic::PartonDistributionKinematic(const PartonDistributionKinematic& other) :
        Kinematic(other), m_x(other.m_x), m_MuF2(other.m_MuF2), m_MuR2(other.m_MuR2) {
}

PartonDistributionKinematic::~PartonDistributionKinematic() {
}

void PartonDistributionKinematic::configure(const ElemUtils::Parameters &parameters) {

    //run for mother
    Kinematic::configure(parameters);

    double value;
    PhysicalUnit::Type unit;

    //x
    if (parameters.isAvailable(PartonDistributionKinematic::KINEMATIC_PARAMETER_NAME_X)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                PartonDistributionKinematic::KINEMATIC_PARAMETER_NAME_X_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setX(value, unit);
        } else {
            setX(value, PhysicalUnit::NONE);
        }
    } else {
        errorMissingParameter(PartonDistributionKinematic::KINEMATIC_PARAMETER_NAME_X);
    }

    //muF2
    if (parameters.isAvailable(PartonDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUF2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                PartonDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUF2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setMuF2(value, unit);
        } else {
            setMuF2(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(PartonDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUF2);
    }

    //muR2
    if (parameters.isAvailable(PartonDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUR2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                PartonDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUR2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setMuR2(value, unit);
        } else {
            setMuR2(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(PartonDistributionKinematic::KINEMATIC_PARAMETER_NAME_MUR2);
    }
}

std::string PartonDistributionKinematic::toString() const {

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

void PartonDistributionKinematic::serialize(ElemUtils::Packet &packet) const {

    Kinematic::serialize(packet);

    packet << m_x << m_MuF2 << m_MuR2;
}

void PartonDistributionKinematic::unserialize(ElemUtils::Packet &packet) {

    Kinematic::unserialize(packet);

    packet >> m_x;
    packet >> m_MuF2;
    packet >> m_MuR2;

    updateHashSum();
}

void PartonDistributionKinematic::serializeIntoStdVector(std::vector<double>& vec) const {

    Kinematic::serializeIntoStdVector(vec);

    m_x.serializeIntoStdVector(vec);
    m_MuF2.serializeIntoStdVector(vec);
    m_MuR2.serializeIntoStdVector(vec);
}

void PartonDistributionKinematic::unserializeFromStdVector(
        std::vector<double>::const_iterator& it,
        const std::vector<double>::const_iterator& end) {

    Kinematic::unserializeFromStdVector(it, end);

    m_x.unserializeFromStdVector(it, end);
    m_MuF2.unserializeFromStdVector(it, end);
    m_MuR2.unserializeFromStdVector(it, end);

    updateHashSum();
}

bool PartonDistributionKinematic::operator ==(const PartonDistributionKinematic& other) const {
    return m_x == other.getX() && m_MuF2 == other.getMuF2() && m_MuR2 == other.getMuR2();
}

bool PartonDistributionKinematic::operator !=(const PartonDistributionKinematic& other) const {
    return !((*this) == other);
}

void PartonDistributionKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_x.toStdString()
                            << m_MuF2.toStdString() << m_MuR2.toStdString()));
}

const PhysicalType<double>& PartonDistributionKinematic::getX() const {
    return m_x;
}

const PhysicalType<double>& PartonDistributionKinematic::getMuF2() const {
    return m_MuF2;
}

const PhysicalType<double>& PartonDistributionKinematic::getMuR2() const {
    return m_MuR2;
}

void PartonDistributionKinematic::setX(const PhysicalType<double>& x) {

    m_x.checkIfSameUnitCategoryAs(x);
    m_x = x;
    updateHashSum();
}

void PartonDistributionKinematic::setMuF2(const PhysicalType<double>& muF2) {

    m_MuF2.checkIfSameUnitCategoryAs(muF2);
    m_MuF2 = muF2;
    updateHashSum();
}

void PartonDistributionKinematic::setMuR2(const PhysicalType<double>& muR2) {

    m_MuR2.checkIfSameUnitCategoryAs(muR2);
    m_MuR2 = muR2;
    updateHashSum();
}

void PartonDistributionKinematic::setX(double x, PhysicalUnit::Type unit) {
    setX(PhysicalType<double>(x, unit));
}

void PartonDistributionKinematic::setMuF2(double muF2, PhysicalUnit::Type unit) {
    setMuF2(PhysicalType<double>(muF2, unit));
}

void PartonDistributionKinematic::setMuR2(double muR2, PhysicalUnit::Type unit) {
    setMuR2(PhysicalType<double>(muR2, unit));
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        PartonDistributionKinematic& kinematic) {

    kinematic.serialize(packet);
    return packet;
}
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        PartonDistributionKinematic& kinematic) {

    kinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
