#include "../../../../../include/partons/beans/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/observable/DDVCS/DDVCSObservableKinematic.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

const std::string DDVCSConvolCoeffFunctionKinematic::DDVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_CLASS_NAME =
        "DDVCSConvolCoeffFunctionKinematic";

DDVCSConvolCoeffFunctionKinematic::DDVCSConvolCoeffFunctionKinematic() :
        ConvolCoeffFunctionKinematic("DDVCSConvolCoeffFunctionKinematic",
                ChannelType::DDVCS), m_Q2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Q2Prim(
                PhysicalType<double>(PhysicalUnit::GEV2)) {
}

DDVCSConvolCoeffFunctionKinematic::DDVCSConvolCoeffFunctionKinematic(double xi,
        double t, double Q2, double Q2Prim, double MuF2, double MuR2) :
        ConvolCoeffFunctionKinematic("DDVCSConvolCoeffFunctionKinematic",
                ChannelType::DDVCS, xi, t, MuF2, MuR2), m_Q2(
                PhysicalType<double>(Q2, PhysicalUnit::GEV2)), m_Q2Prim(
                PhysicalType<double>(Q2Prim, PhysicalUnit::GEV2)) {
}

DDVCSConvolCoeffFunctionKinematic::DDVCSConvolCoeffFunctionKinematic(
        const PhysicalType<double> &xi, const PhysicalType<double> &t,
        const PhysicalType<double> &Q2, const PhysicalType<double> &Q2Prim,
        const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2) :
        ConvolCoeffFunctionKinematic("DDVCSConvolCoeffFunctionKinematic",
                ChannelType::DDVCS, xi, t, MuF2, MuR2), m_Q2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Q2Prim(
                PhysicalType<double>(PhysicalUnit::GEV2)) {

    m_Q2.checkIfSameUnitCategoryAs(Q2);
    m_Q2Prim.checkIfSameUnitCategoryAs(Q2Prim);

    m_Q2 = Q2;
    m_Q2Prim = Q2Prim;
}

DDVCSConvolCoeffFunctionKinematic::DDVCSConvolCoeffFunctionKinematic(
        const ElemUtils::GenericType &xi, const ElemUtils::GenericType &t,
        const ElemUtils::GenericType &Q2, const ElemUtils::GenericType &Q2Prim,
        const ElemUtils::GenericType &MuF2, const ElemUtils::GenericType &MuR2) :
        ConvolCoeffFunctionKinematic("DDVCSConvolCoeffFunctionKinematic",
                ChannelType::DDVCS, xi, t, MuF2, MuR2), m_Q2(
                PhysicalType<double>(Q2, PhysicalUnit::GEV2)), m_Q2Prim(
                PhysicalType<double>(Q2Prim, PhysicalUnit::GEV2)) {
}

DDVCSConvolCoeffFunctionKinematic::DDVCSConvolCoeffFunctionKinematic(
        const DDVCSConvolCoeffFunctionKinematic &other) :
        ConvolCoeffFunctionKinematic(other), m_Q2(other.m_Q2), m_Q2Prim(
                other.m_Q2Prim) {
}

DDVCSConvolCoeffFunctionKinematic::~DDVCSConvolCoeffFunctionKinematic() {
}

void DDVCSConvolCoeffFunctionKinematic::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    ConvolCoeffFunctionKinematic::configure(parameters);

    double value;
    PhysicalUnit::Type unit;

    //Q2
    if (parameters.isAvailable(
            DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setQ2(value, unit);
        } else {
            setQ2(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(
                DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2);
    }

    //Q2Prim
    if (parameters.isAvailable(
            DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2PRIM)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2PRIM_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setQ2Prim(value, unit);
        } else {
            setQ2Prim(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(
                DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2PRIM);
    }

}

std::string DDVCSConvolCoeffFunctionKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << ConvolCoeffFunctionKinematic::toString();

    if (m_Q2.isInitialized())
        formatter << "Q2: " << m_Q2.toString() << ' ';
    if (m_Q2Prim.isInitialized())
        formatter << "Q2Prim: " << m_Q2Prim.toString() << ' ';

    return formatter.str();
}

void DDVCSConvolCoeffFunctionKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_xi.toStdString()
                            << m_t.toStdString() << m_Q2.toStdString()
                            << m_Q2Prim.toStdString() << m_MuF2.toStdString()
                            << m_MuR2.toStdString()));
}

void DDVCSConvolCoeffFunctionKinematic::serialize(
        ElemUtils::Packet& packet) const {

    ConvolCoeffFunctionKinematic::serialize(packet);

    packet << m_Q2;
    packet << m_Q2Prim;

}

void DDVCSConvolCoeffFunctionKinematic::unserialize(ElemUtils::Packet& packet) {

    ConvolCoeffFunctionKinematic::unserialize(packet);

    packet >> m_Q2;
    packet >> m_Q2Prim;

    updateHashSum();
}

void DDVCSConvolCoeffFunctionKinematic::serializeIntoStdVector(
        std::vector<double>& vec) const {

    ConvolCoeffFunctionKinematic::serializeIntoStdVector(vec);

    m_Q2.serializeIntoStdVector(vec);
    m_Q2Prim.serializeIntoStdVector(vec);

}

void DDVCSConvolCoeffFunctionKinematic::unserializeFromStdVector(
        std::vector<double>::const_iterator& it,
        const std::vector<double>::const_iterator& end) {

    ConvolCoeffFunctionKinematic::unserializeFromStdVector(it, end);

    m_Q2.unserializeFromStdVector(it, end);
    m_Q2Prim.unserializeFromStdVector(it, end);

    updateHashSum();
}

bool DDVCSConvolCoeffFunctionKinematic::operator ==(
        const DDVCSConvolCoeffFunctionKinematic& other) const {
    return m_xi == other.getXi() && m_t == other.getT()
            && m_MuF2 == other.getMuF2() && m_MuR2 == other.getMuR2()
            && m_Q2 == other.getQ2() && m_Q2Prim == other.getQ2Prim();
}

bool DDVCSConvolCoeffFunctionKinematic::operator !=(
        const DDVCSConvolCoeffFunctionKinematic& other) const {
    return !((*this) == other);
}

const PhysicalType<double>& DDVCSConvolCoeffFunctionKinematic::getQ2() const {
    return m_Q2;
}

void DDVCSConvolCoeffFunctionKinematic::setQ2(const PhysicalType<double>& Q2) {

    m_Q2.checkIfSameUnitCategoryAs(Q2);
    m_Q2 = Q2;
    updateHashSum();
}

void DDVCSConvolCoeffFunctionKinematic::setQ2(double Q2,
        PhysicalUnit::Type unit) {
    setQ2(PhysicalType<double>(Q2, unit));
}

const PhysicalType<double>& DDVCSConvolCoeffFunctionKinematic::getQ2Prim() const {
    return m_Q2Prim;
}

void DDVCSConvolCoeffFunctionKinematic::setQ2Prim(
        const PhysicalType<double>& Q2Prim) {

    m_Q2Prim.checkIfSameUnitCategoryAs(Q2Prim);
    m_Q2Prim = Q2Prim;
    updateHashSum();
}

void DDVCSConvolCoeffFunctionKinematic::setQ2Prim(double Q2Prim,
        PhysicalUnit::Type unit) {
    setQ2Prim(PhysicalType<double>(Q2Prim, unit));
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        DDVCSConvolCoeffFunctionKinematic& kinematic) {

    kinematic.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        DDVCSConvolCoeffFunctionKinematic& kinematic) {

    kinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
