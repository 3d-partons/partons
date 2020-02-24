#include "../../../../../include/partons/modules/convol_coeff_function/TCS/TCSCFFStandard.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <NumA/utils/MathUtils.h>
#include <cmath>
#include <utility>

#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/gpd/GPDModule.h"
#include "../../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongStandard.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"

namespace PARTONS {

const unsigned int TCSCFFStandard::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSCFFStandard("TCSCFFStandard"));

TCSCFFStandard::TCSCFFStandard(const std::string &className) :
        TCSConvolCoeffFunctionModule(className), m_nf(0), m_pRunningAlphaStrongModule(
                0), m_Zeta(0.), m_logQ2PrimOverMu2(0.), m_QPrim(0.), m_alphaSOver2Pi(
                0.), m_quarkDiagonal(0.), m_gluonDiagonal(0.), m_realPartSubtractQuark(
                0.), m_imaginaryPartSubtractQuark(0.), m_realPartSubtractGluon(
                0.), m_imaginaryPartSubtractGluon(0.), m_CF(4. / 3.) {
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::H,
                    &TCSConvolCoeffFunctionModule::computeUnpolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::E,
                    &TCSConvolCoeffFunctionModule::computeUnpolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Ht,
                    &TCSConvolCoeffFunctionModule::computePolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Et,
                    &TCSConvolCoeffFunctionModule::computePolarized));

    initFunctorsForIntegrations();
}

//TODO Call mother init function
void TCSCFFStandard::resolveObjectDependencies() {
    TCSConvolCoeffFunctionModule::resolveObjectDependencies();

    setIntegrator(NumA::IntegratorType1D::DEXP);

    m_pRunningAlphaStrongModule =
            Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                    RunningAlphaStrongStandard::classId);
}

void TCSCFFStandard::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    TCSConvolCoeffFunctionModule::prepareSubModules(subModulesData);

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

RunningAlphaStrongModule* TCSCFFStandard::getRunningAlphaStrongModule() const {
    return m_pRunningAlphaStrongModule;
}

void TCSCFFStandard::setRunningAlphaStrongModule(
        RunningAlphaStrongModule* pRunningAlphaStrongModule) {
    m_pModuleObjectFactory->updateModulePointerReference(
            m_pRunningAlphaStrongModule, pRunningAlphaStrongModule);
    m_pRunningAlphaStrongModule = pRunningAlphaStrongModule;
}

void TCSCFFStandard::initFunctorsForIntegrations() {
    m_pConvolReKernelQuark1V = NumA::Integrator1D::newIntegrationFunctor(this,
            &TCSCFFStandard::ConvolReKernelQuark1V);
    m_pConvolReKernelQuark2V = NumA::Integrator1D::newIntegrationFunctor(this,
            &TCSCFFStandard::ConvolReKernelQuark2V);
    m_pConvolImKernelQuark1V = NumA::Integrator1D::newIntegrationFunctor(this,
            &TCSCFFStandard::ConvolImKernelQuark1V);
    m_pConvolImKernelQuark2V = NumA::Integrator1D::newIntegrationFunctor(this,
            &TCSCFFStandard::ConvolImKernelQuark2V);
    m_pConvolReKernelGluon1V = NumA::Integrator1D::newIntegrationFunctor(this,
            &TCSCFFStandard::ConvolReKernelGluon1V);
    m_pConvolReKernelGluon2V = NumA::Integrator1D::newIntegrationFunctor(this,
            &TCSCFFStandard::ConvolReKernelGluon2V);
    m_pConvolImKernelGluon1V = NumA::Integrator1D::newIntegrationFunctor(this,
            &TCSCFFStandard::ConvolImKernelGluon1V);
    m_pConvolImKernelGluon2V = NumA::Integrator1D::newIntegrationFunctor(this,
            &TCSCFFStandard::ConvolImKernelGluon2V);
    m_pConvolReKernelQuark1A = NumA::Integrator1D::newIntegrationFunctor(this,
            &TCSCFFStandard::ConvolReKernelQuark1A);
    m_pConvolReKernelQuark2A = NumA::Integrator1D::newIntegrationFunctor(this,
            &TCSCFFStandard::ConvolReKernelQuark2A);
    m_pConvolImKernelQuark1A = NumA::Integrator1D::newIntegrationFunctor(this,
            &TCSCFFStandard::ConvolImKernelQuark1A);
    m_pConvolImKernelQuark2A = NumA::Integrator1D::newIntegrationFunctor(this,
            &TCSCFFStandard::ConvolImKernelQuark2A);
    m_pConvolReKernelGluon1A = NumA::Integrator1D::newIntegrationFunctor(this,
            &TCSCFFStandard::ConvolReKernelGluon1A);
    m_pConvolReKernelGluon2A = NumA::Integrator1D::newIntegrationFunctor(this,
            &TCSCFFStandard::ConvolReKernelGluon2A);
    m_pConvolImKernelGluon1A = NumA::Integrator1D::newIntegrationFunctor(this,
            &TCSCFFStandard::ConvolImKernelGluon1A);
    m_pConvolImKernelGluon2A = NumA::Integrator1D::newIntegrationFunctor(this,
            &TCSCFFStandard::ConvolImKernelGluon2A);
}

TCSCFFStandard::TCSCFFStandard(const TCSCFFStandard &other) :
        TCSConvolCoeffFunctionModule(other) {

    m_nf = other.m_nf;

    if (other.m_pRunningAlphaStrongModule != 0) {
        m_pRunningAlphaStrongModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pRunningAlphaStrongModule);
    } else {
        m_pRunningAlphaStrongModule = 0;
    }

    m_Zeta = other.m_Zeta;
    m_logQ2PrimOverMu2 = other.m_logQ2PrimOverMu2;
    m_QPrim = other.m_QPrim;
    m_alphaSOver2Pi = other.m_alphaSOver2Pi;
    m_quarkDiagonal = other.m_quarkDiagonal;
    m_gluonDiagonal = other.m_gluonDiagonal;

    m_realPartSubtractQuark = other.m_realPartSubtractQuark;
    m_imaginaryPartSubtractQuark = other.m_imaginaryPartSubtractQuark;
    m_realPartSubtractGluon = other.m_realPartSubtractGluon;
    m_imaginaryPartSubtractGluon = other.m_imaginaryPartSubtractGluon;

    m_CF = other.m_CF;

    initFunctorsForIntegrations();
}

