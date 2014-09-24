#include "DVCSCFFModule.h"

#include <TMath.h>
#include <cmath>
#include <stdexcept>

#include "../../beans/gpd/GPDComputeType.h"
#include "../../beans/gpd/GPDOutputData.h"
#include "../../beans/gpd/GPDResultData.h"
#include "../../FundamentalPhysicalConstants.h"
#include "../../services/ModuleObjectFactory.h"
#include "../../utils/stringUtils/Formatter.h"
#include "../alphaS/RunningAlphaStrong.h"
#include "../GPDModule.h"

// Initialise GK11GPDModule::ID with a unique name.
const std::string DVCSCFFModule::moduleID = "DVCSCFFModule";

// Define a useless static boolean variable to enable registerModule() to be executed before the main() function.
// Because global variables have program scope, and are initialised before main() is called.
// !!! CARE !!! variable name must be unique (ex : isGK11ModelRegistered).
static bool isDVCSCFFModuleRegistered =
        ModuleObjectFactory::getInstance()->registerModule(new DVCSCFFModule());

//TODO implementer l'ID en question (voir GK11Model)
DVCSCFFModule::DVCSCFFModule() :
        CFFModule(DVCSCFFModule::moduleID), m_CF(4. / 3.) {

}

DVCSCFFModule* DVCSCFFModule::clone() const {
    return new DVCSCFFModule(*this);
}

DVCSCFFModule::~DVCSCFFModule() {

}

std::complex<double> DVCSCFFModule::compute(const double &xB, const double &t,
        const double Q2, const double &MuF, const double &MuR,
        const QCDOrderType &qcdOrderType) {
    std::complex<double> result;

    if (!isModuleConfigured()) {
        throw std::runtime_error("[DVCSCFFModule] wrong configuration !");
    }

    //TODO convert xB to xi
    updateVariables(xB, Q2, MuF, MuR);

    computeDiagonalGPD(t, MuF, MuR);
    computeSubtractionFunctions(qcdOrderType);
    computeIntegrals(qcdOrderType);

    return result;
}

void DVCSCFFModule::computeDiagonalGPD(const double & t, const double & MuF,
        const double & MuR) {
    GPDOutputData gpdOutputData = m_pGPDModule->compute(m_xi, m_xi, t, MuF,
            MuR);
    GPDResultData* pGPDResultData = gpdOutputData.getGPDResultData(
            GPDComputeType::H);

    //TODO compute CFF singlet
    //computeCFFSinglet(pGPDResultData);
    m_quarkDiagonal = pGPDResultData->getSinglet();
    m_gluonDiagonal = 2. * pGPDResultData->getGluon();
}

