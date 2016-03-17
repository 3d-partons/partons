#include "../../../include/partons/modules/ActiveFlavorsModule.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <algorithm>

ActiveFlavorsModule::ActiveFlavorsModule(const std::string &className) :
        ModuleObject(className) {
}

ActiveFlavorsModule::~ActiveFlavorsModule() {

}

ActiveFlavorsModule::ActiveFlavorsModule(const ActiveFlavorsModule& other) :
        ModuleObject(other) {
    m_nfFunctionOfMu = other.m_nfFunctionOfMu;
}

void ActiveFlavorsModule::init() {
    // sort vector of intervals by nf value
    std::sort(m_nfFunctionOfMu.begin(), m_nfFunctionOfMu.end());

    checkCurveIntegrity();
}

void ActiveFlavorsModule::addNfInterval(unsigned short nfValue,
        double lowerBound, double upperBound) {
    m_nfFunctionOfMu.push_back(
            NfInterval(nfValue, lowerBound, upperBound,
                    m_nfFunctionOfMu.size()));
}

NfInterval ActiveFlavorsModule::getNfInterval(double Mu) {
    NfInterval result;

    // case Mu <= 0
    if (Mu <= m_nfFunctionOfMu[0].getLowerBound()) {
        result = m_nfFunctionOfMu[0];
    }
    // case Mu > last threshold
    else if (Mu
            >= m_nfFunctionOfMu[m_nfFunctionOfMu.size() - 1].getUpperBound()) {
        result = m_nfFunctionOfMu[m_nfFunctionOfMu.size() - 1];
        result.setUpperBound(Mu);
    }
    // case 0 < Mu < last threshold
    else {
        bool thresholdReach = false;

        for (unsigned int i = 0;
                i != m_nfFunctionOfMu.size() && !thresholdReach; i++) {
            if (Mu < m_nfFunctionOfMu[i].getUpperBound()
                    && Mu >= m_nfFunctionOfMu[i].getLowerBound()) {
                result = m_nfFunctionOfMu[i];
                thresholdReach = true;
            }
        }
    }

    return result;
}

std::vector<NfInterval> ActiveFlavorsModule::getNfIntervals(double MuMin,
        double MuMax) {

    std::vector<NfInterval> results;

    // case MuMin > MuMax
    if (MuMin > MuMax) {
        warn(__func__,
                "MuMin > MuMax : threshold suppressed by overriding lower bound and upper bound ; nf will be constant");

        NfInterval nfInterval = getNfInterval(MuMin);
        // delete thresholds by overriding lowerBound & upperBound
        // and swap lowerBound & upperBound value to perform backward evolution later in the code with math integration
        nfInterval.setLowerBound(MuMax);
        nfInterval.setUpperBound(MuMin);
        results.push_back(nfInterval);
    }
    // case MuMin < MuMax
    else {
        NfInterval nfMinInterval = getNfInterval(MuMin);
        NfInterval nfMaxInterval = getNfInterval(MuMax);

        // retrieve intermediate intervals between nfMinInterval and nfMaxInterval
        for (unsigned int i = nfMinInterval.getIndex();
                i != nfMaxInterval.getIndex() + 1; i++) {
            results.push_back(m_nfFunctionOfMu[i]);
        }
    }

    return results;
}

void ActiveFlavorsModule::initModule() {
}

void ActiveFlavorsModule::isModuleWellConfigured() {
}

void ActiveFlavorsModule::checkCurveIntegrity() {
    // first vector must have at least one entry
    if (m_nfFunctionOfMu.empty()) {
        error(__func__, "there is no nfInterval defined");
    }

    double previousNf = 0;

    for (unsigned short i = 0; i != m_nfFunctionOfMu.size(); i++) {
        // check NF between 1 and 6
        if ((m_nfFunctionOfMu[i].getNf() < 1)
                || (m_nfFunctionOfMu[i].getNf() > 6)) {
            error(__func__, "nf out of range ; must be  0 < nf < 7");
        }

        //check if nf is contigus step by 1
        if (i == 0) {
            previousNf = m_nfFunctionOfMu[i].getNf();
        } else {
            if (m_nfFunctionOfMu[i].getNf() != (previousNf + 1)) {
                error(__func__, "nf not contigus step by 1");
            }
            previousNf = m_nfFunctionOfMu[i].getNf();
        }
    }

}

std::string ActiveFlavorsModule::toString() {
    ElemUtils::Formatter formater;

    for (unsigned int i = 0; i != m_nfFunctionOfMu.size(); i++)
        formater << m_nfFunctionOfMu[i].toString();
    return formater.str();
}
