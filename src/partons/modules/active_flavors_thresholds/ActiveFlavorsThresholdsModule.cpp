#include "../../../../include/partons/modules/active_flavors_thresholds/ActiveFlavorsThresholdsModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <algorithm>
#include <iterator>

#include "../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

const std::string ActiveFlavorsThresholdsModule::ACTIVE_FLAVORS_THRESHOLDS_MODULE_CLASS_NAME =
        "ActiveFlavorsThresholdsModule";

ActiveFlavorsThresholdsModule::ActiveFlavorsThresholdsModule(
        const std::string &className) :
        ModuleObject(className, ChannelType::UNDEFINED) {
}

ActiveFlavorsThresholdsModule::~ActiveFlavorsThresholdsModule() {
}

ActiveFlavorsThresholdsModule::ActiveFlavorsThresholdsModule(
        const ActiveFlavorsThresholdsModule& other) :
        ModuleObject(other) {
    m_nfFunctionOfMu2 = other.m_nfFunctionOfMu2;
}

void ActiveFlavorsThresholdsModule::configure(
        const ElemUtils::Parameters &parameters) {
    ModuleObject::configure(parameters);
}

void ActiveFlavorsThresholdsModule::resolveObjectDependencies() {
}

std::string ActiveFlavorsThresholdsModule::toString() const {

    ElemUtils::Formatter formater;

    for (unsigned int i = 0; i != m_nfFunctionOfMu2.size(); i++)
        formater << m_nfFunctionOfMu2[i].toString();

    return formater.str();
}

void ActiveFlavorsThresholdsModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    ModuleObject::prepareSubModules(subModulesData);
}

void ActiveFlavorsThresholdsModule::isModuleWellConfigured() {
    //not run (used)
}

void ActiveFlavorsThresholdsModule::initModule() {
    //not run (used)
}

ActiveFlavorsThresholds ActiveFlavorsThresholdsModule::getNfInterval(
        double Mu2) const {

    //check Mu2
    if (Mu2 <= 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Illegal value of Mu2: " << Mu2);
    }

    //check if any values defined
    if (m_nfFunctionOfMu2.empty()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "there is no nfInterval defined");
    }

    //result
    ActiveFlavorsThresholds result;

    // case Mu2 < first interval
    if (Mu2 < (m_nfFunctionOfMu2.begin())->getLowerBound()) {

        result = *(m_nfFunctionOfMu2.begin());
        result.setLowerBound(Mu2);

        warn(__func__,
                ElemUtils::Formatter() << "Mu2 " << Mu2
                        << " smaller than the first interval. The first interval is returned with the modified lower bound equal "
                        << Mu2);
    }
    // case Mu2 >= last interval
    else if (Mu2 >= (m_nfFunctionOfMu2.rbegin())->getUpperBound()) {

        result = *(m_nfFunctionOfMu2.rbegin());
        result.setUpperBound(Mu2);

        warn(__func__,
                ElemUtils::Formatter() << "Mu2 " << Mu2
                        << " grater than the last interval. The last interval is returned with the modified upper bound equal "
                        << Mu2);
    }
    // case first interval <= Mu2 < last threshold
    else {

        //iterator
        std::vector<ActiveFlavorsThresholds>::const_iterator it;

        //loop
        for (it = m_nfFunctionOfMu2.begin(); it != m_nfFunctionOfMu2.end();
                it++) {
            if (Mu2 >= it->getLowerBound() && Mu2 < it->getUpperBound()) {
                result = *it;
                break;
            }
        }
    }

    return result;
}

std::vector<ActiveFlavorsThresholds> ActiveFlavorsThresholdsModule::getNfIntervals(
        double Mu2Min, double Mu2Max) const {

    //check Mu2
    if (Mu2Min <= 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Illegal value of Mu2Min: "
                        << Mu2Min);
    }

    if (Mu2Max <= 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Illegal value of Mu2Max: "
                        << Mu2Max);
    }

    //check if any values defined
    if (m_nfFunctionOfMu2.empty()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "there is no nfInterval defined");
    }

    //result
    std::vector<ActiveFlavorsThresholds> results;

    // case Mu2Min > Mu2Max
    if (Mu2Min > Mu2Max) {

        //warning
        warn(__func__,
                "Mu2Min > Mu2Max, return one interval with nf corresponding to Mu2Min and limits [Mu2Max:Mu2Min)");

        //get interval for MuF2Min
        ActiveFlavorsThresholds nfInterval = getNfInterval(Mu2Min);

        // delete thresholds by overriding lowerBound & upperBound
        // and swap lowerBound & upperBound value to perform backward evolution later in the code with math integration
        nfInterval.setLowerBound(Mu2Max);
        nfInterval.setUpperBound(Mu2Min);

        //add
        results.push_back(nfInterval);
    }
    // case Mu2Min < Mu2Max
    else {

        //get intervals for Mu2Min and Mu2Max
        ActiveFlavorsThresholds nfMinInterval = getNfInterval(Mu2Min);
        ActiveFlavorsThresholds nfMaxInterval = getNfInterval(Mu2Max);

        // retrieve intermediate intervals between nfMinInterval and nfMaxInterval
        for (unsigned int i = nfMinInterval.getIndex();
                i != nfMaxInterval.getIndex() + 1; i++) {
            results.push_back(m_nfFunctionOfMu2.at(i));
        }
    }

    return results;
}

void ActiveFlavorsThresholdsModule::addNfInterval(unsigned short nfValue,
        double lowerBound, double upperBound) {

    //add
    m_nfFunctionOfMu2.push_back(
            ActiveFlavorsThresholds(nfValue, lowerBound, upperBound,
                    m_nfFunctionOfMu2.size()));

    //sort
    std::sort(m_nfFunctionOfMu2.begin(), m_nfFunctionOfMu2.end());

    //check
    checkCurveIntegrity();
}

void ActiveFlavorsThresholdsModule::reset() {
    m_nfFunctionOfMu2.clear();
}

void ActiveFlavorsThresholdsModule::checkCurveIntegrity() {

    // first vector must have at least one entry
    if (m_nfFunctionOfMu2.empty()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "there is no nfInterval defined");
    }

    unsigned short previousNf = 0;

    for (unsigned short i = 0; i != m_nfFunctionOfMu2.size(); i++) {

        // check NF between 1 and 6
        if ((m_nfFunctionOfMu2[i].getNf() < 1)
                || (m_nfFunctionOfMu2[i].getNf() > 6)) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "nf out of range ; must be  0 < nf < 7");
        }

        //check if nf is contiguous step by 1
        if (i == 0) {
            previousNf = m_nfFunctionOfMu2[i].getNf();
        } else {
            if (m_nfFunctionOfMu2[i].getNf() != (previousNf + 1)) {
                throw ElemUtils::CustomException(getClassName(), __func__,
                        "nf not contiguous step by 1");
            }
            previousNf = m_nfFunctionOfMu2[i].getNf();
        }
    }
}

} /* namespace PARTONS */
