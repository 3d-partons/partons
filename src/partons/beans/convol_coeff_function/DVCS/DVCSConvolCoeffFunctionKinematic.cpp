#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"

#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/thread/Packet.h>

#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/beans/observable/ObservableKinematic.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic() :
        Kinematic("DVCSConvolCoeffFunctionKinematic"), m_binId(0), m_xi(0.), m_t(
                0.), m_Q2(0.), m_MuF2(0.), m_MuR2(0.) {
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(
        const ElemUtils::Parameters &parameters) :
        Kinematic("DVCSConvolCoeffFunctionKinematic"), m_binId(0), m_xi(0.), m_t(
                0.), m_Q2(0.), m_MuF2(0.), m_MuR2(0.) {
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
    if (parameters.isAvailable(ObservableKinematic::PARAMETER_NAME_Q2)) {
        m_Q2 = parameters.getLastAvailable().toDouble();
    } else {
        error(__func__,
                ElemUtils::Formatter() << "Missing parameter <"
                        << ObservableKinematic::PARAMETER_NAME_Q2 << ">");
    }

    //TODO remove from kinematic
    m_MuF2 =
            parameters.get(GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2).toDouble();
    m_MuR2 =
            parameters.get(GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUR2).toDouble();
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(double xi,
        double t, double Q2, double MuF2, double MuR2) :
        Kinematic("DVCSConvolCoeffFunctionKinematic"), m_binId(0), m_xi(xi), m_t(
                t), m_Q2(Q2), m_MuF2(MuF2), m_MuR2(MuR2) {
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(
        unsigned int binId, double xi, double t, double Q2, double MuF2,
        double MuR2) :
        Kinematic("DVCSConvolCoeffFunctionKinematic"), m_binId(binId), m_xi(xi), m_t(
                t), m_Q2(Q2), m_MuF2(MuF2), m_MuR2(MuR2) {
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(
        const ElemUtils::GenericType &xi, const ElemUtils::GenericType &t,
        const ElemUtils::GenericType &Q2, const ElemUtils::GenericType &MuF2,
        const ElemUtils::GenericType &MuR2) :
        Kinematic("DVCSConvolCoeffFunctionKinematic"), m_xi(xi.toDouble()), m_t(
                t.toDouble()), m_Q2(Q2.toDouble()), m_MuF2(MuF2.toDouble()), m_MuR2(
                MuR2.toDouble()) {

}

DVCSConvolCoeffFunctionKinematic::~DVCSConvolCoeffFunctionKinematic() {
}

std::string DVCSConvolCoeffFunctionKinematic::toString() {
    return ElemUtils::Formatter() << Kinematic::toString() << " m_binId = "
            << m_binId << " m_xi = " << m_xi << " m_t = " << m_t << " m_Q2 = "
            << m_Q2 << " m_MuF2 = " << m_MuF2 << " m_MuR2 = " << m_MuR2;
}

unsigned int DVCSConvolCoeffFunctionKinematic::getBinId() const {
    return m_binId;
}

double DVCSConvolCoeffFunctionKinematic::getMuF2() const {
    return m_MuF2;
}

double DVCSConvolCoeffFunctionKinematic::getMuR2() const {
    return m_MuR2;
}

double DVCSConvolCoeffFunctionKinematic::getQ2() const {
    return m_Q2;
}

double DVCSConvolCoeffFunctionKinematic::getT() const {
    return m_t;
}

double DVCSConvolCoeffFunctionKinematic::getXi() const {
    return m_xi;
}

void DVCSConvolCoeffFunctionKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_xi << m_t << m_Q2 << m_MuF2
                            << m_MuR2));
}

void DVCSConvolCoeffFunctionKinematic::serialize(
        ElemUtils::Packet& packet) const {
    Kinematic::serialize(packet);

    packet << m_xi << m_t << m_Q2 << m_MuF2 << m_MuR2;
}

void DVCSConvolCoeffFunctionKinematic::unserialize(ElemUtils::Packet& packet) {
    Kinematic::unserialize(packet);

    packet >> m_xi;
    packet >> m_t;
    packet >> m_Q2;
    packet >> m_MuF2;
    packet >> m_MuR2;
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

void DVCSConvolCoeffFunctionKinematic::setBinId(unsigned int binId) {
    m_binId = binId;
}

void DVCSConvolCoeffFunctionKinematic::setMuF2(double MuF2) {
    m_MuF2 = MuF2;
    updateHashSum();
}

void DVCSConvolCoeffFunctionKinematic::setMuR2(double MuR2) {
    m_MuR2 = MuR2;
    updateHashSum();
}

void DVCSConvolCoeffFunctionKinematic::setQ2(double Q2) {
    m_Q2 = Q2;
    updateHashSum();
}

void DVCSConvolCoeffFunctionKinematic::setT(double t) {
    m_t = t;
    updateHashSum();
}

void DVCSConvolCoeffFunctionKinematic::setXi(double xi) {
    m_xi = xi;
    updateHashSum();
}
