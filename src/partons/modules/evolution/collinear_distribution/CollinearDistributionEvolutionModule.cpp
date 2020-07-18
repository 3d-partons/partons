#include "../../../../../include/partons/modules/evolution/collinear_distribution/CollinearDistributionEvolutionModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <NumA/linear_algebra/vector/VectorD.h>
#include <utility>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/parton_distribution/CollinearDistributionKinematic.h"
#include "../../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../../include/partons/modules/active_flavors_thresholds/ActiveFlavorsThresholdsModule.h"
#include "../../../../../include/partons/modules/collinear_distribution/CollinearDistributionModule.h"
#include "../../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"

namespace PARTONS {

const std::string CollinearDistributionEvolutionModule::COLLINEAR_DISTRIBUTION_EVOLUTION_MODULE_CLASS_NAME =
        "CollinearDistributionEvolutionModule";

CollinearDistributionEvolutionModule::CollinearDistributionEvolutionModule(const std::string &className) :
        ModuleObject(className, ChannelType::UNDEFINED), MathIntegratorModule(), m_pCollinearDistributionModule(
                0), m_x(0.), m_MuF2(0.), m_MuR2(0.), m_MuF2_ref(
                0.), m_currentCollinearDistributionComputeType(CollinearDistributionType::UNDEFINED), m_nFlavors(0), m_nFlavors_ref(
                0), m_qcdOrderType(PerturbativeQCDOrderType::UNDEFINED), m_pRunningAlphaStrong(
                0), m_pActiveFlavorsModule(0) {

    //reset
    m_PartonDistributionAtMuF2_ref = PartonDistribution();

    //nf = 1
    conversionMatrices.insert(std::make_pair(1,

    NumA::MatrixD(2, 2,	//
//	CollinearDistribution		u(-) u(+)
            +1., +1., // u(x)
            -1., +1.  // u(-x)
            ) * (1. / 2.)));

    //nf = 2
    conversionMatrices.insert(std::make_pair(2,

    NumA::MatrixD(4, 4,	//
//	CollinearDistribution		u(-) d(-) u-d  Sum(+)
            +1., +1., +2., +1., // u(x)
            -3., +1., +2., +1., // u(-x)
            +1., +1., -2., +1., // d(x)
            +1., -3., -2., +1.  // d(-x)
            ) * (1. / 4.)));

    //nf = 3
    conversionMatrices.insert(std::make_pair(3,

    NumA::MatrixD(6, 6, //
//	CollinearDistribution		u(-) d(-) s(-) u-d  u-s  Sum(+)
            +1., +1., +1., +2., +2., +1., // u(x)
            -5., +1., +1., +2., +2., +1., // u(-x)
            +1., +1., +1., -4., +2., +1., // d(x)
            +1., -5., +1., -4., +2., +1., // d(-x)
            +1., +1., +1., +2., -4., +1., // s(x)
            +1., +1., -5., +2., -4., +1.  // s(-x)
            ) * (1. / 6.)));

    //nf = 4
    conversionMatrices.insert(std::make_pair(4,

    NumA::MatrixD(8, 8, //
//	CollinearDistribution		u(-) d(-) s(-) c(-) u-d  u-s  u-c  Sum(+)
            +1., +1., +1., +1., +2., +2., +2., +1., // u(x)
            +7., +1., +1., +1., +2., +2., +2., +1., // u(-x)
            +1., +1., +1., +1., -6., +2., +2., +1., // d(x)
            +1., -7., +1., +1., -6., +2., +2., +1., // d(-x)
            +1., +1., +1., +1., +2., -6., +2., +1., // s(x)
            +1., +1., -7., +1., +2., -6., +2., +1., // s(-x)
            +1., +1., +1., +1., +2., +2., -6., +1., // c(x)
            +1., +1., +1., -7., +2., +2., -6., +1.  // c(-x)
            ) * (1. / 8.)));

    //nf = 5
    conversionMatrices.insert(std::make_pair(5,

    NumA::MatrixD(10, 10, //
//	CollinearDistribution		u(-) d(-) s(-) c(-) t(-) u-d  u-s  u-c  u-t  Sum(+)
            +1., +1., +1., +1., +1., +2., +2., +2., +2., +1., // u(x)
            -9., +1., +1., +1., +1., +2., +2., +2., +2., +1., // u(-x)
            +1., +1., +1., +1., +1., -8., +2., +2., +2., +1., // d(x)
            +1., -9., +1., +1., +1., -8., +2., +2., +2., +1., // d(-x)
            +1., +1., +1., +1., +1., +2., -8., +2., +2., +1., // s(x)
            +1., +1., -9., +1., +1., +2., -8., +2., +2., +1., // s(-x)
            +1., +1., +1., +1., +1., +2., +2., -8., +2., +1., // c(x)
            +1., +1., +1., -9., +1., +2., +2., -8., +2., +1., // c(-x)
            +1., +1., +1., +1., +1., +2., +2., +2., -8., +1., // t(x)
            +1., +1., +1., +1., -9., +2., +2., +2., -8., +1.  // t(-x)
            ) * (1. / 10.)));

    //nf = 6
    conversionMatrices.insert(std::make_pair(6,

    NumA::MatrixD(12, 12, //
//	CollinearDistribution		u(-) d(-) s(-) c(-) t(-) b(-) u-d  u-s  u-c  u-t  u-b  Sum(+)
            +1., +1., +1., +1., +1., +1., +2., +2., +2., +2., +2., +1., // u(x)
            -11., +1., +1., +1., +1., +1., +2., +2., +2., +2., +2., +1., // u(-x)
            +1., +1., +1., +1., +1., +1., -10., +2., +2., +2., +2., +1., // d(x)
            +1., -11., +1., +1., +1., +1., -10., +2., +2., +2., +2., +1., // d(-x)
            +1., +1., +1., +1., +1., +1., +2., -10., +2., +2., +2., +1., // s(x)
            +1., +1., -11., +1., +1., +1., +2., -10., +2., +2., +2., +1., // s(-x)
            +1., +1., +1., +1., +1., +1., +2., +2., -10., +2., +2., +1., // c(x)
            +1., +1., +1., -11., +1., +1., +2., +2., -10., +2., +2., +1., // c(-x)
            +1., +1., +1., +1., +1., +1., +2., +2., +2., -10., +2., +1., // t(x)
            +1., +1., +1., +1., -11., +1., +2., +2., +2., -10., +2., +1., // t(-x)
            +1., +1., +1., +1., +1., +1., +2., +2., +2., +2., -10., +1., // b(x)
            +1., +1., +1., +1., +1., -11., +2., +2., +2., +2., -10., +1. // b(-x)
            ) * (1. / 12.)));

    //init functors
    initFunctorsForIntegrations();
}

CollinearDistributionEvolutionModule::CollinearDistributionEvolutionModule(const CollinearDistributionEvolutionModule &other) :
        ModuleObject(other), MathIntegratorModule(other) {

    m_pCollinearDistributionModule = other.m_pCollinearDistributionModule;
    m_x = other.m_x;
    m_MuF2 = other.m_MuF2;
    m_MuR2 = other.m_MuR2;
    m_MuF2_ref = other.m_MuF2_ref;
    m_currentCollinearDistributionComputeType = other.m_currentCollinearDistributionComputeType;

    m_nFlavors = other.m_nFlavors;
    m_nFlavors_ref = other.m_nFlavors_ref;

    m_qcdOrderType = other.m_qcdOrderType;

    if (other.m_pRunningAlphaStrong != 0) {
        m_pRunningAlphaStrong = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pRunningAlphaStrong);
    } else {
        m_pRunningAlphaStrong = 0;
    }

