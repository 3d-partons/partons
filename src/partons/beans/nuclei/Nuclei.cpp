/*
 * Nuclei.cpp
 *
 *  Created on: Mar 11, 2020
 *      Author: partons
 */

#include "../../../../include/partons/beans/nuclei/Nuclei.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <vector>

#include "../../../../include/partons/Partons.h"

namespace PARTONS {

const std::string Nuclei::PARAMETER_NAME_SET_AZ = "AZ";

Nuclei::Nuclei(const std::string& className) :
        m_className(className), m_A(1), m_Z(1) {
    isWellConfigured();
}

Nuclei::Nuclei(const std::string& className, size_t A, size_t Z) :
        m_className(className), m_A(A), m_Z(Z) {
    isWellConfigured();
}

Nuclei::Nuclei(const Nuclei &other) :
        m_className(other.m_className), m_A(other.m_A), m_Z(other.m_Z) {
}

Nuclei::~Nuclei() {
}

void Nuclei::configure(const ElemUtils::Parameters& parameters) {

    // several CFFs
    if (parameters.isAvailable(Nuclei::PARAMETER_NAME_SET_AZ)) {

        // to string
        std::string temp_str = parameters.getLastAvailable().getString();

        // split string
        std::vector<std::string> CFFValues = ElemUtils::StringUtils::split(
                temp_str, '|');

        if (CFFValues.size() != 2) {
            throw ElemUtils::CustomException(m_className, __func__,
                    ElemUtils::Formatter() << "Parameter "
                            << Nuclei::PARAMETER_NAME_SET_AZ
                            << " should provide values of the form 'A|Z'");
        }

        m_A = ElemUtils::GenericType(CFFValues.at(0)).toUInt();
        m_Z = ElemUtils::GenericType(CFFValues.at(1)).toUInt();

        Partons::getInstance()->getLoggerManager()->info(m_className, __func__,
                ElemUtils::Formatter() << "A/Z numbers set to " << m_A << "/"
                        << m_Z);

        isWellConfigured();
    }

}

void Nuclei::isWellConfigured() const {

    if (m_Z > m_A) {
        throw ElemUtils::CustomException(m_className, __func__,
                ElemUtils::Formatter()
                        << "Atomic number higher than mass number, Z = " << m_Z
                        << " A = " << m_A);
    }
}

void Nuclei::setAZ(size_t A, size_t Z) {

    m_A = A;
    m_Z = Z;

    isWellConfigured();
}

size_t Nuclei::getA() const {
    return m_A;
}

size_t Nuclei::getZ() const {
    return m_Z;
}

size_t Nuclei::getN() const {
    return m_A - m_Z;
}

} /* namespace PARTONS */
