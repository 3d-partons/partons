#include "../../../../../include/partons/modules/convol_coeff_function/GAM2/GAM2CFFStandard.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <utility>

#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/modules/gpd/GPDModule.h"
#include "../../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongStandard.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"

#include <gsl/gsl_sf_dilog.h>
#include <gsl/gsl_sf_result.h>
#include <gsl/gsl_integration.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_plain.h>
#include <gsl/gsl_monte_miser.h>
#include <gsl/gsl_monte_vegas.h>

#include <iostream>

namespace PARTONS {

const unsigned int GAM2CFFStandard::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GAM2CFFStandard("GAM2CFFStandard"));

GAM2CFFStandard::GAM2CFFStandard(const std::string &className) :
        GAM2ConvolCoeffFunctionModule(className), m_pRunningAlphaStrongModule(
                0), m_CF(4. / 3.), m_alphaSOver2Pi(0.), m_quark_diagonal_V(0.), m_quark_diagonal_A(
                0.) {

    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::H,
                    &GAM2ConvolCoeffFunctionModule::computeUnpolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::E,
                    &GAM2ConvolCoeffFunctionModule::computeUnpolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Ht,
                    &GAM2ConvolCoeffFunctionModule::computePolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Et,
                    &GAM2ConvolCoeffFunctionModule::computePolarized));

    m_iepsilon = std::complex<double>(0., 1.E-4);
    m_reim = 2;
    m_phiDep = true;

    initFunctorsForIntegrations();
}

GAM2CFFStandard::GAM2CFFStandard(const GAM2CFFStandard &other) :
        GAM2ConvolCoeffFunctionModule(other) {

    if (other.m_pRunningAlphaStrongModule != 0) {
        m_pRunningAlphaStrongModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pRunningAlphaStrongModule);
    } else {
        m_pRunningAlphaStrongModule = 0;
    }

    m_alphaSOver2Pi = other.m_alphaSOver2Pi;
    m_CF = other.m_CF;
    m_quark_diagonal_V = other.m_quark_diagonal_V;
    m_quark_diagonal_A = other.m_quark_diagonal_A;

    m_iepsilon = other.m_iepsilon;
    m_reim = other.m_reim;
    m_phiDep = other.m_phiDep;

    initFunctorsForIntegrations();
}

GAM2CFFStandard* GAM2CFFStandard::clone() const {
    return new GAM2CFFStandard(*this);
}

void GAM2CFFStandard::resolveObjectDependencies() {
    GAM2ConvolCoeffFunctionModule::resolveObjectDependencies();

    setIntegrator(NumA::IntegratorType1D::DEXP);

    m_pRunningAlphaStrongModule =
            Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                    RunningAlphaStrongStandard::classId);
}

void GAM2CFFStandard::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    GAM2ConvolCoeffFunctionModule::prepareSubModules(subModulesData);

    std::map<std::string, BaseObjectData>::const_iterator it;

    it = subModulesData.find(
            RunningAlphaStrongModule::RUNNING_ALPHA_STRONG_MODULE_CLASS_NAME);

    if (it != subModulesData.end()) {

        if (m_pRunningAlphaStrongModule != 0) {
            setRunningAlphaStrongModule(0);
            m_pRunningAlphaStrongModule = 0;
        }

        if (!m_pRunningAlphaStrongModule) {
            m_pRunningAlphaStrongModule =
                    Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                            (it->second).getModuleClassName());
            info(__func__,
                    ElemUtils::Formatter()
                            << "Configure with RunningAlphaStrongModule = "
                            << m_pRunningAlphaStrongModule->getClassName());
            m_pRunningAlphaStrongModule->configure(
                    (it->second).getParameters());
        }
    }
}

void GAM2CFFStandard::initFunctorsForIntegrations() {

    m_pConvol_NLO_V_x = NumA::Integrator1D::newIntegrationFunctor(this,
            &GAM2CFFStandard::Convol_NLO_V_x);

    m_pConvol_NLO_V_x_Sym = NumA::Integrator1D::newIntegrationFunctor(this,
            &GAM2CFFStandard::Convol_NLO_V_x_Sym);

    m_pConvol_NLO_V_x_Sym_Const = NumA::Integrator1D::newIntegrationFunctor(
            this, &GAM2CFFStandard::Convol_NLO_V_x_Sym_Const);
}

GAM2CFFStandard::~GAM2CFFStandard() {

    if (m_pRunningAlphaStrongModule) {
        setRunningAlphaStrongModule(0);
        m_pRunningAlphaStrongModule = 0;
    }

    if (m_pConvol_NLO_V_x) {
        delete m_pConvol_NLO_V_x;
        m_pConvol_NLO_V_x = 0;
    }

    if (m_pConvol_NLO_V_x_Sym) {
        delete m_pConvol_NLO_V_x_Sym;
        m_pConvol_NLO_V_x_Sym = 0;
    }

    if (m_pConvol_NLO_V_x_Sym_Const) {
        delete m_pConvol_NLO_V_x_Sym_Const;
        m_pConvol_NLO_V_x_Sym_Const = 0;
    }
}

void GAM2CFFStandard::initModule() {
    GAM2ConvolCoeffFunctionModule::initModule();

    m_alphaSOver2Pi = m_pRunningAlphaStrongModule->compute(m_MuR2)
            / (2. * Constant::PI);
}

void GAM2CFFStandard::isModuleWellConfigured() {

    GAM2ConvolCoeffFunctionModule::isModuleWellConfigured();

    if (m_pGPDModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "GPDModule* is NULL");
    }

    if (m_qcdOrderType == PerturbativeQCDOrderType::UNDEFINED) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "QCDOrderType is UNDEFINED");
    }

    if (m_qcdOrderType != PerturbativeQCDOrderType::LO
            && m_qcdOrderType != PerturbativeQCDOrderType::NLO) {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Erroneous input, perturbative QCD order can only be LO or NLO. Here Order = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }

    if (getMathIntegrator() == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "MathIntegrationMode is UNDEFINED");
    }

    if (m_pRunningAlphaStrongModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "RunningAlphaStrongModule* is NULL");
    }
}