TCSCFFStandard* TCSCFFStandard::clone() const {
    return new TCSCFFStandard(*this);
}

//TODO comment gérer le cycle de vie des modules membres
TCSCFFStandard::~TCSCFFStandard() {

    // destroy alphaS
    if (m_pRunningAlphaStrongModule != 0) {
        setRunningAlphaStrongModule(0);
        m_pRunningAlphaStrongModule = 0;
    }

    // destroy functors
    if (m_pConvolReKernelQuark1V) {
        delete m_pConvolReKernelQuark1V;
        m_pConvolReKernelQuark1V = 0;
    }

    if (m_pConvolReKernelQuark2V) {
        delete m_pConvolReKernelQuark2V;
        m_pConvolReKernelQuark2V = 0;
    }

    if (m_pConvolImKernelQuark1V) {
        delete m_pConvolImKernelQuark1V;
        m_pConvolImKernelQuark1V = 0;
    }

    if (m_pConvolImKernelQuark2V) {
        delete m_pConvolImKernelQuark2V;
        m_pConvolImKernelQuark2V = 0;
    }

    if (m_pConvolReKernelGluon1V) {
        delete m_pConvolReKernelGluon1V;
        m_pConvolReKernelGluon1V = 0;
    }

    if (m_pConvolReKernelGluon2V) {
        delete m_pConvolReKernelGluon2V;
        m_pConvolReKernelGluon2V = 0;
    }

    if (m_pConvolImKernelGluon1V) {
        delete m_pConvolImKernelGluon1V;
        m_pConvolImKernelGluon1V = 0;
    }

    if (m_pConvolImKernelGluon2V) {
        delete m_pConvolImKernelGluon2V;
        m_pConvolImKernelGluon2V = 0;
    }

    if (m_pConvolReKernelQuark1A) {
        delete m_pConvolReKernelQuark1A;
        m_pConvolReKernelQuark1A = 0;
    }

    if (m_pConvolReKernelQuark2A) {
        delete m_pConvolReKernelQuark2A;
        m_pConvolReKernelQuark2A = 0;
    }

    if (m_pConvolImKernelQuark1A) {
        delete m_pConvolImKernelQuark1A;
        m_pConvolImKernelQuark1A = 0;
    }

    if (m_pConvolImKernelQuark2A) {
        delete m_pConvolImKernelQuark2A;
        m_pConvolImKernelQuark2A = 0;
    }

    if (m_pConvolReKernelGluon1A) {
        delete m_pConvolReKernelGluon1A;
        m_pConvolReKernelGluon1A = 0;
    }

    if (m_pConvolReKernelGluon2A) {
        delete m_pConvolReKernelGluon2A;
        m_pConvolReKernelGluon2A = 0;
    }

    if (m_pConvolImKernelGluon1A) {
        delete m_pConvolImKernelGluon1A;
        m_pConvolImKernelGluon1A = 0;
    }

    if (m_pConvolImKernelGluon2A) {
        delete m_pConvolImKernelGluon2A;
        m_pConvolImKernelGluon2A = 0;
    }
}

void TCSCFFStandard::initModule() {
    // init parent module before
    TCSConvolCoeffFunctionModule::initModule();

    m_nf = 3;
    m_QPrim = sqrt(m_Q2Prim);
    m_Zeta = 2. * m_xi / (1 + m_xi);
    m_logQ2PrimOverMu2 = log(m_Q2Prim / m_MuF2);

    m_alphaSOver2Pi = m_pRunningAlphaStrongModule->compute(m_MuR2)
            / (2. * Constant::PI);

    debug(__func__,
            ElemUtils::Formatter() << "m_Q2Prim=" << m_Q2Prim << " m_QPrim= " << m_QPrim
                    << " m_MuF2=" << m_MuF2 << " m_Zeta= " << m_Zeta
                    << " m_logQ2OverMu2=" << m_logQ2PrimOverMu2
                    << " m_nbOfActiveFlavour=" << m_nf << " m_alphaSOver2Pi="
                    << m_alphaSOver2Pi);
}