    if (other.m_pActiveFlavorsModule != 0) {
        m_pActiveFlavorsModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pActiveFlavorsModule);
    } else {
        m_pActiveFlavorsModule = 0;
    }

    m_PartonDistributionAtMuF2_ref = other.m_PartonDistributionAtMuF2_ref;

    conversionMatrices = other.conversionMatrices;

    initFunctorsForIntegrations();
}

CollinearDistributionEvolutionModule::~CollinearDistributionEvolutionModule() {

    if (m_pRunningAlphaStrong) {
        setRunningAlphaStrongModule(0);
        m_pRunningAlphaStrong = 0;
    }

    if (m_pActiveFlavorsModule) {
        setActiveFlavorsModule(0);
        m_pActiveFlavorsModule = 0;
    }

    if (p_intIntegrateComputeOutputNS) {
        delete p_intIntegrateComputeOutputNS;
        p_intIntegrateComputeOutputNS = 0;
    }

    if (p_intIntegrateComputeOutputS) {
        delete p_intIntegrateComputeOutputS;
        p_intIntegrateComputeOutputS = 0;
    }

    if (p_intIntegrateComputeOutputG) {
        delete p_intIntegrateComputeOutputG;
        p_intIntegrateComputeOutputG = 0;
    }
}

void CollinearDistributionEvolutionModule::configure(const ElemUtils::Parameters &parameters) {

    //run for mother classes
    ModuleObject::configure(parameters);
    MathIntegratorModule::configureIntegrator(parameters);

    //check if there
    if (parameters.isAvailable(
            PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE)) {

        // try to set m_qcdOrderType by standard way
        try {
            m_qcdOrderType =
                    static_cast<PerturbativeQCDOrderType::Type>(parameters.getLastAvailable().toUInt());

        } catch (const std::exception &e) {

            // if an exception is raised it means that it's a string configuration value
            m_qcdOrderType = PerturbativeQCDOrderType(
                    parameters.getLastAvailable().getString()).getType();
        }

        info(__func__,
                ElemUtils::Formatter()
                        << PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE
                        << " configured with value = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }
}

void CollinearDistributionEvolutionModule::resolveObjectDependencies() {

    //run for mother class
    ModuleObject::resolveObjectDependencies();

    //set integration type
    setIntegrator(NumA::IntegratorType1D::TRAPEZOIDALLOG);
}

void CollinearDistributionEvolutionModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    //run for mother class
    ModuleObject::prepareSubModules(subModulesData);

    //search
    std::map<std::string, BaseObjectData>::const_iterator it;

    //RunningAlphaStrongModule
    it = subModulesData.find(
            RunningAlphaStrongModule::RUNNING_ALPHA_STRONG_MODULE_CLASS_NAME);

    //check if there
    if (it != subModulesData.end()) {

        //reset
        if (m_pRunningAlphaStrong != 0) {
            setRunningAlphaStrongModule(0);
            m_pRunningAlphaStrong = 0;
        }

        //set
        if (m_pRunningAlphaStrong == 0) {
            m_pRunningAlphaStrong =
                    Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                            (it->second).getModuleClassName());

            info(__func__,
                    ElemUtils::Formatter()
                            << "Configured with RunningAlphaStrongModule = "
                            << m_pRunningAlphaStrong->getClassName());

            m_pRunningAlphaStrong->configure((it->second).getParameters());
            m_pRunningAlphaStrong->prepareSubModules(
                    (it->second).getSubModules());
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << getClassName()
                        << " is RunningAlphaStrongModule dependent and you have not provided one");
    }

    //ActiveFlavorsModule
    it =
            subModulesData.find(
                    ActiveFlavorsThresholdsModule::ACTIVE_FLAVORS_THRESHOLDS_MODULE_CLASS_NAME);

    //check if there
    if (it != subModulesData.end()) {

        //reset
        if (m_pActiveFlavorsModule != 0) {
            setActiveFlavorsModule(0);
            m_pActiveFlavorsModule = 0;
        }

        //set
        if (m_pActiveFlavorsModule == 0) {
            m_pActiveFlavorsModule =
                    Partons::getInstance()->getModuleObjectFactory()->newActiveFlavorsThresholdsModule(
                            (it->second).getModuleClassName());

            info(__func__,
                    ElemUtils::Formatter()
                            << "Configured with ActiveFlavorsModule = "
                            << m_pActiveFlavorsModule->getClassName());

            m_pActiveFlavorsModule->configure((it->second).getParameters());
            m_pActiveFlavorsModule->prepareSubModules(
                    (it->second).getSubModules());
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << getClassName()
                        << " is ActiveFlavorsModule dependent and you have not provided one");
    }
}

