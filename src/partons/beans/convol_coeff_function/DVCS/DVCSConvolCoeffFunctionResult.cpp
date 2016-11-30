#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <utility>

#include "../../../../../include/partons/utils/compare/CompareUtils.h"
#include "../../../../../include/partons/utils/compare/ComparisonData.h"

DVCSConvolCoeffFunctionResult::DVCSConvolCoeffFunctionResult() :
        Result("DVCSConvolCoeffFunctionResult"), m_channel(
                ObservableChannel::DVCS) {

}

DVCSConvolCoeffFunctionResult::DVCSConvolCoeffFunctionResult(
        DVCSConvolCoeffFunctionKinematic kinematic) :
        Result("DVCSConvolCoeffFunctionResult"), m_channel(
                ObservableChannel::DVCS), m_kinematic(kinematic) {
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
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Cannot find result for GPDComputeType = "
                        << GPDType(gpdComputeType).toString());
    }

    return m_it->second;
}

std::string DVCSConvolCoeffFunctionResult::toString() const {
    std::map<GPDType::Type, std::complex<double> >::const_iterator it;

    ElemUtils::Formatter formatter;
    formatter << "[" << getClassName() << "] computed by "
            << getComputationModuleName() << '\n';
    formatter << "kinematic(" << m_kinematic.toString() << ") \n";

    for (it = m_resultsByGPDType.begin(); it != m_resultsByGPDType.end();
            it++) {
        formatter << " CFF_" << GPDType(it->first).toString() << " = "
                << (it->second).real() << " ; " << (it->second).imag() << "\n";
    }

    return formatter.str();
}

std::vector<GPDType> DVCSConvolCoeffFunctionResult::listGPDTypeComputed() {
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

const DVCSConvolCoeffFunctionKinematic& DVCSConvolCoeffFunctionResult::getKinematic() const {
    return m_kinematic;
}

void DVCSConvolCoeffFunctionResult::setKinematic(
        const DVCSConvolCoeffFunctionKinematic& kinematic) {
    m_kinematic = kinematic;
}

bool DVCSConvolCoeffFunctionResult::isAvailable(GPDType::Type gpdType) {
    m_it = m_resultsByGPDType.find(gpdType);

    return (m_it != m_resultsByGPDType.end()) ? true : false;
}

std::complex<double> DVCSConvolCoeffFunctionResult::getLastAvailable() const {
    return m_it->second;
}

const std::map<GPDType::Type, std::complex<double> >& DVCSConvolCoeffFunctionResult::getResultsByGpdType() const {
    return m_resultsByGPDType;
}

void DVCSConvolCoeffFunctionResult::setResultsByGpdType(
        const std::map<GPDType::Type, std::complex<double> >& resultsByGpdType) {
    m_resultsByGPDType = resultsByGpdType;
}

void DVCSConvolCoeffFunctionResult::compare(
        ComparisonReport &rootComparisonReport,
        const DVCSConvolCoeffFunctionResult &referenceObject,
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

const std::complex<double>& DVCSConvolCoeffFunctionResult::getResult(
        GPDType::Type gpdType) const {
    std::map<GPDType::Type, std::complex<double> >::const_iterator it =
            m_resultsByGPDType.find(gpdType);

    if (it == m_resultsByGPDType.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Cannot find complex value from GPDType = "
                        << GPDType(gpdType).toString());
    }

    return (it->second);
}

const ObservableChannel& DVCSConvolCoeffFunctionResult::getChannel() const {
    return m_channel;
}

void DVCSConvolCoeffFunctionResult::setChannel(
        const ObservableChannel& channel) {
    m_channel = channel;
}

bool DVCSConvolCoeffFunctionResult::operator <(
        const DVCSConvolCoeffFunctionResult& other) const {
    return (m_kinematic < other.m_kinematic);
}