void TCSCFFStandard::isModuleWellConfigured() {
    // check parent module before
    TCSConvolCoeffFunctionModule::isModuleWellConfigured();

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

std::complex<double> TCSCFFStandard::computeUnpolarized() {

    computeDiagonalGPD();
    computeSubtractionFunctionsV();
    return computeIntegralsV();
}

std::complex<double> TCSCFFStandard::computePolarized() {

    computeDiagonalGPD();
    computeSubtractionFunctionsA();
    return computeIntegralsA();
}

void TCSCFFStandard::computeDiagonalGPD() {
    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(m_xi, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    //TODO compute CFF singlet; FAIT; vérifier le résultat du calcul
    m_quarkDiagonal = computeSquareChargeAveragedGPD(partonDistribution);
    m_gluonDiagonal = 2.
            * partonDistribution.getGluonDistribution().getGluonDistribution();

    //   debug( __func__,
    //      	                ElemUtils::Formatter()<<"    q diagonal = "<< m_quarkDiagonal <<"   g diagonal = "<< m_gluonDiagonal);

}

double TCSCFFStandard::computeSquareChargeAveragedGPD(
        const PartonDistribution &partonDistribution) {
//TODO comment faire evoluer le calcul si de nouvelles saveurs de quark entrent en jeux
    double result = 0.;

    result +=
            (partonDistribution.getQuarkDistribution(QuarkFlavor::UP).getQuarkDistributionPlus())
                    * Constant::U2_ELEC_CHARGE;

    result +=
            (partonDistribution.getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistributionPlus())
                    * Constant::D2_ELEC_CHARGE;
    result +=
            (partonDistribution.getQuarkDistribution(QuarkFlavor::STRANGE).getQuarkDistributionPlus())
                    * Constant::S2_ELEC_CHARGE;

//    std::vector<> = gpdResultData.listQuarkTypeComputed()
//
//    for()
//    {
//        if( MuF || Q >< c.getSinglet() )
//    }

    return result;
}

void TCSCFFStandard::computeSubtractionFunctionsV() {
    double LogZeta = log(m_Zeta);
    double LogInvZeta = log((1. - m_Zeta) / m_Zeta);
    double LogInvZeta2 = LogInvZeta * LogInvZeta;
    double DiLogInvZeta = NumA::MathUtils::DiLog(1. - 1. / m_Zeta);
    double Pi2 = Constant::PI * Constant::PI;

    double RealPartSubtractQuarkLO; // Real part of eq. (B2)
    double ImaginaryPartSubtractQuarkLO; // Imaginary part of eq. (B2)

    // LO, 1 / 1 - z

    RealPartSubtractQuarkLO = -LogInvZeta;

    ImaginaryPartSubtractQuarkLO = -Constant::PI; // change of sign

    // Computation of subtraction terms in eq. (8) and (9)
    // Sums up previous contributions and takes care of LO / NLO and P(olarized) / U(npolarized) subtleties

    // LO, real and imaginary parts

    m_realPartSubtractQuark = RealPartSubtractQuarkLO;
    m_imaginaryPartSubtractQuark = ImaginaryPartSubtractQuarkLO;

    m_realPartSubtractGluon = 0.;
    m_imaginaryPartSubtractGluon = 0.;

    // NLO, real and imaginary parts

    if (m_qcdOrderType == PerturbativeQCDOrderType::NLO) {
        double RealPartSubtractQuarkNLOV; // Real part of eq. (B4)
        double ImaginaryPartSubtractQuarkNLOV; // Imaginary part of eq. (B4)
        double RealPartSubtractGluonNLOV; // Real part of eq. (B6)
        double ImaginaryPartSubtractGluonNLOV; // Imaginary part of eq. (B6)
        // Collinear Subtraction Functions
        double ReGluonCV;
        double ImGluonCV;
        double ReQuarkCV;
        double ImQuarkCV;

        // NLO, quark, vector, eq. (B4)

        RealPartSubtractQuarkNLOV = Pi2 / 2. - 3. * DiLogInvZeta
                + LogInvZeta * (Pi2 + 9. + 3. * LogZeta - LogInvZeta2 / 3.);
        RealPartSubtractQuarkNLOV += m_logQ2PrimOverMu2
                * (Pi2 - 3. * LogInvZeta - LogInvZeta2);
        RealPartSubtractQuarkNLOV *= m_CF / 2.;

        ImaginaryPartSubtractQuarkNLOV = Pi2 / 3. + 9. + 3. * LogZeta
                - LogInvZeta2 - m_logQ2PrimOverMu2 * (2. * LogInvZeta + 3);
        ImaginaryPartSubtractQuarkNLOV *= -Constant::PI * m_CF / 2.;

        // Quark Collinear subtraction functions as in my notes
        ReQuarkCV = Pi2 - 3. * LogInvZeta - LogInvZeta2;
        ReQuarkCV *= m_CF / 2.;
        ImQuarkCV = 2. * LogInvZeta + 3.;
        ImQuarkCV *= Constant::PI * m_CF / 2.;

        // Modifications of subtraction functions
        RealPartSubtractQuarkNLOV += -Constant::PI * ImQuarkCV;
        ImaginaryPartSubtractQuarkNLOV *= -1.;
        ImaginaryPartSubtractQuarkNLOV += -Constant::PI * ReQuarkCV;

        // NLO, gluon, vector, eq. (B6)

        RealPartSubtractGluonNLOV = -1. + Pi2 / 3. * (1. - 3. / 4. * m_Zeta)
                + DiLogInvZeta - LogZeta * LogInvZeta;
        RealPartSubtractGluonNLOV += (2. - m_Zeta) * LogInvZeta
                * (1. - LogInvZeta / 4.);
        RealPartSubtractGluonNLOV += m_logQ2PrimOverMu2 / 2.
                * (1. - (2. - m_Zeta) * LogInvZeta);
        RealPartSubtractGluonNLOV *= 1 / (2. * m_xi);

        ImaginaryPartSubtractGluonNLOV = (2. - m_Zeta)
                * (2. - m_logQ2PrimOverMu2 - LogInvZeta) - 2. * LogZeta;
        ImaginaryPartSubtractGluonNLOV *= -Constant::PI / (4. * m_xi);

        // Gluon Subtraction functions as in my notes
        ReGluonCV = 1. - (2. - m_Zeta) * LogInvZeta;
        ReGluonCV *= 1. / (4. * m_xi);
        ImGluonCV = 1. - m_Zeta / 2.;
        ImGluonCV *= Constant::PI * 1. / (2. * m_xi);

        // Modifications of subtractions functions
        RealPartSubtractGluonNLOV += -Constant::PI * ImGluonCV;
        ImaginaryPartSubtractGluonNLOV *= -1.;
        ImaginaryPartSubtractGluonNLOV += -Constant::PI * ReGluonCV;

        // Real part, quark contribution

        m_realPartSubtractQuark += m_alphaSOver2Pi * RealPartSubtractQuarkNLOV;

        // Real part, gluon contribution

        m_realPartSubtractGluon += m_alphaSOver2Pi * RealPartSubtractGluonNLOV;

        // Imaginary part, quark contribution

        m_imaginaryPartSubtractQuark += m_alphaSOver2Pi
                * ImaginaryPartSubtractQuarkNLOV;

        // Imaginary part, gluon contribution

        m_imaginaryPartSubtractGluon += m_alphaSOver2Pi
                * ImaginaryPartSubtractGluonNLOV;

    }
}

void TCSCFFStandard::computeSubtractionFunctionsA() {
    double LogZeta = log(m_Zeta);
    double LogInvZeta = log((1. - m_Zeta) / m_Zeta);
    double LogInvZeta2 = LogInvZeta * LogInvZeta;
    double DiLogInvZeta = NumA::MathUtils::DiLog(1. - 1. / m_Zeta);
    double Pi2 = Constant::PI * Constant::PI;

    double RealPartSubtractQuarkLO; // Real part of eq. (B2)
    double ImaginaryPartSubtractQuarkLO; // Imaginary part of eq. (B2)

    // LO, 1 / 1 - z

    RealPartSubtractQuarkLO = LogInvZeta; // change of sign

    ImaginaryPartSubtractQuarkLO = Constant::PI;

    // Computation of subtraction terms in eq. (8) and (9)
    // Sums up previous contributions and takes care of LO / NLO and P(olarized) / U(npolarized) subtleties

    // LO, real and imaginary parts

    m_realPartSubtractQuark = RealPartSubtractQuarkLO;
    m_imaginaryPartSubtractQuark = ImaginaryPartSubtractQuarkLO;

    m_realPartSubtractGluon = 0.;
    m_imaginaryPartSubtractGluon = 0.;

    // NLO, real and imaginary parts

    if (m_qcdOrderType == PerturbativeQCDOrderType::NLO) {
        double RealPartSubtractQuarkNLOA; // Real part of eq. (B4)
        double ImaginaryPartSubtractQuarkNLOA; // Imaginary part of eq. (B4)
        double RealPartSubtractGluonNLOA; // Real part of eq. (B6)
        double ImaginaryPartSubtractGluonNLOA; // Imaginary part of eq. (B6)
        // Collinear Subtraction functions
        double ReGluonCA;
        double ImGluonCA;
        double ReQuarkCA;
        double ImQuarkCA;

        // NLO, quark, axial, eq. (B3)

        RealPartSubtractQuarkNLOA = Pi2 / 6. - DiLogInvZeta
                + LogInvZeta * (Pi2 + 9. + LogZeta - LogInvZeta2 / 3.);
        RealPartSubtractQuarkNLOA += m_logQ2PrimOverMu2
                * (Pi2 - 3. * LogInvZeta - LogInvZeta2);
        RealPartSubtractQuarkNLOA *= m_CF / 2.;

        ImaginaryPartSubtractQuarkNLOA = Pi2 / 3. + 9. + LogZeta - LogInvZeta2
                - m_logQ2PrimOverMu2 * (2. * LogInvZeta + 3);
        ImaginaryPartSubtractQuarkNLOA *= -Constant::PI * m_CF / 2.;

        // Quark Subtraction functions as in my notes
        ReQuarkCA = Pi2 - 3. * LogInvZeta - LogInvZeta2;
        ReQuarkCA *= m_CF / 2.;
        ImQuarkCA = 2. * LogInvZeta + 3.;
        ImQuarkCA *= Constant::PI * m_CF / 2.;

        // Modifications of subtraction functions
        RealPartSubtractQuarkNLOA *= -1.;
        RealPartSubtractQuarkNLOA += Constant::PI * ImQuarkCA;
        ImaginaryPartSubtractQuarkNLOA += Constant::PI * ReQuarkCA;

        // NLO, gluon, axial, eq. (B5)

        RealPartSubtractGluonNLOA = 1. + Pi2 / 4. * m_Zeta
                + m_Zeta * LogInvZeta * (1. - LogInvZeta / 4.);
        RealPartSubtractGluonNLOA += -m_logQ2PrimOverMu2 / 2.
                * (1. + m_Zeta * LogInvZeta);
        RealPartSubtractGluonNLOA *= 1 / (2. * m_xi);

        ImaginaryPartSubtractGluonNLOA = 2. - LogInvZeta - m_logQ2PrimOverMu2;
        ImaginaryPartSubtractGluonNLOA *= -Constant::PI * m_Zeta / (4. * m_xi);

        // Gluon Subtraction functions as in my notes
        ReGluonCA = -1. - m_Zeta * LogInvZeta;
        ReGluonCA *= 1. / (4. * m_xi);
        ImGluonCA = m_Zeta;
        ImGluonCA *= Constant::PI / (4. * m_xi);

        // Modifications of subtraction functions
        RealPartSubtractGluonNLOA *= -1.;
        RealPartSubtractGluonNLOA += Constant::PI * ImGluonCA;
        ImaginaryPartSubtractGluonNLOA += Constant::PI * ReGluonCA;

        // Real part, quark contribution

        m_realPartSubtractQuark += m_alphaSOver2Pi * RealPartSubtractQuarkNLOA;

        // Real part, gluon contribution

        m_realPartSubtractGluon += m_alphaSOver2Pi * RealPartSubtractGluonNLOA;

        // Imaginary part, quark contribution

        m_imaginaryPartSubtractQuark += m_alphaSOver2Pi
                * ImaginaryPartSubtractQuarkNLOA;

        // Imaginary part, gluon contribution

        m_imaginaryPartSubtractGluon += m_alphaSOver2Pi
                * ImaginaryPartSubtractGluonNLOA;

    }
}

std::complex<double> TCSCFFStandard::computeIntegralsV() {
    double IntegralRealPartKernelQuark1 = 0.; // Integral between 0 and fZeta in real part of amplitude
    double IntegralRealPartKernelQuark2 = 0.; // Integral between fZeta and 1 in real part of amplitude
    double IntegralImaginaryPartKernelQuark1 = 0.; // Integral between 0 and fZeta in imaginary part of amplitude
    double IntegralImaginaryPartKernelQuark2 = 0.; // Integral between fZeta and 1 in imaginary part of amplitude

    double IntegralRealPartKernelGluon1 = 0.; // Integral between 0 and fZeta in real part of amplitude
    double IntegralRealPartKernelGluon2 = 0.; // Integral between fZeta and 1 in real part of amplitude
    double IntegralImaginaryPartKernelGluon1 = 0.; // Integral between 0 and fZeta in imaginary part of amplitude
    double IntegralImaginaryPartKernelGluon2 = 0.; // Integral between fZeta and 1 in imaginary part of amplitude

    double SumSqrCharges; // Sum of square of electric charges of active quark flavours

    // Sums up integrals and subtraction constants to get real and imaginary parts of CFF.

    if (m_qcdOrderType != PerturbativeQCDOrderType::LO
            && m_qcdOrderType != PerturbativeQCDOrderType::NLO) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Erroneous input perturbative QCD order can only be LO or NLO. Here Order = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }

    // Compute sum of active quark electric charges squared

    switch (m_nf) {

    case 3:
        SumSqrCharges = 2. / 3.;
        break;

    case 4:
        SumSqrCharges = 10. / 9.;
        break;

    case 5:
        SumSqrCharges = 11. / 9.;
        break;

    case 6:
        SumSqrCharges = 15. / 9.;
        break;

    default:
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Erroneous input number of active quark flavours should be an integer between 3 and 6. Number of active quark flavours = "
                        << m_nf);
    }

    // Quark sector

    std::vector<double> emptyParameters;

    IntegralRealPartKernelQuark1 = integrate(m_pConvolReKernelQuark1V, 0.,
            +m_xi, emptyParameters);

    IntegralRealPartKernelQuark2 = integrate(m_pConvolReKernelQuark2V, +m_xi,
            +1, emptyParameters);

    IntegralImaginaryPartKernelQuark1 = integrate(m_pConvolImKernelQuark1V, 0.,
            +m_xi, emptyParameters);

    IntegralImaginaryPartKernelQuark2 = integrate(m_pConvolImKernelQuark2V,
            +m_xi, +1, emptyParameters);

    // Gluon sector

    if (m_qcdOrderType == PerturbativeQCDOrderType::NLO) {

        IntegralRealPartKernelGluon1 = integrate(m_pConvolReKernelGluon1V, 0.,
                +m_xi, emptyParameters);

        IntegralRealPartKernelGluon2 = integrate(m_pConvolReKernelGluon2V,
                +m_xi, +1, emptyParameters);

        IntegralImaginaryPartKernelGluon1 = integrate(m_pConvolImKernelGluon1V,
                0., +m_xi, emptyParameters);

        IntegralImaginaryPartKernelGluon2 = integrate(m_pConvolImKernelGluon2V,
                +m_xi, +1, emptyParameters);
    }

    // Compute Subtraction constants (different at LO or NLO)

    computeSubtractionFunctionsV();

    // Compute real and imaginary part of CFF according to eq. (8) and eq. (9)

    double fRealPartCFF = IntegralRealPartKernelQuark1
            + IntegralRealPartKernelQuark2
            + m_quarkDiagonal * m_realPartSubtractQuark;
    fRealPartCFF += SumSqrCharges
            * (IntegralRealPartKernelGluon1 + IntegralRealPartKernelGluon2
                    + m_gluonDiagonal * m_realPartSubtractGluon);
    // Multiplication by SumSqrCharges corrects in mistake in eq. (9)

    double fImaginaryPartCFF = IntegralImaginaryPartKernelQuark1
            + IntegralImaginaryPartKernelQuark2
            + m_quarkDiagonal * m_imaginaryPartSubtractQuark;
    fImaginaryPartCFF += SumSqrCharges
            * (IntegralImaginaryPartKernelGluon1
                    + IntegralImaginaryPartKernelGluon2
                    + m_gluonDiagonal * m_imaginaryPartSubtractGluon);
    // Multiplication by SumSqrCharges corrects in mistake in eq. (9)

    return std::complex<double>(fRealPartCFF, fImaginaryPartCFF);
}

