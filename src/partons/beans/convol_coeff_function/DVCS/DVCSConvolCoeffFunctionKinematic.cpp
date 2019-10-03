#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

const std::string DVCSConvolCoeffFunctionKinematic::DVCS_CONVOL_COEFF_FUNCTION_KNEMATIC_CLASS_NAME =
        "DVCSConvolCoeffFunctionKinematic";

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic() :
        ConvolCoeffFunctionKinematic("DVCSConvolCoeffFunctionKinematic",
                ChannelType::DVCS), m_Q2(
                PhysicalType<double>(PhysicalUnit::GEV2)) {
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(double xi,
        double t, double Q2, double MuF2, double MuR2) :
        ConvolCoeffFunctionKinematic("DVCSConvolCoeffFunctionKinematic",
                ChannelType::DVCS, xi, t, MuF2, MuR2), m_Q2(
                PhysicalType<double>(Q2, PhysicalUnit::GEV2)) {
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(
        const PhysicalType<double> &xi, const PhysicalType<double> &t,
        const PhysicalType<double> &Q2, const PhysicalType<double> &MuF2,
        const PhysicalType<double> &MuR2) :
        ConvolCoeffFunctionKinematic("DVCSConvolCoeffFunctionKinematic",
                ChannelType::DVCS, xi, t, MuF2, MuR2), m_Q2(
                PhysicalType<double>(PhysicalUnit::GEV2)) {

    m_Q2.checkIfSameUnitCategoryAs(Q2);

    m_Q2 = Q2;
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(
        const ElemUtils::GenericType &xi, const ElemUtils::GenericType &t,
        const ElemUtils::GenericType &Q2, const ElemUtils::GenericType &MuF2,
        const ElemUtils::GenericType &MuR2) :
        ConvolCoeffFunctionKinematic("DVCSConvolCoeffFunctionKinematic",
                ChannelType::DVCS, xi, t, MuF2, MuR2), m_Q2(
                PhysicalType<double>(Q2, PhysicalUnit::GEV2)) {
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(
        const DVCSConvolCoeffFunctionKinematic &other) :
        ConvolCoeffFunctionKinematic(other), m_Q2(other.m_Q2) {
}

DVCSConvolCoeffFunctionKinematic::~DVCSConvolCoeffFunctionKinematic() {
}

void DVCSConvolCoeffFunctionKinematic::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    ConvolCoeffFunctionKinematic::configure(parameters);

    double value;
    PhysicalUnit::Type unit;

    //Q2
    if (parameters.isAvailable(
            DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setQ2(value, unit);
        } else {
            setQ2(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(
                DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2);
    }
}

std::string DVCSConvolCoeffFunctionKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << ConvolCoeffFunctionKinematic::toString();

    if (m_Q2.isInitialized())
        formatter << "Q2: " << m_Q2.toString() << ' ';

    return formatter.str();
}

void DVCSConvolCoeffFunctionKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_xi.toStdString()
                            << m_t.toStdString() << m_Q2.toStdString()
                            << m_MuF2.toStdString() << m_MuR2.toStdString()));
}

void DVCSConvolCoeffFunctionKinematic::serialize(
        ElemUtils::Packet& packet) const {

    ConvolCoeffFunctionKinematic::serialize(packet);

    packet << m_Q2;
}

void DVCSConvolCoeffFunctionKinematic::unserialize(ElemUtils::Packet& packet) {

    ConvolCoeffFunctionKinematic::unserialize(packet);

    packet >> m_Q2;

    updateHashSum();
}

void DVCSConvolCoeffFunctionKinematic::serializeIntoStdVector(
        std::vector<double>& vec) const {

    ConvolCoeffFunctionKinematic::serializeIntoStdVector(vec);

    m_Q2.serializeIntoStdVector(vec);
}

void DVCSConvolCoeffFunctionKinematic::unserializeFromStdVector(
        std::vector<double>::const_iterator& it,
        const std::vector<double>::const_iterator& end) {

    ConvolCoeffFunctionKinematic::unserializeFromStdVector(it, end);

    m_Q2.unserializeFromStdVector(it, end);

    updateHashSum();
}

bool DVCSConvolCoeffFunctionKinematic::operator ==(
        const DVCSConvolCoeffFunctionKinematic& other) const {
    return m_xi == other.getXi() && m_t == other.getT()
            && m_MuF2 == other.getMuF2() && m_MuR2 == other.getMuR2()
            && m_Q2 == other.getQ2();
}

bool DVCSConvolCoeffFunctionKinematic::operator !=(
        const DVCSConvolCoeffFunctionKinematic& other) const {
    return !((*this) == other);
}

const PhysicalType<double>& DVCSConvolCoeffFunctionKinematic::getQ2() const {
    return m_Q2;
}

void DVCSConvolCoeffFunctionKinematic::setQ2(const PhysicalType<double>& Q2) {

    m_Q2.checkIfSameUnitCategoryAs(Q2);
    m_Q2 = Q2;
    updateHashSum();
}

void DVCSConvolCoeffFunctionKinematic::setQ2(double Q2,
        PhysicalUnit::Type unit) {
    setQ2(PhysicalType<double>(Q2, unit));
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        DVCSConvolCoeffFunctionKinematic& kinematic) {

    kinematic.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        DVCSConvolCoeffFunctionKinematic& kinematic) {

    kinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