void CollinearDistributionEvolutionModule::isModuleWellConfigured() {

    //perform list of checks (pointers first)
    if (!m_pCollinearDistributionModule) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Pointer to collinear distribution module is NULL");
    }

    if (!m_pRunningAlphaStrong) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Pointer to RunningAlphaStrong module is NULL");
    }

    if (!m_pActiveFlavorsModule) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Pointer to ActiveFlavorsModule module is NULL");
    }

    if (m_x < -1. || m_x > 1.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "x is out of range: " << m_x);
    }

    if (m_MuF2 <= 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "muF2 is out of range: " << m_MuF2);
    }

    if (m_MuR2 <= 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "muR2 is out of range:" << m_MuR2);
    }

    if (m_MuF2_ref <= 0.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "muF2_ref is out of range: "
                        << m_MuF2_ref);
    }

    if (m_currentCollinearDistributionComputeType == CollinearDistributionType::UNDEFINED) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "collinear distribution type is UNDEFINED");
    }

    if (m_qcdOrderType == PerturbativeQCDOrderType::UNDEFINED) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "pQCD order is UNDEFINED");
    }
}

void CollinearDistributionEvolutionModule::initModule() {

    //evaluate for reference point
    m_PartonDistributionAtMuF2_ref = m_pCollinearDistributionModule->compute(
            CollinearDistributionKinematic(m_x, m_MuF2_ref, m_MuR2),
            m_currentCollinearDistributionComputeType);

    //evaluate nf and muF2 and muF2_ref
    m_nFlavors =
            (m_pActiveFlavorsModule) ?
                    ((m_pActiveFlavorsModule->getNfInterval(m_MuF2)).getNf()) :
                    (0);

    if (m_nFlavors < 1 || m_nFlavors > 6) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "nFlavors at muF2 is out of range:"
                        << m_nFlavors);
    }

    m_nFlavors_ref =
            (m_pActiveFlavorsModule) ?
                    ((m_pActiveFlavorsModule->getNfInterval(m_MuF2_ref)).getNf()) :
                    (0);

    if (m_nFlavors_ref < 1 || m_nFlavors_ref > 6) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "nFlavors at muF2_ref is out of range:"
                        << m_nFlavors_ref);
    }
}

