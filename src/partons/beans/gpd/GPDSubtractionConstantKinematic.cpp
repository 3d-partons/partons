#include "../../../../include/partons/beans/gpd/GPDSubtractionConstantKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

const std::string GPDSubtractionConstantKinematic::GPD_SUBTRACTION_CONSTANT_KNEMATIC_CLASS_NAME =
        "GPDSubtractionConstantKinematic";

const std::string GPDSubtractionConstantKinematic::KINEMATIC_PARAMETER_NAME_T =
        "t";
const std::string GPDSubtractionConstantKinematic::KINEMATIC_PARAMETER_NAME_MUF2 =
        "MuF2";
const std::string GPDSubtractionConstantKinematic::KINEMATIC_PARAMETER_NAME_MUR2 =
        "MuR2";

const std::string GPDSubtractionConstantKinematic::KINEMATIC_PARAMETER_NAME_T_UNIT =
        "t_unit";
const std::string GPDSubtractionConstantKinematic::KINEMATIC_PARAMETER_NAME_MUF2_UNIT =
        "MuF2_unit";
const std::string GPDSubtractionConstantKinematic::KINEMATIC_PARAMETER_NAME_MUR2_UNIT =
        "MuR2_unit";

GPDSubtractionConstantKinematic::GPDSubtractionConstantKinematic() :
        Kinematic("GPDSubtractionConstantKinematic", ChannelType::UNDEFINED), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_MuF2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(PhysicalUnit::GEV2)) {
}

GPDSubtractionConstantKinematic::GPDSubtractionConstantKinematic(double t,
        double MuF2, double MuR2) :
        Kinematic("GPDSubtractionConstantKinematic", ChannelType::UNDEFINED), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_MuF2(
                PhysicalType<double>(MuF2, PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(MuR2, PhysicalUnit::GEV2)) {
}

GPDSubtractionConstantKinematic::GPDSubtractionConstantKinematic(
        const PhysicalType<double> &t, const PhysicalType<double> &MuF2,
        const PhysicalType<double> &MuR2) :
        Kinematic("GPDSubtractionConstantKinematic", ChannelType::UNDEFINED), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_MuF2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(PhysicalUnit::GEV2)) {

    m_t.checkIfSameUnitCategoryAs(t);
    m_MuF2.checkIfSameUnitCategoryAs(MuF2);
    m_MuR2.checkIfSameUnitCategoryAs(MuR2);

    m_t = t;
    m_MuF2 = MuF2;
    m_MuR2 = MuR2;
}

GPDSubtractionConstantKinematic::GPDSubtractionConstantKinematic(
        const ElemUtils::GenericType& t, const ElemUtils::GenericType& MuF2,
        const ElemUtils::GenericType& MuR2) :
        Kinematic("GPDSubtractionConstantKinematic", ChannelType::UNDEFINED), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_MuF2(
                PhysicalType<double>(MuF2, PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(MuR2, PhysicalUnit::GEV2)) {
}

GPDSubtractionConstantKinematic::GPDSubtractionConstantKinematic(
        const GPDSubtractionConstantKinematic& other) :
        Kinematic(other), m_t(other.m_t), m_MuF2(other.m_MuF2), m_MuR2(
                other.m_MuR2) {
}

GPDSubtractionConstantKinematic::~GPDSubtractionConstantKinematic() {
}

void GPDSubtractionConstantKinematic::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    Kinematic::configure(parameters);

    double value;
    PhysicalUnit::Type unit;

    //t
    if (parameters.isAvailable(
            GPDSubtractionConstantKinematic::KINEMATIC_PARAMETER_NAME_T)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                GPDSubtractionConstantKinematic::KINEMATIC_PARAMETER_NAME_T_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setT(value, unit);
        } else {
            setT(value);
        }
    } else {
        errorMissingParameter(
                GPDSubtractionConstantKinematic::KINEMATIC_PARAMETER_NAME_T);
    }

    //muF2
    if (parameters.isAvailable(
            GPDSubtractionConstantKinematic::KINEMATIC_PARAMETER_NAME_MUF2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                GPDSubtractionConstantKinematic::KINEMATIC_PARAMETER_NAME_MUF2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setMuF2(value, unit);
        } else {
            setMuF2(value);
        }
    } else {
        errorMissingParameter(
                GPDSubtractionConstantKinematic::KINEMATIC_PARAMETER_NAME_MUF2);
    }

    //muR2
    if (parameters.isAvailable(
            GPDSubtractionConstantKinematic::KINEMATIC_PARAMETER_NAME_MUR2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                GPDSubtractionConstantKinematic::KINEMATIC_PARAMETER_NAME_MUR2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setMuR2(value, unit);
        } else {
            setMuR2(value);
        }
    } else {
        errorMissingParameter(
                GPDSubtractionConstantKinematic::KINEMATIC_PARAMETER_NAME_MUR2);
    }
}

