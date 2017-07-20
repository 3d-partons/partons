#include "../../../include/partons/modules/DoubleDistributionModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <utility>

#include "../../../include/partons/beans/double_distribution/DoubleDistributionResult.h"
#include "../../../include/partons/modules/overlap/IncompleteGPDModule.h"
#include "../../../include/partons/modules/radon_inverse/RadonInverseModule.h"

DoubleDistributionModule::DoubleDistributionModule(const std::string& className) :
        ModuleObject(className), m_beta(0.), m_alpha(0.), m_t(0.), m_MuF2(0.), m_MuR2(
                0.), m_doubleDistributionType(
                DoubleDistributionType::UNDEFINED), m_inversionDependent(false), m_pRadonInverse(
                0), m_pIncompleteGPD(0) {

    m_listOfAvailableDoubleDistributionFunctions.insert(
            std::make_pair(DoubleDistributionType::F,
                    &DoubleDistributionModule::computeF));
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
    m_inversionDependent = other.m_inversionDependent;

    if (other.m_pRadonInverse != 0) {
        m_pRadonInverse = (other.m_pRadonInverse)->clone();
    } else {
        m_pRadonInverse = 0;
    }

    if (other.m_pIncompleteGPD != 0) {
        m_pIncompleteGPD = (other.m_pIncompleteGPD)->clone();
    } else {
        m_pIncompleteGPD = 0;
    }
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
        for (m_it = m_listOfAvailableDoubleDistributionFunctions.begin();
                m_it != m_listOfAvailableDoubleDistributionFunctions.end();
                m_it++) {
            PartonDistribution partonDistribution = ((*this).*(m_it->second))();

            result.addPartonDistribution(m_it->first, partonDistribution);
        }
        break;
    }
    default: {
        m_it = m_listOfAvailableDoubleDistributionFunctions.find(
                m_doubleDistributionType);
        if (m_it != m_listOfAvailableDoubleDistributionFunctions.end()) {
            PartonDistribution partonDistribution = ((*this).*(m_it->second))();

            result.addPartonDistribution(m_it->first, partonDistribution);
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
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
    throw ElemUtils::CustomException(getClassName(), __func__,
            ElemUtils::Formatter()
                    << "Cannot run this function from SUperClass, you must define it in a ChildClass");
}

PartonDistribution DoubleDistributionModule::computeG() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            ElemUtils::Formatter()
                    << "Cannot run this function from SUperClass, you must define it in a ChildClass");
}

PartonDistribution DoubleDistributionModule::computeK() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            ElemUtils::Formatter()
                    << "Cannot run this function from SUperClass, you must define it in a ChildClass");
}

bool DoubleDistributionModule::isInversionDependent() const {
    return m_inversionDependent;
}

void DoubleDistributionModule::setInversionDependent(bool inversionDependent) {
    m_inversionDependent = inversionDependent;
}

double DoubleDistributionModule::getAlpha() const {
    return m_alpha;
}

void DoubleDistributionModule::setAlpha(double alpha) {
    m_alpha = alpha;
}

double DoubleDistributionModule::getBeta() const {
    return m_beta;
}

void DoubleDistributionModule::setBeta(double beta) {
    m_beta = beta;
}

double DoubleDistributionModule::getMuF2() const {
    return m_MuF2;
}

void DoubleDistributionModule::setMuF2(double muF2) {
    m_MuF2 = muF2;
}

double DoubleDistributionModule::getMuR2() const {
    return m_MuR2;
}

void DoubleDistributionModule::setMuR2(double muR2) {
    m_MuR2 = muR2;
}

double DoubleDistributionModule::getT() const {
    return m_t;
}

void DoubleDistributionModule::setT(double t) {
    m_t = t;
}

List<DoubleDistributionType> DoubleDistributionModule::getListOfAvailableDDTypeForComputation() const {
    std::map<DoubleDistributionType::Type,
            PartonDistribution (DoubleDistributionModule::*)()>::const_iterator it;
    List<DoubleDistributionType> listOfAvailableDDTypeForComputation;

    for (it = m_listOfAvailableDoubleDistributionFunctions.begin();
            it != m_listOfAvailableDoubleDistributionFunctions.end(); it++) {
        listOfAvailableDDTypeForComputation.add(it->first);
    }

    return listOfAvailableDDTypeForComputation;
}

RadonInverseModule* DoubleDistributionModule::getRadonInverseModule() const {
    return m_pRadonInverse;
}

void DoubleDistributionModule::setRadonInverseModule(
        RadonInverseModule* pRadonInverse) {
    m_pRadonInverse = pRadonInverse;
}

IncompleteGPDModule* DoubleDistributionModule::getIncompleteGPDModule() const {
    return m_pIncompleteGPD;
}

void DoubleDistributionModule::setIncompleteGPDModule(
        IncompleteGPDModule* pIncompleteGPD) {
    m_pIncompleteGPD = pIncompleteGPD;
}

void DoubleDistributionModule::configure(const ElemUtils::Parameters &parameters){
	ModuleObject::configure(parameters);
}

void DoubleDistributionModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    ModuleObject::prepareSubModules(subModulesData);
}
