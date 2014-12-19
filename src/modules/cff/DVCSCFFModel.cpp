#include "DVCSCFFModel.h"

#include <Math/AllIntegrationTypes.h>
#include <Math/Integrator.h>
#include <TMath.h>
#include <cmath>
#include <map>
#include <stdexcept>
#include <utility>

#include "../../beans/gpd/GPDComputeType.h"
#include "../../beans/gpd/GPDOutputData.h"
#include "../../beans/gpd/GPDQuarkFlavorData.h"
#include "../../beans/gpd/GPDResultData.h"
#include "../../beans/QCDOrderType.h"
#include "../../beans/QuarkFlavor.h"
#include "../../FundamentalPhysicalConstants.h"
#include "../../services/ModuleObjectFactory.h"
#include "../../utils/logger/LoggerManager.h"
#include "../../utils/stringUtils/Formatter.h"
#include "../alphaS/RunningAlphaStrong.h"
#include "../GPDModule.h"
#include "../math/RootIntegrationMode.h"

// Initialise [class]::moduleID with a unique name.
const std::string DVCSCFFModel::moduleID =
        ModuleObjectFactory::getInstance()->registerModule(
                new DVCSCFFModel("DVCSCFFModel"));

DVCSCFFModel::DVCSCFFModel(const std::string &className)
        : CFFModule(className), m_Zeta(0.), m_logQ2OverMu2(0.), m_Q(0.), m_nbOfActiveFlavour(
                0), m_alphaSOver2Pi(0.), m_quarkDiagonal(0.), m_gluonDiagonal(
                0.), m_realPartSubtractQuark(0.), m_imaginaryPartSubtractQuark(
                0.), m_realPartSubtractGluon(0.), m_imaginaryPartSubtractGluon(
                0.), m_CF(4. / 3.)/*, m_integrator(
 ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR, 0., 1.e-3)*/{

    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDComputeType::H, &CFFModule::computeUnpolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDComputeType::E, &CFFModule::computeUnpolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDComputeType::Ht, &CFFModule::computePolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDComputeType::Et, &CFFModule::computePolarized));

    //TODO le passer en configuration
    m_pMathIntegratorModule =
            ModuleObjectFactory::getInstance()->getMathIntegratorModule(
                    RootIntegrationMode::moduleID);
}

DVCSCFFModel::DVCSCFFModel(const DVCSCFFModel &other)
        : CFFModule(other) {
    m_Zeta = other.m_Zeta;
    m_logQ2OverMu2 = other.m_logQ2OverMu2;
    m_Q = other.m_Q;
    m_nbOfActiveFlavour = other.m_nbOfActiveFlavour;
    m_alphaSOver2Pi = other.m_alphaSOver2Pi;
    m_quarkDiagonal = other.m_quarkDiagonal;
    m_gluonDiagonal = other.m_gluonDiagonal;

    m_realPartSubtractQuark = other.m_realPartSubtractQuark;
    m_imaginaryPartSubtractQuark = other.m_imaginaryPartSubtractQuark;
    m_realPartSubtractGluon = other.m_realPartSubtractGluon;
    m_imaginaryPartSubtractGluon = other.m_imaginaryPartSubtractGluon;

    m_CF = other.m_CF;
}

DVCSCFFModel* DVCSCFFModel::clone() const {
    return new DVCSCFFModel(*this);
}

DVCSCFFModel::~DVCSCFFModel() {

}

void DVCSCFFModel::initModule() {
    CFFModule::initModule();

    m_Q = sqrt(m_Q2);
    m_Zeta = 2. * m_xi / (1 + m_xi);
    m_logQ2OverMu2 = 2. * log(m_Q / m_MuF);
    m_nbOfActiveFlavour = 3;

    RunningAlphaStrong* Alpha = new RunningAlphaStrong();
    Alpha->SetRunningScale(m_MuR);
    m_alphaSOver2Pi = Alpha->GetAlphaS() / (2. * PI);
    delete Alpha;
}

void DVCSCFFModel::isModuleWellConfigured() {
    CFFModule::isModuleWellConfigured();

    if (m_pGPDModule == 0) {
        throw std::runtime_error("[DVCSCFFModel] GPDModule* is NULL");
    }
    if (m_qcdOrderType == QCDOrderType::UNDEFINED) {
        throw std::runtime_error("[DVCSCFFModel] QCDOrderType is UNDEFINED");
    }
    if (m_pMathIntegratorModule == 0) {
        throw std::runtime_error(
                "[DVCSCFFModel] MathIntegratorModule* is NULL");
    }
}

//TODO voir pourquoi CFFInputData se retrouve NULL lors de la copie de CFFOutputData
//CFFOutputData DVCSCFFModel::compute(const double xB, const double t,
//        const double Q2, const double MuF, const double MuR,
//        GPDComputeType::Type gpdComputeType) {
//
//    m_pLoggerManager->debug(getClassName(), __func__, "entered");
//
//    return CFFModule::preCompute(xB, t, Q2, MuF, MuR, gpdComputeType);
//}