std::complex<double> TCSCFFStandard::computeIntegralsA() {
    double IntegralRealPartKernelQuark1 = 0.; // Integral between 0 and fZeta in real part of amplitude
    double IntegralRealPartKernelQuark2 = 0.; // Integral between fZeta and 1 in real part of amplitude
    double IntegralImaginaryPartKernelQuark1 = 0.; // Integral between 0 and fZeta in imaginary part of amplitude
    double IntegralImaginaryPartKernelQuark2 = 0.; // Integral between fZeta and 1 in imaginary part of amplitude

    double IntegralRealPartKernelGluon1 = 0.; // Integral between 0 and fZeta in real part of amplitude
    double IntegralRealPartKernelGluon2 = 0.; // Integral between fZeta and 1 in real part of amplitude
    double IntegralImaginaryPartKernelGluon1 = 0.; // Integral between 0 and fZeta in imaginary part of amplitude
    double IntegralImaginaryPartKernelGluon2 = 0.; // Integral between fZeta and 1 in imaginary part of amplitude

    double SumSqrCharges; // Sum of square of electric charges of active quark flavours

    // Sums up integrals and subtraction constants to get real and imaginary parts of CFF.

    if (m_qcdOrderType != PerturbativeQCDOrderType::LO
            && m_qcdOrderType != PerturbativeQCDOrderType::NLO) {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Erroneous input perturbative QCD order can only be LO or NLO. Here Order = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());

    }

    // Compute sum of active quark electric charges squared

    switch (m_nf) {

    case 3:
        SumSqrCharges = 2. / 3.;
        break;

    case 4:
        SumSqrCharges = 10. / 9.;
        break;

    case 5:
        SumSqrCharges = 11. / 9.;
        break;

    case 6:
        SumSqrCharges = 15. / 9.;
        break;

    default:
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Erroneous input number of active quark flavours should be an integer between 3 and 6. Number of active quark flavours = "
                        << m_nf);
    }

    // Quark sector

    std::vector<double> emptyParameters;

    IntegralRealPartKernelQuark1 = integrate(m_pConvolReKernelQuark1A, 0.,
            +m_xi, emptyParameters);

    IntegralRealPartKernelQuark2 = integrate(m_pConvolReKernelQuark2A, +m_xi,
            +1, emptyParameters);

    IntegralImaginaryPartKernelQuark1 = integrate(m_pConvolImKernelQuark1A, 0.,
            +m_xi, emptyParameters);

    IntegralImaginaryPartKernelQuark2 = integrate(m_pConvolImKernelQuark2A,
            +m_xi, +1, emptyParameters);

    // Gluon sector

    if (m_qcdOrderType == PerturbativeQCDOrderType::NLO) {
        IntegralRealPartKernelGluon1 = integrate(m_pConvolReKernelGluon1A, 0.,
                +m_xi, emptyParameters);

        IntegralRealPartKernelGluon2 = integrate(m_pConvolReKernelGluon2A,
                +m_xi, +1, emptyParameters);

        IntegralImaginaryPartKernelGluon1 = integrate(m_pConvolImKernelGluon1A,
                0., +m_xi, emptyParameters);

        IntegralImaginaryPartKernelGluon2 = integrate(m_pConvolImKernelGluon2A,
                +m_xi, +1, emptyParameters);
    }

    // Compute Subtraction constants (different at LO or NLO)

    computeSubtractionFunctionsA();

    // Compute real and imaginary part of CFF according to eq. (8) and eq. (9)

    double realPartCFF = IntegralRealPartKernelQuark1
            + IntegralRealPartKernelQuark2
            + m_quarkDiagonal * m_realPartSubtractQuark;
    //	cout << fpQCDOrder << "      RealPartCFF Quark = " << fRealPartCFF << endl ;
    realPartCFF += SumSqrCharges
            * (IntegralRealPartKernelGluon1 + IntegralRealPartKernelGluon2
                    + m_gluonDiagonal * m_realPartSubtractGluon);
    //	cout << fpQCDOrder << "      RealPartCFF Gluon = " << SumSqrCharges * ( IntegralRealPartKernelGluon1 + IntegralRealPartKernelGluon2 + fGluonDiagonal * fRealPartSubtractGluon ) << endl ;
    // Multiplication by SumSqrCharges corrects in mistake in eq. (9)

    double imaginaryPartCFF = IntegralImaginaryPartKernelQuark1
            + IntegralImaginaryPartKernelQuark2
            + m_quarkDiagonal * m_imaginaryPartSubtractQuark;
    //	cout << fpQCDOrder << "      ImaginaryPartCFF Quark = " << fImaginaryPartCFF << endl ;
    imaginaryPartCFF += SumSqrCharges
            * (IntegralImaginaryPartKernelGluon1
                    + IntegralImaginaryPartKernelGluon2
                    + m_gluonDiagonal * m_imaginaryPartSubtractGluon);
    //	cout << fpQCDOrder << "      ImaginaryPartCFF Gluon = " << SumSqrCharges * ( IntegralImaginaryPartKernelGluon + fGluonDiagonal * fImaginaryPartSubtractGluon ) << endl ;
    // Multiplication by SumSqrCharges corrects in mistake in eq. (9)
    debug(__func__,
            ElemUtils::Formatter() << "    integral RE = " << realPartCFF
                    << "   Integral IM = " << imaginaryPartCFF);

    return std::complex<double>(realPartCFF, imaginaryPartCFF);
}