PartonDistribution CollinearDistributionEvolutionModule::compute(double x,
        double MuF2, double MuR2, CollinearDistributionModule* pCollinearDistributionModule,
        CollinearDistributionType::Type colldistType) {

    //variables
    std::vector<double> parameters(1,
            static_cast<double>(QuarkNonSingletCombination::UNDEFINED));
    double diff;

    //pre-compute (set internal variables, etc.)
    preCompute(x, MuF2, MuR2, pCollinearDistributionModule, colldistType);

    //initialize
    initModule();

    //check if input data are fine
    isModuleWellConfigured();

    //do the evolution for nf given by the target scale
    std::map<QuarkNonSingletCombination::Type, double> NSDiff;
    double SDiff, GDiff;

    if (m_nFlavors >= 1) {

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::UP_NONSINGLET);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::UP_NONSINGLET,
                        diff));
    }

    if (m_nFlavors >= 2) {

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::DOWN_NONSINGLET);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::DOWN_NONSINGLET,
                        diff));

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::UP_MINUS_DOWN);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_DOWN,
                        diff));
    }

    if (m_nFlavors >= 3) {

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::STRANGE_NONSINGLET);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::STRANGE_NONSINGLET,
                        diff));

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::UP_MINUS_STRANGE);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_STRANGE,
                        diff));
    }

    if (m_nFlavors >= 4) {

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::CHARM_NONSINGLET);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::CHARM_NONSINGLET,
                        diff));

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::UP_MINUS_CHARM);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_CHARM,
                        diff));
    }

    if (m_nFlavors >= 5) {

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::TOP_NONSINGLET);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::TOP_NONSINGLET,
                        diff));

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::UP_MINUS_TOP);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_TOP, diff));
    }

    if (m_nFlavors == 6) {

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::BOTTOM_NONSINGLET);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::BOTTOM_NONSINGLET,
                        diff));

        parameters.at(0) =
                static_cast<double>(QuarkNonSingletCombination::UP_MINUS_BOTTOM);
        diff = integrate(p_intIntegrateComputeOutputNS, m_MuF2_ref, m_MuF2,
                parameters);

        NSDiff.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_BOTTOM,
                        diff));
    }

    parameters.at(0) =
            static_cast<double>(QuarkNonSingletCombination::UNDEFINED);

    SDiff = integrate(p_intIntegrateComputeOutputS, m_MuF2_ref, m_MuF2,
            parameters);

    GDiff = integrate(p_intIntegrateComputeOutputG, m_MuF2_ref, m_MuF2,
            parameters);

    //apply difference (new flavors can arrive!)

    //NS, S and G for reference scale
    std::map<QuarkNonSingletCombination::Type, double> NS0 = getNS(
            m_nFlavors_ref, m_PartonDistributionAtMuF2_ref);
    double S0 = getS(m_nFlavors_ref, m_PartonDistributionAtMuF2_ref);
    double G0 = getG(m_PartonDistributionAtMuF2_ref);

    //apply (add result at reference scale to the map containing difference: after this operation the map will contain the end result)
    std::map<QuarkNonSingletCombination::Type, double>::iterator it1;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it2;

    for (it1 = NSDiff.begin(); it1 != NSDiff.end(); it1++) {

        it2 = NS0.find(it1->first);

        if (it2 != NS0.end())
            it1->second += it2->second;
    }

    SDiff += S0;
    GDiff += G0;

    //back to flavor basis and return the result
    return getPartonDistribution(m_nFlavors, NSDiff, SDiff, GDiff);
}

