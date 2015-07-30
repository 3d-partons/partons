#include "DoubleDistributionModule.h"

#include <utility>

#include "../beans/double_distribution/DoubleDistributionResult.h"
#include "../utils/stringUtils/Formatter.h"

DoubleDistributionModule::DoubleDistributionModule(const std::string& className) :
        ModuleObject(className), m_beta(0.), m_alpha(0.), m_t(0.), m_MuF2(0.), m_MuR2(
                0.), m_doubleDistributionType(DoubleDistributionType::UNDEFINED) {
}

DoubleDistributionModule::~DoubleDistributionModule() {
}

DoubleDistributionModule::DoubleDistributionModule(
        const DoubleDistributionModule& other) :
        ModuleObject(other) {
    m_beta = other.m_beta;
    m_alpha = other.m_alpha;
    m_t = other.m_t;
    m_MuF2 = other.m_MuF2;
    m_MuR2 = other.m_MuR2;
    m_doubleDistributionType = other.m_doubleDistributionType;
}

void DoubleDistributionModule::preCompute(double beta, double alpha, double t,
        double MuF2, double MuR2,
        DoubleDistributionType::Type doubleDistributionType) {
    m_beta = beta;
    m_alpha = alpha;
    m_t = t;
    m_MuF2 = MuF2;
    m_MuR2 = MuR2;
    m_doubleDistributionType = doubleDistributionType;

    // execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();
}

DoubleDistributionResult DoubleDistributionModule::compute(double beta,
        double alpha, double t, double MuF2, double MuR2,
        DoubleDistributionType::Type doubleDistributionType) {

    preCompute(beta, alpha, t, MuF2, MuR2, doubleDistributionType);

    DoubleDistributionResult result;
    switch (m_doubleDistributionType) {
    case DoubleDistributionType::ALL: {
        for (m_it = m_listOfAvailableDualDistributionFunctions.begin();
                m_it != m_listOfAvailableDualDistributionFunctions.end();
                m_it++) {
            PartonDistribution partonDistribution = ((*this).*(m_it->second))();

            result.addPartonDistribution(m_it->first, partonDistribution);
        }
        break;
    }
    default: {
        m_it = m_listOfAvailableDualDistributionFunctions.find(
                m_doubleDistributionType);
        if (m_it != m_listOfAvailableDualDistributionFunctions.end()) {
            PartonDistribution partonDistribution = ((*this).*(m_it->second))();

            result.addPartonDistribution(m_it->first, partonDistribution);
        } else {
            throwException(__func__,
                    Formatter()
                            << DoubleDistributionType(m_doubleDistributionType).toString()
                            << " is not available for this model");
        }
        break;
    }
    }

    return result;
}

void DoubleDistributionModule::initModule() {
    // Nothing to do
}

//TODO add region check for each kinematic variables
void DoubleDistributionModule::isModuleWellConfigured() {
}

PartonDistribution DoubleDistributionModule::computeF() {
    throwException(__func__,
            Formatter()
                    << "Cannot run this function from SUperClass, you must define it in a ChildClass");
}

PartonDistribution DoubleDistributionModule::computeG() {
    throwException(__func__,
            Formatter()
                    << "Cannot run this function from SUperClass, you must define it in a ChildClass");
}

PartonDistribution DoubleDistributionModule::computeK() {
    throwException(__func__,
            Formatter()
                    << "Cannot run this function from SUperClass, you must define it in a ChildClass");
}