void DVCSCFFModule::computeSubtractionFunctions(
        const QCDOrderType &qcdOrderType) {
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

    if (qcdOrderType.getType() == QCDOrderType::NLO) {

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

void DVCSCFFModule::computeIntegrals(const QCDOrderType &qcdOrderType) {
    double IntegralRealPartKernelQuark1; // Integral between 0 and fZeta in real part of amplitude
    double IntegralRealPartKernelQuark2; // Integral between fZeta and 1 in real part of amplitude
    double IntegralImaginaryPartKernelQuark; // Integral between 0 and fZeta in imaginary part of amplitude

    double IntegralRealPartKernelGluon1; // Integral between 0 and fZeta in real part of amplitude
    double IntegralRealPartKernelGluon2; // Integral between fZeta and 1 in real part of amplitude
    double IntegralImaginaryPartKernelGluon; // Integral between 0 and fZeta in imaginary part of amplitude

    double SumSqrCharges; // Sum of square of electric charges of active quark flavours

    // Sums up integrals and subtraction constants to get real and imaginary parts of CFF.

    if (qcdOrderType.getType() != QCDOrderType::LO
            && qcdOrderType.getType() != QCDOrderType::NLO) {

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
    /*
     // Define functions and compute integrals

     double* DummyPointer = NULL; // dummy variable for parameters in Integral method of TF1 (no parameters for us)
     ROOT::Math::Integrator Integrator(
     ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR, 0., 1.e-3);

     //  ROOT::Math::Integrator Integrator( ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR, 0., 1.e-9 );

     //  Integrator.SetAbsTolerance( 0. );
     //   Integrator.SetRelTolerance( 0.001 );
     //   Integrator.SetNPoints( 1 );

     // Quark sector

     TF1 ConvolRealPartKernelQuark1("ConvolReKernelQuark1", this,
     &TDVCSComptonFormFactorH::ConvolReKernelQuark1, 0., 1., 0,
     "TDVCSComptonFormFactorH", "ConvolReKernelQuark1");
     ROOT::Math::WrappedTF1 WrappedConvolRealPartKernelQuark1(
     ConvolRealPartKernelQuark1);
     Integrator.SetFunction(WrappedConvolRealPartKernelQuark1);
     //  IntegralRealPartKernelQuark1 =  Integrator.Integral( 1.e-8, +fXi );
     IntegralRealPartKernelQuark1 = Integrator.Integral(0., +m_xi);

     TF1 ConvolRealPartKernelQuark2("ConvolReKernelQuark2", this,
     &TDVCSComptonFormFactorH::ConvolReKernelQuark2, 0., 1., 0,
     "TDVCSComptonFormFactorH", "ConvolReKernelQuark2");
     ROOT::Math::WrappedTF1 WrappedConvolRealPartKernelQuark2(
     ConvolRealPartKernelQuark2);
     Integrator.SetFunction(WrappedConvolRealPartKernelQuark2);
     IntegralRealPartKernelQuark2 = Integrator.Integral(+m_xi, +1.);

     TF1 ConvolImaginaryPartKernelQuark("ConvolImKernelQuark", this,
     &TDVCSComptonFormFactorH::ConvolImKernelQuark, 0., 1., 0,
     "TDVCSComptonFormFactorH", "ConvolImKernelQuark");
     ROOT::Math::WrappedTF1 WrappedConvolImaginaryPartKernelQuark(
     ConvolImaginaryPartKernelQuark);
     Integrator.SetFunction(WrappedConvolImaginaryPartKernelQuark);
     IntegralImaginaryPartKernelQuark = Integrator.Integral(+m_xi, +1.);

     // Gluon sector

     IntegralRealPartKernelGluon1 = 0.;
     IntegralRealPartKernelGluon2 = 0.;
     IntegralImaginaryPartKernelGluon = 0.;

     if (qcdOrderType.getType() == QCDOrderType::NLO) {

     TF1 ConvolRealPartKernelGluon1("ConvolReKernelGluon1", this,
     &TDVCSComptonFormFactorH::ConvolReKernelGluon1, 0., 1., 0,
     "TDVCSComptonFormFactorH", "ConvolReKernelGluon1");
     ROOT::Math::WrappedTF1 WrappedConvolRealPartKernelGluon1(
     ConvolRealPartKernelGluon1);
     Integrator.SetFunction(WrappedConvolRealPartKernelGluon1);
     //     IntegralRealPartKernelGluon1 =  Integrator.Integral( 1.e-8, +fXi );
     IntegralRealPartKernelGluon1 = Integrator.Integral(0., +m_xi);

     TF1 ConvolRealPartKernelGluon2("ConvolReKernelGluon2", this,
     &TDVCSComptonFormFactorH::ConvolReKernelGluon2, 0., 1., 0,
     "TDVCSComptonFormFactorH", "ConvolReKernelGluon2");
     ROOT::Math::WrappedTF1 WrappedConvolRealPartKernelGluon2(
     ConvolRealPartKernelGluon2);
     Integrator.SetFunction(WrappedConvolRealPartKernelGluon2);
     IntegralRealPartKernelGluon2 = Integrator.Integral(+m_xi, +1.);

     TF1 ConvolImaginaryPartKernelGluon("ConvolImKernelGluon", this,
     &TDVCSComptonFormFactorH::ConvolImKernelGluon, 0., 1., 0,
     "TDVCSComptonFormFactorH", "ConvolImKernelGluon");
     ROOT::Math::WrappedTF1 WrappedConvolImaginaryPartKernelGluon(
     ConvolImaginaryPartKernelGluon);
     Integrator.SetFunction(WrappedConvolImaginaryPartKernelGluon);
     IntegralImaginaryPartKernelGluon = Integrator.Integral(+m_xi, +1.);
     }

     // Compute Subtraction constants (different at LO or NLO)

     computeSubtractionFunctions(qcdOrderType);

     // Compute real and imaginary part of CFF according to eq. (8) and eq. (9)

     fRealPartCFF = IntegralRealPartKernelQuark1 + IntegralRealPartKernelQuark2
     + fQuarkDiagonal * fRealPartSubtractQuark;
     fRealPartCFF += SumSqrCharges
     * (IntegralRealPartKernelGluon1 + IntegralRealPartKernelGluon2
     + fGluonDiagonal * fRealPartSubtractGluon);
     // Multiplication by SumSqrCharges corrects in mistake in eq. (9)

     fImaginaryPartCFF = IntegralImaginaryPartKernelQuark
     + fQuarkDiagonal * fImaginaryPartSubtractQuark;
     fImaginaryPartCFF += SumSqrCharges
     * (IntegralImaginaryPartKernelGluon
     + fGluonDiagonal * fImaginaryPartSubtractGluon);
     // Multiplication by SumSqrCharges corrects in mistake in eq. (9)

     delete DummyPointer;
     */
}

void DVCSCFFModule::updateVariables(const double & xB, const double & Q2,
        const double & MuF, const double & MuR) {
    m_xi = xB / (2 - xB);
    m_Q = sqrt(Q2);
    m_Zeta = 2. * m_xi / (1 + m_xi);
    m_logQ2OverMu2 = 2. * log(m_Q / MuF);
    m_nbOfActiveFlavour = 3;

    RunningAlphaStrong* Alpha = new RunningAlphaStrong();
    Alpha->SetRunningScale(MuR);
    m_alphaSOver2Pi = Alpha->GetAlphaS() / (2. * PI);
    delete Alpha;
}

bool DVCSCFFModule::isModuleConfigured() {
    bool isModuleConfigured = false;

    if (m_pGPDModule != 0 && m_qcdOrderType != 0) {
        isModuleConfigured = true;
    }

    return isModuleConfigured;
}
