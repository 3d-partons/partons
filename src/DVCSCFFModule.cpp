#include "DVCSCFFModule.h"

#include <TMath.h>
#include <cmath>
#include <stdexcept>

#include "BaseModuleFactory.h"
#include "GPDModule.h"
#include "models/FundamentalPhysicalConstants.h"
#include "models/GPDComputeType.h"
#include "models/GPDOutputData.h"
#include "models/GPDResultData.h"
#include "models/QCDOrderType.h"
#include "RunningAlphaStrong.h"

// Initialise GK11GPDModule::ID with a unique name.
const std::string DVCSCFFModule::moduleID = "DVCSCFFModule";

// Define a useless static boolean variable to enable registerModule() to be executed before the main() function.
// Because global variables have program scope, and are initialised before main() is called.
// !!! CARE !!! variable name must be unique (ex : isGK11ModelRegistered).
static bool isDVCSCFFModuleRegistered =
        BaseModuleFactory::getInstance()->registerModule(new DVCSCFFModule());

//TODO implementer l'ID en question (voir GK11Model)
DVCSCFFModule::DVCSCFFModule() :
        CFFModule(DVCSCFFModule::moduleID), m_CF(4. / 3.) {

}

DVCSCFFModule::~DVCSCFFModule() {

}

std::pair<double, double> DVCSCFFModule::compute(const double &xB,
        const double &t, const double Q2, const double &MuF, const double &MuR,
        const QCDOrderType &qcdOrderType) {
    std::pair<double, double> result;

    if (!isModuleConfigured()) {
        throw std::runtime_error("[DVCSCFFModule] wrong configuration !");
    }

    //TODO convert xB to xi
    updateVariables(xB, Q2, MuF, MuR);

    computeDiagonalGPD(t, MuF, MuR);
    computeSubtractionFunctions(qcdOrderType);
    computeIntegrals();

    return result;
}

void DVCSCFFModule::computeDiagonalGPD(const double & t, const double & MuF,
        const double & MuR) {
    GPDOutputData* pGPDOutputData = m_pGPDModule->compute(m_xi, m_xi, t, MuF,
            MuR);
    GPDResultData* pGPDResultData = pGPDOutputData->getGPDResultData(
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

void DVCSCFFModule::computeIntegrals() {

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
