#include "../../../../../include/partons/beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

const std::string TCSConvolCoeffFunctionKinematic::TCS_CONVOL_COEFF_FUNCTION_KNEMATIC_CLASS_NAME =
        "TCSConvolCoeffFunctionKinematic";

TCSConvolCoeffFunctionKinematic::TCSConvolCoeffFunctionKinematic() :
        ConvolCoeffFunctionKinematic("TCSConvolCoeffFunctionKinematic",
                ChannelType::TCS), m_Q2Prim(
                PhysicalType<double>(PhysicalUnit::GEV2)) {
}

TCSConvolCoeffFunctionKinematic::TCSConvolCoeffFunctionKinematic(double xi,
        double t, double Q2Prim, double MuF2, double MuR2) :
        ConvolCoeffFunctionKinematic("TCSConvolCoeffFunctionKinematic",
                ChannelType::TCS, xi, t, MuF2, MuR2), m_Q2Prim(
                PhysicalType<double>(Q2Prim, PhysicalUnit::GEV2)) {
}

TCSConvolCoeffFunctionKinematic::TCSConvolCoeffFunctionKinematic(
        const PhysicalType<double> &xi, const PhysicalType<double> &t,
        const PhysicalType<double> &Q2Prim, const PhysicalType<double> &MuF2,
        const PhysicalType<double> &MuR2) :
        ConvolCoeffFunctionKinematic("TCSConvolCoeffFunctionKinematic",
                ChannelType::TCS, xi, t, MuF2, MuR2), m_Q2Prim(
                PhysicalType<double>(PhysicalUnit::GEV2)) {

    m_Q2Prim.checkIfSameUnitCategoryAs(Q2Prim);

    m_Q2Prim = Q2Prim;
}

TCSConvolCoeffFunctionKinematic::TCSConvolCoeffFunctionKinematic(
        const ElemUtils::GenericType &xi, const ElemUtils::GenericType &t,
        const ElemUtils::GenericType &Q2Prim,
        const ElemUtils::GenericType &MuF2, const ElemUtils::GenericType &MuR2) :
        ConvolCoeffFunctionKinematic("TCSConvolCoeffFunctionKinematic",
                ChannelType::TCS, xi, t, MuF2, MuR2), m_Q2Prim(
                PhysicalType<double>(Q2Prim, PhysicalUnit::GEV2)) {
}

TCSConvolCoeffFunctionKinematic::TCSConvolCoeffFunctionKinematic(
        const TCSConvolCoeffFunctionKinematic &other) :
        ConvolCoeffFunctionKinematic(other), m_Q2Prim(other.m_Q2Prim) {
}

TCSConvolCoeffFunctionKinematic::~TCSConvolCoeffFunctionKinematic() {
}

void TCSConvolCoeffFunctionKinematic::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    ConvolCoeffFunctionKinematic::configure(parameters);

    double value;
    PhysicalUnit::Type unit;

    //Q2'
    if (parameters.isAvailable(
            TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2PRIM)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2PRIM_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setQ2Prim(value, unit);
        } else {
            setQ2Prim(value);
        }
    } else {
        errorMissingParameter(
                TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2PRIM);
    }
}

std::string TCSConvolCoeffFunctionKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << ConvolCoeffFunctionKinematic::toString();

    if (m_Q2Prim.isInitialized())
        formatter << "Q2': " << m_Q2Prim.toString() << ' ';

    return formatter.str();
}

void TCSConvolCoeffFunctionKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_xi.getValue() << m_t.getValue()
                            << m_Q2Prim.getValue() << m_MuF2.getValue()
                            << m_MuR2.getValue()));
}

void TCSConvolCoeffFunctionKinematic::serialize(
        ElemUtils::Packet& packet) const {

    ConvolCoeffFunctionKinematic::serialize(packet);

    packet << m_Q2Prim;
}

void TCSConvolCoeffFunctionKinematic::unserialize(ElemUtils::Packet& packet) {

    ConvolCoeffFunctionKinematic::unserialize(packet);

    packet >> m_Q2Prim;

    updateHashSum();
}

bool TCSConvolCoeffFunctionKinematic::operator ==(
        const TCSConvolCoeffFunctionKinematic& other) const {
    return m_xi == other.getXi() && m_t == other.getT()
            && m_MuF2 == other.getMuF2() && m_MuR2 == other.getMuR2()
            && m_Q2Prim == other.getQ2Prim();
}

bool TCSConvolCoeffFunctionKinematic::operator !=(
        const TCSConvolCoeffFunctionKinematic& other) const {
    return !((*this) == other);
}

const PhysicalType<double>& TCSConvolCoeffFunctionKinematic::getQ2Prim() const {
    return m_Q2Prim;
}

void TCSConvolCoeffFunctionKinematic::setQ2Prim(
        const PhysicalType<double>& Q2Prim) {

    m_Q2Prim.checkIfSameUnitCategoryAs(Q2Prim);
    m_Q2Prim = Q2Prim;
    updateHashSum();
}

void TCSConvolCoeffFunctionKinematic::setQ2Prim(double Q2Prim,
        PhysicalUnit::Type unit) {
    setQ2Prim(PhysicalType<double>(Q2Prim, unit));
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        TCSConvolCoeffFunctionKinematic& kinematic) {

    kinematic.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        TCSConvolCoeffFunctionKinematic& kinematic) {

    kinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