void CollinearDistributionEvolutionModule::preCompute(double x, double MuF2,
        double MuR2, CollinearDistributionModule* pCollinearDistributionModule, CollinearDistributionType::Type colldistType) {

    //copy variables
    m_pCollinearDistributionModule = pCollinearDistributionModule;
    m_x = x;
    m_MuF2 = MuF2;
    m_MuR2 = MuR2;
    m_MuF2_ref = (m_pCollinearDistributionModule) ? (m_pCollinearDistributionModule->getMuF2Ref()) : (0.);
    m_currentCollinearDistributionComputeType = colldistType;
}

double CollinearDistributionEvolutionModule::integrateComputeOutputNS(double MuF2,
        std::vector<double> par) {
    return nonSingletMu2FDerivative(MuF2,
            static_cast<QuarkNonSingletCombination::Type>(par.at(0)));
}

double CollinearDistributionEvolutionModule::integrateComputeOutputS(double MuF2,
        std::vector<double> par) {
    return singletMuF2Derivative(MuF2);
}

double CollinearDistributionEvolutionModule::integrateComputeOutputG(double MuF2,
        std::vector<double> par) {
    return gluonMuF2Derivative(MuF2);
}

std::map<QuarkNonSingletCombination::Type, double> CollinearDistributionEvolutionModule::getNS(
        const size_t nFlavors,
        const PartonDistribution& partonDistribution) const {

    //check if nf is right
    if (nFlavors < 1 || nFlavors > 6) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Illegal number of flavors: "
                        << nFlavors);
    }

    //result
    std::map<QuarkNonSingletCombination::Type, double> result;

    //reference to quark content
    const std::map<QuarkFlavor::Type, QuarkDistribution>& quarkContent =
            partonDistribution.getQuarkDistributions();

    //iterators
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it_u;
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it_d;
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it_s;
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it_c;
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it_t;
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it_b;

    //search and insert
    if (nFlavors >= 1) {

        it_u = findInQuarkFlavorMap(quarkContent, QuarkFlavor::UP,
                "quarkContent");

        result.insert(
                std::make_pair(QuarkNonSingletCombination::UP_NONSINGLET,
                        (it_u->second).getQuarkDistributionMinus()));
    }

    if (nFlavors >= 2) {

        it_d = findInQuarkFlavorMap(quarkContent, QuarkFlavor::DOWN,
                "quarkContent");

        result.insert(
                std::make_pair(QuarkNonSingletCombination::DOWN_NONSINGLET,
                        (it_d->second).getQuarkDistributionMinus()));
        result.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_DOWN,
                        (it_u->second).getQuarkDistribution()
                                - (it_d->second).getQuarkDistribution()));
    }

    if (nFlavors >= 3) {

        it_s = findInQuarkFlavorMap(quarkContent, QuarkFlavor::STRANGE,
                "quarkContent");

        result.insert(
                std::make_pair(QuarkNonSingletCombination::STRANGE_NONSINGLET,
                        (it_s->second).getQuarkDistributionMinus()));
        result.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_STRANGE,
                        (it_u->second).getQuarkDistribution()
                                - (it_s->second).getQuarkDistribution()));
    }

    if (nFlavors >= 4) {

        it_c = findInQuarkFlavorMap(quarkContent, QuarkFlavor::CHARM,
                "quarkContent");

        result.insert(
                std::make_pair(QuarkNonSingletCombination::CHARM_NONSINGLET,
                        (it_c->second).getQuarkDistributionMinus()));
        result.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_CHARM,
                        (it_u->second).getQuarkDistribution()
                                - (it_c->second).getQuarkDistribution()));
    }

    if (nFlavors >= 5) {

        it_t = findInQuarkFlavorMap(quarkContent, QuarkFlavor::TOP,
                "quarkContent");

        result.insert(
                std::make_pair(QuarkNonSingletCombination::TOP_NONSINGLET,
                        (it_t->second).getQuarkDistributionMinus()));
        result.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_TOP,
                        (it_u->second).getQuarkDistribution()
                                - (it_t->second).getQuarkDistribution()));
    }

    if (nFlavors == 6) {

        it_b = findInQuarkFlavorMap(quarkContent, QuarkFlavor::BOTTOM,
                "quarkContent");

        result.insert(
                std::make_pair(QuarkNonSingletCombination::BOTTOM_NONSINGLET,
                        (it_b->second).getQuarkDistributionMinus()));
        result.insert(
                std::make_pair(QuarkNonSingletCombination::UP_MINUS_BOTTOM,
                        (it_u->second).getQuarkDistribution()
                                - (it_b->second).getQuarkDistribution()));
    }

    //return
    return result;
}

