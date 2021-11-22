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

    initFunctorsForIntegrations();

  //  m_lisk = LiSK::LiSK<std::complex<double> >(3);
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

    initFunctorsForIntegrations();

   // m_lisk = LiSK::LiSK<std::complex<double> >(3);
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

    m_pConvol_NLO_V_Re = NumA::Integrator1D::newIntegrationFunctor(this,
            &GAM2CFFStandard::Convol_NLO_V_Re);
    m_pConvol_NLO_V_Im = NumA::Integrator1D::newIntegrationFunctor(this,
            &GAM2CFFStandard::Convol_NLO_V_Im);

    m_pConvol_NLO_V_Re_Sym = NumA::Integrator1D::newIntegrationFunctor(this,
            &GAM2CFFStandard::Convol_NLO_V_Re_Sym);
    m_pConvol_NLO_V_Im_Sym = NumA::Integrator1D::newIntegrationFunctor(this,
            &GAM2CFFStandard::Convol_NLO_V_Im_Sym);

    m_pConvol_NLO_V_Re_Sym_Const = NumA::Integrator1D::newIntegrationFunctor(
            this, &GAM2CFFStandard::Convol_NLO_V_Re_Sym_Const);
    m_pConvol_NLO_V_Im_Sym_Const = NumA::Integrator1D::newIntegrationFunctor(
            this, &GAM2CFFStandard::Convol_NLO_V_Im_Sym_Const);
}