std::complex<double> TCSCFFStandard::KernelQuarkNLOV(double x) {
    double z = x / m_xi;

    std::complex<double> LogOneMinusz(0., 0.);
    if (x < m_xi) {
// remplacer z par x/xi
        LogOneMinusz = std::complex<double>(0., 0.);
        LogOneMinusz = std::complex<double>(log((1. - z) / 2.), 0.);
    }
    if (x > m_xi) {
        LogOneMinusz = std::complex<double>(log((z - 1.) / 2.), -Constant::PI);
    }

    return KernelQuarkNLOA(x) - (m_CF / (1. + z)) * LogOneMinusz;
}

/*!
 * \fn KernelQuark( double x )
 *
 * T^{q, V/A} in appendix A, eq. (A1).
 *
 */
std::complex<double> TCSCFFStandard::KernelQuarkV(double x) {

    //std::complex<double> z = std::complex<double>(x / m_xi, 0.);
    //std::complex<double> quark();

    double z = x / m_xi;

//    TComplex z = TComplex(x / fXi, 0.);
//    TComplex Quark;

    std::complex<double> quark(1. / (1. - z), 0.);

    if (m_qcdOrderType == PerturbativeQCDOrderType::NLO) {
        quark += m_alphaSOver2Pi * KernelQuarkNLOV(x);
    }

    return std::conj(quark); // conjugation
}