double CollinearDistributionEvolutionModule::getS(const size_t nFlavors,
        const PartonDistribution& partonDistribution) const {

    //check if nf is right
    if (nFlavors < 1 || nFlavors > 6) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Illegal number of flavors: "
                        << nFlavors);
    }

    //result
    double result = 0.;

    //reference to quark content
    const std::map<QuarkFlavor::Type, QuarkDistribution>& quarkContent =
            partonDistribution.getQuarkDistributions();

    //iterator
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it;

    //search and insert
    if (nFlavors >= 1) {

        it = findInQuarkFlavorMap(quarkContent, QuarkFlavor::UP,
                "quarkContent");
        result += (it->second).getQuarkDistributionPlus();
    }

    if (nFlavors >= 2) {

        it = findInQuarkFlavorMap(quarkContent, QuarkFlavor::DOWN,
                "quarkContent");
        result += (it->second).getQuarkDistributionPlus();
    }

    if (nFlavors >= 3) {

        it = findInQuarkFlavorMap(quarkContent, QuarkFlavor::STRANGE,
                "quarkContent");
        result += (it->second).getQuarkDistributionPlus();
    }

    if (nFlavors >= 4) {

        it = findInQuarkFlavorMap(quarkContent, QuarkFlavor::CHARM,
                "quarkContent");
        result += (it->second).getQuarkDistributionPlus();
    }

    if (nFlavors >= 5) {

        it = findInQuarkFlavorMap(quarkContent, QuarkFlavor::TOP,
                "quarkContent");
        result += (it->second).getQuarkDistributionPlus();
    }

    if (nFlavors == 6) {

        it = findInQuarkFlavorMap(quarkContent, QuarkFlavor::BOTTOM,
                "quarkContent");
        result += (it->second).getQuarkDistributionPlus();
    }

    //result
    return result;
}

double CollinearDistributionEvolutionModule::getG(
        const PartonDistribution& partonDistribution) const {
    return partonDistribution.getGluonDistribution().getGluonDistribution();
}

