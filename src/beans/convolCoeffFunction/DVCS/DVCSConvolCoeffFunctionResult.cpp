#include "DVCSConvolCoeffFunctionResult.h"

#include <utility>

#include "../../../utils/stringUtils/Formatter.h"

DVCSConvolCoeffFunctionResult::DVCSConvolCoeffFunctionResult() :
        BaseObject("DVCSConvolCoeffFunctionResult") {

}

DVCSConvolCoeffFunctionResult::DVCSConvolCoeffFunctionResult(
        DVCSConvolCoeffFunctionKinematic kinematic) :
        BaseObject("DVCSConvolCoeffFunctionResult"), m_kinematic(kinematic) {
}

DVCSConvolCoeffFunctionResult::~DVCSConvolCoeffFunctionResult() {
}

void DVCSConvolCoeffFunctionResult::add(GPDType::Type gpdComputeType,
        std::complex<double> result) {
    m_resultsByGPDType.insert(std::make_pair(gpdComputeType, result));
}

std::complex<double> DVCSConvolCoeffFunctionResult::get(
        GPDType::Type gpdComputeType) {
    m_it = m_resultsByGPDType.find(gpdComputeType);
    if (m_it == m_resultsByGPDType.end()) {
        throwException(__func__,
                Formatter() << "Cannot find result for GPDComputeType = "
                        << GPDType(gpdComputeType).toString());
    }

    return m_it->second;
}

std::string DVCSConvolCoeffFunctionResult::toString() {
    Formatter formatter;
    formatter << "[" << getClassName() << "]" << "\n";
    formatter << m_kinematic.toString() << "\n";

    for (m_it = m_resultsByGPDType.begin(); m_it != m_resultsByGPDType.end();
            m_it++) {
        formatter << "CFF_" << GPDType(m_it->first).toString() << " = "
                << (m_it->second).real() << ", " << (m_it->second).imag()
                << "\n";
    }

    return formatter.str();
}