std::complex<double> TCSCFFStandard::KernelQuarkA(double x) {
    //std::complex<double> z = std::complex<double>(x / m_xi, 0.);
    //std::complex<double> quark();

    double z = x / m_xi;

//    TComplex z = TComplex(x / fXi, 0.);
//    TComplex Quark;

    std::complex<double> quark(1. / (1. - z), 0.);

    if (m_qcdOrderType == PerturbativeQCDOrderType::NLO) {
        quark += m_alphaSOver2Pi * KernelQuarkNLOA(x);
    }

    return -std::conj(quark); // minus conjugation
}

/*!
 * \fn KernelGluon( double x )
 *
 * T^{g, V/A} in appendix A, eq. (A1).
 *
 */
std::complex<double> TCSCFFStandard::KernelGluonV(double x) {

    std::complex<double> gluon(0., 0.);

    if (m_qcdOrderType == PerturbativeQCDOrderType::NLO) {
        gluon += m_alphaSOver2Pi * KernelGluonNLOV(x);
    }

    return std::conj(gluon); // conjugation
}

std::complex<double> TCSCFFStandard::KernelGluonA(double x) {

    std::complex<double> gluon(0., 0.);

    if (m_qcdOrderType == PerturbativeQCDOrderType::NLO) {
        gluon += m_alphaSOver2Pi * KernelGluonNLOA(x);
    }

    return -std::conj(gluon); // minus conjugation
}

/*
 * \fn double ConvolReKernelQuark1( double* x, double* Parameters );
 *
 * eq. (8), real part of amplitude, terms integrated for X between 0 and fZeta.
 * Equivalently x integration domain ranges between -fXi and +fXi.
 * Warning : grid is defined for x > 0.
 *
 * Expressions are modified in order to integrate between 0 and fXi, hence explicitely avoiding GPD behaviour at x = 0.
 *
 */
double TCSCFFStandard::ConvolReKernelQuark1V(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = computeSquareChargeAveragedGPD(partonDistribution);

    // Integrated function
    double Convol = (EvalGPD - m_quarkDiagonal) * KernelQuarkV(x).real();
    Convol += (-EvalGPD - m_quarkDiagonal) * KernelQuarkV(-x).real();

    Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

    debug(__func__,
            ElemUtils::Formatter() << "x = " << x << " | convol = " << Convol);

    return Convol;
}

/*
 * \fn double ConvolReKernelQuark2( double* x, double* Parameters );
 *
 * eq. (8), real part of amplitude, terms integrated between fZeta and 1.
 * Equivalently x integration domain ranges between fXi and 1.
 *
 */
double TCSCFFStandard::ConvolReKernelQuark2V(double x,
        std::vector<double> params) {

    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = computeSquareChargeAveragedGPD(partonDistribution);

    double Convol = EvalGPD - m_quarkDiagonal;
    Convol *= KernelQuarkV(x).real();

    Convol += -KernelQuarkV(-x).real() * EvalGPD;

    Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

    debug(__func__,
            ElemUtils::Formatter() << "x = " << x << " | convol = " << Convol);

    return Convol;
}

/*
 * \fn double ConvolImKernelQuark1( double* x, double* Parameters );
 *
 * eq. (8), imaginary part of amplitude, terms integrated between 0. and fZeta.
 * Equivalently x integration domain ranges between 0. and fXi.
 *
 */
double TCSCFFStandard::ConvolImKernelQuark1V(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = computeSquareChargeAveragedGPD(partonDistribution);

    double Convol = (EvalGPD - m_quarkDiagonal) * KernelQuarkV(x).imag();
    Convol += (-EvalGPD - m_quarkDiagonal) * KernelQuarkV(-x).imag();

    Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

    debug(__func__,
            ElemUtils::Formatter() << "x = " << x << " | convol = " << Convol);

    return Convol;
}

/*
 * \fn double ConvolImKernelQuark2( double* x, double* Parameters );
 *
 * eq. (8), imaginary part of amplitude, terms integrated between fZeta and 1.
 * Equivalently x integration domain ranges between fXi and 1.
 *
 */