void GAM2CFFStandard::computeDiagonalGPD_V() {

    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(m_xi, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    m_quark_diagonal_V = computeCubedChargeAveragedGPD(partonDistribution);
}

void GAM2CFFStandard::computeDiagonalGPD_A() {

    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(m_xi, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    m_quark_diagonal_A = computeCubedChargeAveragedGPD(partonDistribution);
}

// Trace \mathcal{A}, NLO paper, Eq. (21)
double GAM2CFFStandard::A(double s, const std::vector<double>& beta,
        const std::vector<double>& ee, const std::vector<double>& ek) const {
    return s
            * (beta[0] * (ee[0] * ek[0] - ee[1] * ek[1])
                    + beta[2] * (ee[1] * ek[1] - ee[2] * ek[2])) / 2.; // 24.11.2021 found bug; was 8, should be 2
}

// LO amplitude for single photons permutation, NLO paper, Eq. (20)
std::complex<double> GAM2CFFStandard::M0(double s, double x, double xi,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) {

    std::complex<double> result;
    result = -4. / s / s;
    result /= ((x + xi) * beta[0] + m_iepsilon) * ((x - xi) * beta[2] - m_iepsilon);
    result *= A(s, beta, ee, ek);

    return result;
}

// Sum of finite parts of amplitudes 2.L/R and 3.L/R, NLO paper, Eqs. (26)-(29)
// NOTE: the factorization scale dependent term is included in the collinear part!
std::complex<double> GAM2CFFStandard::M23LR(double s, double x, double xi,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) {
    std::complex<double> result;
    result = -5.;
    result += std::log((-(x + xi) * beta[0] / 2. / xi - m_iepsilon));
    result += std::log(((x - xi) * beta[2] / 2. / xi - m_iepsilon));
    result *= M0(s, x, xi, beta, ee, ek);
    result *= m_CF * m_alphaSOver2Pi;

    return result;
}

// M3M amplitude, NLO paper, Eq. (30)
std::complex<double> GAM2CFFStandard::M3M(double s, double x, double xi,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) {

    std::complex<double> D1 = (x + xi) * beta[0];
    std::complex<double> D3 = -(x - xi) * beta[2];

    std::complex<double> p1 = 1.
            - (D1 * log((-D1 - m_iepsilon) / (2. * xi))
                    - D3 * log((-D3 - m_iepsilon) / (2. * xi))) / (D1 - D3);

    std::complex<double> p2 = D1 / (D1 - D3)
            * (1.
                    + (-2. * D1 + D3) / (D1 - D3)
                            * log((D1 + m_iepsilon) / (D3 + m_iepsilon)));

    std::complex<double> p3 = D3 / (D1 - D3)
            * (1.
                    + (D1 - 2. * D3) / (D1 - D3)
                            * log((D1 + m_iepsilon) / (D3 + m_iepsilon)));

    std::complex<double> trA = ((beta[0] * ee[0] * ek[0]
            - beta[0] * ee[1] * ek[1] + beta[2] * ee[1] * ek[1]
            - beta[2] * ee[2] * ek[2]) * s) / 2.;

    std::complex<double> trB = -2
            * (beta[0] * ee[0] * ek[0] - beta[0] * ee[1] * ek[1]
                    - beta[2] * ee[1] * ek[1] - beta[2] * ee[2] * ek[2]) * s;

    std::complex<double> trC = 2
            * (beta[0] * ee[0] * ek[0] + beta[0] * ee[1] * ek[1]
                    + beta[2] * ee[1] * ek[1] - beta[2] * ee[2] * ek[2]) * s;

    return M0(s, x, xi, beta, ee, ek) * m_CF * m_alphaSOver2Pi / 2.
            * (p1 + (trB / trA) * p2 + (trC / trA) * p3);

}

// Trace structures in amplitudes 4.L and 5.L,
// NLO paper, Eqs. (43)-(44), (A2), (B1), see also the Mathematica notebook Traces.
// Tr_4/5L_Fnab is the trace multiplying the function F_nab (see NLO paper)

double GAM2CFFStandard::Tr_4L_F210(double xi, double s,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) const {

    return -4. * s
            * (-2. * (std::pow(beta[0], 2.) - std::pow(beta[1], 2.))
                    * (ee[0] * ek[0] - ee[1] * ek[1]) * s * xi
                    + std::pow(beta[2], 2.)
                            * (ee[1] * ek[1] - 3. * ee[2] * ek[2]) * s * xi
                    + beta[2]
                            * (2. * ek[0] * ek[1] * ek[2]
                                    - (beta[0] - 2. * beta[1]) * ee[0] * ek[0]
                                            * s * xi
                                    + (beta[0] - 2. * beta[1]) * ee[1] * ek[1]
                                            * s * xi));
}

double GAM2CFFStandard::Tr_4L_F201(double xi, double s,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) const {

    return 2.
            * (std::pow(beta[0], 2.) * (-(ee[0] * ek[0]) + ee[1] * ek[1])
                    + 2. * beta[1] * beta[2]
                            * (-(ee[1] * ek[1]) + ee[2] * ek[2])
                    + beta[0]
                            * (-2. * beta[1] * ee[0] * ek[0]
                                    + 4. * beta[2] * ee[0] * ek[0]
                                    + 2. * beta[1] * ee[1] * ek[1]
                                    - 9. * beta[2] * ee[1] * ek[1]
                                    + 3. * beta[2] * ee[2] * ek[2]))
            * std::pow(s, 2.);
}

double GAM2CFFStandard::Tr_4L_F211(double xi, double s,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) const {

    return -2.
            * (2. * std::pow(beta[1], 2.) * (ee[0] * ek[0] - ee[1] * ek[1])
                    + beta[1] * beta[2]
                            * (2. * ee[0] * ek[0] + 3. * ee[1] * ek[1]
                                    - 3. * ee[2] * ek[2])
                    + beta[0] * beta[2] * (ee[0] * ek[0] - ee[2] * ek[2]))
            * std::pow(s, 2.);
}

double GAM2CFFStandard::Tr_4L_F220(double xi, double s,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) const {

    return 4. * beta[2] * s
            * (2. * ek[0] * ek[1] * ek[2]
                    + (beta[1] + beta[2]) * ee[0] * ek[0] * s * xi
                    - (beta[1] * ee[1] * ek[1] + beta[2] * ee[2] * ek[2]) * s
                            * xi);
}

double GAM2CFFStandard::Tr_4L_F221(double xi, double s,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) const {

    return 2. * beta[1]
            * (beta[1] * ee[0] * ek[0] + beta[2] * ee[0] * ek[0]
                    - beta[1] * ee[1] * ek[1] - beta[2] * ee[2] * ek[2])
            * std::pow(s, 2.);
}

double GAM2CFFStandard::Tr_4L_F100(double xi, double s,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) const {

    return 2.
            * (beta[0] * ee[0] * ek[0] + 2 * beta[1] * ee[0] * ek[0]
                    - 2. * beta[2] * ee[0] * ek[0] - beta[0] * ee[1] * ek[1]
                    - 2. * beta[1] * ee[1] * ek[1]
                    + 3. * beta[2] * ee[1] * ek[1]
                    - 3. * beta[2] * ee[2] * ek[2]) * s;
}

double GAM2CFFStandard::Tr_4L_F110(double xi, double s,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) const {

    return 2.
            * (-3. * beta[1] * ee[0] * ek[0] - beta[2] * ee[0] * ek[0]
                    + 3. * beta[1] * ee[1] * ek[1] + beta[2] * ee[2] * ek[2])
            * s;
}

double GAM2CFFStandard::Tr_4L_G(double xi, double s,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) const {

    return 2. * beta[2]
            * (beta[0] * ee[0] * ek[0] - beta[0] * ee[1] * ek[1]
                    + beta[2] * ee[1] * ek[1] - beta[2] * ee[2] * ek[2])
            * std::pow(s, 2.);
}

double GAM2CFFStandard::Tr_5L_F201(double xi, double s,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) const {

    return 4.
            * (3. * beta[0] * ee[0] * ek[0] - 5. * beta[0] * ee[1] * ek[1]
                    - beta[1] * ee[1] * ek[1] + 2. * beta[0] * ee[2] * ek[2]
                    + beta[1] * ee[2] * ek[2]) * s;
}

double GAM2CFFStandard::Tr_5L_F210(double xi, double s,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) const {

    return 8. * ek[1] * (ek[0] * ek[2] - beta[2] * ee[1] * s * xi)
            - 24. * ek[2] * (ek[0] * ek[1] - beta[2] * ee[2] * s * xi);
}

double GAM2CFFStandard::Tr_5L_F211(double xi, double s,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) const {

    return 4.
            * (-(beta[0] * ee[0] * ek[0]) - 3. * beta[1] * ee[1] * ek[1]
                    + beta[0] * ee[2] * ek[2] + 3. * beta[1] * ee[2] * ek[2])
            * s;
}

double GAM2CFFStandard::Tr_5L_F220(double xi, double s,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) const {

    return -8 * ek[0] * (ek[1] * ek[2] + beta[2] * ee[0] * s * xi)
            - 8 * ek[2] * (ek[0] * ek[1] - beta[2] * ee[2] * s * xi); // 05.10.2021 modified
}

double GAM2CFFStandard::Tr_5L_F221(double xi, double s,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) const {

    return -4. * beta[1] * ee[0] * ek[0] * s + 4. * beta[1] * ee[2] * ek[2] * s;
}

double GAM2CFFStandard::Tr_5L_F100(double xi, double s,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) const {

    return -4. * (2 * ee[0] * ek[0] - 3. * ee[1] * ek[1] + 3. * ee[2] * ek[2]);
}

double GAM2CFFStandard::Tr_5L_F110(double xi, double s,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) const {

    return 4. * (ee[0] * ek[0] - ee[2] * ek[2]);
}

double GAM2CFFStandard::Tr_5L_G(double xi, double s,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) const {

    return -2.
            * (beta[0] * ee[0] * ek[0] - beta[0] * ee[1] * ek[1]
                    + beta[2] * ee[1] * ek[1] - beta[2] * ee[2] * ek[2]) * s;
}

//Sign
double GAM2CFFStandard::sgn(double x) const {
    return (x >= 0.) ? (1) : (-1);
}

// Amplitudes 4.L/R and 5.L/R, NLO paper, Eqs. (43)-(44)
// For .R amplitudes, see the comment below Eq. (44) in the NLO paper
// ATTENTION! Terms from the last lines of (43)-(44) are included in M_scale

std::complex<double> GAM2CFFStandard::M4L(double s, double x, double xi,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) {

    std::complex<double> result(0., 0.);

    result += F210(x, xi, beta, s) * Tr_4L_F210(xi, s, beta, ee, ek);

    result += (x + xi) * F201(x, xi, beta, s) * Tr_4L_F201(xi, s, beta, ee, ek);

    result += (x + xi) * F211(x, xi, beta, s) * Tr_4L_F211(xi, s, beta, ee, ek);

    result += F220(x, xi, beta, s) * Tr_4L_F220(xi, s, beta, ee, ek);

    result += (x + xi) * F221(x, xi, beta, s) * Tr_4L_F221(xi, s, beta, ee, ek);

    result += s * F100(x, xi, beta, s) * Tr_4L_F100(xi, s, beta, ee, ek);

    result += s * F110(x, xi, beta, s) * Tr_4L_F110(xi, s, beta, ee, ek);

    result *= -m_CF * m_alphaSOver2Pi / 2.;
    result /= std::pow(s, 3.);
    result /= ((x - xi) * beta[2] - m_iepsilon);

    return result;
}

std::complex<double> GAM2CFFStandard::M4L(double s, double x, double xi,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek, double z) {

    std::complex<double> result(0., 0.);

    result += 2. * (x + xi) * G(x, xi, beta, z) * Tr_4L_G(xi, s, beta, ee, ek);

    result *= -m_CF * m_alphaSOver2Pi / 2.;
    result /= std::pow(s, 3.);
    result /= ((x - xi) * beta[2] - m_iepsilon);

    return result;
}

std::complex<double> GAM2CFFStandard::M5L(double s, double x, double xi,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) {

    std::complex<double> result(0., 0.);

    result = log(((xi - x) / 2. / xi + m_iepsilon * sgn(beta[2])));
    result *= 2. * A(s, beta, ee, ek) / beta[2];
    result /= ((x + xi) * beta[0] + m_iepsilon);

    result += F210(x, xi, beta, s) * Tr_5L_F210(xi, s, beta, ee, ek);

    result += (x + xi) * F201(x, xi, beta, s) * Tr_5L_F201(xi, s, beta, ee, ek);

    result += (x + xi) * F211(x, xi, beta, s) * Tr_5L_F211(xi, s, beta, ee, ek);

    result -= F220(x, xi, beta, s) * Tr_5L_F220(xi, s, beta, ee, ek);

    result -= (x + xi) * F221(x, xi, beta, s) * Tr_5L_F221(xi, s, beta, ee, ek);

    result += s * F100(x, xi, beta, s) * Tr_5L_F100(xi, s, beta, ee, ek);

    result -= s * F110(x, xi, beta, s) * Tr_5L_F110(xi, s, beta, ee, ek);

    result *= -m_CF * m_alphaSOver2Pi / 4.;
    result /= s * s;
    result /= xi;

    return result;
}

std::complex<double> GAM2CFFStandard::M5L(double s, double x, double xi,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek, double z) {

    std::complex<double> result(0., 0.);

    result += 2. * (x + xi) * G(x, xi, beta, z) * Tr_5L_G(xi, s, beta, ee, ek);

    result *= -m_CF * m_alphaSOver2Pi / 4.;
    result /= s * s;
    result /= xi;

    return result;
}

std::complex<double> GAM2CFFStandard::M4R(double s, double x, double xi,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) {

    std::vector<double> betaPrim = beta;
    std::vector<double> eePrim = ee;
    std::vector<double> ekPrim = ek;

    betaPrim[0] = -beta[2];
    betaPrim[1] = -beta[1];
    betaPrim[2] = -beta[0];

    eePrim[0] = ee[2];
    eePrim[2] = ee[0];

    ekPrim[0] = ek[2];
    ekPrim[2] = ek[0];

    return M4L(s, x, -xi, betaPrim, eePrim, ekPrim);
}

std::complex<double> GAM2CFFStandard::M4R(double s, double x, double xi,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek, double z) {

    std::vector<double> betaPrim = beta;
    std::vector<double> eePrim = ee;
    std::vector<double> ekPrim = ek;

    betaPrim[0] = -beta[2];
    betaPrim[1] = -beta[1];
    betaPrim[2] = -beta[0];

    eePrim[0] = ee[2];
    eePrim[2] = ee[0];

    ekPrim[0] = ek[2];
    ekPrim[2] = ek[0];

    return M4L(s, x, -xi, betaPrim, eePrim, ekPrim, z);
}

std::complex<double> GAM2CFFStandard::M5R(double s, double x, double xi,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek) {

    std::vector<double> betaPrim = beta;
    std::vector<double> eePrim = ee;
    std::vector<double> ekPrim = ek;

    betaPrim[0] = -beta[2];
    betaPrim[1] = -beta[1];
    betaPrim[2] = -beta[0];

    eePrim[0] = ee[2];
    eePrim[2] = ee[0];

    ekPrim[0] = ek[2];
    ekPrim[2] = ek[0];

    return M5L(s, x, -xi, betaPrim, eePrim, ekPrim);
}

std::complex<double> GAM2CFFStandard::M5R(double s, double x, double xi,
        const std::vector<double>& beta, const std::vector<double>& ee,
        const std::vector<double>& ek, double z) {

    std::vector<double> betaPrim = beta;
    std::vector<double> eePrim = ee;
    std::vector<double> ekPrim = ek;

    betaPrim[0] = -beta[2];
    betaPrim[1] = -beta[1];
    betaPrim[2] = -beta[0];

    eePrim[0] = ee[2];
    eePrim[2] = ee[0];

    ekPrim[0] = ek[2];
    ekPrim[2] = ek[0];

    return M5L(s, x, -xi, betaPrim, eePrim, ekPrim, z);
}

// Artefact of using a different definition of the hard scale in OG's MSc thesis,
// see the comment after Eq. (44) in NLO paper
std::complex<double> GAM2CFFStandard::M_scale(double s, double x, double xi,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    std::complex<double> result;
    result = (x + xi) / xi
            * log(((xi - x) / 2. / xi + m_iepsilon * sgn(beta[2])));
    result -= (x - xi) / xi
            * log(((xi + x) / 2. / xi + m_iepsilon * sgn(beta[0])));
    result *= M0(s, x, xi, beta, ee, ek);
    result *= -m_CF * m_alphaSOver2Pi / 2. * log(2. * xi);

    return result;
}

// The collinear term.
// For now, it implements formulas (3.105)-(3.106) from OG's MSc thesis
// In particular, it is the contribution from the single permutation of photons!
// Do not mistake it with Eq. (40) in NLO paper.
// TODO re-write it so that it implements Eq. (40) from NLO paper
std::complex<double> GAM2CFFStandard::Ccoll(double s, double x, double xi,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    std::complex<double> result(3., .0);
    result += (x + xi) / xi * log(((xi - x) / 2. / xi + m_iepsilon * sgn(beta[2])));
    result -= (x - xi) / xi * log(((xi + x) / 2. / xi + m_iepsilon * sgn(beta[0])));
    result *= M0(s, x, xi, beta, ee, ek);
    result *= m_CF * m_alphaSOver2Pi / 2.;

    return result;

}

double GAM2CFFStandard::NLO_V_permutation(double x, double z,
        const std::vector<double>& params) {

    std::vector<double> beta(3);

    beta[0] = params[0];
    beta[1] = params[1];
    beta[2] = params[2];

    std::vector<double> ee(3);

    ee[0] = params[3];
    ee[1] = params[4];
    ee[2] = params[5];

    std::vector<double> ek(3);

    ek[0] = params[6];
    ek[1] = params[7];
    ek[2] = params[8];

    double s = params[9];

    bool isReal = bool(params[10]);

    std::complex<double> resultComplex(0., 0.);

    resultComplex += M4L(s, x, m_xi, beta, ee, ek, z);
    resultComplex += M4L(s, -x, m_xi, beta, ee, ek, z);
    resultComplex += M4R(s, x, m_xi, beta, ee, ek, z);
    resultComplex += M4R(s, -x, m_xi, beta, ee, ek, z);

    resultComplex += M5L(s, x, m_xi, beta, ee, ek, z);
    resultComplex += M5L(s, -x, m_xi, beta, ee, ek, z);
    resultComplex += M5R(s, x, m_xi, beta, ee, ek, z);
    resultComplex += M5R(s, -x, m_xi, beta, ee, ek, z);

    return ((isReal) ? (resultComplex.real()) : (resultComplex.imag()));
}

// Vector NLO amplitude - a single permutation of photons
// See the description at the beginning of IV.A in NLO paper
double GAM2CFFStandard::NLO_V_permutation(double x,
    const std::vector<double>& params) {

    std::vector<double> beta(3);

    beta[0] = params[0];
    beta[1] = params[1];
    beta[2] = params[2];

    std::vector<double> ee(3);

    ee[0] = params[3];
    ee[1] = params[4];
    ee[2] = params[5];

    std::vector<double> ek(3);

    ek[0] = params[6];
    ek[1] = params[7];
    ek[2] = params[8];

    double s = params[9];

    bool isReal = bool(params[10]);

    std::complex<double> resultComplex(0., 0.);

    resultComplex += M23LR(s, x, m_xi, beta, ee, ek);
    resultComplex += M23LR(s, -x, m_xi, beta, ee, ek);
    resultComplex += M3M(s, x, m_xi, beta, ee, ek);
    resultComplex += M3M(s, -x, m_xi, beta, ee, ek);

    resultComplex += M4L(s, x, m_xi, beta, ee, ek);
    resultComplex += M4L(s, -x, m_xi, beta, ee, ek);
    resultComplex += M4R(s, x, m_xi, beta, ee, ek);
    resultComplex += M4R(s, -x, m_xi, beta, ee, ek);

    resultComplex += M5L(s, x, m_xi, beta, ee, ek);
    resultComplex += M5L(s, -x, m_xi, beta, ee, ek);
    resultComplex += M5R(s, x, m_xi, beta, ee, ek);
    resultComplex += M5R(s, -x, m_xi, beta, ee, ek);

    resultComplex += M_scale(s, x, m_xi, beta, ee, ek);
    resultComplex += M_scale(s, -x, m_xi, beta, ee, ek);

    resultComplex += log(2. * m_xi * s / m_MuF2) * Ccoll(s, x, m_xi, beta, ee, ek);
    resultComplex += log(2. * m_xi * s / m_MuF2) * Ccoll(s, -x, m_xi, beta, ee, ek);

    return ((isReal) ? (resultComplex.real()) : (resultComplex.imag()));
}

double GAM2CFFStandard::computeCubedChargeAveragedGPD(
        const PartonDistribution &partonDistribution) {
    double result = 0.;

    result +=
            (partonDistribution.getQuarkDistribution(QuarkFlavor::UP).getQuarkDistributionMinus()) // Minus or Plus?
            * Constant::U2_ELEC_CHARGE * Constant::U_ELEC_CHARGE;

    result +=
            (partonDistribution.getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistributionMinus())
                    * Constant::D2_ELEC_CHARGE * Constant::D_ELEC_CHARGE;
    result +=
            (partonDistribution.getQuarkDistribution(QuarkFlavor::STRANGE).getQuarkDistributionMinus())
                    * Constant::S2_ELEC_CHARGE * Constant::S_ELEC_CHARGE;

    return result;
}

double GAM2CFFStandard::NLO_V(double x,
        const std::vector<double>& params) {

    /*
     * Vector params consists of:
     * 3 parameters beta_i
     * 3 parameters eij
     * ee[0] = epsilon2 * epsilon3
     * ee[1] = epsilon1 * epsilon3
     * ee[2] = epsilon1 * epsilon2
     * 3 parameters ek_i (explained below)
     * Parameter s
     */

    double beta0[3];
    for (int i = 0; i < 3; i++) {
        beta0[i] = params[i];
    }

    double ee0[3];
    for (int i = 0; i < 3; i++) {
        ee0[i] = params[3 + i];
    }

    /*
     * See definitions in Appendix A in NLO paper
     *
     * ek[i][j] = epsilon_i * k_j
     * where
     * 0 - incoming photon
     * 1 - outgoing photon 1
     * 2 - outgoing photon 2
     * I use ek[i][j] = -ek[i][k] where all i,j,k are different
     * ek[0] = e1 * k2
     * ek[1] = e2 * k1
     * ek[2] = e3 * k1
     */
    double ek0[3][3];

    ek0[0][0] = 0.;
    ek0[1][1] = 0.;
    ek0[2][2] = 0.;
    ek0[0][1] = params[6];
    ek0[0][2] = -params[6];
    ek0[1][0] = params[7];
    ek0[1][2] = -params[7];
    ek0[2][0] = params[8];
    ek0[2][1] = -params[8];

    double s0 = params[9];

    //take either real or imaginary part
    bool isReal0 = bool(params[10]);

    //result
    double result = 0.;

    /*
     * Consider all 6 permutations of photons
     */

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == j)
                continue;
            for (int k = 0; k < 3; k++) {
                if (k == i || k == j)
                    continue;

                std::vector<double> thisParams(11);

                thisParams[0] = beta0[i];
                thisParams[1] = beta0[j];
                thisParams[2] = beta0[k];

                thisParams[3] = ee0[i];
                thisParams[4] = ee0[j];
                thisParams[5] = ee0[k];

                thisParams[6] = ek0[i][j];
                thisParams[7] = ek0[j][i];
                thisParams[8] = ek0[k][i];

                thisParams[9] = s0;

                thisParams[10] = double(isReal0);

                result += NLO_V_permutation(x, thisParams);
            }
        }
    }

    return result;
}

