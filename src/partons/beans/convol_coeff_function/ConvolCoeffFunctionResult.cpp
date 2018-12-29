#include "../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <utility>

#include "../../../../include/partons/utils/compare/CompareUtils.h"
#include "../../../../include/partons/utils/compare/ComparisonData.h"

namespace PARTONS {

ConvolCoeffFunctionResult::ConvolCoeffFunctionResult(
        const std::string& className, ChannelType::Type channelType) :
        Result(className, channelType) {
}

ConvolCoeffFunctionResult::ConvolCoeffFunctionResult(
        const ConvolCoeffFunctionResult& other) :
        Result(other) {

    m_resultsByGPDType = other.m_resultsByGPDType;
    m_it = other.m_it;
}

ConvolCoeffFunctionResult::~ConvolCoeffFunctionResult() {
}

std::string ConvolCoeffFunctionResult::toString() const {

    ElemUtils::Formatter formatter;

    formatter << Result::toString();

    std::map<GPDType::Type, std::complex<double> >::const_iterator it;

    for (it = m_resultsByGPDType.begin(); it != m_resultsByGPDType.end();
            it++) {
        formatter << "\tCFF_" << GPDType(it->first).toString() << ": Re: "
                << (it->second).real() << " Im: " << (it->second).imag()
                << '\n';
    }

    return formatter.str();
}

void ConvolCoeffFunctionResult::addResult(GPDType::Type gpdType,
        const std::complex<double>& result) {

    std::map<GPDType::Type, std::complex<double> >::const_iterator it =
            m_resultsByGPDType.find(gpdType);

    if (it != m_resultsByGPDType.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Result for GPDType = "
                        << GPDType(gpdType).toString() << " exists");
    }

    m_resultsByGPDType.insert(std::make_pair(gpdType, result));
}

const std::complex<double>& ConvolCoeffFunctionResult::getResult(
        GPDType::Type gpdType) const {

    std::map<GPDType::Type, std::complex<double> >::const_iterator it =
            m_resultsByGPDType.find(gpdType);

    if (it == m_resultsByGPDType.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Cannot find result for GPDType = "
                        << GPDType(gpdType).toString());
    }

    return (it->second);
}

bool ConvolCoeffFunctionResult::isAvailable(GPDType::Type gpdType) {
    m_it = m_resultsByGPDType.find(gpdType);

    return (m_it != m_resultsByGPDType.end()) ? true : false;
}

std::complex<double> ConvolCoeffFunctionResult::getLastAvailable() const {
    return m_it->second;
}

void ConvolCoeffFunctionResult::compare(ComparisonReport &rootComparisonReport,
        const ConvolCoeffFunctionResult &referenceObject,
        std::string parentObjectInfo) const {

    //TODO faire un test pour valider la cinématique associée

    //TODO tester la taille des listes avant de faire le test
    for (std::map<GPDType::Type, std::complex<double> >::const_iterator it =
            m_resultsByGPDType.begin(); it != m_resultsByGPDType.end(); it++) {

        ComparisonData comparisonData = CompareUtils::compareComplex("",
                it->second, referenceObject.getResult(it->first),
                rootComparisonReport.getTolerances(),
                ElemUtils::Formatter() << parentObjectInfo << " "
                        << GPDType(it->first).toString());
        rootComparisonReport.addComparisonData(comparisonData);
    }
}

std::vector<GPDType> ConvolCoeffFunctionResult::listGPDTypeComputed() {

    std::vector<GPDType> list;

    std::map<GPDType::Type, std::complex<double> >::const_iterator it;

    if (m_resultsByGPDType.size() != 0) {

        for (it = m_resultsByGPDType.begin(); it != m_resultsByGPDType.end();
                ++it) {
            list.push_back(it->first);
        }
    }

    return list;
}

const std::map<GPDType::Type, std::complex<double> >& ConvolCoeffFunctionResult::getResultsByGpdType() const {
    return m_resultsByGPDType;
}

void ConvolCoeffFunctionResult::setResultsByGpdType(
        const std::map<GPDType::Type, std::complex<double> >& resultsByGpdType) {
    m_resultsByGPDType = resultsByGpdType;
}

} /* namespace PARTONS */