std::complex<double> DVCSCFFModel::computeUnpolarized() {

    computeDiagonalGPD();
    computeSubtractionFunctionsV();
    return computeIntegralsV();
}

std::complex<double> DVCSCFFModel::computePolarized() {

    computeDiagonalGPD();
    computeSubtractionFunctionsA();
    return computeIntegralsV();
}

void DVCSCFFModel::computeDiagonalGPD() {
    GPDOutputData gpdOutputData = m_pGPDModule->compute(m_xi, m_xi, m_t, m_MuF,
            m_MuR, m_currentGPDComputeType);
    GPDResultData* pGPDResultData = gpdOutputData.getGPDResultData(
            m_currentGPDComputeType);

    //TODO compute CFF singlet; FAIT; vérifier le résultat du calcul
    m_quarkDiagonal = computeSquareChargeAveragedGPD(gpdOutputData);
    m_gluonDiagonal = 2. * pGPDResultData->getGluon();
}

double DVCSCFFModel::computeSquareChargeAveragedGPD(
        GPDOutputData &gpdOutputData) {
//TODO comment faire evoluer le calcul si de nouvelles saveurs de quark entrent en jeux
    double result = 0.;
    GPDResultData* gpdResultData = gpdOutputData.getGPDResultData(
            m_currentGPDComputeType);
    result +=
            gpdResultData->getGPDQuarkFlavorData(QuarkFlavor::UP)->getPartonDistributionSinglet()
                    * U2_ELEC_CHARGE;
    result +=
            gpdResultData->getGPDQuarkFlavorData(QuarkFlavor::DOWN)->getPartonDistributionSinglet()
                    * D2_ELEC_CHARGE;
    result +=
            gpdResultData->getGPDQuarkFlavorData(QuarkFlavor::STRANGE)->getPartonDistributionSinglet()
                    * S2_ELEC_CHARGE;

//    std::vector<> = gpdResultData.listQuarkTypeComputed()
//
//    for()
//    {
//        if( MuF || Q >< c.getSinglet() )
//    }

    return result;
}