double GAM2CFFStandard::NLO_V(double x, double z,
        const std::vector<double>& params) {

    /*
     * Vector params consists of:
     * 3 parameters beta_i
     * 3 parameters eij
     * ee[0] = epsilon2 * epsilon3
     * ee[1] = epsilon1 * epsilon3
     * ee[2] = epsilon1 * epsilon2
     * 3 parameters ek_i (explained below)
     * Parameter s
     */

    double beta0[3];
    for (int i = 0; i < 3; i++) {
        beta0[i] = params[i];
    }

    double ee0[3];
    for (int i = 0; i < 3; i++) {
        ee0[i] = params[3 + i];
    }

    /*
     * See definitions in Appendix A in NLO paper
     *
     * ek[i][j] = epsilon_i * k_j
     * where
     * 0 - incoming photon
     * 1 - outgoing photon 1
     * 2 - outgoing photon 2
     * I use ek[i][j] = -ek[i][k] where all i,j,k are different
     * ek[0] = e1 * k2
     * ek[1] = e2 * k1
     * ek[2] = e3 * k1
     */
    double ek0[3][3];

    ek0[0][0] = 0.;
    ek0[1][1] = 0.;
    ek0[2][2] = 0.;
    ek0[0][1] = params[6];
    ek0[0][2] = -params[6];
    ek0[1][0] = params[7];
    ek0[1][2] = -params[7];
    ek0[2][0] = params[8];
    ek0[2][1] = -params[8];

    double s0 = params[9];

    //take either real or imaginary part
    bool isReal0 = bool(params[10]);

    //result
    double result = 0.;

    /*
     * Consider all 6 permutations of photons
     */

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == j)
                continue;
            for (int k = 0; k < 3; k++) {
                if (k == i || k == j)
                    continue;

                std::vector<double> thisParams(12);

                thisParams[0] = beta0[i];
                thisParams[1] = beta0[j];
                thisParams[2] = beta0[k];

                thisParams[3] = ee0[i];
                thisParams[4] = ee0[j];
                thisParams[5] = ee0[k];

                thisParams[6] = ek0[i][j];
                thisParams[7] = ek0[j][i];
                thisParams[8] = ek0[k][i];

                thisParams[9] = s0;

                thisParams[10] = double(isReal0);

                result += NLO_V_permutation(x, z, thisParams);
            }
        }
    }

    return result;
}