double TCSCFFStandard::ConvolImKernelQuark2V(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = computeSquareChargeAveragedGPD(partonDistribution);

    double Convol = EvalGPD - m_quarkDiagonal;
    Convol *= KernelQuarkV(x).imag();
    Convol += -KernelQuarkV(-x).imag() * EvalGPD;
    Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

    debug(__func__,
            ElemUtils::Formatter() << "x = " << x << " | convol = " << Convol);

    return Convol;
}

/*
 * \fn double ConvolReKernelGluon1( double* x, double* Parameters );
 *
 * eq. (9), real part of amplitude, terms integrated between 0 and fZeta.
 * Equivalently x integration domain ranges between -fXi and +fXi.
 * Warning : grid is defined for x > 0.
 *
 * Expressions are modified in order to integrate between 0 and fXi, hence explicitely avoiding GPD behaviour at x = 0.
 *
 */
double TCSCFFStandard::ConvolReKernelGluon1V(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = 2
            * partonDistribution.getGluonDistribution().getGluonDistribution();

    double Convol = (EvalGPD - m_gluonDiagonal) * KernelGluonV(x).real();
    Convol += (+EvalGPD - m_gluonDiagonal) * KernelGluonV(-x).real();

    Convol /= (m_xi * m_xi * m_nf);

    debug(__func__,
            ElemUtils::Formatter() << "x = " << x << " | convol = " << Convol);

    return Convol;
}

/*
 * \fn double ConvolReKernelGluon2( double* x, double* Parameters );
 *
 * eq. (8), real part of amplitude, terms integrated between fZeta and 1.
 * Equivalently x integration domain ranges between fXi and 1.
 *
 */
double TCSCFFStandard::ConvolReKernelGluon2V(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    double EvalGPD = 2
            * partonDistribution.getGluonDistribution().getGluonDistribution();

    double Convol = EvalGPD - m_gluonDiagonal;
    Convol *= KernelGluonV(x).real();

    Convol += +KernelGluonV(-x).real() * EvalGPD;

    Convol /= (m_xi * m_xi * m_nf); // In eq. (8), ( ( 2 - fZeta ) / fZeta )^2 = 1 / fXi^2

    debug(__func__,
            ElemUtils::Formatter() << "x = " << x << " | convol = " << Convol);

    return Convol;
}

/*
 * \fn double ConvolImKernelGluon1( double* x, double* Parameters );
 *
 * eq. (9), imaginary part of amplitude, terms integrated between 0 and fZeta.
 * Equivalently x integration domain ranges between 0 and fXi.
 *
 */
double TCSCFFStandard::ConvolImKernelGluon1V(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    double EvalGPD = 2
            * partonDistribution.getGluonDistribution().getGluonDistribution();

    double Convol = (EvalGPD - m_gluonDiagonal) * KernelGluonV(x).imag();
    Convol += (EvalGPD - m_gluonDiagonal) * KernelGluonV(-x).imag();
    Convol /= (m_xi * m_xi * m_nf); // In eq. (8), ( ( 2 - fZeta ) / fZeta )^2 = 1 / fXi^2

    debug(__func__,
            ElemUtils::Formatter() << "x = " << x << " | convol = " << Convol);

    return Convol;
}

/*
 * \fn double ConvolImKernelGluon2( double* x, double* Parameters );
 *
 * eq. (9), imaginary part of amplitude, terms integrated between fZeta and 1.
 * Equivalently x integration domain ranges between fXi and 1.
 *
 */
double TCSCFFStandard::ConvolImKernelGluon2V(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    double EvalGPD = 2
            * partonDistribution.getGluonDistribution().getGluonDistribution();

    double Convol = EvalGPD - m_gluonDiagonal;
    Convol *= KernelGluonV(x).imag();
    Convol += KernelGluonV(-x).imag() * EvalGPD;
    Convol /= (m_xi * m_xi * m_nf); // In eq. (8), ( ( 2 - fZeta ) / fZeta )^2 = 1 / fXi^2

    debug(__func__,
            ElemUtils::Formatter() << "x = " << x << " | convol = " << Convol);

    return Convol;
}

std::complex<double> TCSCFFStandard::KernelGluonNLOV(double x) {
    debug(__func__, "entered");

    double z = x / m_xi;

    std::complex<double> LogOneMinusz(0., 0.);
    std::complex<double> GluonVCollinear(0., 0.); // Collinear part for the vector gluon
    if (x < m_xi) {
        // remplacer z par x/xi
        LogOneMinusz = std::complex<double>(log((1. - z) / 2.), 0.);
    }
    if (x > m_xi) {
        LogOneMinusz = std::complex<double>(log((z - 1.) / 2.), -Constant::PI);
    }
// GluonNLOA is copied in an unchanged form from KernelGluonNLOA
    std::complex<double> GluonNLOA(LogOneMinusz);
    GluonNLOA += (m_logQ2PrimOverMu2 - 2.);
    GluonNLOA *= (1. / (1. - (z * z)) + LogOneMinusz / ((1. + z) * (1. + z)));
    GluonNLOA += -LogOneMinusz * LogOneMinusz / (2. * (1. + z) * (1. + z));
    GluonNLOA *= (m_nf / 2.);

    std::complex<double> GluonNLOV(LogOneMinusz);
    GluonNLOV += (m_logQ2PrimOverMu2 - 2.);
    GluonNLOV /= 1. - z;
    GluonNLOV += LogOneMinusz / (1. + z);
    GluonNLOV *= (m_nf / 2.);
    GluonNLOV += -GluonNLOA; // function above is used instead of KernelGluonNLOA

// Collinear function as in notes
    GluonVCollinear = -1. / (1. - (z * z))
            - LogOneMinusz / ((1. + z) * (1. + z));
    GluonVCollinear += 1. / (1. - z);
    GluonVCollinear *= (m_nf / 2.);

    return GluonNLOV + std::complex<double>(0., Constant::PI) * GluonVCollinear;
}