GAM2CFFStandard::~GAM2CFFStandard() {

    if (m_pRunningAlphaStrongModule) {
        setRunningAlphaStrongModule(0);
        m_pRunningAlphaStrongModule = 0;
    }

    if (m_pConvol_NLO_V_Re) {
        delete m_pConvol_NLO_V_Re;
        m_pConvol_NLO_V_Re = 0;
    }

    if (m_pConvol_NLO_V_Im) {
        delete m_pConvol_NLO_V_Im;
        m_pConvol_NLO_V_Im = 0;
    }

    if (m_pConvol_NLO_V_Re_Sym) {
        delete m_pConvol_NLO_V_Re_Sym;
        m_pConvol_NLO_V_Re_Sym = 0;
    }

    if (m_pConvol_NLO_V_Im_Sym) {
        delete m_pConvol_NLO_V_Im_Sym;
        m_pConvol_NLO_V_Im_Sym = 0;
    }

    if (m_pConvol_NLO_V_Re_Sym_Const) {
        delete m_pConvol_NLO_V_Re_Sym_Const;
        m_pConvol_NLO_V_Re_Sym_Const = 0;
    }

    if (m_pConvol_NLO_V_Im_Sym_Const) {
        delete m_pConvol_NLO_V_Im_Sym_Const;
        m_pConvol_NLO_V_Im_Sym_Const = 0;
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

std::complex<double> iepsilon(0., 1.E-5); // infinitesimal part inserted 'by hand'

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

// Trace \mathcal{A}, see Eq. 25
double GAM2CFFStandard::A(double s, std::vector<double> beta,
        std::vector<double> ee, std::vector<double> ek) {
    return s
            * (beta[0] * (ee[0] * ek[0] - ee[1] * ek[1])
                    + beta[2] * (ee[1] * ek[1] - ee[2] * ek[2])) / 8.;
}

std::complex<double> GAM2CFFStandard::M0(double s, double x, double xi,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    std::complex<double> result;
    result = -4. / s / s;
    result /= ((x + xi) * beta[0] + iepsilon) * ((x - xi) * beta[2] - iepsilon);
    result *= A(s, beta, ee, ek);

    return result;
}

std::complex<double> GAM2CFFStandard::M23LR(double s, double x, double xi,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {
    std::complex<double> result;
    result = -5.;
    result += std::log((-(x + xi) * beta[0] / 2. / xi - iepsilon));
    result += std::log(((x - xi) * beta[2] / 2. / xi - iepsilon));
    result *= M0(s, x, xi, beta, ee, ek);
    result *= m_CF * m_alphaSOver2Pi;

    return result;
}

// Eq. 49
std::complex<double> GAM2CFFStandard::M3M(double s, double x, double xi,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {
    std::complex<double> result(0., 0.);

    std::complex<double> aux(0., 0.);

    aux += std::log((-(x - xi) * beta[2] + iepsilon))
            - std::log(((x + xi) * beta[0] + iepsilon));
    aux *= 2. * (x + xi) * beta[0] + (x - xi) * beta[2];
    aux += (x + xi) * beta[0] + (x - xi) * beta[2];
    aux *= 0.5 * s
            * (beta[0] * ee[1] * ek[1] - beta[0] * ee[0] * ek[0]
                    + beta[2] * ee[2] * ek[2] + beta[2] * ee[1] * ek[1]);
    aux *= (x + xi) * beta[0];
    aux /= (x + xi) * beta[0] + (x - xi) * beta[2];
    aux /= (x + xi) * beta[0] + (x - xi) * beta[2];

    result += aux;

    aux = -std::log((-(x - xi) * beta[2] + iepsilon))
            + std::log(((x + xi) * beta[0] + iepsilon));
    aux *= (x + xi) * beta[0] + 2. * (x - xi) * beta[2];
    aux += (x + xi) * beta[0] + (x - xi) * beta[2];
    aux *= 0.5 * s
            * (beta[0] * ee[1] * ek[1] + beta[0] * ee[0] * ek[0]
                    - beta[2] * ee[2] * ek[2] + beta[2] * ee[1] * ek[1]);
    aux *= (x - xi) * beta[2];
    aux /= (x + xi) * beta[0] + (x - xi) * beta[2];
    aux /= (x + xi) * beta[0] + (x - xi) * beta[2];

    result -= aux;

    aux = -(x - xi) * beta[2]
            * std::log(((x - xi) * beta[2] / 2. / xi - iepsilon));
    aux -= (x + xi) * beta[0]
            * std::log((-(x + xi) * beta[0] / 2. / xi - iepsilon));
    aux /= (x + xi) * beta[0] + (x - xi) * beta[2];
    aux += 1.;
    aux *= A(s, beta, ee, ek);

    result += aux;

    result *= -m_CF * m_alphaSOver2Pi / 2. / s / s;
    result /= ((x + xi) * beta[0] + iepsilon) * ((x - xi) * beta[2] - iepsilon);

    return result;
}

// Trace structures in amplitudes 4.L and 5.L - Eqs. 50 and 51
// Tr_4/5L_Fnab stands by the function F(n, a, b, ...)

double GAM2CFFStandard::Tr_4L_F210(double xi, double s,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

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
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

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
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    return -2.
            * (2. * std::pow(beta[1], 2.) * (ee[0] * ek[0] - ee[1] * ek[1])
                    + beta[1] * beta[2]
                            * (2. * ee[0] * ek[0] + 3. * ee[1] * ek[1]
                                    - 3. * ee[2] * ek[2])
                    + beta[0] * beta[2] * (ee[0] * ek[0] - ee[2] * ek[2]))
            * std::pow(s, 2.);
}

double GAM2CFFStandard::Tr_4L_F220(double xi, double s,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    return 4. * beta[2] * s
            * (2. * ek[0] * ek[1] * ek[2]
                    + (beta[1] + beta[2]) * ee[0] * ek[0] * s * xi
                    - (beta[1] * ee[1] * ek[1] + beta[2] * ee[2] * ek[2]) * s
                            * xi);
}

double GAM2CFFStandard::Tr_4L_F221(double xi, double s,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    return 2. * beta[1]
            * (beta[1] * ee[0] * ek[0] + beta[2] * ee[0] * ek[0]
                    - beta[1] * ee[1] * ek[1] - beta[2] * ee[2] * ek[2])
            * std::pow(s, 2.);
}

double GAM2CFFStandard::Tr_4L_F100(double xi, double s,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    return 2.
            * (beta[0] * ee[0] * ek[0] + 2 * beta[1] * ee[0] * ek[0]
                    - 2. * beta[2] * ee[0] * ek[0] - beta[0] * ee[1] * ek[1]
                    - 2. * beta[1] * ee[1] * ek[1]
                    + 3. * beta[2] * ee[1] * ek[1]
                    - 3. * beta[2] * ee[2] * ek[2]) * s;
}

double GAM2CFFStandard::Tr_4L_F110(double xi, double s,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    return 2.
            * (-3. * beta[1] * ee[0] * ek[0] - beta[2] * ee[0] * ek[0]
                    + 3. * beta[1] * ee[1] * ek[1] + beta[2] * ee[2] * ek[2])
            * s;
}

double GAM2CFFStandard::Tr_4L_G(double xi, double s, std::vector<double> beta,
        std::vector<double> ee, std::vector<double> ek) {

    return 2. * beta[2]
            * (beta[0] * ee[0] * ek[0] - beta[0] * ee[1] * ek[1]
                    + beta[2] * ee[1] * ek[1] - beta[2] * ee[2] * ek[2])
            * std::pow(s, 2.);
}

double GAM2CFFStandard::Tr_5L_F201(double xi, double s,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    return 4.
            * (3. * beta[0] * ee[0] * ek[0] - 5. * beta[0] * ee[1] * ek[1]
                    - beta[1] * ee[1] * ek[1] + 2. * beta[0] * ee[2] * ek[2]
                    + beta[1] * ee[2] * ek[2]) * s;
}

double GAM2CFFStandard::Tr_5L_F210(double xi, double s,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    return 8. * ek[1] * (ek[0] * ek[2] - beta[2] * ee[1] * s * xi)
            - 24. * ek[2] * (ek[0] * ek[1] - beta[2] * ee[2] * s * xi);
}

double GAM2CFFStandard::Tr_5L_F211(double xi, double s,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    return 4.
            * (-(beta[0] * ee[0] * ek[0]) - 3. * beta[1] * ee[1] * ek[1]
                    + beta[0] * ee[2] * ek[2] + 3. * beta[1] * ee[2] * ek[2])
            * s;
}

double GAM2CFFStandard::Tr_5L_F220(double xi, double s,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    return -8 * ek[0] * (ek[1] * ek[2] + beta[2] * ee[0] * s * xi)
            - 8 * ek[2] * (ek[0] * ek[1] - beta[2] * ee[2] * s * xi); // 05.10.2021 modified
}

double GAM2CFFStandard::Tr_5L_F221(double xi, double s,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    return -4. * beta[1] * ee[0] * ek[0] * s + 4. * beta[1] * ee[2] * ek[2] * s;
}

double GAM2CFFStandard::Tr_5L_F100(double xi, double s,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    return -4. * (2 * ee[0] * ek[0] - 3. * ee[1] * ek[1] + 3. * ee[2] * ek[2]);
}

double GAM2CFFStandard::Tr_5L_F110(double xi, double s,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    return 4. * (ee[0] * ek[0] - ee[2] * ek[2]);
}

double GAM2CFFStandard::Tr_5L_G(double xi, double s, std::vector<double> beta,
        std::vector<double> ee, std::vector<double> ek) {

    return -2.
            * (beta[0] * ee[0] * ek[0] - beta[0] * ee[1] * ek[1]
                    + beta[2] * ee[1] * ek[1] - beta[2] * ee[2] * ek[2]) * s;
}



// Functions F(n, a, b, ...)

std::complex<double> GAM2CFFStandard::F100(std::complex<double> a,
        std::complex<double> b, std::complex<double> c) {
    std::complex<double> result(0., 0.);

    result += std::log(b / a) * std::log((a + c) / a);
    result += -std::log(((a + c) * (b + c)) / (a * b))
            * std::log((a + b + c) / a);
    result += m_lisk.Li(2,-(c / a)) + m_lisk.Li(2,-(c / b));
    result += -m_lisk.Li(2,-((c * (a + b + c)) / (a * b)));
    result /= -c;

    return result;
}

std::complex<double> GAM2CFFStandard::F110(std::complex<double> a,
        std::complex<double> b, std::complex<double> c) {
    std::complex<double> result(0., 0.);
    std::complex<double> aux(0., 0.);

    aux = -c + (b + c) * std::log((b + c) / b);
    aux *= a * std::log(a / b);

    result += aux;

    aux = c * (a + b + c);
    aux += -a * (b + c) * std::log(((a + c) * (b + c)) / (a * b));
    aux *= std::log((a + b + c) / b);

    result += aux;

    aux = m_lisk.Li(2,-(c / a)) + m_lisk.Li(2,-(c / b));
    aux += -m_lisk.Li(2,-((c * (a + b + c)) / (a * b)));
    aux *= a * (b + c);

    result += aux;

    result /= c * c * (b + c);

    return result;
}

std::complex<double> GAM2CFFStandard::F210(std::complex<double> a,
        std::complex<double> b, std::complex<double> c) {
    std::complex<double> result(0., 0.);
    result += std::log(a + b + c) - std::log(a);
    result /= b * (b + c);

    return result;
}

std::complex<double> GAM2CFFStandard::F211(std::complex<double> a,
        std::complex<double> b, std::complex<double> c) {
//    return (6.*(b + c)*std::log(b)*(-a - (a + c)*std::log((a + c)/a)) + 6.*(a + c)*std::log(a)*(c + (b + c)*(std::log(b) - std::log(b + c))) +
//            (a + c)*(b + c)*(std::pow(Constant::PI,2.) + 6.*std::log(b + c) + 6.*(-1. + std::log(c/b))*std::log((b + c)/b) - 3.*std::pow(std::log((b + c)/b),2.)) +
//            6.*(-(c*(a + b + 2.*c)) + (a + c)*(b + c)*std::log(((a + c)*(b + c))/(a*b)))*std::log(a + b + c) -
//            6.*(a + c)*(b + c)*(Li2(-(c/a)) + Li2(b/(b + c)) - Li2(-((c*(a + b + c))/(a*b)))))/(6.*std::pow(c,2.)*(a + c)*(b + c));

    return -((std::log((a + c)/a) + std::log(b/a)*(-(c/(a + c)) + std::log((a + c)/a)) + std::log((b + c)/b) +
            (c*(a + b + 2.*c)*std::log((a + b + c)/a))/((a + c)*(b + c)) -
            std::log(((a + c)*(b + c))/(a*b))*(1. + std::log((a + b + c)/a)) + m_lisk.Li(2,-(c/a)) +
            m_lisk.Li(2,-(c/b)) - m_lisk.Li(2,-((c*(a + b + c))/(a*b))))/std::pow(c,2));
}

std::complex<double> GAM2CFFStandard::F220(std::complex<double> a,
        std::complex<double> b, std::complex<double> c) {
//    std::complex<double> result (0., 0.);
//
//    result = std::log(b) - std::log(b + c) + std::log( (b + c) / b );
//    result *= - a * (b + c);
//    result += c * c;
//    result *= b + c;
//
//    result += a * c * c * ( std::log(a) - std::log( a + b + c ) );
//
//    result /= b * c * c;
//    result /= (b + c) * (b + c);
    return (b + c + a * std::log(a / b) - a * std::log((a + b + c) / b))
            / (b * std::pow(b + c, 2.));

    //return result;
}

std::complex<double> GAM2CFFStandard::F221(std::complex<double> a,
        std::complex<double> b, std::complex<double> c) {

//    return ((-((a + c)*(b + c)*(3.*std::pow(c,2.) + a*(b + c)*std::pow(Constant::PI,2.))) +
//                3.*std::pow(b + c,2.)*std::log(b)*(3.*std::pow(a,2.) + a*c - std::pow(c,2.) + 2.*a*(a + c)*std::log((a + c)/a)) +
//                3.*a*(a + c)*std::log(a)*(-(c*(2.*b + 3.*c)) - 2.*std::pow(b + c,2.)*(std::log(b) - std::log(b + c))) +
//                3.*(-3.*a*(a + c)*std::pow(b + c,2.)*std::log(b + c) - a*(a + c)*std::pow(b + c,2.)*(-3. + 2.*std::log(c/b))*std::log((b + c)/b) +
//                   a*(a + c)*std::pow(b + c,2.)*std::pow(std::log((b + c)/b),2.) +
//                   (c*(c*std::pow(b + c,2.) + std::pow(a,2.)*(2.*b + 3.*c) + a*(2.*std::pow(b,2.) + 6.*b*c + 5.*std::pow(c,2.))) -
//                      2.*a*(a + c)*std::pow(b + c,2.)*std::log(((a + c)*(b + c))/(a*b)))*std::log(a + b + c)))/((a + c)*std::pow(b + c,2.)) +
//             6.*a*(Li2(-(c/a)) + Li2(b/(b + c)) - Li2(-((c*(a + b + c))/(a*b)))))/(3.*std::pow(c,3.));
    return (-(std::pow(c,2)*(b + c)) + a*std::log(a/b)*
                (-(c*(2.*b + 3.*c)) + 2.*std::pow(b + c,2)*std::log((b + c)/b)) +
               ((c*(c*std::pow(b + c,2) + std::pow(a,2)*(2.*b + 3.*c) +
                       a*(2.*std::pow(b,2) + 6.*b*c + 5.*std::pow(c,2))) -
                    2.*a*(a + c)*std::pow(b + c,2)*std::log(((a + c)*(b + c))/(a*b)))*std::log((a + b + c)/b))/
                (a + c) + 2.*a*std::pow(b + c,2)*(m_lisk.Li(2,-(c/a)) + m_lisk.Li(2,-(c/b)) -
                        m_lisk.Li(2,-((c*(a + b + c))/(a*b)))))/(std::pow(c,3)*std::pow(b + c,2));
}

std::complex<double> GAM2CFFStandard::G(std::complex<double> a,
        std::complex<double> b, std::complex<double> c) {


//    return (-((a + c) * std::pow(Constant::PI, 2.))
//            - 3. * c * std::pow(std::log(-b), 2.) - 6. * c * std::log(b)
//            + 3. * c * std::pow(std::log(-a - b - c), 2.)
//            + 6. * (a + c) * std::log(-b) * std::log((a + c) / a)
//            + 6. * a * std::log(-a) * std::log((b + c) / b)
//            + 3. * std::log((b + c) / b)
//                    * (2. * c * std::log(-a)
//                            + (a + c)
//                                    * (-2. * std::log(c / b)
//                                            + std::log((b + c) / b)))
//            - 6. * (a + c) * std::log(-a - b - c)
//                    * std::log(((a + c) * (b + c)) / (a * b))
//            + 6. * c * std::log(a + b + c)
//            + 6. * (a + c)
//                    * (Li2(-(c / a)) + Li2(b / (b + c))
//                            - Li2(-((c * (a + b + c)) / (a * b)))))
//            / (6. * a * c * (a + c));

    return

            (pow(M_PI,2)*log(-a) - 3.*pow(log(-b),2)*log((a + c)/a) - 3.*pow(log(-a),2)*log((b + c)/b) +
            6.*log(-a)*log(c/b)*log((b + c)/b) - 3.*log(-a)*pow(log((b + c)/b),2) +
            3.*pow(log(-a - b - c),2)*log(((a + c)*(b + c))/(a*b)) - 6.*log(-b)*m_lisk.Li(2,-(c/a)) -
            6.*log(-a)*m_lisk.Li(2,b/(b + c)) + 6.*log(-a - b - c)*m_lisk.Li(2,-((c*(a + b + c))/(a*b))) + 6.*m_lisk.Li(3,-(c/a)) +
            6.*m_lisk.Li(3,-(c/b)) - 6.*m_lisk.Li(3,-((c*(a + b + c))/(a*b))))/(6.*c);
}

std::complex<double> GAM2CFFStandard::GNew(double x, double xi, const std::vector<double>& beta) {

    return ((1. + std::log(-iepsilon - beta[0]*(x + xi)))*(beta[1]*(x + xi) + beta[1]*(x + xi)*std::log(-iepsilon - beta[0]*(x + xi)) +
    2.*(iepsilon + 2.*beta[2]*xi)*std::log((iepsilon + 2.*beta[2]*xi)/(iepsilon + 2.*beta[2]*xi + beta[1]*(x + xi)))) +
    2.*(iepsilon + 2.*beta[2]*xi)*m_lisk.Li(2,(beta[1]*(x + xi))/(iepsilon + 2.*beta[2]*xi + beta[1]*(x + xi))))/
    (2.*beta[1]*(iepsilon + 2*beta[2]*xi)*(iepsilon + 2*beta[2]*xi + beta[1]*(x + xi))) -
    ((1. + std::log(-iepsilon + 2.*beta[2]*xi - beta[0]*(x + xi)))*
            (beta[1]*(x + xi) + beta[1]*(x + xi)*std::log(-iepsilon + 2.*beta[2]*xi - beta[0]*(x + xi)) +
            2.*(iepsilon + 2.*beta[2]*xi)*std::log(((iepsilon + 2.*beta[2]*xi)*(iepsilon + beta[0]*(x + xi) + beta[1]*(x + xi)))/
            ((iepsilon + beta[0]*(x + xi))*(iepsilon + 2.*beta[2]*xi + beta[1]*(x + xi))))) +
            2.*(iepsilon + 2.*beta[2]*xi)*m_lisk.Li(2,(beta[1]*(x + xi)*(iepsilon - 2.*beta[2]*xi + beta[0]*(x + xi)))/
            ((iepsilon + beta[0]*(x + xi))*(iepsilon + 2.*beta[2]*xi + beta[1]*(x + xi)))))/
            (2.*beta[1]*(iepsilon + 2.*beta[2]*xi)*(iepsilon + 2.*beta[2]*xi + beta[1]*(x + xi))) -
            (-std::pow(Constant::PI,2.) + 3.*std::log((-iepsilon + beta[1]*(x + xi) + beta[2]*(x + xi))/(-iepsilon + beta[2]*(x + xi)))*
            (2. + 2.*std::log(-iepsilon - 2.*beta[2]*xi) - 2.*std::log((beta[1]*(x + xi))/(-iepsilon + beta[2]*(x + xi))) +
            std::log((-iepsilon + beta[1]*(x + xi) + beta[2]*(x + xi))/(-iepsilon + beta[2]*(x + xi)))) +
            6.*m_lisk.Li(2,(-iepsilon + beta[2]*(x + xi))/(-iepsilon + beta[1]*(x + xi) + beta[2]*(x + xi))))/(6.*beta[1]);
}

std::complex<double> GAM2CFFStandard::F210New(double x, double xi, const std::vector<double>& beta){

    return std::log( ( -(x-xi) * beta[2] + iepsilon ) / ( (x+xi) * beta[0] + iepsilon ) )
            / ( 2. * xi * beta[2] ) / ( (x + xi ) * beta[1] + 2. * xi * beta[2] + iepsilon );
}

std::complex<double> GAM2CFFStandard::F201New(double x, double xi, const std::vector<double>& beta){

    return std::log( ( -(x-xi) * beta[2] + iepsilon ) / ( 2. * xi * beta[2] ) )
            / ( (x+xi) * beta[0] + iepsilon ) / ( -(x+xi) * beta[2] + iepsilon );
}

std::complex<double> GAM2CFFStandard::F110New(double x, double xi, const std::vector<double>& beta){

    return (-(beta[0]*(x + xi)*std::log(iepsilon + beta[0]*(x + xi))) +
            (2*beta[0]*x - 2*beta[1]*xi)*std::log((beta[0]*x - beta[1]*xi + iepsilon)/
               (-((beta[0] + beta[1])*xi) + iepsilon)))/
          (- 0.9 * iepsilon + beta[0]*(x + xi)*(beta[0]*x - (beta[0] + 2*beta[1])*xi)) +
         (std::pow(Constant::PI,2)/6. - m_lisk.Li(2,(2*(beta[0] + beta[1])*xi)/
              (iepsilon + (beta[0] + beta[1])*xi)) +
            m_lisk.Li(2,(-(beta[0]*x) + beta[0]*xi + 2*beta[1]*xi)/
              (iepsilon + (beta[0] + beta[1])*xi)) -
            m_lisk.Li(2,(-(beta[0]*x) + beta[0]*xi + 2*beta[1]*xi)/
              (iepsilon + 2*(beta[0] + beta[1])*xi)))/
          (- 0.9 * iepsilon + beta[0]*(x + xi));
}

std::complex<double> GAM2CFFStandard::F100New(double x, double xi, const std::vector<double>& beta){

    return -( std::log( (2. * xi * beta[2] ) / ( (x+xi) * beta[0] + iepsilon))
        * std::log( (-(x+xi)*beta[2] + iepsilon) / ( (x+xi) * beta[0] + iepsilon) )
        - std::log( ( -(x+xi) * beta[2] + iepsilon ) * ( (x+xi) * beta[1] + 2. * xi * beta[2] + iepsilon )
                / ( (2. * xi * beta[2] ) * ( (x+xi) * beta[0] + iepsilon )) )
        * std::log( (-(x-xi)*beta[2] + iepsilon) / ( (x+xi) * beta[0] + iepsilon ))
        + m_lisk.Li(2, - ( (x+xi) * beta[1] + iepsilon) / ( (x+xi) * beta[0] + iepsilon))
        + m_lisk.Li(2, -( (x+xi)*beta[1] + iepsilon ) / (2. * xi * beta[0] ) )
        - m_lisk.Li(2, beta[1] * ( -(x-xi) * beta[2] + iepsilon )/ (2. * xi * beta[2]) / beta[0] ));

}


double sgn(double x) {
    return (x >= 0.) ? (1) : (-1);
}

// Amplitudes 4.L and 4.R, see Eqs. 50 and 51

std::complex<double> GAM2CFFStandard::M4L(double s, double x, double xi,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {


    std::complex<double> result(0., 0.);

    result += F210New(x, xi, beta)
            * Tr_4L_F210(xi, s, beta, ee, ek);

    result += (x + xi)
            * F201New(x, xi, beta)
            * Tr_4L_F201(xi, s, beta, ee, ek);

    result += (x + xi)
            * F211(((x + xi) * beta[0] + iepsilon ),
                    ((2. * xi * beta[2]) + iepsilon ), ((x + xi) * beta[1]) + iepsilon)
                    * Tr_4L_F211(xi, s, beta, ee, ek);
//
    result += F220(((x + xi) * beta[0] + iepsilon ),
            ((2. * xi * beta[2]) + iepsilon ), ((x + xi) * beta[1]) + iepsilon)
            * Tr_4L_F220(xi, s, beta, ee, ek);

    result += (x + xi)
            * F221(((x + xi) * beta[0] + iepsilon ),
                    ((2. * xi * beta[2]) + iepsilon ),
                    ((x + xi) * beta[1]) + iepsilon) * Tr_4L_F221(xi, s, beta, ee, ek);
//
    result += s * F100New(x, xi, beta) * Tr_4L_F100(xi, s, beta, ee, ek);
//
    result += s * F110(((x + xi) * beta[0] + iepsilon ),
            ((2. * xi * beta[2]) + iepsilon ), ((x + xi) * beta[1]) + iepsilon) * Tr_4L_F110(xi, s, beta, ee, ek);

//    result += 2. /** (x + xi)*/
//            * G(((x + xi) * beta[0] + iepsilon * sgn(beta[0])),
//                    ((2. * xi * beta[2]) + iepsilon * sgn(beta[2])),
//                    ((x + xi) * beta[1])) * Tr_4L_G(xi, s, beta, ee, ek);

    result += 2. /** (x + xi)*/
            * GNew(x, xi, beta) * Tr_4L_G(xi, s, beta, ee, ek);

    result *= -m_CF * m_alphaSOver2Pi / 2.;
    result /= std::pow(s, 3.);
    result /= ((x - xi) * beta[2] - iepsilon);

    return result;
}

std::complex<double> GAM2CFFStandard::M5L(double s, double x, double xi,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {


    std::complex<double> result(0., 0.);

    result = log(((xi - x) / 2. / xi + iepsilon * sgn(beta[2])));
    result *= 2. * A(s, beta, ee, ek) / beta[2];
    result /= ((x + xi) * beta[0] + iepsilon);

    result += F210New(x, xi, beta)
            * Tr_5L_F210(xi, s, beta, ee, ek);

    result += (x + xi)
            * F201New(x, xi, beta) * Tr_5L_F201(xi, s, beta, ee, ek);

    result += (x + xi)
            * F211(((x + xi) * beta[0] + iepsilon ),
                    ((2. * xi * beta[2]) + iepsilon ), ((x + xi) * beta[1]) + iepsilon)
                    * Tr_5L_F211(xi, s, beta, ee, ek);
//
    result -= F220(((x + xi) * beta[0] + iepsilon ),
            ((2. * xi * beta[2]) + iepsilon ), ((x + xi) * beta[1]))
            * Tr_5L_F220(xi, s, beta, ee, ek);
////
    result -= (x + xi)
            * F221(((x + xi) * beta[0] + iepsilon ),
                    ((2. * xi * beta[2]) + iepsilon ),
                    ((x + xi) * beta[1]) + iepsilon) * Tr_5L_F221(xi, s, beta, ee, ek);
//
    result += s * F100New(x, xi, beta) * Tr_5L_F100(xi, s, beta, ee, ek);
//
    result -= s* F110(((x + xi) * beta[0] + iepsilon ),
            ((2. * xi * beta[2]) + iepsilon ), ((x + xi) * beta[1]) + iepsilon) * Tr_5L_F110(xi, s, beta, ee, ek);

//    result += 2. /** (x + xi)*/
//            * G(((x + xi) * beta[0] + iepsilon * sgn(beta[0])),
//                    ((2. * xi * beta[2]) + iepsilon * sgn(beta[2])),
//                    ((x + xi) * beta[1])) * Tr_5L_G(xi, s, beta, ee, ek);

        result += 2. /** (x + xi)*/
                * GNew(x, xi, beta) * Tr_5L_G(xi, s, beta, ee, ek);

    result *= -m_CF * m_alphaSOver2Pi / 4.;
    result /= s * s;
    result /= xi;

    return result;
}

std::complex<double> GAM2CFFStandard::M4R(double s, double x, double xi,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

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

std::complex<double> GAM2CFFStandard::M5R(double s, double x, double xi,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

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

// Eq. 52
std::complex<double> GAM2CFFStandard::M_scale(double s, double x, double xi,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    std::complex<double> result;
    result = (x + xi) / xi * log(((xi - x) / 2. / xi + iepsilon * sgn(beta[2])));
    result -= (x - xi) / xi * log(((xi + x) / 2. / xi + iepsilon * sgn(beta[0])));
    result *= M0(s, x, xi, beta, ee, ek);
    result *= -m_CF * m_alphaSOver2Pi / 2. * log(2. * xi);

    return result;
}

// The collinear term
std::complex<double> GAM2CFFStandard::Ccoll(double s, double x, double xi,
        std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    std::complex<double> result(3., .0);
    result += (x + xi) * log(((xi - x) / 2. / xi + iepsilon * sgn(beta[2])));
    result -= (x - xi) * log(((xi + x) / 2. / xi + iepsilon * sgn(beta[0])));
    result *= M0(s, x, xi, beta, ee, ek);
    result *= m_CF * m_alphaSOver2Pi / 2.;

    return result;

}

// Vector NLO amplitude - a single permutation of photons
std::complex<double> GAM2CFFStandard::NLO_V_permutation(double s, double x,
        double xi, std::vector<double> beta, std::vector<double> ee,
        std::vector<double> ek) {

    std::complex<double> result(0., 0.);

    result += M23LR(s, x, xi, beta, ee, ek);
    result += M23LR(s, -x, xi, beta, ee, ek);

    result += M3M(s, x, xi, beta, ee, ek);
    result += M3M(s, -x, xi, beta, ee, ek);

    result += M4L(s, x, xi, beta, ee, ek);
    result += M4L(s, -x, xi, beta, ee, ek);
    result += M4R(s, x, xi, beta, ee, ek);
    result += M4R(s, -x, xi, beta, ee, ek);

    result += M5L(s, x, xi, beta, ee, ek);
    result += M5L(s, -x, xi, beta, ee, ek);
    result += M5R(s, x, xi, beta, ee, ek);
    result += M5R(s, -x, xi, beta, ee, ek);

    result += M_scale(s, x, xi, beta, ee, ek);
    result += M_scale(s, -x, xi, beta, ee, ek);

    result += log( 2. * xi * s / m_MuF2) * Ccoll(s, x, xi, beta, ee, ek);
    result += log( 2. * xi * s / m_MuF2) * Ccoll(s, -x, xi, beta, ee, ek);

    return result;
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

std::complex<double> GAM2CFFStandard::Convol_NLO_V(double x,
        std::vector<double> params) {

    std::vector<double> beta0;
    for (int i = 0; i < 3; i++) {
        beta0.push_back(params[i]);
    }

    std::vector<double> ee0;
    for (int i = 3; i < 6; i++) {
        ee0.push_back(params[i]);
    }

    /*
     * ek[i][j] = epsilon_i * k_j
     * where
     * 0 - incoming photon
     * 1 - outgoing photon 1
     * 2 - outgoing photon 2
     * I use ek[i][j] = -ek[i][k] where all i,j,k are different
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

    double s = params[9];

    std::complex<double> result(0., 0.);

    /*
     *
     */

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == j)
                continue;
            for (int k = 0; k < 3; k++) {
                if (k == i || k == j)
                    continue;
                std::vector<double> beta;
                std::vector<double> ee;
                std::vector<double> ek;


                beta.push_back(beta0[i]);
                beta.push_back(beta0[j]);
                beta.push_back(beta0[k]);

                ee.push_back(ee0[i]);
                ee.push_back(ee0[j]);
                ee.push_back(ee0[k]);

                ek.push_back(ek0[i][j]);
                ek.push_back(ek0[j][i]);
                ek.push_back(ek0[k][i]);


                result += NLO_V_permutation(s, x, m_xi, beta, ee, ek);
            }
        }
    }

    return result;
}

double GAM2CFFStandard::Convol_NLO_V_Re(double x, std::vector<double> params) {

    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    double EvalGPD = computeCubedChargeAveragedGPD(partonDistribution);

    double Convol = (Convol_NLO_V(x, params)).real();
    Convol *= EvalGPD;

    return Convol;
}

double GAM2CFFStandard::Convol_NLO_V_Re_Sym(double x,
        std::vector<double> params) {

    double x0 = params.at(10);
    double gpdXXi = params.at(11);

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

    double Convol = (Convol_NLO_V(x0 - x, params) * EvalGPDA
            + Convol_NLO_V(x0 + x, params) * EvalGPDB).real();

    return Convol;
}

double GAM2CFFStandard::Convol_NLO_V_Re_Sym_Const(double x,
        std::vector<double> params) {

    double x0 = params.at(10);
    double gpdXXi = params.at(11);

    double EvalGPDA = gpdXXi;
    double EvalGPDB = gpdXXi;

    double Convol = (Convol_NLO_V(x0 - x, params) * EvalGPDA
            + Convol_NLO_V(x0 + x, params) * EvalGPDB).real();

    return Convol;
}

double GAM2CFFStandard::Convol_NLO_V_Im(double x, std::vector<double> params) {

    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    double EvalGPD = computeCubedChargeAveragedGPD(partonDistribution);

    double Convol = (Convol_NLO_V(x, params)).imag();
    Convol *= EvalGPD;

    return Convol;
}

double GAM2CFFStandard::Convol_NLO_V_Im_Sym(double x,
        std::vector<double> params) {

    double x0 = params.at(10);
      double gpdXXi = params.at(11);

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

      double Convol = (Convol_NLO_V(x0 - x, params) * EvalGPDA
              + Convol_NLO_V(x0 + x, params) * EvalGPDB).imag();

      return Convol;
}

double GAM2CFFStandard::Convol_NLO_V_Im_Sym_Const(double x,
        std::vector<double> params) {

    double x0 = params.at(10);
    double gpdXXi = params.at(11);

    double EvalGPDA = gpdXXi;
    double EvalGPDB = gpdXXi;

    double Convol = (Convol_NLO_V(x0 - x, params) * EvalGPDA
            + Convol_NLO_V(x0 + x, params) * EvalGPDB).imag();

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
//    double pt2 = -(m_uPrim * tPrim) / (m_uPrim + tPrim);
//    double alpha = m_uPrim / (m_uPrim + tPrim);
//    double alphabar = 1. - alpha;

    double alpha = -m_uPrim*(1/(2*m_xi*s) - M2/(s*s*(1 - m_xi*m_xi)));
    double alphabar = 1 - alpha - M2/s*2. *m_xi/(1 - m_xi*m_xi);
    double pt2 = -m_uPrim*alphabar;

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
    //TODO to include phi-dependence comment two lines below:
    Parameters.push_back(-double(m_polG0 == m_polG2));
    Parameters.push_back(-double(m_polG1 == m_polG0));
    //TODO ... and uncomment the four below:
//    Parameters.push_back(-double(PolarizationType::LIN_TRANS_X_PLUS == m_polG2) * cos(m_phi)
//                        - double(PolarizationType::LIN_TRANS_Y_PLUS == m_polG2) * sin(m_phi) );
//    Parameters.push_back(-double(PolarizationType::LIN_TRANS_X_PLUS == m_polG1) * cos(m_phi)
//                            - double(PolarizationType::LIN_TRANS_Y_PLUS == m_polG1) * sin(m_phi) );

    // ek0[i][j] = epsilon_i * k_j
    // This matrix will be used to make the vector ek[i] for a given permutation
    // See Eq. 16

    //TODO to include phi-dependence comment the line below:
    Parameters.push_back(sqrt(pt2) * double(m_polG0 == PolarizationType::LIN_TRANS_X_PLUS));
    //TODO ... and uncomment that one:
//    Parameters.push_back(sqrt(pt2) * cos(m_phi) ); // phi - dependence included
    Parameters.push_back(sqrt(pt2) * double(m_polG1 == PolarizationType::LIN_TRANS_X_PLUS) / alpha);
    Parameters.push_back(-sqrt(pt2) * double(m_polG2 == PolarizationType::LIN_TRANS_X_PLUS) / alphabar);

    Parameters.push_back(s);

    double result_Re = 0.;
    double result_Im = 0.;

   // std::cout << "s = " << s << std::endl;

    // LO part
    computeDiagonalGPD_V();
    //TODO to include phi-dependence comment the lines below:
    result_Im = (alpha - alphabar) * double(m_polG1 == m_polG2)
            * double(m_polG0 == PolarizationType::LIN_TRANS_X_PLUS);
    result_Im -= double(m_polG0 == m_polG2) * double(m_polG1 == PolarizationType::LIN_TRANS_X_PLUS);
    result_Im += double(m_polG1 == m_polG0) * double(m_polG2 == PolarizationType::LIN_TRANS_X_PLUS);
    //TODO... and uncomment these:
//    result_Im = (alpha - alphabar) * double(m_polG1 == m_polG2)
//            * cos(m_phi);
//    result_Im -= (double(PolarizationType::LIN_TRANS_X_PLUS == m_polG2) * cos(m_phi)
//                + double(PolarizationType::LIN_TRANS_Y_PLUS == m_polG2) * sin(m_phi) )
//                  * double(m_polG1 == PolarizationType::LIN_TRANS_X_PLUS);
//    result_Im += (double(PolarizationType::LIN_TRANS_X_PLUS == m_polG1) * cos(m_phi)
//                + double(PolarizationType::LIN_TRANS_Y_PLUS == m_polG1) * sin(m_phi) )
//                    * double(m_polG2 == PolarizationType::LIN_TRANS_X_PLUS);
//    //TODO up to this line.

    result_Im *= sqrt(pt2);
    result_Im *= m_quark_diagonal_V;
    result_Im *= -2. * Constant::PI / s / alpha / alphabar / m_xi;

//    std::cout << m_polG0 << "\t" << m_polG1 << "\t" << m_polG2 << std::endl;

    if (m_qcdOrderType == PerturbativeQCDOrderType::NLO) {

        std::cout << "NLO" << std::endl;

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

//        std::cout << "DEBUG: xi: " << m_xi << std::endl;
//
//        for (size_t i = 0; i < range.size(); i++) {
//            std::cout << "DEBUG: range: " << range.at(i) << std::endl;
//        }

//        iepsilon = std::complex<double> (0.,  1.E-4);
//        std::cout << "DEBUG: epsilon:" << iepsilon << std::endl;
//        gslIntegrationWrapper(m_pConvol_NLO_V_Re, range,  Parameters);

        //TODO
        iepsilon = std::complex<double>(0., 1.E-8);
        result_Re += gslIntegrationWrapper(m_pConvol_NLO_V_Re, m_pConvol_NLO_V_Re_Sym, m_pConvol_NLO_V_Re_Sym_Const,range,
                Parameters);
        result_Im += gslIntegrationWrapper(m_pConvol_NLO_V_Im, m_pConvol_NLO_V_Im_Sym, m_pConvol_NLO_V_Im_Sym_Const, range,
                Parameters);

//        iepsilon = std::complex<double>(0., 1.E-6);
//            result_Re += gslIntegrationWrapper(m_pConvol_NLO_V_Re, m_pConvol_NLO_V_Re_Sym, m_pConvol_NLO_V_Re_Sym_Const, range,
//                    Parameters);
//
//            iepsilon = std::complex<double>(0., 1.E-8);
//                     result_Re += gslIntegrationWrapper(m_pConvol_NLO_V_Re, m_pConvol_NLO_V_Re_Sym, m_pConvol_NLO_V_Re_Sym_Const, range,
//                             Parameters);

//        result_Re += gslIntegrationWrapper(m_pConvol_NLO_V_Re, range,
//                Parameters);
//        result_Im += gslIntegrationWrapper(m_pConvol_NLO_V_Im, range,
//                  Parameters);

//    result_Re += gslIntegrationWrapper(m_pConvol_NLO_V_Re, -1+0.0001, -m_xi - 0.001,
//            Parameters);
//    result_Re += gslIntegrationWrapper(m_pConvol_NLO_V_Re, -m_xi - 0.001, -m_xi + 0.001,
//                Parameters);
//    result_Re += gslIntegrationWrapper(m_pConvol_NLO_V_Re, -m_xi + 0.001, m_xi - 0.001,
//                    Parameters);
//    result_Re += gslIntegrationWrapper(m_pConvol_NLO_V_Re, m_xi - 0.001, m_xi + 0.001,
//                    Parameters);
//    result_Re += gslIntegrationWrapper(m_pConvol_NLO_V_Re, m_xi + 0.001, 1.-0.001,
//                    Parameters);
//
//    result_Im += gslIntegrationWrapper(m_pConvol_NLO_V_Im, -1+0.0001, -m_xi - 0.001,
//            Parameters);
//    result_Im += gslIntegrationWrapper(m_pConvol_NLO_V_Im, -m_xi - 0.001, -m_xi + 0.001,
//            Parameters);
//    result_Im += gslIntegrationWrapper(m_pConvol_NLO_V_Im, -m_xi + 0.001, m_xi - 0.001,
//            Parameters);
//    result_Im += gslIntegrationWrapper(m_pConvol_NLO_V_Im, m_xi - 0.001, m_xi + 0.001,
//            Parameters);
//    result_Im += gslIntegrationWrapper(m_pConvol_NLO_V_Im, m_xi + 0.001, 1.-0.001,
//            Parameters);
    }
//    std::cout << "CHECK" << std::endl;
//    std::cout <<  integrate(m_pConvol_NLO_V_Im, -m_xi, m_xi,
//            Parameters) << std::endl;
//    std::cout << gslIntegrationWrapper(m_pConvol_NLO_V_Im, -m_xi, m_xi, Parameters) << std::endl;
//    std::cout << "CHECK" << std::endl;

    // do sprawdzania F-ow
//    std::complex<double> I(0., 1.);
//    std::cout << " i  = " << I << std::endl;
//    std::cout<< "F210(0.1, 0.2, 0.3)  " << F210(0.1, 0.2, 0.3) << std::endl;
//    std::cout<< "F210(0.1+i, 0.2+i, 0.3+i)  " << F210(0.1 + I, 0.2 + I, 0.3 + I) << std::endl;
//    std::cout<< "F211((0.1, 0.2, 0.3)  " << F211(0.1, 0.2, 0.3) << std::endl;
//    std::cout<< "F211(0.1+i, 0.2+i, 0.3+i)  " << F211(0.1 + I, 0.2 + I, 0.3 + I) << std::endl;
//    std::cout<< "F220(0.1, 0.2, 0.3)  " << F220(0.1, 0.2, 0.3) << std::endl;
//    std::cout<< "F220(0.1+i, 0.2+i, 0.3+i)  " << F220(0.1 + I, 0.2 + I, 0.3 + I) << std::endl;
//    std::cout<< "F221(0.1, 0.2, 0.3)  " << F221(0.1, 0.2, 0.3) << std::endl;
//    std::cout<< "F221(0.1+i, 0.2+i, 0.3+i)  " << F221(0.1 + I, 0.2 + I, 0.3 + I) << std::endl;
//    std::cout<< "G(0.1, 0.2, 0.3)  " << G(0.1, 0.2, 0.3) << std::endl;
//    std::cout<< "G(0.1+i, 0.2+i, 0.3+i)  " << G (0.1 + I, 0.2 + I, 0.3 + I) << std::endl;
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

//double GAM2CFFStandard::exampleIntegration(double x,
//        std::vector<double> params) {
//    return m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
//            m_currentGPDComputeType).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution();
//}

double GAM2CFFStandardIntegrationFunction(double x, void* p) {

    GAM2CFFStandardIntegrationParameters* par =
            static_cast<GAM2CFFStandardIntegrationParameters*>(p);

    return (par->m_pIntegrator)->operator()(x, par->m_parameters);
}

double GAM2CFFStandard::gslIntegrationWrapper(NumA::FunctionType1D* functor, NumA::FunctionType1D* functorSym,NumA::FunctionType1D* functorSymConst,
        const std::vector<double>& limits, std::vector<double>& params) {

    //result
    double result = 0.;

    //prepare for symmetric x0
    params.push_back(0.);

    //prepare for symmetric gpd at x=xi
    params.push_back(0.);


//    for (size_t i = 0; i < range.size() - 1; i++) {
//
//           double thisResult = integrate(functor, range.at(i), range.at(i + 1),
//                   params);
//           std::cout << "DEBUG: range: " << range.at(i) << " " << range.at(i + 1)
//                   << " result: " << thisResult << std::endl;
//           result += thisResult;
//       }

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

        limitsSym.push_back(std::make_pair(limits.at(i) - range, limits.at(i) + range));

        params.at(10) = limits.at(i);
        params.at(11) = computeCubedChargeAveragedGPD(m_pGPDModule->compute(
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

//    GAM2CFFStandardIntegrationParameters integrationParameters;
//    integrationParameters.m_pIntegrator = functor;
//    integrationParameters.m_parameters = params;
//
//    gsl_integration_workspace* w = gsl_integration_workspace_alloc(1000);
//
//    double result, error;
//
//    gsl_function F;
//    F.function = &GAM2CFFStandardIntegrationFunction;
//    F.params = &integrationParameters;
//
//    double* rangeTab = new double[range.size()];
//
//    for (size_t i = 0; i < range.size(); i++) {
//        rangeTab[i] = range.at(i);
//    }
//
//    gsl_integration_qagp(&F, rangeTab, range.size(), 0., 1.E-1, 1000, w,
//            &result, &error);
//
//    delete[] rangeTab;
//    gsl_integration_workspace_free(w);

    return result;
}

} /* namespace PARTONS */