void DVCSCFFModel::computeSubtractionFunctionsV() {
    double LogZeta = log(m_Zeta);
    double LogInvZeta = log((1. - m_Zeta) / m_Zeta);
    double LogInvZeta2 = LogInvZeta * LogInvZeta;
    double DiLogInvZeta = TMath::DiLog(1. - 1. / m_Zeta);
    double Pi2 = PI * PI;

    double RealPartSubtractQuarkLO; // Real part of eq. (B2)
    double ImaginaryPartSubtractQuarkLO; // Imaginary part of eq. (B2)
    double RealPartSubtractQuarkNLOV; // Real part of eq. (B4)
    double ImaginaryPartSubtractQuarkNLOV; // Imaginary part of eq. (B4)
    double RealPartSubtractGluonNLOV; // Real part of eq. (B6)
    double ImaginaryPartSubtractGluonNLOV; // Imaginary part of eq. (B6)

    // LO, 1 / 1 - z

    RealPartSubtractQuarkLO = -LogInvZeta;

    ImaginaryPartSubtractQuarkLO = PI;

    // NLO, quark, vector, eq. (B4)

    RealPartSubtractQuarkNLOV = Pi2 / 2. - 3. * DiLogInvZeta
            + LogInvZeta * (Pi2 + 9. + 3. * LogZeta - LogInvZeta2 / 3.);
    RealPartSubtractQuarkNLOV += m_logQ2OverMu2
            * (Pi2 - 3. * LogInvZeta - LogInvZeta2);
    RealPartSubtractQuarkNLOV *= m_CF / 2.;

    ImaginaryPartSubtractQuarkNLOV = Pi2 / 3. + 9. + 3. * LogZeta - LogInvZeta2
            - m_logQ2OverMu2 * (2. * LogInvZeta + 3);
    ImaginaryPartSubtractQuarkNLOV *= -PI * m_CF / 2.;

    // NLO, gluon, vector, eq. (B6)

    RealPartSubtractGluonNLOV = -1. + Pi2 / 3. * (1. - 3. / 4. * m_Zeta)
            + DiLogInvZeta - LogZeta * LogInvZeta;
    RealPartSubtractGluonNLOV += (2. - m_Zeta) * LogInvZeta
            * (1. - LogInvZeta / 4.);
    RealPartSubtractGluonNLOV += m_logQ2OverMu2 / 2.
            * (1. - (2. - m_Zeta) * LogInvZeta);
    RealPartSubtractGluonNLOV *= 1 / (2. * m_xi);

    ImaginaryPartSubtractGluonNLOV = (2. - m_Zeta)
            * (2. - m_logQ2OverMu2 - LogInvZeta) - 2. * LogZeta;
    ImaginaryPartSubtractGluonNLOV *= -PI / (4. * m_xi);

    // Computation of subtraction terms in eq. (8) and (9)
    // Sums up previous contributions and takes care of LO / NLO and P(olarized) / U(npolarized) subtleties

    // LO, real and imaginary parts

    m_realPartSubtractQuark = RealPartSubtractQuarkLO;
    m_imaginaryPartSubtractQuark = ImaginaryPartSubtractQuarkLO;

    m_realPartSubtractGluon = 0.;
    m_imaginaryPartSubtractGluon = 0.;

    // NLO, real and imaginary parts

    if (m_qcdOrderType == QCDOrderType::NLO) {

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

void DVCSCFFModel::computeSubtractionFunctionsA() {
    double LogZeta = log(m_Zeta);
    double LogInvZeta = log((1. - m_Zeta) / m_Zeta);
    double LogInvZeta2 = LogInvZeta * LogInvZeta;
    double DiLogInvZeta = TMath::DiLog(1. - 1. / m_Zeta);
    double Pi2 = PI * PI;

    double RealPartSubtractQuarkLO; // Real part of eq. (B2)
    double ImaginaryPartSubtractQuarkLO; // Imaginary part of eq. (B2)
    double RealPartSubtractQuarkNLOA; // Real part of eq. (B4)
    double ImaginaryPartSubtractQuarkNLOA; // Imaginary part of eq. (B4)
    double RealPartSubtractGluonNLOA; // Real part of eq. (B6)
    double ImaginaryPartSubtractGluonNLOA; // Imaginary part of eq. (B6)

    // LO, 1 / 1 - z

    RealPartSubtractQuarkLO = -LogInvZeta;

    ImaginaryPartSubtractQuarkLO = PI;

    // NLO, quark, axial, eq. (B3)

    RealPartSubtractQuarkNLOA = Pi2 / 6. - DiLogInvZeta
            + LogInvZeta * (Pi2 + 9. + LogZeta - LogInvZeta2 / 3.);
    RealPartSubtractQuarkNLOA += m_logQ2OverMu2
            * (Pi2 - 3. * LogInvZeta - LogInvZeta2);
    RealPartSubtractQuarkNLOA *= m_CF / 2.;

    ImaginaryPartSubtractQuarkNLOA = Pi2 / 3. + 9. + LogZeta - LogInvZeta2
            - m_logQ2OverMu2 * (2. * LogInvZeta + 3);
    ImaginaryPartSubtractQuarkNLOA *= -PI * m_CF / 2.;

    // NLO, gluon, axial, eq. (B5)

    RealPartSubtractGluonNLOA = 1. + Pi2 / 4. * m_Zeta
            + m_Zeta * LogInvZeta * (1. - LogInvZeta / 4.);
    RealPartSubtractGluonNLOA += -m_logQ2OverMu2 / 2.
            * (1. + m_Zeta * LogInvZeta);
    RealPartSubtractGluonNLOA *= 1 / (2. * m_xi);

    ImaginaryPartSubtractGluonNLOA = 2. - LogInvZeta - m_logQ2OverMu2;
    ImaginaryPartSubtractGluonNLOA *= -PI * m_Zeta / (4. * m_xi);

    // Computation of subtraction terms in eq. (8) and (9)
    // Sums up previous contributions and takes care of LO / NLO and P(olarized) / U(npolarized) subtleties

    // LO, real and imaginary parts

    m_realPartSubtractQuark = RealPartSubtractQuarkLO;
    m_imaginaryPartSubtractQuark = ImaginaryPartSubtractQuarkLO;

    m_realPartSubtractGluon = 0.;
    m_imaginaryPartSubtractGluon = 0.;

    // NLO, real and imaginary parts

    if (m_qcdOrderType == QCDOrderType::NLO) {

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

std::complex<double> DVCSCFFModel::computeIntegralsV() {
    double IntegralRealPartKernelQuark1 = 0.; // Integral between 0 and fZeta in real part of amplitude
    double IntegralRealPartKernelQuark2 = 0.; // Integral between fZeta and 1 in real part of amplitude
    double IntegralImaginaryPartKernelQuark = 0.; // Integral between 0 and fZeta in imaginary part of amplitude

    double IntegralRealPartKernelGluon1 = 0.; // Integral between 0 and fZeta in real part of amplitude
    double IntegralRealPartKernelGluon2 = 0.; // Integral between fZeta and 1 in real part of amplitude
    double IntegralImaginaryPartKernelGluon = 0.; // Integral between 0 and fZeta in imaginary part of amplitude

    double SumSqrCharges; // Sum of square of electric charges of active quark flavours

    // Sums up integrals and subtraction constants to get real and imaginary parts of CFF.

    if (m_qcdOrderType != QCDOrderType::LO
            && m_qcdOrderType != QCDOrderType::NLO) {

        Formatter()
                << "[DVCSCFFModule::computeIntegrals] Erroneous input perturbative QCD order can only be LO or NLO. Here Order = "
                /*<< qcdOrderType.toString()*/;
        throw std::runtime_error("");
    }

    // Compute sum of active quark electric charges squared

    switch (m_nbOfActiveFlavour) {

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
        throw std::runtime_error(
                Formatter()
                        << "[DVCSCFFModule::computeIntegrals] Erroneous input number of active quark flavours should be an integer between 3 and 6. Number of active quark flavours = "
                        << m_nbOfActiveFlavour);
    }

    // Define functions and compute integrals

    ROOT::Math::Integrator integrator(
            ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR, 0., 1.e-3);

    //  ROOT::Math::Integrator Integrator( ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR, 0., 1.e-9 );

    //  Integrator.SetAbsTolerance( 0. );
    //   Integrator.SetRelTolerance( 0.001 );
    //   Integrator.SetNPoints( 1 );

    // Quark sector

    IntegralRealPartKernelQuark1 = m_pMathIntegratorModule->compute(
            DVCSCFFModel::CONVOL_RE_KERNEL_QUARK_1V, this, 0., +m_xi);

    IntegralRealPartKernelQuark2 = m_pMathIntegratorModule->compute(
            DVCSCFFModel::CONVOL_RE_KERNEL_QUARK_2V, this, +m_xi, +1);

    IntegralImaginaryPartKernelQuark = m_pMathIntegratorModule->compute(
            DVCSCFFModel::CONVOL_IM_KERNEL_QUARK_V, this, +m_xi, +1);

    // Gluon sector

    if (m_qcdOrderType == QCDOrderType::NLO) {
        IntegralRealPartKernelGluon1 = m_pMathIntegratorModule->compute(
                DVCSCFFModel::CONVOL_RE_KERNEL_GLUON_1V, this, 0., +m_xi);

        IntegralRealPartKernelGluon2 = m_pMathIntegratorModule->compute(
                DVCSCFFModel::CONVOL_RE_KERNEL_GLUON_2V, this, +m_xi, +1);

        IntegralImaginaryPartKernelGluon = m_pMathIntegratorModule->compute(
                DVCSCFFModel::CONVOL_IM_KERNEL_GLUON_V, this, +m_xi, +1);
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

    double fImaginaryPartCFF = IntegralImaginaryPartKernelQuark
            + m_quarkDiagonal * m_imaginaryPartSubtractQuark;
    fImaginaryPartCFF += SumSqrCharges
            * (IntegralImaginaryPartKernelGluon
                    + m_gluonDiagonal * m_imaginaryPartSubtractGluon);
    // Multiplication by SumSqrCharges corrects in mistake in eq. (9)

    return std::complex<double>(fRealPartCFF, fImaginaryPartCFF);
}

std::complex<double> DVCSCFFModel::computeIntegralsA() {
    double IntegralRealPartKernelQuark1 = 0.; // Integral between 0 and fZeta in real part of amplitude
    double IntegralRealPartKernelQuark2 = 0.; // Integral between fZeta and 1 in real part of amplitude
    double IntegralImaginaryPartKernelQuark = 0.; // Integral between 0 and fZeta in imaginary part of amplitude

    double IntegralRealPartKernelGluon1 = 0.; // Integral between 0 and fZeta in real part of amplitude
    double IntegralRealPartKernelGluon2 = 0.; // Integral between fZeta and 1 in real part of amplitude
    double IntegralImaginaryPartKernelGluon = 0.; // Integral between 0 and fZeta in imaginary part of amplitude

    double SumSqrCharges; // Sum of square of electric charges of active quark flavours

    // Sums up integrals and subtraction constants to get real and imaginary parts of CFF.

    if (m_qcdOrderType != QCDOrderType::LO
            && m_qcdOrderType != QCDOrderType::NLO) {

        throw std::runtime_error(
                Formatter()
                        << "[DVCSCFFModule::computeIntegralsA] Erroneous input perturbative QCD order can only be LO or NLO. Here Order = "
                        << QCDOrderType(m_qcdOrderType).toString());

    }

    // Compute sum of active quark electric charges squared

    switch (m_nbOfActiveFlavour) {

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
        throw std::runtime_error(
                Formatter()
                        << "[DVCSCFFModule::computeIntegralsA] Erroneous input number of active quark flavours should be an integer between 3 and 6. Number of active quark flavours = "
                        << m_nbOfActiveFlavour);
    }

    // Define functions and compute integrals

    ROOT::Math::Integrator Integrator(
            ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR, 0., 1.e-3);

    //  ROOT::Math::Integrator Integrator( ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR, 0., 1.e-9 );

    //  Integrator.SetAbsTolerance( 0. );
    //   Integrator.SetRelTolerance( 0.001 );
    //   Integrator.SetNPoints( 1 );

    // Quark sector

    IntegralRealPartKernelQuark1 = m_pMathIntegratorModule->compute(
            DVCSCFFModel::CONVOL_RE_KERNEL_QUARK_1A, this, 0., +m_xi);

    IntegralRealPartKernelQuark2 = m_pMathIntegratorModule->compute(
            DVCSCFFModel::CONVOL_RE_KERNEL_QUARK_2A, this, +m_xi, +1);

    IntegralImaginaryPartKernelQuark = m_pMathIntegratorModule->compute(
            DVCSCFFModel::CONVOL_IM_KERNEL_QUARK_A, this, +m_xi, +1);

    // Gluon sector

    if (m_qcdOrderType == QCDOrderType::NLO) {
        IntegralRealPartKernelGluon1 = m_pMathIntegratorModule->compute(
                DVCSCFFModel::CONVOL_RE_KERNEL_GLUON_1A, this, 0., +m_xi);

        IntegralRealPartKernelGluon2 = m_pMathIntegratorModule->compute(
                DVCSCFFModel::CONVOL_RE_KERNEL_GLUON_2A, this, +m_xi, +1);

        IntegralImaginaryPartKernelGluon = m_pMathIntegratorModule->compute(
                DVCSCFFModel::CONVOL_IM_KERNEL_GLUON_A, this, +m_xi, +1);
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

    double imaginaryPartCFF = IntegralImaginaryPartKernelQuark
            + m_quarkDiagonal * m_imaginaryPartSubtractQuark;
    //	cout << fpQCDOrder << "      ImaginaryPartCFF Quark = " << fImaginaryPartCFF << endl ;
    imaginaryPartCFF += SumSqrCharges
            * (IntegralImaginaryPartKernelGluon
                    + m_gluonDiagonal * m_imaginaryPartSubtractGluon);
    //	cout << fpQCDOrder << "      ImaginaryPartCFF Gluon = " << SumSqrCharges * ( IntegralImaginaryPartKernelGluon + fGluonDiagonal * fImaginaryPartSubtractGluon ) << endl ;
    // Multiplication by SumSqrCharges corrects in mistake in eq. (9)

    return std::complex<double>(realPartCFF, imaginaryPartCFF);
}

std::complex<double> DVCSCFFModel::KernelQuarkNLOV(double x) {
    double z = x / m_xi;

    std::complex<double> LogOneMinusz(0., 0.);
    if (x < m_xi) {
// remplacer z par x/xi
        LogOneMinusz = std::complex<double>(0., 0.);
        LogOneMinusz = std::complex<double>(TMath::Log((1. - z) / 2.), 0.);
    }
    if (x > m_xi) {
        LogOneMinusz = std::complex<double>(TMath::Log((z - 1.) / 2.), -PI);
    }

    return KernelQuarkNLOA(x) - (m_CF / (1. + z)) * LogOneMinusz;
}

/*!
 * \fn KernelQuark( double x )
 *
 * T^{q, V/A} in appendix A, eq. (A1).
 *
 */
std::complex<double> DVCSCFFModel::KernelQuarkV(double x) {
    //std::complex<double> z = std::complex<double>(x / m_xi, 0.);
    //std::complex<double> quark();

    double z = x / m_xi;

//    TComplex z = TComplex(x / fXi, 0.);
//    TComplex Quark;

    std::complex<double> quark(1. / (1. - z), 0.);

    if (m_qcdOrderType == QCDOrderType::NLO) {

        quark += m_alphaSOver2Pi * KernelQuarkNLOV(x);

        if (m_qcdOrderType != QCDOrderType::LO
                && m_qcdOrderType != QCDOrderType::NLO) {

            throw std::runtime_error(
                    Formatter()
                            << "[DVCSCFFModule::KernelQuarkV] Erroneous input, perturbative QCD order can only be LO or NLO. Here Order = "
                            << QCDOrderType(m_qcdOrderType).toString());
            //TODO throw an exception
        }
    }

    return quark;
}

std::complex<double> DVCSCFFModel::KernelQuarkA(double x) {
    //std::complex<double> z = std::complex<double>(x / m_xi, 0.);
    //std::complex<double> quark();

    double z = x / m_xi;

//    TComplex z = TComplex(x / fXi, 0.);
//    TComplex Quark;

    std::complex<double> quark(1. / (1. - z), 0.);

    if (m_qcdOrderType == QCDOrderType::NLO) {

        quark += m_alphaSOver2Pi * KernelQuarkNLOA(x);

        if (m_qcdOrderType != QCDOrderType::LO
                && m_qcdOrderType != QCDOrderType::NLO) {

            throw std::runtime_error(
                    Formatter()
                            << "[DVCSCFFModule::KernelQuarkA] Erroneous input, perturbative QCD order can only be LO or NLO. Here Order = "
                            << QCDOrderType(m_qcdOrderType).toString());
            //TODO throw an exception
        }
    }

    return quark;
}

/*!
 * \fn KernelGluon( double x )
 *
 * T^{g, V/A} in appendix A, eq. (A1).
 *
 */
std::complex<double> DVCSCFFModel::KernelGluonV(double x) {

    std::complex<double> gluon(0., 0.);

    if (m_qcdOrderType == QCDOrderType::NLO) {

        gluon += m_alphaSOver2Pi * KernelGluonNLOV(x);

        if (m_qcdOrderType != QCDOrderType::LO
                && m_qcdOrderType != QCDOrderType::NLO) {
            Formatter()
                    << "[DVCSCFFModule::KernelGluonV] Erroneous input, perturbative QCD order can only be LO or NLO. Here Order = "
                    << QCDOrderType(m_qcdOrderType).toString();
            //TODO throw an exception
        }
    }

    return gluon;
}

std::complex<double> DVCSCFFModel::KernelGluonA(double x) {

    std::complex<double> gluon(0., 0.);

    if (m_qcdOrderType == QCDOrderType::NLO) {

        gluon += m_alphaSOver2Pi * KernelGluonNLOA(x);

        if (m_qcdOrderType != QCDOrderType::LO
                && m_qcdOrderType != QCDOrderType::NLO) {
            Formatter()
                    << "[DVCSCFFModule::KernelGluonA] Erroneous input, perturbative QCD order can only be LO or NLO. Here Order = "
                    << QCDOrderType(m_qcdOrderType).toString();
            //TODO throw an exception
        }
    }

    return gluon;
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
double DVCSCFFModel::ConvolReKernelQuark1V(const double x) {

    GPDOutputData gpdOutputData = m_pGPDModule->compute(x, m_xi, m_t, m_MuF,
            m_MuR, m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = computeSquareChargeAveragedGPD(gpdOutputData);

    // Integrated function
    double Convol = (EvalGPD - m_quarkDiagonal) * KernelQuarkV(x).real();
    Convol += (-EvalGPD - m_quarkDiagonal) * KernelQuarkV(-x).real();

    Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

    return Convol;
}

/*
 * \fn double ConvolReKernelQuark2( double* x, double* Parameters );
 *
 * eq. (8), real part of amplitude, terms integrated between fZeta and 1.
 * Equivalently x integration domain ranges between fXi and 1.
 *
 */
double DVCSCFFModel::ConvolReKernelQuark2V(const double x) {
    GPDOutputData gpdOutputData = m_pGPDModule->compute(x, m_xi, m_t, m_MuF,
            m_MuR, m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = computeSquareChargeAveragedGPD(gpdOutputData);

    double Convol = EvalGPD - m_quarkDiagonal;
    Convol *= KernelQuarkV(x).real();

    Convol += -KernelQuarkV(-x).real() * EvalGPD;

    Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

    return Convol;
}

/*
 * \fn double ConvolImKernelQuark( double* x, double* Parameters );
 *
 * eq. (8), imaginary part of amplitude, terms integrated between fZeta and 1.
 * Equivalently x integration domain ranges between fXi and 1.
 *
 */
double DVCSCFFModel::ConvolImKernelQuarkV(const double x) {
    GPDOutputData gpdOutputData = m_pGPDModule->compute(x, m_xi, m_t, m_MuF,
            m_MuR, m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = computeSquareChargeAveragedGPD(gpdOutputData);

    double Convol = EvalGPD - m_quarkDiagonal;
    Convol *= KernelQuarkV(x).imag();
    Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

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
double DVCSCFFModel::ConvolReKernelGluon1V(const double x) {
    GPDOutputData gpdOutputData = m_pGPDModule->compute(x, m_xi, m_t, m_MuF,
            m_MuR, m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD =
            2.
                    * (gpdOutputData.getGPDResultData(m_currentGPDComputeType))->getGluon();

    double Convol = (EvalGPD - m_gluonDiagonal) * KernelGluonV(x).real();
    Convol += (+EvalGPD - m_gluonDiagonal) * KernelGluonV(-x).real();

    Convol /= (m_xi * m_xi * m_nbOfActiveFlavour);

    return Convol;
}

/*
 * \fn double ConvolReKernelGluon2( double* x, double* Parameters );
 *
 * eq. (8), real part of amplitude, terms integrated between fZeta and 1.
 * Equivalently x integration domain ranges between fXi and 1.
 *
 */
double DVCSCFFModel::ConvolReKernelGluon2V(const double x) {
    GPDOutputData gpdOutputData = m_pGPDModule->compute(x, m_xi, m_t, m_MuF,
            m_MuR, m_currentGPDComputeType);

    double EvalGPD =
            2.
                    * (gpdOutputData.getGPDResultData(m_currentGPDComputeType))->getGluon();

    double Convol = EvalGPD - m_gluonDiagonal;
    Convol *= KernelGluonV(x).real();

    Convol += +KernelGluonV(-x).real() * EvalGPD;

    Convol /= (m_xi * m_xi * m_nbOfActiveFlavour); // In eq. (8), ( ( 2 - fZeta ) / fZeta )^2 = 1 / fXi^2

    return Convol;
}

/*
 * \fn double ConvolImKernelGluon( double* x, double* Parameters );
 *
 * eq. (9), imaginary part of amplitude, terms integrated between fZeta and 1.
 * Equivalently x integration domain ranges between fXi and 1.
 *
 */
double DVCSCFFModel::ConvolImKernelGluonV(const double x) {
    GPDOutputData gpdOutputData = m_pGPDModule->compute(x, m_xi, m_t, m_MuF,
            m_MuR, m_currentGPDComputeType);

    double EvalGPD =
            2.
                    * (gpdOutputData.getGPDResultData(m_currentGPDComputeType))->getGluon();

    double Convol = EvalGPD - m_gluonDiagonal;
    Convol *= KernelGluonV(x).imag();
    Convol /= (m_xi * m_xi * m_nbOfActiveFlavour); // In eq. (8), ( ( 2 - fZeta ) / fZeta )^2 = 1 / fXi^2

    return Convol;
}

std::complex<double> DVCSCFFModel::KernelGluonNLOV(double x) {
    m_pLoggerManager->debug(getClassName(), __func__, "entered");

    double z = x / m_xi;

    std::complex<double> LogOneMinusz(0., 0.);
    if (x < m_xi) {
        // remplacer z par x/xi
        LogOneMinusz = std::complex<double>(TMath::Log((1. - z) / 2.), 0.);
    }
    if (x > m_xi) {
        LogOneMinusz = std::complex<double>(TMath::Log((z - 1.) / 2.), -PI);
    }

    std::complex<double> GluonNLOV(LogOneMinusz);
    GluonNLOV += (m_logQ2OverMu2 - 2.);
    GluonNLOV /= 1. - z;
    GluonNLOV += LogOneMinusz / (1. + z);
    GluonNLOV *= (m_nbOfActiveFlavour / 2.);
    GluonNLOV += -KernelGluonNLOA(x);

    return GluonNLOV;
}

std::complex<double> DVCSCFFModel::KernelGluonNLOA(double x) {
    double z = x / m_xi;
    std::complex<double> LogOneMinusz(0., 0.);
    if (x < m_xi) {
        // remplacer z par x/xi
        LogOneMinusz = std::complex<double>(TMath::Log((1. - z) / 2.), 0.);
    }
    if (x > m_xi) {
        LogOneMinusz = std::complex<double>(TMath::Log((z - 1.) / 2.), -PI);
    }

    std::complex<double> GluonNLOA(LogOneMinusz);
    GluonNLOA += (m_logQ2OverMu2 - 2.);
    GluonNLOA *= 1 / (1 - z * z);
    GluonNLOA *= (1. / (1. - (z * z)) + LogOneMinusz / ((1. + z) * (1. + z)));
    GluonNLOA += -LogOneMinusz * LogOneMinusz / (2. * (1. + z) * (1. + z));
    GluonNLOA *= (m_nbOfActiveFlavour / 2.);

    return GluonNLOA;
}

std::complex<double> DVCSCFFModel::KernelQuarkNLOA(double x) {

    double z = x / m_xi;
    std::complex<double> LogOneMinusz(0., 0.);
    if (x < m_xi) {
        // remplacer z par x/xi
        LogOneMinusz = std::complex<double>(TMath::Log((1. - z) / 2.), 0.);
    }
    if (x > m_xi) {
        LogOneMinusz = std::complex<double>(TMath::Log((z - 1.) / 2.), -PI);
    }

    std::complex<double> QuarkNLOA(m_logQ2OverMu2);
    QuarkNLOA += LogOneMinusz / 2. - (3. / 4.);
    QuarkNLOA *= 2. * LogOneMinusz + 3.;
    QuarkNLOA += -(27. / 4.) - (1. - z) / (1. + z) * LogOneMinusz;
    QuarkNLOA *= m_CF / 2. * (1. - z);

    return QuarkNLOA;
}

double DVCSCFFModel::ConvolReKernelQuark1A(const double x) {
    GPDOutputData gpdOutputData = m_pGPDModule->compute(x, m_xi, m_t, m_MuF,
            m_MuR, m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = computeSquareChargeAveragedGPD(gpdOutputData);

    double Convol = (EvalGPD - m_quarkDiagonal) * KernelQuarkA(x).real();
    Convol += (+EvalGPD - m_quarkDiagonal) * KernelQuarkA(-x).real();

    Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

    return Convol;
}

double DVCSCFFModel::ConvolReKernelQuark2A(const double x) {
    GPDOutputData gpdOutputData = m_pGPDModule->compute(x, m_xi, m_t, m_MuF,
            m_MuR, m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = computeSquareChargeAveragedGPD(gpdOutputData);

    double Convol = EvalGPD - m_quarkDiagonal;
    Convol *= KernelQuarkA(x).real();

    Convol += +KernelQuarkA(-x).real() * EvalGPD;

    Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

    return Convol;
}

double DVCSCFFModel::ConvolImKernelQuarkA(const double x) {
    GPDOutputData gpdOutputData = m_pGPDModule->compute(x, m_xi, m_t, m_MuF,
            m_MuR, m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = computeSquareChargeAveragedGPD(gpdOutputData);

    double Convol = EvalGPD - m_quarkDiagonal;
    Convol *= KernelQuarkA(x).imag();
    Convol /= m_xi; // In eq. (8), ( 2 - fZeta ) / fZeta = 1 / fXi

    return Convol;
}

double DVCSCFFModel::ConvolReKernelGluon1A(const double x) {
    GPDOutputData gpdOutputData = m_pGPDModule->compute(x, m_xi, m_t, m_MuF,
            m_MuR, m_currentGPDComputeType);

    double EvalGPD =
            2.
                    * (gpdOutputData.getGPDResultData(m_currentGPDComputeType))->getGluon();

    double Convol = (EvalGPD - m_gluonDiagonal) * KernelGluonA(x).real();
    Convol += (-EvalGPD - m_gluonDiagonal) * KernelGluonA(-x).real();

    Convol /= (m_xi * m_xi * m_nbOfActiveFlavour);

    return Convol;
}

double DVCSCFFModel::ConvolReKernelGluon2A(const double x) {
    GPDOutputData gpdOutputData = m_pGPDModule->compute(x, m_xi, m_t, m_MuF,
            m_MuR, m_currentGPDComputeType);

    double EvalGPD =
            2.
                    * (gpdOutputData.getGPDResultData(m_currentGPDComputeType))->getGluon();

    double Convol = EvalGPD - m_gluonDiagonal;
    Convol *= KernelGluonA(x).real();

    Convol += -KernelGluonA(-x).real() * EvalGPD;

    Convol /= (m_xi * m_xi * m_nbOfActiveFlavour); // In eq. (8), ( ( 2 - fZeta ) / fZeta )^2 = 1 / fXi^2

    return Convol;
}

double DVCSCFFModel::ConvolImKernelGluonA(const double x) {
    GPDOutputData gpdOutputData = m_pGPDModule->compute(x, m_xi, m_t, m_MuF,
            m_MuR, m_currentGPDComputeType);

    double EvalGPD =
            2.
                    * (gpdOutputData.getGPDResultData(m_currentGPDComputeType))->getGluon();

    double Convol = EvalGPD - m_gluonDiagonal;
    Convol *= KernelGluonA(x).imag();
    Convol /= (m_xi * m_xi * m_nbOfActiveFlavour); // In eq. (8), ( ( 2 - fZeta ) / fZeta )^2 = 1 / fXi^2

    return Convol;
}

double DVCSCFFModel::functionsToIntegrate(const double * x,
        const double * parameters) {
    double result = 0.;

    switch (m_currentFunctionToIntegrate) {
    case DVCSCFFModel::CONVOL_RE_KERNEL_QUARK_1V: {
        result = ConvolReKernelQuark1V(x[0]);
        break;
    }
    case DVCSCFFModel::CONVOL_RE_KERNEL_QUARK_2V: {
        result = ConvolReKernelQuark2V(x[0]);
        break;
    }
    case DVCSCFFModel::CONVOL_IM_KERNEL_QUARK_V: {
        result = ConvolImKernelQuarkV(x[0]);
        break;
    }
    case DVCSCFFModel::CONVOL_RE_KERNEL_GLUON_1V: {
        result = ConvolReKernelGluon1V(x[0]);
        break;
    }
    case DVCSCFFModel::CONVOL_RE_KERNEL_GLUON_2V: {
        result = ConvolReKernelGluon2V(x[0]);
        break;
    }
    case DVCSCFFModel::CONVOL_IM_KERNEL_GLUON_V: {
        result = ConvolImKernelGluonV(x[0]);
        break;
    }
    case DVCSCFFModel::CONVOL_RE_KERNEL_QUARK_1A: {
        result = ConvolReKernelQuark1A(x[0]);
        break;
    }
    case DVCSCFFModel::CONVOL_RE_KERNEL_QUARK_2A: {
        result = ConvolReKernelQuark2A(x[0]);
        break;
    }
    case DVCSCFFModel::CONVOL_IM_KERNEL_QUARK_A: {
        result = ConvolImKernelQuarkA(x[0]);
        break;
    }
    case DVCSCFFModel::CONVOL_RE_KERNEL_GLUON_1A: {
        result = ConvolReKernelGluon1A(x[0]);
        break;
    }
    case DVCSCFFModel::CONVOL_RE_KERNEL_GLUON_2A: {
        result = ConvolReKernelGluon2A(x[0]);
        break;
    }
    case DVCSCFFModel::CONVOL_IM_KERNEL_GLUON_A: {
        result = ConvolImKernelGluonA(x[0]);
        break;
    }
    }

    return result;
}
