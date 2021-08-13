#include "../../../../../include/partons/beans/convol_coeff_function/DVMP/DVMPConvolCoeffFunctionKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/thread/Packet.h>
#include <iterator>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

const std::string DVMPConvolCoeffFunctionKinematic::DVMP_CONVOL_COEFF_FUNCTION_KNEMATIC_CLASS_NAME =
        "DVMPConvolCoeffFunctionKinematic";

const std::string DVMPConvolCoeffFunctionKinematic::KINEMATIC_PARAMETER_NAME_MESON_POLARIZATION =
        "mesonPolarization";
const std::string DVMPConvolCoeffFunctionKinematic::KINEMATIC_PARAMETER_NAME_MESON_POLARIZATION_UNIT =
        "mesonPolarization_unit";

DVMPConvolCoeffFunctionKinematic::DVMPConvolCoeffFunctionKinematic() :
        ConvolCoeffFunctionKinematic("DVMPConvolCoeffFunctionKinematic",
                ChannelType::DVMP), m_Q2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_mesonType(
                MesonType::UNDEFINED), m_mesonPolarization(
                PolarizationType::UNDEFINED) {
}

DVMPConvolCoeffFunctionKinematic::DVMPConvolCoeffFunctionKinematic(double xi,
        double t, double Q2, double MuF2, double MuR2,
        MesonType::Type mesonType, PolarizationType::Type mesonPolarization) :
        ConvolCoeffFunctionKinematic("DVMPConvolCoeffFunctionKinematic",
                ChannelType::DVMP, xi, t, MuF2, MuR2), m_Q2(
                PhysicalType<double>(Q2, PhysicalUnit::GEV2)), m_mesonType(
                mesonType), m_mesonPolarization(mesonPolarization) {
}

DVMPConvolCoeffFunctionKinematic::DVMPConvolCoeffFunctionKinematic(
        const PhysicalType<double> &xi, const PhysicalType<double> &t,
        const PhysicalType<double> &Q2, const PhysicalType<double> &MuF2,
        const PhysicalType<double> &MuR2, MesonType::Type mesonType,
        PolarizationType::Type mesonPolarization) :
        ConvolCoeffFunctionKinematic("DVMPConvolCoeffFunctionKinematic",
                ChannelType::DVMP, xi, t, MuF2, MuR2), m_Q2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_mesonType(
                mesonType), m_mesonPolarization(mesonPolarization) {

    m_Q2.checkIfSameUnitCategoryAs(Q2);

    m_Q2 = Q2;
}

DVMPConvolCoeffFunctionKinematic::DVMPConvolCoeffFunctionKinematic(
        const ElemUtils::GenericType &xi, const ElemUtils::GenericType &t,
        const ElemUtils::GenericType &Q2, const ElemUtils::GenericType &MuF2,
        const ElemUtils::GenericType &MuR2, MesonType::Type mesonType,
        PolarizationType::Type mesonPolarization) :
        ConvolCoeffFunctionKinematic("DVMPConvolCoeffFunctionKinematic",
                ChannelType::DVMP, xi, t, MuF2, MuR2), m_Q2(
                PhysicalType<double>(Q2, PhysicalUnit::GEV2)), m_mesonType(
                mesonType), m_mesonPolarization(mesonPolarization) {
}

DVMPConvolCoeffFunctionKinematic::DVMPConvolCoeffFunctionKinematic(
        const DVMPConvolCoeffFunctionKinematic &other) :
        ConvolCoeffFunctionKinematic(other), m_Q2(other.m_Q2), m_mesonType(
                other.m_mesonType), m_mesonPolarization(
                other.m_mesonPolarization) {
}

DVMPConvolCoeffFunctionKinematic::~DVMPConvolCoeffFunctionKinematic() {
}

void DVMPConvolCoeffFunctionKinematic::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    ConvolCoeffFunctionKinematic::configure(parameters);

    double value;
    PhysicalUnit::Type unit;

    //Q2
    if (parameters.isAvailable(
            DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setQ2(value, unit);
        } else {
            setQ2(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(
                DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2);
    }

    //meson type
    if (parameters.isAvailable(
            DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_MESON_TYPE)) {
        setMesonType(
                MesonType::fromString(
                        parameters.getLastAvailable().getString()));
    } else {
        errorMissingParameter(
                DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_MESON_TYPE);
    }

    //meson polarization
    if (parameters.isAvailable(
            DVMPConvolCoeffFunctionKinematic::KINEMATIC_PARAMETER_NAME_MESON_POLARIZATION)) {
        setMesonPolarization(
                PolarizationType::fromString(
                        parameters.getLastAvailable().getString()));
    } else {
        errorMissingParameter(
                DVMPConvolCoeffFunctionKinematic::KINEMATIC_PARAMETER_NAME_MESON_POLARIZATION);
    }
}

std::string DVMPConvolCoeffFunctionKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << ConvolCoeffFunctionKinematic::toString();

    if (m_Q2.isInitialized())
        formatter << "Q2: " << m_Q2.toString() << ' ';
    formatter << "meson type: " << MesonType(m_mesonType).toString() << ' ';
    formatter << "meson polarization: "
            << PolarizationType(m_mesonPolarization).toString() << ' ';

    return formatter.str();
}

void DVMPConvolCoeffFunctionKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_xi.toStdString()
                            << m_t.toStdString() << m_Q2.toStdString()
                            << m_MuF2.toStdString() << m_MuR2.toStdString()
                            << MesonType(m_mesonType).toString()
                            << PolarizationType(m_mesonPolarization).toString()));
}

void DVMPConvolCoeffFunctionKinematic::serialize(
        ElemUtils::Packet& packet) const {

    ConvolCoeffFunctionKinematic::serialize(packet);

    packet << m_Q2 << MesonType(m_mesonType)
            << PolarizationType(m_mesonPolarization);
}

void DVMPConvolCoeffFunctionKinematic::unserialize(ElemUtils::Packet& packet) {

    ConvolCoeffFunctionKinematic::unserialize(packet);

    packet >> m_Q2;

    MesonType mesonType;
    packet >> mesonType;
    m_mesonType = mesonType.getType();

    PolarizationType mesonPolarization;
    packet >> mesonPolarization;
    m_mesonPolarization = mesonPolarization.getType();

    updateHashSum();
}

void DVMPConvolCoeffFunctionKinematic::serializeIntoStdVector(
        std::vector<double>& vec) const {

    ConvolCoeffFunctionKinematic::serializeIntoStdVector(vec);

    m_Q2.serializeIntoStdVector(vec);
    vec.push_back(static_cast<double>(m_mesonType));
    vec.push_back(static_cast<double>(m_mesonPolarization));
}

void DVMPConvolCoeffFunctionKinematic::unserializeFromStdVector(
        std::vector<double>::const_iterator& it,
        const std::vector<double>::const_iterator& end) {

    ConvolCoeffFunctionKinematic::unserializeFromStdVector(it, end);

    m_Q2.unserializeFromStdVector(it, end);
    m_mesonType = static_cast<MesonType::Type>(*it);
    it++;
    m_mesonPolarization = static_cast<PolarizationType::Type>(*it);
    it++;

    updateHashSum();
}

bool DVMPConvolCoeffFunctionKinematic::operator ==(
        const DVMPConvolCoeffFunctionKinematic& other) const {
    return m_xi == other.getXi() && m_t == other.getT()
            && m_MuF2 == other.getMuF2() && m_MuR2 == other.getMuR2()
            && m_Q2 == other.getQ2() && m_mesonType == other.getMesonType()
            && m_mesonPolarization == other.getMesonPolarization();
}

bool DVMPConvolCoeffFunctionKinematic::operator !=(
        const DVMPConvolCoeffFunctionKinematic& other) const {
    return !((*this) == other);
}

const PhysicalType<double>& DVMPConvolCoeffFunctionKinematic::getQ2() const {
    return m_Q2;
}

MesonType::Type DVMPConvolCoeffFunctionKinematic::getMesonType() const {
    return m_mesonType;
}

PolarizationType::Type DVMPConvolCoeffFunctionKinematic::getMesonPolarization() const {
    return m_mesonPolarization;
}

void DVMPConvolCoeffFunctionKinematic::setQ2(const PhysicalType<double>& Q2) {

    m_Q2.checkIfSameUnitCategoryAs(Q2);
    m_Q2 = Q2;
    updateHashSum();
}

void DVMPConvolCoeffFunctionKinematic::setQ2(double Q2,
        PhysicalUnit::Type unit) {
    setQ2(PhysicalType<double>(Q2, unit));
}

void DVMPConvolCoeffFunctionKinematic::setMesonType(MesonType::Type mesonType) {

    m_mesonType = mesonType;
    updateHashSum();
}

void DVMPConvolCoeffFunctionKinematic::setMesonPolarization(
        PolarizationType::Type mesonPolarization) {

    m_mesonPolarization = mesonPolarization;
    updateHashSum();
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        DVMPConvolCoeffFunctionKinematic& kinematic) {

    kinematic.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        DVMPConvolCoeffFunctionKinematic& kinematic) {

    kinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