PartonDistribution CollinearDistributionEvolutionModule::getPartonDistribution(
        const size_t nFlavors,
        const std::map<QuarkNonSingletCombination::Type, double>& NS, double S,
        double G) const {

    //check if nf is right
    if (nFlavors < 1 || nFlavors > 6) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Illegal number of flavors: "
                        << nFlavors);
    }

    //select appropraite conversion matrix
    std::map<size_t, NumA::MatrixD>::const_iterator it =
            conversionMatrices.find(nFlavors);

    if (it == conversionMatrices.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No key " << nFlavors
                        << " in map conversionMatrices");
    }

    //result
    PartonDistribution partonDistribution;

    //values of CollinearDistributions for [0]: +x and [1]: -x
    double u[2], d[2], s[2], c[2], t[2], b[2];

    //iteratoes
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSu;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSd;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSs;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSc;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSt;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSb;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSumd;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSums;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSumc;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSumt;
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it_NSumb;

    //search
    if (nFlavors >= 1) {

        it_NSu = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::UP_NONSINGLET, "NS");
    }

    if (nFlavors >= 2) {

        it_NSd = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::DOWN_NONSINGLET, "NS");
        it_NSumd = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::UP_MINUS_DOWN, "NS");
    }

    if (nFlavors >= 3) {

        it_NSs = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::STRANGE_NONSINGLET, "NS");
        it_NSums = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::UP_MINUS_STRANGE, "NS");
    }

    if (nFlavors >= 4) {

        it_NSc = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::CHARM_NONSINGLET, "NS");
        it_NSumc = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::UP_MINUS_CHARM, "NS");
    }

    if (nFlavors >= 5) {

        it_NSt = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::TOP_NONSINGLET, "NS");
        it_NSumt = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::UP_MINUS_TOP, "NS");
    }

    if (nFlavors == 6) {

        it_NSb = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::BOTTOM_NONSINGLET, "NS");
        it_NSumb = findInQuarkNonSingletCombinationMap(NS,
                QuarkNonSingletCombination::UP_MINUS_BOTTOM, "NS");
    }

    //insert and evaluate
    NumA::VectorD input;

    if (nFlavors == 1) {

        input.push_back(it_NSu->second);
        input.push_back(S);

        u[0] = (it->second).getLine(0) * input;
        u[1] = (it->second).getLine(1) * input;

        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::UP, u[0], u[0] + u[1],
                        u[0] - u[1]));
    }

    if (nFlavors == 2) {

        input.push_back(it_NSu->second);
        input.push_back(it_NSd->second);
        input.push_back(it_NSumd->second);
        input.push_back(S);

        u[0] = (it->second).getLine(0) * input;
        u[1] = (it->second).getLine(1) * input;
        d[0] = (it->second).getLine(2) * input;
        d[1] = (it->second).getLine(3) * input;

        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::UP, u[0], u[0] + u[1],
                        u[0] - u[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::DOWN, d[0], d[0] + d[1],
                        d[0] - d[1]));
    }

    if (nFlavors == 3) {

        input.push_back(it_NSu->second);
        input.push_back(it_NSd->second);
        input.push_back(it_NSs->second);
        input.push_back(it_NSumd->second);
        input.push_back(it_NSums->second);
        input.push_back(S);

        u[0] = (it->second).getLine(0) * input;
        u[1] = (it->second).getLine(1) * input;
        d[0] = (it->second).getLine(2) * input;
        d[1] = (it->second).getLine(3) * input;
        s[0] = (it->second).getLine(4) * input;
        s[1] = (it->second).getLine(5) * input;

        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::UP, u[0], u[0] + u[1],
                        u[0] - u[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::DOWN, d[0], d[0] + d[1],
                        d[0] - d[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::STRANGE, s[0], s[0] + s[1],
                        s[0] - s[1]));
    }

    if (nFlavors == 4) {

        input.push_back(it_NSu->second);
        input.push_back(it_NSd->second);
        input.push_back(it_NSs->second);
        input.push_back(it_NSc->second);
        input.push_back(it_NSumd->second);
        input.push_back(it_NSums->second);
        input.push_back(it_NSumc->second);
        input.push_back(S);

        u[0] = (it->second).getLine(0) * input;
        u[1] = (it->second).getLine(1) * input;
        d[0] = (it->second).getLine(2) * input;
        d[1] = (it->second).getLine(3) * input;
        s[0] = (it->second).getLine(4) * input;
        s[1] = (it->second).getLine(5) * input;
        c[0] = (it->second).getLine(6) * input;
        c[1] = (it->second).getLine(7) * input;

        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::UP, u[0], u[0] + u[1],
                        u[0] - u[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::DOWN, d[0], d[0] + d[1],
                        d[0] - d[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::STRANGE, s[0], s[0] + s[1],
                        s[0] - s[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::CHARM, c[0], c[0] + c[1],
                        c[0] - c[1]));
    }

    if (nFlavors == 5) {

        input.push_back(it_NSu->second);
        input.push_back(it_NSd->second);
        input.push_back(it_NSs->second);
        input.push_back(it_NSc->second);
        input.push_back(it_NSt->second);
        input.push_back(it_NSumd->second);
        input.push_back(it_NSums->second);
        input.push_back(it_NSumc->second);
        input.push_back(it_NSumt->second);
        input.push_back(S);

        u[0] = (it->second).getLine(0) * input;
        u[1] = (it->second).getLine(1) * input;
        d[0] = (it->second).getLine(2) * input;
        d[1] = (it->second).getLine(3) * input;
        s[0] = (it->second).getLine(4) * input;
        s[1] = (it->second).getLine(5) * input;
        c[0] = (it->second).getLine(6) * input;
        c[1] = (it->second).getLine(7) * input;
        t[0] = (it->second).getLine(8) * input;
        t[1] = (it->second).getLine(9) * input;

        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::UP, u[0], u[0] + u[1],
                        u[0] - u[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::DOWN, d[0], d[0] + d[1],
                        d[0] - d[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::STRANGE, s[0], s[0] + s[1],
                        s[0] - s[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::CHARM, c[0], c[0] + c[1],
                        c[0] - c[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::TOP, t[0], t[0] + t[1],
                        t[0] - t[1]));
    }

    if (nFlavors == 6) {

        input.push_back(it_NSu->second);
        input.push_back(it_NSd->second);
        input.push_back(it_NSs->second);
        input.push_back(it_NSc->second);
        input.push_back(it_NSt->second);
        input.push_back(it_NSb->second);
        input.push_back(it_NSumd->second);
        input.push_back(it_NSums->second);
        input.push_back(it_NSumc->second);
        input.push_back(it_NSumt->second);
        input.push_back(it_NSumb->second);
        input.push_back(S);

        u[0] = (it->second).getLine(0) * input;
        u[1] = (it->second).getLine(1) * input;
        d[0] = (it->second).getLine(2) * input;
        d[1] = (it->second).getLine(3) * input;
        s[0] = (it->second).getLine(4) * input;
        s[1] = (it->second).getLine(5) * input;
        c[0] = (it->second).getLine(6) * input;
        c[1] = (it->second).getLine(7) * input;
        t[0] = (it->second).getLine(8) * input;
        t[1] = (it->second).getLine(9) * input;
        b[0] = (it->second).getLine(10) * input;
        b[1] = (it->second).getLine(11) * input;

        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::UP, u[0], u[0] + u[1],
                        u[0] - u[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::DOWN, d[0], d[0] + d[1],
                        d[0] - d[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::STRANGE, s[0], s[0] + s[1],
                        s[0] - s[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::CHARM, c[0], c[0] + c[1],
                        c[0] - c[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::TOP, t[0], t[0] + t[1],
                        t[0] - t[1]));
        partonDistribution.addQuarkDistribution(
                QuarkDistribution(QuarkFlavor::BOTTOM, b[0], b[0] + b[1],
                        b[0] - b[1]));
    }

    //add gluons
    partonDistribution.setGluonDistribution(GluonDistribution(G));

    //return
    return partonDistribution;
}

inline std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator CollinearDistributionEvolutionModule::findInQuarkFlavorMap(
        const std::map<QuarkFlavor::Type, QuarkDistribution>& map,
        QuarkFlavor::Type key, const std::string& mapName) const {

    //search
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it =
            map.find(key);

    //check
    if (it == map.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No key "
                        << QuarkFlavor(key).toString() << " in map "
                        << mapName);
    }

    //return
    return it;
}

std::map<QuarkNonSingletCombination::Type, double>::const_iterator CollinearDistributionEvolutionModule::findInQuarkNonSingletCombinationMap(
        const std::map<QuarkNonSingletCombination::Type, double>& map,
        QuarkNonSingletCombination::Type key,
        const std::string& mapName) const {

    //search
    std::map<QuarkNonSingletCombination::Type, double>::const_iterator it =
            map.find(key);

    //check
    if (it == map.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No key "
                        << QuarkNonSingletCombination(key).toString()
                        << " in map " << mapName);
    }

    //return
    return it;
}

void CollinearDistributionEvolutionModule::initFunctorsForIntegrations() {

    //set functors
    p_intIntegrateComputeOutputNS = NumA::Integrator1D::newIntegrationFunctor(
            this, &CollinearDistributionEvolutionModule::integrateComputeOutputNS);
    p_intIntegrateComputeOutputS = NumA::Integrator1D::newIntegrationFunctor(
            this, &CollinearDistributionEvolutionModule::integrateComputeOutputS);
    p_intIntegrateComputeOutputG = NumA::Integrator1D::newIntegrationFunctor(
            this, &CollinearDistributionEvolutionModule::integrateComputeOutputG);
}

PerturbativeQCDOrderType::Type CollinearDistributionEvolutionModule::getQcdOrderType() const {
    return m_qcdOrderType;
}

void CollinearDistributionEvolutionModule::setQcdOrderType(
        PerturbativeQCDOrderType::Type qcdOrderType) {
    m_qcdOrderType = qcdOrderType;
}

RunningAlphaStrongModule* CollinearDistributionEvolutionModule::getRunningAlphaStrongModule() const {
    return m_pRunningAlphaStrong;
}

void CollinearDistributionEvolutionModule::setRunningAlphaStrongModule(
        RunningAlphaStrongModule* runningAlphaStrongModule) {

    m_pModuleObjectFactory->updateModulePointerReference(m_pRunningAlphaStrong,
            runningAlphaStrongModule);
    m_pRunningAlphaStrong = runningAlphaStrongModule;
}

ActiveFlavorsThresholdsModule* CollinearDistributionEvolutionModule::getActiveFlavorsModule() const {
    return m_pActiveFlavorsModule;
}

void CollinearDistributionEvolutionModule::setActiveFlavorsModule(
        ActiveFlavorsThresholdsModule* activeFlavorsModule) {

    m_pModuleObjectFactory->updateModulePointerReference(m_pActiveFlavorsModule,
            activeFlavorsModule);
    m_pActiveFlavorsModule = activeFlavorsModule;
}

} /* namespace PARTONS */

