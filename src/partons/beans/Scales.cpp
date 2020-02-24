#include "../../../include/partons/beans/Scales.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

Scales::Scales() :
        BaseObject("Scale"), m_MuF2(PhysicalType<double>(PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(PhysicalUnit::GEV2)) {
}

Scales::Scales(const ElemUtils::Parameters &parameters) :
        BaseObject("Scale"), m_MuF2(PhysicalType<double>(PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(PhysicalUnit::GEV2)) {

    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2)) {
        m_MuF2.setValue(parameters.getLastAvailable().toDouble());
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2);
    }

    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2)) {
        m_MuR2.setValue(parameters.getLastAvailable().toDouble());
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2);
    }
}

Scales::Scales(double MuF2, double MuR2) :
        BaseObject("Scale"), m_MuF2(
                PhysicalType<double>(MuF2, PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(MuR2, PhysicalUnit::GEV2)) {
}

Scales::Scales(const PhysicalType<double>& MuF2,
        const PhysicalType<double>& MuR2) :
        BaseObject("Scale"), m_MuF2(PhysicalType<double>(PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(PhysicalUnit::GEV2)) {

    if (PhysicalUnit(m_MuF2.getUnit()).getUnitCategory()
            != PhysicalUnit(MuF2.getUnit()).getUnitCategory()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Unit categories of two PhysicalType objects are different. First: "
                        << m_MuF2.toString() << " Second: " << MuF2.toString());
    }

    if (PhysicalUnit(m_MuR2.getUnit()).getUnitCategory()
            != PhysicalUnit(MuR2.getUnit()).getUnitCategory()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Unit categories of two PhysicalType objects are different. First: "
                        << m_MuR2.toString() << " Second: " << MuR2.toString());
    }

    m_MuF2 = MuF2;
    m_MuR2 = MuR2;
}

Scales::Scales(const ElemUtils::GenericType& MuF2,
        const ElemUtils::GenericType& MuR2) :
        BaseObject("Scale"), m_MuF2(
                PhysicalType<double>(MuF2, PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(MuR2, PhysicalUnit::GEV2)) {
}

Scales::Scales(const Scales &other) :
        BaseObject(other), m_MuF2(other.m_MuF2), m_MuR2(other.m_MuR2) {
}

Scales::~Scales() {
}

std::string Scales::toString() const {
    return ElemUtils::Formatter() << "m_MuF2 = " << m_MuF2.toString()
            << "  m_MuR2 = " << m_MuR2.toString();
}

const PhysicalType<double>& Scales::getMuF2() const {
    return m_MuF2;
}

const PhysicalType<double>& Scales::getMuR2() const {
    return m_MuR2;
}

void Scales::setMuF2(const PhysicalType<double>& muF2) {

    if (PhysicalUnit(m_MuF2.getUnit()).getUnitCategory()
            != PhysicalUnit(muF2.getUnit()).getUnitCategory()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Unit categories of two PhysicalType objects are different. First: "
                        << m_MuF2.toString() << " Second: " << muF2.toString());
    }

    m_MuF2 = muF2;
}

void Scales::setMuR2(const PhysicalType<double>& muR2) {

    if (PhysicalUnit(m_MuR2.getUnit()).getUnitCategory()
            != PhysicalUnit(muR2.getUnit()).getUnitCategory()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Unit categories of two PhysicalType objects are different. First: "
                        << m_MuR2.toString() << " Second: " << muR2.toString());
    }

    m_MuR2 = muR2;
}

void Scales::setMuF2(double muF2) {
    m_MuF2.setValue(muF2);
}

void Scales::setMuR2(double muR2) {
    m_MuR2.setValue(muR2);
}

} /* namespace PARTONS */