std::complex<double> TCSCFFStandard::KernelGluonNLOA(double x) {
    debug(__func__, "entered");
//
//    debug( __func__,
//               ElemUtils::Formatter() << "x= " << x );

    double z = x / m_xi;
    std::complex<double> LogOneMinusz(0., 0.);
    if (x < m_xi) {
        // remplacer z par x/xi
        LogOneMinusz = std::complex<double>(log((1. - z) / 2.), 0.);
    }
    if (x > m_xi) {
        LogOneMinusz = std::complex<double>(log((z - 1.) / 2.), -Constant::PI);
    }

    std::complex<double> GluonNLOA(LogOneMinusz);
    std::complex<double> GluonACollinear(0., 0.); // Collinear part of the axial gluon
    GluonNLOA += (m_logQ2PrimOverMu2 - 2.);
    GluonNLOA *= (1. / (1. - (z * z)) + LogOneMinusz / ((1. + z) * (1. + z)));
    GluonNLOA += -LogOneMinusz * LogOneMinusz / (2. * (1. + z) * (1. + z));
    GluonNLOA *= (m_nf / 2.);

// Collinear function as in notes
    GluonACollinear = 1. / (1. - (z * z))
            + LogOneMinusz / ((1. + z) * (1. + z));
    GluonACollinear *= (m_nf / 2.);

    return GluonNLOA + std::complex<double>(0., Constant::PI) * GluonACollinear;
}

std::complex<double> TCSCFFStandard::KernelQuarkNLOA(double x) {

    double z = x / m_xi;
    std::complex<double> LogOneMinusz(0., 0.);
    std::complex<double> QuarkCollinear(0., 0.); // definition of the collinear part
    if (x < m_xi) {
        // remplacer z par x/xi
        LogOneMinusz = std::complex<double>(log((1. - z) / 2.), 0.);
    }
    if (x > m_xi) {
        LogOneMinusz = std::complex<double>(log((z - 1.) / 2.), -Constant::PI);
    }

    std::complex<double> QuarkNLOA(m_logQ2PrimOverMu2);
    QuarkNLOA += LogOneMinusz / 2. - (3. / 4.);
    QuarkNLOA *= 2. * LogOneMinusz + 3.;
    QuarkNLOA += -(27. / 4.) - (1. - z) / (1. + z) * LogOneMinusz;
    QuarkNLOA *= m_CF / (2. * (1. - z));

// Collinear part defined as in notes
    QuarkCollinear = 2. * LogOneMinusz + 3.;
    QuarkCollinear *= m_CF / (2. * (1. - z));

    return QuarkNLOA + std::complex<double>(0., Constant::PI) * QuarkCollinear;
}

double TCSCFFStandard::ConvolReKernelQuark1A(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = computeSquareChargeAveragedGPD(partonDistribution);

    double Convol = (EvalGPD - m_quarkDiagonal) * KernelQuarkA(x).real();
    Convol += (+EvalGPD - m_quarkDiagonal) * KernelQuarkA(-x).real();

    Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

    debug(__func__,
            ElemUtils::Formatter() << "x = " << x << " | convol = " << Convol);

    return Convol;
}

double TCSCFFStandard::ConvolReKernelQuark2A(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = computeSquareChargeAveragedGPD(partonDistribution);

    double Convol = EvalGPD - m_quarkDiagonal;
    Convol *= KernelQuarkA(x).real();

    Convol += +KernelQuarkA(-x).real() * EvalGPD;

    Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

    debug(__func__,
            ElemUtils::Formatter() << "x = " << x << " | convol = " << Convol);

    return Convol;
}

double TCSCFFStandard::ConvolImKernelQuark1A(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = computeSquareChargeAveragedGPD(partonDistribution);

    double Convol = (EvalGPD - m_quarkDiagonal) * KernelQuarkA(x).imag();
    Convol += (EvalGPD - m_quarkDiagonal) * KernelQuarkA(-x).imag();
    Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

    debug(__func__,
            ElemUtils::Formatter() << "x = " << x << " | convol = " << Convol);

    return Convol;
}

double TCSCFFStandard::ConvolImKernelQuark2A(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = computeSquareChargeAveragedGPD(partonDistribution);

    double Convol = EvalGPD - m_quarkDiagonal;
    Convol *= KernelQuarkA(x).imag();
    Convol += KernelQuarkA(-x).imag() * EvalGPD;
    Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

    debug(__func__,
            ElemUtils::Formatter() << "x = " << x << " | convol = " << Convol);

    return Convol;
}

double TCSCFFStandard::ConvolReKernelGluon1A(double x,
        std::vector<double> params) {

    debug(__func__, "Entered");

    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    double EvalGPD = 2
            * partonDistribution.getGluonDistribution().getGluonDistribution();

    double Convol = (EvalGPD - m_gluonDiagonal) * KernelGluonA(x).real();
    Convol += (-EvalGPD - m_gluonDiagonal) * KernelGluonA(-x).real();

    Convol /= (m_xi * m_xi * m_nf);

    debug(__func__,
            ElemUtils::Formatter() << "x = " << x << " | convol = " << Convol);

    return Convol;
}

double TCSCFFStandard::ConvolReKernelGluon2A(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    double EvalGPD = 2
            * partonDistribution.getGluonDistribution().getGluonDistribution();

    double Convol = EvalGPD - m_gluonDiagonal;
    Convol *= KernelGluonA(x).real();

    Convol += -KernelGluonA(-x).real() * EvalGPD;

    Convol /= (m_xi * m_xi * m_nf); // In eq. (8), ( ( 2 - fZeta ) / fZeta )^2 = 1 / fXi^2

    debug(__func__,
            ElemUtils::Formatter() << "x = " << x << " | convol = " << Convol);

    return Convol;
}

double TCSCFFStandard::ConvolImKernelGluon1A(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    double EvalGPD = 2
            * partonDistribution.getGluonDistribution().getGluonDistribution();

    double Convol = (EvalGPD - m_gluonDiagonal) * KernelGluonA(x).imag();
    Convol += (-EvalGPD - m_gluonDiagonal) * KernelGluonA(-x).imag();
    Convol /= (m_xi * m_xi * m_nf); // In eq. (8), ( ( 2 - fZeta ) / fZeta )^2 = 1 / fXi^2

    debug(__func__,
            ElemUtils::Formatter() << "x = " << x << " | convol = " << Convol);

    return Convol;
}

double TCSCFFStandard::ConvolImKernelGluon2A(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    double EvalGPD = 2
            * partonDistribution.getGluonDistribution().getGluonDistribution();

    double Convol = EvalGPD - m_gluonDiagonal;
    Convol *= KernelGluonA(x).imag();
    Convol += -KernelGluonA(-x).imag() * EvalGPD;
    Convol /= (m_xi * m_xi * m_nf); // In eq. (8), ( ( 2 - fZeta ) / fZeta )^2 = 1 / fXi^2

    debug(__func__,
            ElemUtils::Formatter() << "x = " << x << " | convol = " << Convol);

    return Convol;
}

} /* namespace PARTONS */
