#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"

#include <utility>

#include "../../../../../include/partons/utils/stringUtils/Formatter.h"

DVCSConvolCoeffFunctionResult::DVCSConvolCoeffFunctionResult() :
        Result("DVCSConvolCoeffFunctionResult") {

}

DVCSConvolCoeffFunctionResult::DVCSConvolCoeffFunctionResult(
        DVCSConvolCoeffFunctionKinematic kinematic) :
        Result("DVCSConvolCoeffFunctionResult"), m_kinematic(kinematic) {
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
    formatter << "[" << getClassName() << "] computed by "
            << getComputationModuleName() << '\n';
    formatter << "kinematic(" << m_kinematic.toString() << ") \n";

    for (m_it = m_resultsByGPDType.begin(); m_it != m_resultsByGPDType.end();
            m_it++) {
        formatter << " CFF_" << GPDType(m_it->first).toString() << " = "
                << (m_it->second).real() << " ; " << (m_it->second).imag()
                << "\n";
    }

    return formatter.str();
}

const DVCSConvolCoeffFunctionKinematic& DVCSConvolCoeffFunctionResult::getKinematic() const {
    return m_kinematic;
}

void DVCSConvolCoeffFunctionResult::setKinematic(
        const DVCSConvolCoeffFunctionKinematic& kinematic) {
    m_kinematic = kinematic;
}

bool DVCSConvolCoeffFunctionResult::isAvailable(GPDType gpdType) {
    m_it = m_resultsByGPDType.find(gpdType);

    return (m_it != m_resultsByGPDType.end()) ? true : false;
}

std::complex<double> DVCSConvolCoeffFunctionResult::getLastAvailable() {
    return m_it->second;
}

const std::map<GPDType::Type, std::complex<double> >& DVCSConvolCoeffFunctionResult::getResultsByGpdType() const {
    return m_resultsByGPDType;
}

void DVCSConvolCoeffFunctionResult::setResultsByGpdType(
        const std::map<GPDType::Type, std::complex<double> >& resultsByGpdType) {
    m_resultsByGPDType = resultsByGpdType;
}