std::string GPDSubtractionConstantKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << Kinematic::toString() << '\n';

    if (m_t.isInitialized())
        formatter << "t: " << m_t.toString() << ' ';
    if (m_MuF2.isInitialized())
        formatter << "muF2: " << m_MuF2.toString() << ' ';
    if (m_MuR2.isInitialized())
        formatter << "muR2: " << m_MuR2.toString() << ' ';

    return formatter.str();
}

void GPDSubtractionConstantKinematic::serialize(
        ElemUtils::Packet &packet) const {

    Kinematic::serialize(packet);

    packet << m_t << m_MuF2 << m_MuR2;
}

void GPDSubtractionConstantKinematic::unserialize(ElemUtils::Packet &packet) {

    Kinematic::unserialize(packet);

    packet >> m_t;
    packet >> m_MuF2;
    packet >> m_MuR2;

    updateHashSum();
}

bool GPDSubtractionConstantKinematic::operator ==(
        const GPDSubtractionConstantKinematic& other) const {
    return m_t == other.getT() && m_MuF2 == other.getMuF2()
            && m_MuR2 == other.getMuR2();
}

bool GPDSubtractionConstantKinematic::operator !=(
        const GPDSubtractionConstantKinematic& other) const {
    return !((*this) == other);
}

void GPDSubtractionConstantKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_t.toStdString()
                            << m_MuF2.toStdString() << m_MuR2.toStdString()));
}

const PhysicalType<double>& GPDSubtractionConstantKinematic::getT() const {
    return m_t;
}

const PhysicalType<double>& GPDSubtractionConstantKinematic::getMuF2() const {
    return m_MuF2;
}

const PhysicalType<double>& GPDSubtractionConstantKinematic::getMuR2() const {
    return m_MuR2;
}

void GPDSubtractionConstantKinematic::setT(const PhysicalType<double>& t) {

    m_t.checkIfSameUnitCategoryAs(t);
    m_t = t;
    updateHashSum();
}

void GPDSubtractionConstantKinematic::setMuF2(
        const PhysicalType<double>& muF2) {

    m_MuF2.checkIfSameUnitCategoryAs(muF2);
    m_MuF2 = muF2;
    updateHashSum();
}

void GPDSubtractionConstantKinematic::setMuR2(
        const PhysicalType<double>& muR2) {

    m_MuR2.checkIfSameUnitCategoryAs(muR2);
    m_MuR2 = muR2;
    updateHashSum();
}

void GPDSubtractionConstantKinematic::setT(double t, PhysicalUnit::Type unit) {
    setT(PhysicalType<double>(t, unit));
}

void GPDSubtractionConstantKinematic::setMuF2(double muF2,
        PhysicalUnit::Type unit) {
    setMuF2(PhysicalType<double>(muF2, unit));
}

void GPDSubtractionConstantKinematic::setMuR2(double muR2,
        PhysicalUnit::Type unit) {
    setMuR2(PhysicalType<double>(muR2, unit));
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        GPDSubtractionConstantKinematic& kinematic) {

    kinematic.serialize(packet);
    return packet;
}
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        GPDSubtractionConstantKinematic& kinematic) {

    kinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