double GAM2CFFStandard::Convol_NLO_V_x(double x, const std::vector<double>& params) {

    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    double Convol = NLO_V(x, params)
            * computeCubedChargeAveragedGPD(partonDistribution);

    if (std::isnan(Convol) || std::isinf(Convol)) {

        warn(__func__,
                ElemUtils::Formatter() << "Value is either nan or inf, x = "
                        << x);
        return 0.;
    }

    return Convol;
}

double GAM2CFFStandard::Convol_NLO_V_xz(double x, double z, const std::vector<double>& params) {

    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    double Convol = NLO_V(x, z, params)
            * computeCubedChargeAveragedGPD(partonDistribution);

    if (std::isnan(Convol) || std::isinf(Convol)) {

        warn(__func__,
                ElemUtils::Formatter() << "Value is either nan or inf, x = "
                        << x << ", z = " << z);
        return 0.;
    }

    return Convol;
}

double GAM2CFFStandard::Convol_NLO_V_x_Sym(double x,
        const std::vector<double>& params) {

    double x0 = params.at(11);
    double gpdXXi = params.at(12);

    PartonDistribution partonDistributionA = m_pGPDModule->compute(
            GPDKinematic(x0 - x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);
    PartonDistribution partonDistributionB = m_pGPDModule->compute(
            GPDKinematic(x0 + x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    double EvalGPDA = computeCubedChargeAveragedGPD(partonDistributionA)
            - gpdXXi;
    double EvalGPDB = computeCubedChargeAveragedGPD(partonDistributionB)
            - gpdXXi;

    double Convol = NLO_V(x0 - x, params) * EvalGPDA
            + NLO_V(x0 + x, params) * EvalGPDB;

    if (std::isnan(Convol) || std::isinf(Convol)) {

        warn(__func__,
                ElemUtils::Formatter() << "Value is either nan or inf, x = "
                        << x);
        return 0.;
    }

    return Convol;
}

double GAM2CFFStandard::Convol_NLO_V_x_Sym_Const(double x,
        const std::vector<double>& params) {

    double x0 = params.at(11);
    double gpdXXi = params.at(12);

    double EvalGPDA = gpdXXi;
    double EvalGPDB = gpdXXi;

    double Convol = NLO_V(x0 - x, params) * EvalGPDA
            + NLO_V(x0 + x, params) * EvalGPDB;

    if (std::isnan(Convol) || std::isinf(Convol)) {

        warn(__func__,
                ElemUtils::Formatter() << "Value is either nan or inf, x = "
                        << x);
        return 0.;
    }

    return Convol;
}

std::complex<double> GAM2CFFStandard::computeUnpolarized() {

    //variables to use
//    m_currentGPDComputeType;
//    m_pRunningAlphaStrongModule;
//    m_pGPDModule;
//    m_qcdOrderType;
//    m_xi;
//    m_t;
//    m_uPrim;
//    m_Mgg2;
//    m_MuF2;
//    m_MuR2;
//    m_polG0;
//    m_polG1;
//    m_polG2;

    if (m_qcdOrderType != PerturbativeQCDOrderType::LO
            && m_qcdOrderType != PerturbativeQCDOrderType::NLO) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Erroneous input perturbative QCD order can only be LO or NLO. Here Order = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }

    double tPrim = m_t - m_Mgg2 - m_uPrim; // MINUS uPrim
    double tau = 2. * m_xi / (1. + m_xi);
    double M2 = Constant::PROTON_MASS * Constant::PROTON_MASS;
    double s = (m_Mgg2 - m_t) / tau / (1. + m_xi);

    double pt2 = -(m_uPrim * tPrim) / (m_uPrim + tPrim);
    double alpha = m_uPrim / (m_uPrim + tPrim);
    double alphabar = 1. - alpha;

//    double alpha = -m_uPrim
//            * (1 / (2 * m_xi * s) - M2 / (s * s * (1 - m_xi * m_xi)));
//    double alphabar = 1 - alpha - M2 / s * 2. * m_xi / (1 - m_xi * m_xi);
//    double pt2 = -m_uPrim * alphabar;

    // beta_i is defined by 2pk_i = beta_i * s
    // {k_i} are the following: {q, -q_1, -q_2)
    // All permutations of {k_i} have to be considered in the amplitude

    std::vector<double> Parameters;

    Parameters.push_back(1.);
    Parameters.push_back(-1. * alpha);
    Parameters.push_back(-1. * alphabar);

    // ee[i] is defined by ee[i] = epsilon_j * epsilon_k, where all i,j,k are different
    // A different notation is used in the paper, here ee[0] = e23, ee[1] = e13, ee[2] = e12

    Parameters.push_back(-double(m_polG1 == m_polG2));

    if(m_phiDep == false){

        Parameters.push_back(-double(m_polG0 == m_polG2));
        Parameters.push_back(-double(m_polG1 == m_polG0));
    }else{

        Parameters.push_back(-double(PolarizationType::LIN_TRANS_X_PLUS == m_polG2) * cos(m_phi)
            - double(PolarizationType::LIN_TRANS_Y_MINUS == m_polG2) * sin(m_phi) );
        Parameters.push_back(-double(PolarizationType::LIN_TRANS_X_PLUS == m_polG1) * cos(m_phi)
            - double(PolarizationType::LIN_TRANS_Y_MINUS == m_polG1) * sin(m_phi) );
    }

    // ek0[i][j] = epsilon_i * k_j
    // This matrix will be used to make the vector ek[i] for a given permutation
    // See Eq. 16

    if(m_phiDep == false){
        Parameters.push_back(
            sqrt(pt2) * double(m_polG0 == PolarizationType::LIN_TRANS_X_PLUS));
    }else{
        Parameters.push_back(sqrt(pt2) * cos(m_phi) ); 
    }

    Parameters.push_back(
            sqrt(pt2) * double(m_polG1 == PolarizationType::LIN_TRANS_X_PLUS)
                    / alpha);
    Parameters.push_back(
            -sqrt(pt2) * double(m_polG2 == PolarizationType::LIN_TRANS_X_PLUS)
                    / alphabar);

    Parameters.push_back(s);

    Parameters.push_back(0.);

    double result_Re = 0.;
    double result_Im = 0.;

    // LO part
    computeDiagonalGPD_V();

    if(m_reim == 1 || m_reim == 2){

    if(m_phiDep == false){

        result_Im = (alpha - alphabar) * double(m_polG1 == m_polG2)
            * double(m_polG0 == PolarizationType::LIN_TRANS_X_PLUS);
        result_Im -= double(m_polG0 == m_polG2)
            * double(m_polG1 == PolarizationType::LIN_TRANS_X_PLUS);
        result_Im += double(m_polG1 == m_polG0)
            * double(m_polG2 == PolarizationType::LIN_TRANS_X_PLUS);
    }else{

        result_Im = (alpha - alphabar) * double(m_polG1 == m_polG2)
            * cos(m_phi);
        result_Im -= (double(PolarizationType::LIN_TRANS_X_PLUS == m_polG2) * cos(m_phi)
            + double(PolarizationType::LIN_TRANS_Y_MINUS == m_polG2) * sin(m_phi) )
            * double(m_polG1 == PolarizationType::LIN_TRANS_X_PLUS);
        result_Im += (double(PolarizationType::LIN_TRANS_X_PLUS == m_polG1) * cos(m_phi) 
            + double(PolarizationType::LIN_TRANS_Y_MINUS == m_polG1) * sin(m_phi) )
            * double(m_polG2 == PolarizationType::LIN_TRANS_X_PLUS);
    }
   
        result_Im *= sqrt(pt2);
        result_Im *= m_quark_diagonal_V;
        result_Im *= -2. * Constant::PI / s / alpha / alphabar / m_xi;
    }

    if (m_qcdOrderType == PerturbativeQCDOrderType::NLO) {

        std::vector<double> range;

        range.push_back(0.);
        range.push_back(1. - 1.E-3);
        range.push_back(m_xi);
        range.push_back(m_xi * (1. + alpha) / (1. - alpha));
        range.push_back(m_xi * (2. - alpha) / alpha);
        range.push_back(m_xi * (1. - 2 * alpha));

        for (std::vector<double>::iterator it = range.begin();
                it != range.end(); it++) {

            if ((*it) < 0.)
                (*it) = fabs(*it);

            if ((*it) > 1.) {
                it = range.erase(it);
                it--;
            }
        }

        std::sort(range.begin(), range.end());

        if(m_reim == 0 || m_reim == 2){
            Parameters.at(10) = double(true);
            result_Re += gslIntegrationWrapper(m_pConvol_NLO_V_x,
                m_pConvol_NLO_V_x_Sym, m_pConvol_NLO_V_x_Sym_Const, range,
                Parameters);
        }

        if(m_reim == 1 || m_reim == 2){
            Parameters.at(10) = double(false);
            result_Im += gslIntegrationWrapper(m_pConvol_NLO_V_x,
                m_pConvol_NLO_V_x_Sym, m_pConvol_NLO_V_x_Sym_Const, range,
                Parameters);
        }
    }

    return std::complex<double>(result_Re, result_Im);
}

std::complex<double> GAM2CFFStandard::computePolarized() {
    return std::complex<double>();
}

RunningAlphaStrongModule* GAM2CFFStandard::getRunningAlphaStrongModule() const {
    return m_pRunningAlphaStrongModule;
}

void GAM2CFFStandard::setRunningAlphaStrongModule(
        RunningAlphaStrongModule* pRunningAlphaStrongModule) {
    m_pModuleObjectFactory->updateModulePointerReference(
            m_pRunningAlphaStrongModule, pRunningAlphaStrongModule);
    m_pRunningAlphaStrongModule = pRunningAlphaStrongModule;
}

double GAM2CFFStandardIntegrationFunction(double* x, size_t dim, void* p) {

    GAM2CFFStandardIntegrationParameters* par =
            static_cast<GAM2CFFStandardIntegrationParameters*>(p);

    return par->m_GAM2CFFStandardGAM2CFFStandard->Convol_NLO_V_xz(x[0], x[1], *(par->m_parameters));
}

double GAM2CFFStandard::gslIntegrationWrapper(NumA::FunctionType1D* functor,
        NumA::FunctionType1D* functorSym, NumA::FunctionType1D* functorSymConst,
        const std::vector<double>& limits, const std::vector<double>& imputParams) {

    //result
    double result = 0.;

    //G

    GAM2CFFStandardIntegrationParameters gam2CFFStandardIntegrationParameters;

    gam2CFFStandardIntegrationParameters.m_GAM2CFFStandardGAM2CFFStandard =
            this;
    gam2CFFStandardIntegrationParameters.m_parameters = &imputParams;

    gsl_monte_function F;
    F.f = &GAM2CFFStandardIntegrationFunction;
    F.dim = 2;
    F.params = &gam2CFFStandardIntegrationParameters;

    double res, err;

    double xl[] = { 0., 0. };
    double xu[] = { 1., 1. };

    gsl_rng_env_setup();
    const gsl_rng_type* T = gsl_rng_default;
    gsl_rng* r = gsl_rng_alloc(T);

    size_t calls = 1000000;

    gsl_monte_vegas_state* s = gsl_monte_vegas_alloc(2);

    for(size_t i = 0; i < 2; i++){

        gsl_monte_vegas_integrate(&F, xl, xu, 2, calls, r, s, &res, &err);

        std::cout << "DEBUG: G integral (loop): " << i << ": chi2: " <<
            gsl_monte_vegas_chisq(s) << " res: "<< res << " +/- " << err << std::endl;

        if(fabs(gsl_monte_vegas_chisq(s)) == 0.) break; //break if 0 integral
        if(fabs(gsl_monte_vegas_chisq(s) - 1.) < 0.5 && fabs(err/res) < 0.02) break; //accept <2% rel. precision
    }

    gsl_monte_vegas_free(s);
    gsl_rng_free(r);

    std::cout << "DEBUG: G integral: " << res << " +/- " << err << std::endl;

    result += res;

    //copy parameters
    std::vector<double> params = imputParams;

    //prepare for symmetric x0
    params.push_back(0.);

    //prepare for symmetric gpd at x=xi
    params.push_back(0.);

    //limits
    std::vector<std::pair<double, double> > limitsSym;

    //loop
    for (size_t i = 1; i < limits.size() - 1; i++) {

        double range =
                (fabs(limits.at(i) - limits.at(i - 1))
                        < fabs(limits.at(i) - limits.at(i + 1))) ?
                        (fabs(limits.at(i) - limits.at(i - 1))) :
                        (fabs(limits.at(i) - limits.at(i + 1)));

        range /= 10.;

        limitsSym.push_back(
                std::make_pair(limits.at(i) - range, limits.at(i) + range));

        params.at(11) = limits.at(i);
        params.at(12) = computeCubedChargeAveragedGPD(
                m_pGPDModule->compute(
                        GPDKinematic(limits.at(i), m_xi, m_t, m_MuF2, m_MuR2),
                        m_currentGPDComputeType));

        double thisResult = integrate(functorSym, 0., range, params);

        std::cout << "DEBUG: range: " << limits.at(i) - range << " -- "
                << limits.at(i) + range << " (S: " << limits.at(i) << ") "
                << " result: " << thisResult << std::endl;

        result += thisResult;

        thisResult = integrate(functorSymConst, 0., range, params);

        std::cout << "DEBUG: range: " << limits.at(i) - range << " -- "
                << limits.at(i) + range << " (SC: " << limits.at(i) << ") "
                << " result: " << thisResult << std::endl;

        result += thisResult;
    }

    for (size_t i = 0; i <= limitsSym.size(); i++) {

        double min, max;

        if (i == 0) {
            min = limits.at(0);
            max = limitsSym.at(0).first;
        } else if (i == limitsSym.size()) {
            min = limitsSym.at(limitsSym.size() - 1).second;
            max = limits.at(limits.size() - 1);
        } else {
            min = limitsSym.at(i - 1).second;
            max = limitsSym.at(i).first;
        }

        double thisResult = integrate(functor, min, max, params);

        std::cout << "DEBUG: range: " << min << " -- " << max << " result: "
                << thisResult << std::endl;

        result += thisResult;
    }

    std::cout << "DEBUG result total: " << result << std::endl;

    return result;
}

std::complex<double> GAM2CFFStandard::F100(double x, double xi,
        const std::vector<double>& beta, double s) {

    std::complex<double> a = ((x + xi) * beta[0] + m_iepsilon);
    std::complex<double> b = ((2. * xi * beta[2]) + m_iepsilon);
    std::complex<double> c = ((x + xi) * beta[1]) + m_iepsilon;

    std::complex<double> result(0., 0.);

    result += std::log(b / a) * std::log((a + c) / a);
    result += -std::log(((a + c) * (b + c)) / (a * b))
            * std::log((a + b + c) / a);
    result += m_lisk.Li(2, -(c / a)) + m_lisk.Li(2, -(c / b));
    result += -m_lisk.Li(2, -((c * (a + b + c)) / (a * b)));
    result /= -c;

    return result;
}

std::complex<double> GAM2CFFStandard::F110(double x, double xi,
        const std::vector<double>& beta, double s) {

    std::complex<double> a = ((x + xi) * beta[0] + m_iepsilon);
    std::complex<double> b = ((2. * xi * beta[2]) + m_iepsilon);
    std::complex<double> c = ((x + xi) * beta[1]) + m_iepsilon;

    std::complex<double> result(0., 0.);
    std::complex<double> aux(0., 0.);

    aux = -c + (b + c) * std::log((b + c) / b);
    aux *= a * std::log(a / b);

    result += aux;

    aux = c * (a + b + c);
    aux += -a * (b + c) * std::log(((a + c) * (b + c)) / (a * b));
    aux *= std::log((a + b + c) / b);

    result += aux;

    aux = m_lisk.Li(2, -(c / a)) + m_lisk.Li(2, -(c / b));
    aux += -m_lisk.Li(2, -((c * (a + b + c)) / (a * b)));
    aux *= a * (b + c);

    result += aux;

    result /= c * c * (b + c);

    return result;
}

std::complex<double> GAM2CFFStandard::F201(double x, double xi,
        const std::vector<double>& beta, double s) {

    return std::log((-(x - xi) * beta[2] + m_iepsilon) / (2. * xi * beta[2]))
            / ((x + xi) * beta[0] + m_iepsilon) / (-(x + xi) * beta[2] + m_iepsilon);
}

std::complex<double> GAM2CFFStandard::F210(double x, double xi,
        const std::vector<double>& beta, double s) {

    return std::log(
            (-(x - xi) * beta[2] + m_iepsilon) / ((x + xi) * beta[0] + m_iepsilon))
            / (2. * xi * beta[2])
            / ((x + xi) * beta[1] + 2. * xi * beta[2] + m_iepsilon);
}

std::complex<double> GAM2CFFStandard::F211(double x, double xi,
        const std::vector<double>& beta, double s) {

    std::complex<double> a = ((x + xi) * beta[0] + m_iepsilon);
    std::complex<double> b = ((2. * xi * beta[2]) + m_iepsilon);
    std::complex<double> c = ((x + xi) * beta[1]) + m_iepsilon;

    return -((std::log((a + c) / a)
            + std::log(b / a) * (-(c / (a + c)) + std::log((a + c) / a))
            + std::log((b + c) / b)
            + (c * (a + b + 2. * c) * std::log((a + b + c) / a))
                    / ((a + c) * (b + c))
            - std::log(((a + c) * (b + c)) / (a * b))
                    * (1. + std::log((a + b + c) / a)) + m_lisk.Li(2, -(c / a))
            + m_lisk.Li(2, -(c / b))
            - m_lisk.Li(2, -((c * (a + b + c)) / (a * b)))) / std::pow(c, 2));
}

std::complex<double> GAM2CFFStandard::F220(double x, double xi,
        const std::vector<double>& beta, double s) {

    std::complex<double> a = ((x + xi) * beta[0] + m_iepsilon);
    std::complex<double> b = ((2. * xi * beta[2]) + m_iepsilon);
    std::complex<double> c = ((x + xi) * beta[1]) + m_iepsilon;

    return (b + c + a * std::log(a / b) - a * std::log((a + b + c) / b))
            / (b * std::pow(b + c, 2.));
}

std::complex<double> GAM2CFFStandard::F221(double x, double xi,
        const std::vector<double>& beta, double s) {

    std::complex<double> a = ((x + xi) * beta[0] + m_iepsilon);
    std::complex<double> b = ((2. * xi * beta[2]) + m_iepsilon);
    std::complex<double> c = ((x + xi) * beta[1]) + m_iepsilon;

    return (-(std::pow(c, 2) * (b + c))
            + a * std::log(a / b)
                    * (-(c * (2. * b + 3. * c))
                            + 2. * std::pow(b + c, 2) * std::log((b + c) / b))
            + ((c
                    * (c * std::pow(b + c, 2)
                            + std::pow(a, 2) * (2. * b + 3. * c)
                            + a
                                    * (2. * std::pow(b, 2) + 6. * b * c
                                            + 5. * std::pow(c, 2)))
                    - 2. * a * (a + c) * std::pow(b + c, 2)
                            * std::log(((a + c) * (b + c)) / (a * b)))
                    * std::log((a + b + c) / b)) / (a + c)
            + 2. * a * std::pow(b + c, 2)
                    * (m_lisk.Li(2, -(c / a)) + m_lisk.Li(2, -(c / b))
                            - m_lisk.Li(2, -((c * (a + b + c)) / (a * b)))))
            / (std::pow(c, 3) * std::pow(b + c, 2));
}

std::complex<double> GAM2CFFStandard::G(double x, double xi,
        const std::vector<double>& beta, double z) {

    const double eps = m_iepsilon.imag();

    const double a = (x + xi) * beta.at(0);
    const double b = (x + xi) * beta.at(1);
    const double c = 2. * xi * beta.at(2);

    const std::complex<double> c_I(0., 1.);

    return (-1. * c_I * z
            * (c + b * z
                    + (c + c_I * eps + (a + b) * z)
                            * log(-1. * c_I * eps - a * z)
                    + (-1. * c_I * eps - a * z)
                            * log(-c - c_I * eps - (a + b) * z)))
            / ((eps - c_I * a * z) * (c + b * z) * (c + c_I * eps + (a + b) * z));
}

void GAM2CFFStandard::setIEpsilon(double iEps) {
    m_iepsilon = std::complex<double>(0., iEps);
}

double GAM2CFFStandard::getIEpsilon() const {
    return m_iepsilon.imag();
}

void GAM2CFFStandard::setReIm(int reim) {
        m_reim = reim; 
}

int GAM2CFFStandard::getReIm() const {
        return m_reim;
}

void GAM2CFFStandard::setPhiDep(bool phiDep) {
        m_phiDep = phiDep; 
}

bool GAM2CFFStandard::getPhiDep() const {
        return m_phiDep;
}

} /* namespace PARTONS */
