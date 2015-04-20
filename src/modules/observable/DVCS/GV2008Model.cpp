#include "GV2008Model.h"

#include <Rtypes.h>
#include <TMath.h>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <iostream>

#include "../../../beans/gpd/GPDComputeType.h"
#include "../../../beans/observable/Observable.h"
#include "../../../FundamentalPhysicalConstants.h"
#include "../../../ModuleObjectFactory.h"
#include "../../../utils/logger/LoggerManager.h"
#include "../../../utils/MathUtils.h"
#include "../../../utils/stringUtils/Formatter.h"

// Initialise [class]::moduleID with a unique name.
const std::string GV2008Model::moduleID =
        ModuleObjectFactory::getInstance()->registerModule(
                new GV2008Model("GV2008Model"));

/*--------------------------------------- Constructors ---------------------------------*/

GV2008Model::GV2008Model(const std::string &className)
        : DVCSModule(className), m_qCM(Vector4D(0., 0., 0., 0.)), m_pCM(
                Vector4D(0., 0., 0., 0.)), m_qpCM(Vector4D(0., 0., 0., 0.)), m_ppCM(
                Vector4D(0., 0., 0., 0.)) {
    m_E = 5.77;
}

/*-------------------------------------- Destructor ------------------------------------*/

GV2008Model::~GV2008Model() {
}

GV2008Model::GV2008Model(const GV2008Model& other)
        : DVCSModule(other) {
    m_qCM = other.m_qCM;
    m_pCM = other.m_pCM;
    m_qpCM = other.m_qpCM;
    m_ppCM = other.m_ppCM;
}

GV2008Model* GV2008Model::clone() const {
    return new GV2008Model(*this);
}

void GV2008Model::initModule() {
    m_Q = sqrt(m_Q2);
    m_powerOfQ.push_back(m_Q2);
    m_powerOfQ.push_back(pow(m_Q, 3));
    m_powerOfQ.push_back(pow(m_Q, 4));
    m_powerOfQ.push_back(pow(m_Q, 5));

    m_powerOfProtonMass.push_back(pow(PROTON_MASS, 2));
    m_powerOfProtonMass.push_back(pow(PROTON_MASS, 4));
    m_powerOfProtonMass.push_back(pow(PROTON_MASS, 6));
    m_powerOfProtonMass.push_back(pow(PROTON_MASS, 8));

    m_xB2 = m_xB * m_xB;

    m_s = m_powerOfProtonMass[0] - m_powerOfQ[0] + m_powerOfQ[0] / m_xB;

    // Boundaries on the m_xB physical region
    // The value of m_xBMin comes from the requirement of omega to be real, and the value of m_xBMax expresses the fact that s >= 0.
    m_xBMin = 2. * m_E * m_powerOfQ[0]
            / (PROTON_MASS * (4 * pow(m_E, 2) - m_powerOfQ[0]));
    m_xBMax = m_powerOfQ[0] / (m_powerOfQ[0] - m_powerOfProtonMass[0]);

    // Omega

    m_y = (-m_powerOfQ[0] + 4 * m_E * PROTON_MASS * m_xB)
            / sqrt(
                    m_powerOfQ[2]
                            + 4 * m_powerOfProtonMass[0] * m_powerOfQ[0]
                                    * m_xB2);

    m_Omega = TMath::Log(m_y + sqrt(-1 + m_y) * sqrt(1 + m_y));

    // m_phaseSpace
    m_phaseSpace = (pow(POSITRON_CHARGE, 6) * m_powerOfQ[0])
            / (4096. * pow(PI, 5)
                    * sqrt(
                            4 * m_powerOfProtonMass[0] * m_powerOfQ[0]
                                    + m_powerOfQ[2] / m_xB2) * m_xB2
                    * pow(
                            m_powerOfQ[0] / (4. * m_xB)
                                    + (sqrt(
                                            4 * m_powerOfProtonMass[0]
                                                    * m_powerOfQ[0]
                                                    + m_powerOfQ[2] / m_xB2)
                                            * TMath::CosH(m_Omega)) / 4., 2));

    // Timelike coordinate

    m_qCM.setE(
            (m_powerOfQ[0] * (1 - 2 * m_xB))
                    / (2.
                            * sqrt(
                                    m_powerOfProtonMass[0]
                                            + (m_powerOfQ[0] * (1 - m_xB))
                                                    / m_xB) * m_xB));
    m_qpCM.setE(
            -(m_powerOfQ[0] * (-1 + m_xB))
                    / (2.
                            * sqrt(
                                    m_powerOfProtonMass[0]
                                            + (m_powerOfQ[0] * (1 - m_xB))
                                                    / m_xB) * m_xB));
    m_pCM.setE(
            (m_powerOfQ[0] + 2 * m_powerOfProtonMass[0] * m_xB)
                    / (2.
                            * sqrt(
                                    m_powerOfProtonMass[0]
                                            + (m_powerOfQ[0] * (1 - m_xB))
                                                    / m_xB) * m_xB));
    m_ppCM.setE(
            (2 * m_powerOfProtonMass[0] + (m_powerOfQ[0] * (1 - m_xB)) / m_xB)
                    / (2.
                            * sqrt(
                                    m_powerOfProtonMass[0]
                                            + (m_powerOfQ[0] * (1 - m_xB))
                                                    / m_xB)));

    // Spacelike coordinates

    m_qpCM.setX(
            sqrt(
                    (-(m_powerOfProtonMass[0] * pow(m_t, 2) * pow(m_xB, 2))
                            + m_powerOfQ[0] * m_t * m_xB
                                    * (m_t * (-1 + m_xB)
                                            - 2 * m_powerOfProtonMass[0] * m_xB)
                            + m_powerOfQ[2]
                                    * (m_t * (-1 + m_xB)
                                            - m_powerOfProtonMass[0]
                                                    * pow(m_xB, 2)))
                            / (m_powerOfQ[2]
                                    + 4 * m_powerOfProtonMass[0] * m_powerOfQ[0]
                                            * pow(m_xB, 2))));
    m_qCM.setZ(
            sqrt(
                    (m_powerOfQ[2]
                            + 4 * m_powerOfProtonMass[0] * m_powerOfQ[0]
                                    * pow(m_xB, 2))
                            / (m_xB
                                    * (m_powerOfQ[0] * (1 - m_xB)
                                            + m_powerOfProtonMass[0] * m_xB)))
                    / 2.);
    m_qpCM.setZ(
            (m_powerOfQ[2] * (-1 + m_xB)
                    - 2 * m_powerOfProtonMass[0] * m_t * pow(m_xB, 2)
                    + 2 * m_powerOfQ[0] * m_xB
                            * (m_t * (-1 + m_xB) - m_powerOfProtonMass[0] * m_xB))
                    / (2. * m_xB
                            * (m_powerOfQ[0] * (-1 + m_xB)
                                    - m_powerOfProtonMass[0] * m_xB)
                            * sqrt(
                                    (m_powerOfQ[2]
                                            + 4 * m_powerOfProtonMass[0]
                                                    * m_powerOfQ[0]
                                                    * pow(m_xB, 2))
                                            / (m_xB
                                                    * (m_powerOfQ[0]
                                                            * (1 - m_xB)
                                                            + m_powerOfProtonMass[0]
                                                                    * m_xB)))));
    m_pCM.setZ(-m_qCM.getZ());
    m_ppCM.setZ(-m_qpCM.getZ());

    m_qpPerp = m_qpCM.getX();

    m_thetag = TMath::ACos(m_qpCM.getZ() / m_qpCM.getE());

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "m_phaseSpace = " << m_phaseSpace);
}

void GV2008Model::isModuleWellConfigured() {

    if (!(m_xBMin < m_xB && m_xB < m_xBMax)) {
        std::cout
                << "TGV : Unrealistic kinematic configuration : m_xB isn't in the physical region !"
                << std::endl;
        std::cout << "    TGV : m_xB = " << m_xB << std::endl;
        std::cout << "    TGV : m_xBMin = " << m_xBMin << std::endl;
        std::cout << "    TGV : m_xBMax = " << m_xBMax << std::endl;
        exit(-1);
    }

    if (m_y < 1.) {
        std::cout << "TGV : ELab " << m_E
                << " GeV is too small. TMath::CosH[ome]= " << m_y << std::endl;
        exit(-1);
    } // end if y

    //TODO mettre une contrainte sur les GPD dont on a besoin pour calculer le modele
}

double GV2008Model::computeWithPhiDependency(double xB, double t, double Q2,
        double phi, CFFOutputData cffOutputData, Observable* pObservable) {

    m_xB = xB;
    m_t = t;
    m_Q2 = Q2;
    m_phi = MathUtils::convertDegreeToRadian(phi);
    m_cffOutputData = cffOutputData;
    m_pObservable = pObservable;

    initModule();
    isModuleWellConfigured();

    return pObservable->compute(this);
}

double GV2008Model::computeCrossSection(double beamHelicity, double beamCharge,
        Vector3D targetPolarization) {
    MakeExactBHCrossSections();
    MakeVCSHelicityAmplitudes();
    MakeExactVCSAndInterfCrossSections();

    return CrossSectionBH(beamHelicity, beamCharge, targetPolarization)
            + CrossSectionVCS(beamHelicity, beamCharge, targetPolarization)
            + CrossSectionInterf(beamHelicity, beamCharge, targetPolarization);
}

double GV2008Model::SqrAmplBH(double beamHelicity, double beamCharge,
        Vector3D targetPolarization) {

    double DDDC = DdirectDcrossed(m_phi);
    double M0, M1, M2, M3;

    M0 = -(SigmaBHPol0[0]
            - SigmaBHPol0[3] * cos(2. * m_phi) * (-1 + TMath::CosH(2 * m_Omega))
            + SigmaBHPol0[1] * TMath::CosH(2 * m_Omega)
            + SigmaBHPol0[2] * cos(m_phi) * TMath::SinH(2 * m_Omega))
            / (4. * DDDC * pow(m_t, 2));

    M1 = -(SigmaBHPolX[0] * beamHelicity * TMath::CosH(m_Omega)
            + SigmaBHPolX[1] * beamHelicity * cos(m_phi) * TMath::SinH(m_Omega))
            / (4. * DDDC * pow(m_t, 2));

    M2 =
            -(beamHelicity * SigmaBHPolY * TMath::Sin(m_phi)
                    * TMath::SinH(m_Omega)) / (4. * DDDC * pow(m_t, 2));

    M3 = -(SigmaBHPolZ[0] * beamHelicity * TMath::CosH(m_Omega)
            + SigmaBHPolZ[1] * beamHelicity * cos(m_phi) * TMath::SinH(m_Omega))
            / (4. * DDDC * pow(m_t, 2));

    Vector3D vM(M1, M2, M3);

    return M0 + (targetPolarization * vM);
}

double GV2008Model::SqrAmplVCSAndInterf(double beamHelicity, double beamCharge,
        Vector3D targetPolarization) {
    MakeVCSHelicityAmplitudes();
    MakeExactVCSAndInterfCrossSections();

    return SqrAmplVCS(beamHelicity, beamCharge, targetPolarization)
            + SqrAmplInterf(beamHelicity, beamCharge, targetPolarization);
}

void GV2008Model::MakeVCSHelicityAmplitudes() {

    std::complex<double> CFF_H = m_cffOutputData.getCFF(GPDComputeType::H);
    std::complex<double> CFF_E = m_cffOutputData.getCFF(GPDComputeType::E);
    std::complex<double> CFF_Ht = m_cffOutputData.getCFF(GPDComputeType::Ht);
    std::complex<double> CFF_Et = m_cffOutputData.getCFF(GPDComputeType::Et);

    RMvcs[0][0] = 0;
    RMvcs[0][1] = 0;
    RMvcs[0][2] = -((-4 * CFF_H.real() * (-1 + m_xB)
            - CFF_E.real() * pow(m_xB, 2)) / (sqrt(1 - m_xB) * (-2 + m_xB)));
    RMvcs[1][0] = 0;
    RMvcs[1][1] = 0;
    RMvcs[1][2] = -((m_qpPerp * CFF_Et.real() * m_xB)
            / (sqrt(1 - m_xB) * (-2 * PROTON_MASS + PROTON_MASS * m_xB)));
    RMvcs[2][0] = 0;
    RMvcs[2][1] = 0;
    RMvcs[2][2] = (m_qpPerp * CFF_E.real()) / (PROTON_MASS * sqrt(1 - m_xB));
    RMvcs[3][0] = 0;
    RMvcs[3][1] = 0;
    RMvcs[3][2] = -((-4 * CFF_Ht.real() * (-1 + m_xB)
            - CFF_Et.real() * pow(m_xB, 2)) / (sqrt(1 - m_xB) * (-2 + m_xB)));

    IMvcs[0][0] = 0;
    IMvcs[0][1] = 0;
    IMvcs[0][2] = -((-4 * CFF_H.imag() * (-1 + m_xB)
            - CFF_E.imag() * pow(m_xB, 2)) / (sqrt(1 - m_xB) * (-2 + m_xB)));
    IMvcs[1][0] = 0;
    IMvcs[1][1] = 0;
    IMvcs[1][2] = -((CFF_Et.imag() * m_qpPerp * m_xB)
            / (sqrt(1 - m_xB) * (-2 * PROTON_MASS + PROTON_MASS * m_xB)));
    IMvcs[2][0] = 0;
    IMvcs[2][1] = 0;
    IMvcs[2][2] = (CFF_E.imag() * m_qpPerp) / (PROTON_MASS * sqrt(1 - m_xB));
    IMvcs[3][0] = 0;
    IMvcs[3][1] = 0;
    IMvcs[3][2] = -((-4 * CFF_Ht.imag() * (-1 + m_xB)
            - CFF_Et.imag() * pow(m_xB, 2)) / (sqrt(1 - m_xB) * (-2 + m_xB)));
}

double GV2008Model::SqrAmplVCS(double beamHelicity, double beamCharge,
        Vector3D targetPolarization) {
    double M0, M1, M2, M3;

    M0 = (SigmaVCSPol0[0]
            - SigmaVCSPol0[3] * TMath::Cos(2 * m_phi)
                    * (-1 + TMath::CosH(2 * m_Omega))
            + SigmaVCSPol0[1] * TMath::CosH(2 * m_Omega)
            + SigmaVCSPol0[4] * beamHelicity * TMath::Sin(m_phi)
                    * TMath::SinH(m_Omega)
            + SigmaVCSPol0[2] * TMath::Cos(m_phi) * TMath::SinH(2 * m_Omega))
            / (2. * m_powerOfQ[0]);

    M1 = (SigmaVCSPolX[0] * beamHelicity * TMath::CosH(m_Omega)
            - SigmaVCSPolX[3] * (-1 + TMath::CosH(2 * m_Omega))
                    * TMath::Sin(2 * m_phi)
            + SigmaVCSPolX[1] * beamHelicity * TMath::Cos(m_phi)
                    * TMath::SinH(m_Omega)
            + SigmaVCSPolX[2] * TMath::Sin(m_phi) * TMath::SinH(2 * m_Omega))
            / (2. * m_powerOfQ[0]);

    M2 = (SigmaVCSPolY[1]
            - SigmaVCSPolY[4] * TMath::Cos(2 * m_phi)
                    * (-1 + TMath::CosH(2 * m_Omega))
            + SigmaVCSPolY[2] * TMath::CosH(2 * m_Omega)
            + SigmaVCSPolY[0] * beamHelicity * TMath::Sin(m_phi)
                    * TMath::SinH(m_Omega)
            + SigmaVCSPolY[3] * TMath::Cos(m_phi) * TMath::SinH(2 * m_Omega))
            / (2. * m_powerOfQ[0]);

    M3 = (SigmaVCSPolZ[0] * beamHelicity * TMath::CosH(m_Omega)
            - SigmaVCSPolZ[3] * (-1 + TMath::CosH(2 * m_Omega))
                    * TMath::Sin(2 * m_phi)
            + SigmaVCSPolZ[1] * beamHelicity * TMath::Cos(m_phi)
                    * TMath::SinH(m_Omega)
            + SigmaVCSPolZ[2] * TMath::Sin(m_phi) * TMath::SinH(2 * m_Omega))
            / (2. * m_powerOfQ[0]);

    Vector3D vM(M1, M2, M3);

    return M0 + (targetPolarization * vM);
}

double GV2008Model::SqrAmplInterf(double beamHelicity, double beamCharge,
        Vector3D targetPolarization) {

    double DDDC = DdirectDcrossed(m_phi);
    double M0, M1, M2, M3;

    M0 =
            -((beamCharge
                    * (SigmaIPol0[0] * TMath::CosH(m_Omega)
                            + SigmaIPol0[4] * TMath::Cos(2 * m_phi)
                                    * (TMath::CosH(m_Omega)
                                            - TMath::CosH(3 * m_Omega))
                            + SigmaIPol0[1] * TMath::CosH(3 * m_Omega)
                            - SigmaIPol0[7] * beamHelicity
                                    * (-1 + TMath::CosH(2 * m_Omega))
                                    * TMath::Sin(2 * m_phi)
                            + SigmaIPol0[6] * beamHelicity * TMath::Sin(m_phi)
                                    * TMath::SinH(2 * m_Omega)
                            + (SigmaIPol0[5] * TMath::Cos(3 * m_phi)
                                    * (3 * TMath::SinH(m_Omega)
                                            - TMath::SinH(3 * m_Omega))) / 3.
                            + TMath::Cos(m_phi)
                                    * (SigmaIPol0[2] * TMath::SinH(m_Omega)
                                            + SigmaIPol0[3]
                                                    * TMath::SinH(3 * m_Omega))))
                    / (DDDC * m_powerOfQ[0] * m_t));

    M1 =
            -((beamCharge
                    * (-(SigmaIPolX[7] * beamHelicity * TMath::Cos(2 * m_phi)
                            * (-1 + TMath::CosH(2 * m_Omega)))
                            + beamHelicity
                                    * (SigmaIPolX[4]
                                            + SigmaIPolX[5]
                                                    * TMath::CosH(2 * m_Omega))
                            + SigmaIPolX[2]
                                    * (TMath::CosH(m_Omega)
                                            - TMath::CosH(3 * m_Omega))
                                    * TMath::Sin(2 * m_phi)
                            + SigmaIPolX[6] * beamHelicity * TMath::Cos(m_phi)
                                    * TMath::SinH(2 * m_Omega)
                            + (SigmaIPolX[3] * TMath::Sin(3 * m_phi)
                                    * (3 * TMath::SinH(m_Omega)
                                            - TMath::SinH(3 * m_Omega))) / 3.
                            + TMath::Sin(m_phi)
                                    * (SigmaIPolX[0] * TMath::SinH(m_Omega)
                                            + SigmaIPolX[1]
                                                    * TMath::SinH(3 * m_Omega))))
                    / (DDDC * m_powerOfQ[0] * m_t));

    M2 =
            -((beamCharge
                    * (SigmaIPolY[0] * TMath::CosH(m_Omega)
                            + SigmaIPolY[4] * TMath::Cos(2 * m_phi)
                                    * (TMath::CosH(m_Omega)
                                            - TMath::CosH(3 * m_Omega))
                            + SigmaIPolY[1] * TMath::CosH(3 * m_Omega)
                            - SigmaIPolY[7] * beamHelicity
                                    * (-1 + TMath::CosH(2 * m_Omega))
                                    * TMath::Sin(2 * m_phi)
                            + SigmaIPolY[6] * beamHelicity * TMath::Sin(m_phi)
                                    * TMath::SinH(2 * m_Omega)
                            + (SigmaIPolY[5] * TMath::Cos(3 * m_phi)
                                    * (3 * TMath::SinH(m_Omega)
                                            - TMath::SinH(3 * m_Omega))) / 3.
                            + TMath::Cos(m_phi)
                                    * (SigmaIPolY[2] * TMath::SinH(m_Omega)
                                            + SigmaIPolY[3]
                                                    * TMath::SinH(3 * m_Omega))))
                    / (DDDC * m_powerOfQ[0] * m_t));

    M3 =
            -((beamCharge
                    * (-(SigmaIPolZ[7] * beamHelicity * TMath::Cos(2 * m_phi)
                            * (-1 + TMath::CosH(2 * m_Omega)))
                            + beamHelicity
                                    * (SigmaIPolZ[4]
                                            + SigmaIPolZ[5]
                                                    * TMath::CosH(2 * m_Omega))
                            + SigmaIPolZ[2]
                                    * (TMath::CosH(m_Omega)
                                            - TMath::CosH(3 * m_Omega))
                                    * TMath::Sin(2 * m_phi)
                            + SigmaIPolZ[6] * beamHelicity * TMath::Cos(m_phi)
                                    * TMath::SinH(2 * m_Omega)
                            + (SigmaIPolZ[3] * TMath::Sin(3 * m_phi)
                                    * (3 * TMath::SinH(m_Omega)
                                            - TMath::SinH(3 * m_Omega))) / 3.
                            + TMath::Sin(m_phi)
                                    * (SigmaIPolZ[0] * TMath::SinH(m_Omega)
                                            + SigmaIPolZ[1]
                                                    * TMath::SinH(3 * m_Omega))))
                    / (DDDC * m_powerOfQ[0] * m_t));

    Vector3D vM(M1, M2, M3);

    return M0 + (targetPolarization * vM);
}

/*---------------------------------- Methods' definition -------------------------------*
 |   - SetBeamEnergy(EBeam)                                                             |
 |	 - MakeKinematics()                                                                 |
 |	 - MakeHelicityAmplitudes()                                                         | 
 |   - MakeExactBHCrossSections()                                                       | 
 |   - MakeExactVCSAndInterfCrossSections()                                             |
 |   - MakeLeadingBHCrossSections()                                                     |
 |   - MakeLeadingVCSAndInterfCrossSections()                                           |
 |	 - DdirectDcrossed(phi)                                                             |
 |	 - SqrAmplBH(Q2,xB,t,phi,BeamHeli,TargetPolar,ReH,ImH,ReE,ImE,ReHT,ImHT,ReET,ImET)  |
 |	 - SqrAmplVCS(Q2,xB,t,phi,BeamHeli,TargetPolar,ReH,ImH,ReE,ImE,ReHT,ImHT,ReET,ImET) |
 |	 - SqrAmplInterf(Q2,xB,t,phi,BeamHeli,TargetPolar,BeamCharge,ReH,ImH,ReE,ImE,ReHT,  |
 |           ImHT,ReET,ImET)                                                            |
 *--------------------------------------------------------------------------------------*/

/*--------------------------- Function SetBeamEnergy(EBeam) ----------------------------*
 | Sets the beam energy in the laboratory frame.                                        |
 *--------------------------------------------------------------------------------------*/

void GV2008Model::SetBeamEnergy(double EBeam) {
    if (InitBeamEnergy == kFALSE) {
        m_E = EBeam;

        // Flag

        InitBeamEnergy = kTRUE;
        if (NoPrint == kFALSE) {
            std::cout << "TGV : Beam Energy Initialization : " << InitBeamEnergy
                    << std::endl;
            std::cout << "    TGV : ELab = " << EBeam << " GeV" << std::endl;
        } // end if NoPrint

    } // end if InitBeamEnergy

} // end SetBeamEnergy

/*----------------------- Function MakeExactBHCrossSections() --------------------------*
 | Computes all the stuff to evaluate the cross section assuming the hadronic helicity  |
 | amplitudes are given, i.e. initializes :                                             |
 |   - the helicity amplitudes Jem,                                                     |
 |   - the SigmaBHPol's.                                                                |
 | No approximations apart those in the computation of the hadronic helicity amplitudes |
 | in terms of the Compton Form Factors.                                                |
 *--------------------------------------------------------------------------------------*/

void GV2008Model::MakeExactBHCrossSections() {
    if (InitExactBHCrossSections == kFALSE) {
        double F1; // Dirac form factor
        double F2; // Pauli form factor
        double Ge, Gm; // Sachs' parametrization

        F1 = (4. * m_powerOfProtonMass[0] - 2.79285 * m_t)
                / (pow(1. - 1.4084507042253522 * m_t, 2)
                        * (4. * m_powerOfProtonMass[0] - 1. * m_t));
        F2 = (7.1714 * m_powerOfProtonMass[0])
                / (pow(1 - 1.4084507042253522 * m_t, 2)
                        * (4 * m_powerOfProtonMass[0] - m_t));
        Gm = 2.79285 / pow(1 - 1.4084507042253522 * m_t, 2);
        Ge = pow(1 - 1.4084507042253522 * m_t, -2);

        /*----------------- Helicity amplitudes of the interference process --------------------*/

        Jem[0][0] = (-2 * sqrt(2.) * m_qCM.getZ() * m_qpCM.getX() * F2
                * (sqrt(m_pCM.getE() - PROTON_MASS)
                        * sqrt(m_ppCM.getE() - PROTON_MASS)
                        - sqrt(m_pCM.getE() + PROTON_MASS)
                                * sqrt(m_ppCM.getE() + PROTON_MASS)) * sqrt(m_s)
                * TMath::Cos(m_thetag / 2.))
                / (PROTON_MASS * sqrt(m_powerOfQ[2] - 4 * m_s * m_t))
                - (2 * sqrt(2.) * (m_qCM.getZ() + m_qpCM.getE()) * Gm
                        * (sqrt(m_ppCM.getE() - PROTON_MASS)
                                * sqrt(m_pCM.getE() + PROTON_MASS)
                                + sqrt(m_pCM.getE() - PROTON_MASS)
                                        * sqrt(m_ppCM.getE() + PROTON_MASS))
                        * sqrt(m_s) * TMath::Sin(m_thetag / 2.))
                        / sqrt(m_powerOfQ[2] - 4 * m_s * m_t);
        Jem[0][1] =
                -((F2
                        * (sqrt(m_pCM.getE() - PROTON_MASS)
                                * sqrt(m_ppCM.getE() - PROTON_MASS)
                                - sqrt(m_pCM.getE() + PROTON_MASS)
                                        * sqrt(m_ppCM.getE() + PROTON_MASS))
                        * (2 * m_powerOfProtonMass[0] + m_powerOfQ[0] + 2 * m_s)
                        * sqrt(-(m_s * m_t)) * TMath::Cos(m_thetag / 2.))
                        / (PROTON_MASS * sqrt(m_s)
                                * sqrt(m_powerOfQ[2] - 4 * m_s * m_t)))
                        - (8 * Gm
                                * (sqrt(m_ppCM.getE() - PROTON_MASS)
                                        * sqrt(m_pCM.getE() + PROTON_MASS)
                                        + sqrt(m_pCM.getE() - PROTON_MASS)
                                                * sqrt(
                                                        m_ppCM.getE()
                                                                + PROTON_MASS))
                                * m_s * sqrt(-m_t)
                                * ((m_qCM.getZ() - m_qpCM.getZ())
                                        * TMath::Cos(m_thetag / 2.)
                                        - m_qpCM.getX()
                                                * TMath::Sin(m_thetag / 2.)))
                                / (m_powerOfQ[0]
                                        * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
        Jem[0][2] = (2 * sqrt(2.) * m_qCM.getZ() * m_qpCM.getX() * F2
                * (sqrt(m_pCM.getE() - PROTON_MASS)
                        * sqrt(m_ppCM.getE() - PROTON_MASS)
                        - sqrt(m_pCM.getE() + PROTON_MASS)
                                * sqrt(m_ppCM.getE() + PROTON_MASS)) * sqrt(m_s)
                * TMath::Cos(m_thetag / 2.))
                / (PROTON_MASS * sqrt(m_powerOfQ[2] - 4 * m_s * m_t))
                + (2 * sqrt(2.) * (m_qCM.getZ() + m_qpCM.getE()) * Gm
                        * (sqrt(m_ppCM.getE() - PROTON_MASS)
                                * sqrt(m_pCM.getE() + PROTON_MASS)
                                + sqrt(m_pCM.getE() - PROTON_MASS)
                                        * sqrt(m_ppCM.getE() + PROTON_MASS))
                        * sqrt(m_s) * TMath::Sin(m_thetag / 2.))
                        / sqrt(m_powerOfQ[2] - 4 * m_s * m_t);
        Jem[1][0] = sqrt(2.) * Gm
                * (-(sqrt(m_ppCM.getE() - PROTON_MASS)
                        * sqrt(m_pCM.getE() + PROTON_MASS))
                        + sqrt(m_pCM.getE() - PROTON_MASS)
                                * sqrt(m_ppCM.getE() + PROTON_MASS))
                * TMath::Cos(m_thetag / 2.);
        Jem[1][1] = 0;
        Jem[1][2] = sqrt(2.) * Gm
                * (-(sqrt(m_ppCM.getE() - PROTON_MASS)
                        * sqrt(m_pCM.getE() + PROTON_MASS))
                        + sqrt(m_pCM.getE() - PROTON_MASS)
                                * sqrt(m_ppCM.getE() + PROTON_MASS))
                * TMath::Cos(m_thetag / 2.);
        Jem[2][0] = (2 * sqrt(2.) * (m_qCM.getZ() - m_qpCM.getE()) * Gm
                * (sqrt(m_ppCM.getE() - PROTON_MASS)
                        * sqrt(m_pCM.getE() + PROTON_MASS)
                        - sqrt(m_pCM.getE() - PROTON_MASS)
                                * sqrt(m_ppCM.getE() + PROTON_MASS)) * sqrt(m_s)
                * TMath::Cos(m_thetag / 2.))
                / sqrt(m_powerOfQ[2] - 4 * m_s * m_t)
                - (2 * sqrt(2.) * m_qCM.getZ() * m_qpCM.getX() * F2
                        * (sqrt(m_pCM.getE() - PROTON_MASS)
                                * sqrt(m_ppCM.getE() - PROTON_MASS)
                                + sqrt(m_pCM.getE() + PROTON_MASS)
                                        * sqrt(m_ppCM.getE() + PROTON_MASS))
                        * sqrt(m_s) * TMath::Sin(m_thetag / 2.))
                        / (PROTON_MASS * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
        Jem[2][1] =
                -((F2
                        * (sqrt(m_pCM.getE() - PROTON_MASS)
                                * sqrt(m_ppCM.getE() - PROTON_MASS)
                                + sqrt(m_pCM.getE() + PROTON_MASS)
                                        * sqrt(m_ppCM.getE() + PROTON_MASS))
                        * (2 * m_powerOfProtonMass[0] + m_powerOfQ[0] + 2 * m_s)
                        * sqrt(-(m_s * m_t)) * TMath::Sin(m_thetag / 2.))
                        / (PROTON_MASS * sqrt(m_s)
                                * sqrt(m_powerOfQ[2] - 4 * m_s * m_t)))
                        - (8 * Gm
                                * (sqrt(m_ppCM.getE() - PROTON_MASS)
                                        * sqrt(m_pCM.getE() + PROTON_MASS)
                                        - sqrt(m_pCM.getE() - PROTON_MASS)
                                                * sqrt(
                                                        m_ppCM.getE()
                                                                + PROTON_MASS))
                                * m_s * sqrt(-m_t)
                                * (m_qpCM.getX() * TMath::Cos(m_thetag / 2.)
                                        + (m_qCM.getZ() - m_qpCM.getZ())
                                                * TMath::Sin(m_thetag / 2.)))
                                / (m_powerOfQ[0]
                                        * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
        Jem[2][2] = (-2 * sqrt(2.) * (m_qCM.getZ() - m_qpCM.getE()) * Gm
                * (sqrt(m_ppCM.getE() - PROTON_MASS)
                        * sqrt(m_pCM.getE() + PROTON_MASS)
                        - sqrt(m_pCM.getE() - PROTON_MASS)
                                * sqrt(m_ppCM.getE() + PROTON_MASS)) * sqrt(m_s)
                * TMath::Cos(m_thetag / 2.))
                / sqrt(m_powerOfQ[2] - 4 * m_s * m_t)
                + (2 * sqrt(2.) * m_qCM.getZ() * m_qpCM.getX() * F2
                        * (sqrt(m_pCM.getE() - PROTON_MASS)
                                * sqrt(m_ppCM.getE() - PROTON_MASS)
                                + sqrt(m_pCM.getE() + PROTON_MASS)
                                        * sqrt(m_ppCM.getE() + PROTON_MASS))
                        * sqrt(m_s) * TMath::Sin(m_thetag / 2.))
                        / (PROTON_MASS * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
        Jem[3][0] = sqrt(2.) * Gm
                * (sqrt(m_ppCM.getE() - PROTON_MASS)
                        * sqrt(m_pCM.getE() + PROTON_MASS)
                        + sqrt(m_pCM.getE() - PROTON_MASS)
                                * sqrt(m_ppCM.getE() + PROTON_MASS))
                * TMath::Sin(m_thetag / 2.);
        Jem[3][1] = 0;
        Jem[3][2] = sqrt(2.) * Gm
                * (sqrt(m_ppCM.getE() - PROTON_MASS)
                        * sqrt(m_pCM.getE() + PROTON_MASS)
                        + sqrt(m_pCM.getE() - PROTON_MASS)
                                * sqrt(m_ppCM.getE() + PROTON_MASS))
                * TMath::Sin(m_thetag / 2.);

        /*--------------------------------BH cross sections ------------------------------------*/

        SigmaBHPol0[0] =
                (-32 * pow(Ge, 2) * m_powerOfProtonMass[0]
                        * (3 * m_powerOfProtonMass[3] * m_t
                                + 3 * m_powerOfProtonMass[2]
                                        * (m_powerOfQ[2] + m_powerOfQ[0] * m_t
                                                - 4 * m_s * m_t)
                                + m_powerOfProtonMass[0]
                                        * (3 * m_powerOfQ[2]
                                                * pow(m_powerOfQ[0] + m_s, 2)
                                                - (m_powerOfQ[0] + m_s)
                                                        * (m_powerOfQ[2]
                                                                + 3
                                                                        * m_powerOfQ[0]
                                                                        * m_s
                                                                + 12
                                                                        * pow(
                                                                                m_s,
                                                                                2))
                                                        * m_t
                                                + 2
                                                        * (m_powerOfQ[2]
                                                                + m_powerOfQ[0]
                                                                        * m_s
                                                                - 3
                                                                        * pow(
                                                                                m_s,
                                                                                2))
                                                        * pow(m_t, 2))
                                + pow(m_powerOfQ[0] + m_s, 2) * m_t
                                        * (3 * m_s * (m_s + m_t)
                                                + m_powerOfQ[0]
                                                        * (3 * m_s + m_t))
                                + m_powerOfProtonMass[1]
                                        * (4 * pow(m_Q, 6)
                                                + m_powerOfQ[0] * m_t
                                                        * (3 * m_s + m_t)
                                                - m_powerOfQ[2]
                                                        * (6 * m_s + m_t)
                                                + 3 * m_s * m_t
                                                        * (6 * m_s + m_t)))
                        + 4 * pow(Gm, 2) * m_t
                                * (6 * m_powerOfProtonMass[3] * m_t
                                        + m_t
                                                * (3
                                                        * pow(
                                                                m_powerOfQ[0]
                                                                        + m_s,
                                                                2)
                                                        * (m_powerOfQ[2]
                                                                + 2
                                                                        * m_powerOfQ[0]
                                                                        * m_s
                                                                + 2
                                                                        * pow(
                                                                                m_s,
                                                                                2))
                                                        + 2 * m_s
                                                                * (m_powerOfQ[0]
                                                                        + m_s)
                                                                * (2
                                                                        * m_powerOfQ[0]
                                                                        + 3
                                                                                * m_s)
                                                                * m_t
                                                        + (3 * m_powerOfQ[2]
                                                                + 4
                                                                        * m_powerOfQ[0]
                                                                        * m_s
                                                                + 3
                                                                        * pow(
                                                                                m_s,
                                                                                2))
                                                                * pow(m_t, 2))
                                        - 2 * m_powerOfProtonMass[2]
                                                * (3 * m_powerOfQ[2]
                                                        - 11 * m_powerOfQ[0]
                                                                * m_t
                                                        + 6 * m_t
                                                                * (2 * m_s + m_t))
                                        + m_powerOfProtonMass[1]
                                                * (-8 * pow(m_Q, 6)
                                                        - 26 * m_powerOfQ[0]
                                                                * m_t
                                                                * (m_s + m_t)
                                                        + m_powerOfQ[2]
                                                                * (12 * m_s
                                                                        + 25
                                                                                * m_t)
                                                        + 3 * m_t
                                                                * (12
                                                                        * pow(
                                                                                m_s,
                                                                                2)
                                                                        + 10
                                                                                * m_s
                                                                                * m_t
                                                                        + pow(
                                                                                m_t,
                                                                                2)))
                                        - 2 * m_powerOfProtonMass[0]
                                                * (3 * pow(m_Q, 8)
                                                        + pow(m_Q, 6)
                                                                * (6 * m_s
                                                                        - 5
                                                                                * m_t)
                                                        + 3 * m_s * m_t
                                                                * pow(
                                                                        2 * m_s
                                                                                + m_t,
                                                                        2)
                                                        + m_powerOfQ[0] * m_t
                                                                * (7
                                                                        * pow(
                                                                                m_s,
                                                                                2)
                                                                        + 2
                                                                                * m_s
                                                                                * m_t
                                                                        - 3
                                                                                * pow(
                                                                                        m_t,
                                                                                        2))
                                                        + m_powerOfQ[2]
                                                                * (3
                                                                        * pow(
                                                                                m_s,
                                                                                2)
                                                                        - 5
                                                                                * m_s
                                                                                * m_t
                                                                        + 7
                                                                                * pow(
                                                                                        m_t,
                                                                                        2)))))
                        / ((m_powerOfProtonMass[1]
                                + 2 * m_powerOfProtonMass[0]
                                        * (m_powerOfQ[0] - m_s)
                                + pow(m_powerOfQ[0] + m_s, 2))
                                * (4 * m_powerOfProtonMass[0] - m_t));
        SigmaBHPol0[1] =
                (-32 * pow(Ge, 2) * m_powerOfProtonMass[0]
                        * (m_powerOfProtonMass[3] * m_t
                                + m_powerOfProtonMass[2]
                                        * (m_powerOfQ[2] + m_powerOfQ[0] * m_t
                                                - 4 * m_s * m_t)
                                + m_powerOfProtonMass[0]
                                        * (m_powerOfQ[2]
                                                * pow(m_powerOfQ[0] + m_s, 2)
                                                + (m_powerOfQ[0] - m_s)
                                                        * (m_powerOfQ[0] + m_s)
                                                        * (5 * m_powerOfQ[0]
                                                                + 4 * m_s) * m_t
                                                - 2
                                                        * (m_powerOfQ[2]
                                                                + m_powerOfQ[0]
                                                                        * m_s
                                                                + pow(m_s, 2))
                                                        * pow(m_t, 2))
                                + pow(m_powerOfQ[0] + m_s, 2) * m_t
                                        * (m_powerOfQ[0] * (m_s - m_t)
                                                + m_s * (m_s + m_t))
                                + m_powerOfProtonMass[1]
                                        * (-4 * pow(m_Q, 6)
                                                + m_powerOfQ[0] * (m_s - m_t)
                                                        * m_t
                                                + m_s * m_t * (6 * m_s + m_t)
                                                + m_powerOfQ[2]
                                                        * (-2 * m_s + 5 * m_t)))
                        + 4 * pow(Gm, 2) * m_t
                                * (2 * m_powerOfProtonMass[3] * m_t
                                        + m_t
                                                * (pow(m_powerOfQ[0] + m_s, 2)
                                                        * (m_powerOfQ[2]
                                                                + 2
                                                                        * m_powerOfQ[0]
                                                                        * m_s
                                                                + 2
                                                                        * pow(
                                                                                m_s,
                                                                                2))
                                                        + 2 * m_s
                                                                * (-2
                                                                        * m_powerOfQ[2]
                                                                        - m_powerOfQ[0]
                                                                                * m_s
                                                                        + pow(
                                                                                m_s,
                                                                                2))
                                                                * m_t
                                                        + (m_powerOfQ[2]
                                                                - 4
                                                                        * m_powerOfQ[0]
                                                                        * m_s
                                                                + pow(m_s, 2))
                                                                * pow(m_t, 2))
                                        - 2 * m_powerOfProtonMass[2]
                                                * (m_powerOfQ[2]
                                                        - 9 * m_powerOfQ[0]
                                                                * m_t
                                                        + 2 * m_t
                                                                * (2 * m_s + m_t))
                                        + m_powerOfProtonMass[1]
                                                * (8 * pow(m_Q, 6)
                                                        - 2 * m_powerOfQ[0]
                                                                * m_t
                                                                * (15 * m_s
                                                                        + 7
                                                                                * m_t)
                                                        + m_powerOfQ[2]
                                                                * (4 * m_s
                                                                        + 19
                                                                                * m_t)
                                                        + m_t
                                                                * (12
                                                                        * pow(
                                                                                m_s,
                                                                                2)
                                                                        + 10
                                                                                * m_s
                                                                                * m_t
                                                                        + pow(
                                                                                m_t,
                                                                                2)))\

                                        - 2 * m_powerOfProtonMass[0]
                                                * (pow(m_Q, 8)
                                                        + pow(m_Q, 6)
                                                                * (2 * m_s + m_t)
                                                        + m_s * m_t
                                                                * pow(
                                                                        2 * m_s
                                                                                + m_t,
                                                                        2)
                                                        - m_powerOfQ[0] * m_t
                                                                * (3
                                                                        * pow(
                                                                                m_s,
                                                                                2)
                                                                        + 10
                                                                                * m_s
                                                                                * m_t
                                                                        + pow(
                                                                                m_t,
                                                                                2))
                                                        + m_powerOfQ[2]
                                                                * (pow(m_s, 2)
                                                                        - 7
                                                                                * m_s
                                                                                * m_t
                                                                        + 5
                                                                                * pow(
                                                                                        m_t,
                                                                                        2)))))
                        / ((m_powerOfProtonMass[1]
                                + 2 * m_powerOfProtonMass[0]
                                        * (m_powerOfQ[0] - m_s)
                                + pow(m_powerOfQ[0] + m_s, 2))
                                * (4 * m_powerOfProtonMass[0] - m_t));
        SigmaBHPol0[2] = (-64 * pow(Ge, 2) * m_powerOfProtonMass[0] * m_Q
                * m_qpPerp * (m_powerOfProtonMass[0] - m_powerOfQ[0] - m_s)
                * (2 * m_powerOfProtonMass[0] * m_powerOfQ[0]
                        - (m_powerOfProtonMass[0] + m_powerOfQ[0] + m_s) * m_t)
                + 16 * pow(Gm, 2) * m_Q * m_qpPerp * m_t
                        * (m_powerOfProtonMass[1]
                                * (-2 * m_powerOfQ[0] + 3 * m_t)
                                + m_t
                                        * (m_powerOfQ[0] * (m_s - m_t)
                                                + m_s * (m_s + m_t))
                                + m_powerOfProtonMass[0]
                                        * (2 * m_powerOfQ[2]
                                                - m_t * (4 * m_s + m_t)
                                                + m_powerOfQ[0]
                                                        * (2 * m_s + 5 * m_t))))
                / (sqrt(
                        m_powerOfProtonMass[1]
                                + 2 * m_powerOfProtonMass[0]
                                        * (m_powerOfQ[0] - m_s)
                                + pow(m_powerOfQ[0] + m_s, 2))
                        * (4 * m_powerOfProtonMass[0] - m_t));
        SigmaBHPol0[3] = (-64 * pow(Ge, 2) * m_powerOfProtonMass[1]
                * m_powerOfQ[0]
                * (m_powerOfProtonMass[1] * m_t
                        + m_s * m_t * (m_powerOfQ[0] + m_s + m_t)
                        + m_powerOfProtonMass[0]
                                * (m_powerOfQ[2] + m_powerOfQ[0] * m_t
                                        - 2 * m_s * m_t))
                - 8 * pow(Gm, 2) * m_powerOfQ[0]
                        * (2 * m_powerOfProtonMass[0] - m_t) * m_t
                        * (m_powerOfProtonMass[1] * m_t
                                + m_s * m_t * (m_powerOfQ[0] + m_s + m_t)
                                + m_powerOfProtonMass[0]
                                        * (m_powerOfQ[2] + m_powerOfQ[0] * m_t
                                                - 2 * m_s * m_t)))
                / ((m_powerOfProtonMass[1]
                        + 2 * m_powerOfProtonMass[0] * (m_powerOfQ[0] - m_s)
                        + pow(m_powerOfQ[0] + m_s, 2))
                        * (4 * m_powerOfProtonMass[0] - m_t));
        SigmaBHPolX[0] = (64 * Ge * Gm * PROTON_MASS * m_qpPerp
                * (-(m_powerOfQ[0] * (m_powerOfQ[0] + m_s))
                        + m_powerOfProtonMass[0] * (m_powerOfQ[0] - m_t)
                        + m_s * m_t)
                * (2 * m_powerOfProtonMass[0] * m_powerOfQ[0]
                        - (m_powerOfProtonMass[0] + m_powerOfQ[0] + m_s) * m_t)
                + 32 * pow(Gm, 2) * PROTON_MASS * m_qpPerp * m_t
                        * (-pow(m_Q, 6) - 3 * m_powerOfQ[2] * m_s
                                - 2 * m_powerOfProtonMass[1]
                                        * (m_powerOfQ[0] - m_t)
                                + m_s * m_t * (2 * m_s + m_t)
                                - m_powerOfQ[0]
                                        * (2 * pow(m_s, 2) + pow(m_t, 2))
                                - m_powerOfProtonMass[0]
                                        * (m_powerOfQ[2]
                                                - 4 * m_powerOfQ[0]
                                                        * (m_s + m_t)
                                                + m_t * (4 * m_s + m_t))))
                / (sqrt(
                        m_powerOfProtonMass[1]
                                + 2 * m_powerOfProtonMass[0]
                                        * (m_powerOfQ[0] - m_s)
                                + pow(m_powerOfQ[0] + m_s, 2))
                        * (4 * m_powerOfProtonMass[0] - m_t));
        SigmaBHPolX[1] = (32 * Ge * Gm * PROTON_MASS * m_Q
                * (-2 * m_powerOfProtonMass[0] * m_powerOfQ[0]
                        + (m_powerOfProtonMass[0] + m_powerOfQ[0] + m_s) * m_t)
                * (4 * m_powerOfProtonMass[0] * m_powerOfQ[2]
                        + (2 * m_powerOfProtonMass[0] + m_powerOfQ[0] - 2 * m_s)
                                * (m_powerOfProtonMass[0] - m_powerOfQ[0] - m_s)
                                * m_t
                        + (m_powerOfProtonMass[0] + m_powerOfQ[0] + 3 * m_s)
                                * pow(m_t, 2))
                + 64 * pow(Gm, 2) * PROTON_MASS * m_Q
                        * (m_powerOfProtonMass[0] - m_s - m_t) * m_t
                        * (m_powerOfProtonMass[1] * m_t
                                + m_s * m_t * (m_powerOfQ[0] + m_s + m_t)
                                + m_powerOfProtonMass[0]
                                        * (m_powerOfQ[2] + m_powerOfQ[0] * m_t
                                                - 2 * m_s * m_t)))
                / ((m_powerOfProtonMass[1]
                        + 2 * m_powerOfProtonMass[0] * (m_powerOfQ[0] - m_s)
                        + pow(m_powerOfQ[0] + m_s, 2))
                        * (4 * m_powerOfProtonMass[0] - m_t));
        SigmaBHPolY = 32 * Ge * Gm * PROTON_MASS * m_Q * (m_powerOfQ[0] - m_t)
                * m_t;
        SigmaBHPolZ[0] = (-128 * Ge * Gm * m_powerOfProtonMass[0]
                * (-(m_powerOfQ[0] * (m_powerOfQ[0] + m_s))
                        + m_powerOfProtonMass[0] * (m_powerOfQ[0] - m_t)
                        + m_s * m_t)
                * (m_powerOfProtonMass[1] * m_t
                        + m_s * m_t * (m_powerOfQ[0] + m_s + m_t)
                        + m_powerOfProtonMass[0]
                                * (m_powerOfQ[2] + m_powerOfQ[0] * m_t
                                        - 2 * m_s * m_t))
                + 16 * pow(Gm, 2) * m_t
                        * (2 * m_powerOfProtonMass[0] * m_powerOfQ[0]
                                - (m_powerOfProtonMass[0] + m_powerOfQ[0] + m_s)
                                        * m_t)
                        * (pow(m_Q, 6) + 3 * m_powerOfQ[2] * m_s
                                + 2 * m_powerOfProtonMass[1]
                                        * (m_powerOfQ[0] - m_t)
                                - m_s * m_t * (2 * m_s + m_t)
                                + m_powerOfQ[0]
                                        * (2 * pow(m_s, 2) + pow(m_t, 2))
                                + m_powerOfProtonMass[0]
                                        * (m_powerOfQ[2]
                                                - 4 * m_powerOfQ[0]
                                                        * (m_s + m_t)
                                                + m_t * (4 * m_s + m_t))))
                / ((m_powerOfProtonMass[1]
                        + 2 * m_powerOfProtonMass[0] * (m_powerOfQ[0] - m_s)
                        + pow(m_powerOfQ[0] + m_s, 2))
                        * (4 * m_powerOfProtonMass[0] - m_t));
        SigmaBHPolZ[1] = (64 * Ge * Gm * m_powerOfProtonMass[0] * m_Q * m_qpPerp
                * (-4 * m_powerOfProtonMass[0] * m_powerOfQ[2]
                        - (2 * m_powerOfProtonMass[0] + m_powerOfQ[0] - 2 * m_s)
                                * (m_powerOfProtonMass[0] - m_powerOfQ[0] - m_s)
                                * m_t
                        - (m_powerOfProtonMass[0] + m_powerOfQ[0] + 3 * m_s)
                                * pow(m_t, 2))
                + 32 * pow(Gm, 2) * m_Q * m_qpPerp * m_t
                        * (-m_powerOfProtonMass[0] + m_s + m_t)
                        * ((m_powerOfQ[0] + m_s) * m_t
                                + m_powerOfProtonMass[0]
                                        * (-2 * m_powerOfQ[0] + m_t)))
                / (sqrt(
                        m_powerOfProtonMass[1]
                                + 2 * m_powerOfProtonMass[0]
                                        * (m_powerOfQ[0] - m_s)
                                + pow(m_powerOfQ[0] + m_s, 2))
                        * (4 * m_powerOfProtonMass[0] - m_t));

    } // end if InitExactBHCrossSections

} // end MakeExactBHCrossSections

/*------------------ Function MakeExactVCSAndInterfCrossSections() ---------------------*
 | Computes all the stuff to evaluate the cross section assuming the hadronic helicity  |
 | amplitudes are given, i.e. initializes :                                             |
 |   - the helicity amplitudes Mvcs,                                                    |
 |   - the expansion coefficients Ur,                                                   |
 |   - the SigmaVCSPol's and SigmaIPol's.                                               |
 | No approximations apart those in the computation of the hadronic helicity amplitudes |
 | in terms of the Compton Form Factors.                                                |
 *--------------------------------------------------------------------------------------*/

void GV2008Model::MakeExactVCSAndInterfCrossSections() {
//	if ( InitExactVCSAndInterfCrossSections == kFALSE )
//	{

    /*-------------- Harmonic expansion coefficients of the VCS cross section --------------*/

    m_Ur[0] = -((m_powerOfQ[2]
            + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0] * sqrt(m_s)
            - 4 * m_s * m_t)
            * (m_powerOfQ[2]
                    + 4 * m_qCM.getE() * (4 * m_qCM.getE() - 3 * m_qCM.getZ())
                            * pow(m_qpPerp, 2)
                    + (m_qCM.getE() + m_qCM.getZ())
                            * (m_qCM.getE() + m_qpCM.getE()) * m_t
                    + m_powerOfQ[0]
                            * ((m_qCM.getE() - m_qpCM.getE())
                                    * (m_qCM.getE() + m_qCM.getZ()
                                            + 2 * m_qpCM.getE())
                                    + 16 * pow(m_qpPerp, 2) + m_t)))
            / (64. * m_qCM.getZ() * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[1] = ((m_powerOfQ[2]
            + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0] * sqrt(m_s)
            - 4 * m_s * m_t)
            * (m_powerOfQ[2]
                    - 4 * m_qCM.getE() * m_qCM.getZ() * pow(m_qpPerp, 2)
                    + (m_qCM.getE() - m_qCM.getZ())
                            * (m_qCM.getE() + m_qpCM.getE()) * m_t
                    + m_powerOfQ[0]
                            * ((m_qCM.getE() - m_qpCM.getE())
                                    * (m_qCM.getE() - m_qCM.getZ()
                                            + 2 * m_qpCM.getE()) + m_t)))
            / (64. * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[2] = ((m_powerOfQ[2]
            + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0] * sqrt(m_s)
            - 4 * m_s * m_t)
            * (m_powerOfQ[2]
                    + 4 * m_qCM.getE() * m_qCM.getZ() * pow(m_qpPerp, 2)
                    + (m_qCM.getE() + m_qCM.getZ())
                            * (m_qCM.getE() + m_qpCM.getE()) * m_t
                    + m_powerOfQ[0]
                            * ((m_qCM.getE() - m_qpCM.getE())
                                    * (m_qCM.getE() + m_qCM.getZ()
                                            + 2 * m_qpCM.getE()) + m_t)))
            / (64. * m_qCM.getZ() * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[3] =
            (m_qpPerp
                    * (m_powerOfQ[2]
                            + m_powerOfQ[0]
                                    * (12 * pow(m_qCM.getE(), 2)
                                            - 16 * m_qCM.getE() * m_qCM.getZ()
                                            - 22 * m_qCM.getE() * m_qpCM.getE()
                                            + 26 * m_qCM.getZ() * m_qpCM.getE()
                                            - 7 * m_t)
                            + 4 * m_qCM.getE()
                                    * (m_qCM.getE() - 4 * m_qCM.getZ()) * m_t)
                    * (m_powerOfQ[2]
                            + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                                    * sqrt(m_s) - 4 * m_s * m_t))
                    / (128. * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s)
                            * sqrt(-m_t) * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[4] = (3 * m_qpPerp
            * (2 * (m_qCM.getE() + m_qCM.getZ()) * m_qpCM.getE() + m_powerOfQ[0]
                    + m_t)
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t))
            / (128. * m_qCM.getZ() * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[5] = (m_qpPerp
            * (5 * m_powerOfQ[2]
                    + m_powerOfQ[0]
                            * (2
                                    * (6 * pow(m_qCM.getE(), 2)
                                            - 7 * m_qCM.getE() * m_qpCM.getE()
                                            + m_qCM.getZ() * m_qpCM.getE())
                                    - 3 * m_t) + 4 * pow(m_qCM.getE(), 2) * m_t)
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t))
            / (128. * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[6] = -((m_powerOfQ[2]
            + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0] * sqrt(m_s)
            - 4 * m_s * m_t)
            * (m_powerOfQ[2]
                    + m_powerOfQ[0]
                            * ((m_qCM.getE() - m_qpCM.getE())
                                    * (m_qCM.getE() - m_qCM.getZ()
                                            + 2 * m_qpCM.getE())
                                    + 2 * pow(m_qpPerp, 2) + m_t)
                    + (m_qCM.getE() - m_qCM.getZ())
                            * (2 * m_qCM.getE() * pow(m_qpPerp, 2)
                                    + (m_qCM.getE() + m_qpCM.getE()) * m_t)))
            / (32. * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[7] = -(m_qpPerp
            * (m_qCM.getE()
                    * (3 * m_qCM.getE() - m_qCM.getZ() - 2 * m_qpCM.getE())
                    * m_powerOfQ[0] + 2 * m_powerOfQ[2]
                    + m_qCM.getE() * (m_qCM.getE() - m_qCM.getZ()) * m_t)
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t))
            / (16. * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[8] =
            (m_qpPerp * sqrt(m_s)
                    * ((4 * pow(m_qCM.getE(), 2)
                            - 3 * m_qCM.getE() * (m_qCM.getZ() - m_qpCM.getE())
                            - 4 * m_qCM.getZ() * m_qpCM.getE()) * m_t
                            + m_powerOfQ[0]
                                    * (-3
                                            * (m_qCM.getE() + m_qCM.getZ()
                                                    - 2 * m_qpCM.getE())
                                            * (m_qCM.getE() - m_qpCM.getE())
                                            + 7 * m_t)))
                    / (4. * sqrt(2.) * m_qCM.getZ()
                            * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[9] = -(m_qpPerp * sqrt(m_s)
            * (m_qCM.getE() * (m_qCM.getZ() + m_qpCM.getE()) * m_t
                    + m_powerOfQ[0]
                            * (-((m_qCM.getE() - m_qCM.getZ()
                                    - 2 * m_qpCM.getE())
                                    * (m_qCM.getE() - m_qpCM.getE())) + m_t)))
            / (4. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[10] = (m_qpPerp * sqrt(m_s)
            * (m_qCM.getE() * (-m_qCM.getZ() + m_qpCM.getE()) * m_t
                    + m_powerOfQ[0]
                            * (-((m_qCM.getE() + m_qCM.getZ()
                                    - 2 * m_qpCM.getE())
                                    * (m_qCM.getE() - m_qpCM.getE())) + m_t)))
            / (4. * sqrt(2.) * m_qCM.getZ()
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[11] =
            (sqrt(m_s)
                    * (3 * pow(m_Q, 6)
                            + m_powerOfQ[2]
                                    * (3 * pow(m_qCM.getE(), 2)
                                            - 7 * m_qCM.getE() * m_qCM.getZ()
                                            + 35 * m_qCM.getE() * m_qpCM.getE()
                                            + 3 * m_qCM.getZ() * m_qpCM.getE()
                                            - 22 * pow(m_qpCM.getE(), 2)
                                            + 12 * pow(m_qpPerp, 2) + 3 * m_t)
                            + 8 * m_qCM.getE()
                                    * (4 * m_qCM.getE() - m_qCM.getZ())
                                    * (m_qCM.getE() + m_qpCM.getE())
                                    * (m_qCM.getE() * pow(m_qpPerp, 2)
                                            + m_qpCM.getE() * m_t)
                            + m_powerOfQ[0]
                                    * (8 * (4 * m_qCM.getE() - m_qCM.getZ())
                                            * m_qpCM.getE()
                                            * (pow(m_qCM.getE(), 2)
                                                    - pow(m_qpCM.getE(), 2))
                                            + 2
                                                    * (22 * pow(m_qCM.getE(), 2)
                                                            + 7 * m_qCM.getZ()
                                                                    * m_qpCM.getE()
                                                            + m_qCM.getE()
                                                                    * (-15
                                                                            * m_qCM.getZ()
                                                                            + 16
                                                                                    * m_qpCM.getE()))
                                                    * pow(m_qpPerp, 2)
                                            + (3 * pow(m_qCM.getE(), 2)
                                                    - 7 * m_qCM.getE()
                                                            * (m_qCM.getZ()
                                                                    - 5
                                                                            * m_qpCM.getE())
                                                    + m_qpCM.getE()
                                                            * (-11
                                                                    * m_qCM.getZ()
                                                                    + 16
                                                                            * m_qpCM.getE()))
                                                    * m_t)))
                    / (16. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]
                            * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[12] = (-3 * sqrt(m_s)
            * (m_powerOfQ[2]
                    + 2
                            * (2 * pow(m_qCM.getE(), 2)
                                    - m_qCM.getE() * m_qCM.getZ()
                                    + m_qCM.getZ() * m_qpCM.getE())
                            * pow(m_qpPerp, 2)
                    + (m_qCM.getE() - m_qCM.getZ())
                            * (m_qCM.getE() + m_qpCM.getE()) * m_t
                    + m_powerOfQ[0]
                            * ((m_qCM.getE() - m_qpCM.getE())
                                    * (m_qCM.getE() - m_qCM.getZ()
                                            + 2 * m_qpCM.getE())
                                    + 4 * pow(m_qpPerp, 2) + m_t)))
            / (16. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[13] = -(sqrt(m_s)
            * (pow(m_Q, 6)
                    + m_powerOfQ[2]
                            * (pow(m_qCM.getE(), 2)
                                    + 3 * m_qCM.getE() * m_qCM.getZ()
                                    + m_qCM.getE() * m_qpCM.getE()
                                    + m_qCM.getZ() * m_qpCM.getE()
                                    - 2 * pow(m_qpCM.getE(), 2)
                                    + 4 * pow(m_qpPerp, 2) + m_t)
                    + 8 * m_qCM.getE() * m_qCM.getZ()
                            * (m_qCM.getE() + m_qpCM.getE())
                            * (m_qCM.getE() * pow(m_qpPerp, 2)
                                    + m_qpCM.getE() * m_t)
                    + m_powerOfQ[0]
                            * (8 * m_qCM.getZ() * m_qpCM.getE()
                                    * (pow(m_qCM.getE(), 2)
                                            - pow(m_qpCM.getE(), 2))
                                    + (4 * pow(m_qCM.getE(), 2)
                                            + 22 * m_qCM.getE() * m_qCM.getZ()
                                            - 6 * m_qCM.getZ() * m_qpCM.getE())
                                            * pow(m_qpPerp, 2)
                                    + (pow(m_qCM.getE(), 2)
                                            + 7 * m_qCM.getZ() * m_qpCM.getE()
                                            + m_qCM.getE()
                                                    * (3 * m_qCM.getZ()
                                                            + m_qpCM.getE()))
                                            * m_t)))
            / (16. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[14] = ((m_qCM.getE() - m_qpCM.getE()) * m_qpPerp * sqrt(m_s)
            * ((-m_qCM.getE() + m_qCM.getZ() + 2 * m_qpCM.getE())
                    * m_powerOfQ[0] + (-m_qCM.getE() + m_qCM.getZ()) * m_t))
            / (4. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[15] = -(sqrt(m_s)
            * (pow(m_Q, 6)
                    + m_powerOfQ[2]
                            * (pow(m_qCM.getE(), 2)
                                    + 3 * m_qCM.getE() * m_qpCM.getE()
                                    - m_qCM.getZ() * m_qpCM.getE()
                                    - 3 * pow(m_qpCM.getE(), 2)
                                    + 4 * pow(m_qpPerp, 2) + m_t)
                    + 2 * m_qCM.getE() * (m_qCM.getE() - m_qCM.getZ())
                            * (m_qCM.getE() + m_qpCM.getE())
                            * (m_qCM.getE() * pow(m_qpPerp, 2)
                                    + m_qpCM.getE() * m_t)
                    + m_powerOfQ[0]
                            * (2 * (m_qCM.getE() - m_qCM.getZ()) * m_qpCM.getE()
                                    * (pow(m_qCM.getE(), 2)
                                            - pow(m_qpCM.getE(), 2))
                                    + 2 * m_qCM.getE()
                                            * (3 * m_qCM.getE()
                                                    - 2 * m_qCM.getZ()
                                                    + m_qpCM.getE())
                                            * pow(m_qpPerp, 2)
                                    + (pow(m_qCM.getE(), 2)
                                            + 3 * m_qCM.getE() * m_qpCM.getE()
                                            - m_qCM.getZ() * m_qpCM.getE()
                                            + pow(m_qpCM.getE(), 2)) * m_t)))
            / (2. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[16] = (m_qpPerp
            * ((m_qCM.getE() - m_qCM.getZ() - 2 * m_qpCM.getE()) * m_powerOfQ[0]
                    + m_qCM.getE() * m_t))
            / (8. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]);
    m_Ur[17] = -((3 * m_qCM.getE() - 7 * m_qCM.getZ() + 10 * m_qpCM.getE())
            * m_powerOfQ[2]
            + 8 * m_qCM.getE() * (4 * m_qCM.getE() - m_qCM.getZ())
                    * (m_qCM.getE() * pow(m_qpPerp, 2) + m_qpCM.getE() * m_t)
            + m_powerOfQ[0]
                    * (8 * (4 * m_qCM.getE() - m_qCM.getZ())
                            * (m_qCM.getE() - m_qpCM.getE()) * m_qpCM.getE()
                            + (32 * m_qCM.getE() - 6 * m_qCM.getZ())
                                    * pow(m_qpPerp, 2)
                            + (3 * m_qCM.getE() - 7 * m_qCM.getZ()
                                    + 16 * m_qpCM.getE()) * m_t))
            / (32. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]);
    m_Ur[18] = (3
            * ((-m_qCM.getE() + m_qCM.getZ() + 2 * m_qpCM.getE())
                    * m_powerOfQ[0] + 2 * m_qCM.getZ() * pow(m_qpPerp, 2)
                    + (-m_qCM.getE() + m_qCM.getZ()) * m_t))
            / (32. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]);
    m_Ur[19] = ((m_qCM.getE() + 3 * m_qCM.getZ() - 2 * m_qpCM.getE())
            * m_powerOfQ[2]
            + m_powerOfQ[0]
                    * (-2 * m_qCM.getZ()
                            * (4 * m_qpCM.getE()
                                    * (-m_qCM.getE() + m_qpCM.getE())
                                    + pow(m_qpPerp, 2))
                            + (m_qCM.getE() + 3 * m_qCM.getZ()) * m_t)
            + 8 * m_qCM.getE() * m_qCM.getZ()
                    * (m_qCM.getE() * pow(m_qpPerp, 2) + m_qpCM.getE() * m_t))
            / (32. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]);
    m_Ur[20] = (m_qpPerp
            * ((m_qCM.getE() + m_qCM.getZ() - 2 * m_qpCM.getE()) * m_powerOfQ[0]
                    + (m_qCM.getE() - 3 * m_qCM.getZ()) * m_t))
            / (8. * sqrt(2.) * m_qCM.getZ());
    m_Ur[21] = (m_qpPerp
            * ((-m_qCM.getE() + m_qCM.getZ() + 2 * m_qpCM.getE())
                    * m_powerOfQ[0] + (-m_qCM.getE() + m_qCM.getZ()) * m_t))
            / (8. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]);
    m_Ur[22] = (m_qpPerp
            * (3 * (m_qCM.getE() + m_qCM.getZ() - 2 * m_qpCM.getE())
                    * m_powerOfQ[0] + (3 * m_qCM.getE() - m_qCM.getZ()) * m_t))
            / (8. * sqrt(2.) * m_qCM.getZ());
    m_Ur[23] = ((m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[2]
            + 2 * m_qCM.getE() * (m_qCM.getE() - m_qCM.getZ())
                    * (m_qCM.getE() * pow(m_qpPerp, 2) + m_qpCM.getE() * m_t)
            + m_powerOfQ[0]
                    * (2 * (m_qCM.getE() - m_qCM.getZ())
                            * (m_qCM.getE() - m_qpCM.getE()\
) * m_qpCM.getE()
                            + 2 * (m_qCM.getE() + m_qCM.getZ())
                                    * pow(m_qpPerp, 2)
                            + (m_qCM.getE() + m_qpCM.getE()) * m_t))
            / (4. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]);
    m_Ur[24] = (m_qpPerp
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t)
            * (3 * m_powerOfQ[2] + 2 * pow(m_qCM.getE(), 2) * m_t
                    - m_powerOfQ[0]
                            * (6 * m_qCM.getE()
                                    * (-m_qCM.getE() + m_qpCM.getE()) + m_t)))
            / (32. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s)
                    * sqrt(-m_t) * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[25] = -(m_qpPerp
            * (2 * m_qCM.getE() * m_qpCM.getE() + m_powerOfQ[0] + m_t)
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t))
            / (32. * sqrt(2.) * m_qCM.getZ() * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[26] = ((3 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
            + 4 * m_qCM.getE() * pow(m_qpPerp, 2)
            + 3 * (m_qCM.getE() + m_qpCM.getE()) * m_t)
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t))
            / (16. * sqrt(2.) * m_powerOfQ[0] * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[27] = -(((m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
            + 4 * m_qCM.getE() * pow(m_qpPerp, 2)
            + (m_qCM.getE() + m_qpCM.getE()) * m_t)
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t))
            / (16. * sqrt(2.) * m_powerOfQ[0] * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[28] = -(m_qCM.getE() * pow(m_qpPerp, 2)
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t))
            / (4. * sqrt(2.) * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[29] = -(sqrt(m_s)
            * ((m_qCM.getE() + m_qpCM.getE()) * m_powerOfQ[2]
                    + 4 * m_qCM.getE() * (m_qCM.getE() + m_qpCM.getE())
                            * (m_qCM.getE() * pow(m_qpPerp, 2)
                                    + m_qpCM.getE() * m_t)
                    + m_powerOfQ[0]
                            * (4 * pow(m_qCM.getE(), 2) * m_qpCM.getE()
                                    - 4 * pow(m_qpCM.getE(), 3)
                                    + 2 * (5 * m_qCM.getE() - m_qpCM.getE())
                                            * pow(m_qpPerp, 2)
                                    + m_qCM.getE() * m_t
                                    + 3 * m_qpCM.getE() * m_t)))
            / (8. * m_powerOfQ[0] * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[30] = -(sqrt(m_s)
            * ((m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * pow(m_qpPerp, 2)
                    + (m_qCM.getE() + m_qpCM.getE()) * m_t))
            / (8. * m_powerOfQ[0] * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[31] = -(m_qpPerp * sqrt(m_s)
            * (m_qCM.getE() * (2 * m_qCM.getE() - m_qpCM.getE()) * m_t
                    + m_powerOfQ[0]
                            * (pow(m_qCM.getE(), 2)
                                    - 3 * m_qCM.getE() * m_qpCM.getE()
                                    + 2 * pow(m_qpCM.getE(), 2) + m_t)))
            / (2. * m_qCM.getZ() * m_powerOfQ[0]
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[32] = -(m_qpPerp * sqrt(m_s)
            * (m_qCM.getE() * m_qpCM.getE() * m_t
                    + m_powerOfQ[0]
                            * (-pow(m_qCM.getE(), 2)
                                    + 3 * m_qCM.getE() * m_qpCM.getE()
                                    - 2 * pow(m_qpCM.getE(), 2) + m_t)))
            / (2. * m_qCM.getZ() * m_powerOfQ[0]
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[33] = -(m_qpPerp * sqrt(m_s)
            * (m_qCM.getE() * (m_qCM.getE() + m_qpCM.getE()) * m_t
                    + m_powerOfQ[0]
                            * (-pow(m_qCM.getE(), 2)
                                    + 3 * m_qCM.getE() * m_qpCM.getE()
                                    - 2 * pow(m_qpCM.getE(), 2) + 2 * m_t)))
            / (2. * m_qCM.getZ() * m_powerOfQ[0]
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[34] = ((m_qCM.getE() - 2 * m_qpCM.getE()) * m_powerOfQ[3]
            + m_qCM.getE() * m_powerOfQ[1] * m_t)
            / (16. * m_qCM.getZ() * m_powerOfQ[3]);
    m_Ur[35] = (m_qpPerp * (m_powerOfQ[0] + 2 * m_t)) / (4. * m_powerOfQ[0]);
    m_Ur[36] = -m_qpPerp / 4.;
    m_Ur[37] = (m_qCM.getE() * m_powerOfQ[2]
            + 4 * pow(m_qCM.getE(), 2)
                    * (m_qCM.getE() * pow(m_qpPerp, 2) + m_qpCM.getE() * m_t)
            + m_powerOfQ[0]
                    * (4 * m_qCM.getE() * (m_qCM.getE() - m_qpCM.getE())
                            * m_qpCM.getE()
                            + 4 * m_qCM.getE() * pow(m_qpPerp, 2)
                            + (m_qCM.getE() + 2 * m_qpCM.getE()) * m_t))
            / (8. * m_qCM.getZ() * m_powerOfQ[0]);
    m_Ur[38] = (m_qpPerp * (-m_powerOfQ[0] + m_t)) / (4. * m_powerOfQ[0]);
    m_Ur[39] = -((m_powerOfQ[2]
            + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0] * sqrt(m_s)
            - 4 * m_s * m_t)
            * (m_powerOfQ[2]
                    + 4 * m_qCM.getE() * (4 * m_qCM.getE() + 3 * m_qCM.getZ())
                            * pow(m_qpPerp, 2)
                    + (m_qCM.getE() - m_qCM.getZ())
                            * (m_qCM.getE() + m_qpCM.getE()) * m_t
                    + m_powerOfQ[0]
                            * ((m_qCM.getE() - m_qpCM.getE())
                                    * (m_qCM.getE() - m_qCM.getZ()
                                            + 2 * m_qpCM.getE())
                                    + 16 * pow(m_qpPerp, 2) + m_t)))
            / (64. * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[40] =
            -(m_qpPerp
                    * (m_powerOfQ[2]
                            + m_powerOfQ[0]
                                    * (12 * pow(m_qCM.getE(), 2)
                                            + 16 * m_qCM.getE() * m_qCM.getZ()
                                            - 22 * m_qCM.getE() * m_qpCM.getE()
                                            - 26 * m_qCM.getZ() * m_qpCM.getE()
                                            - 7 * m_t)\

                            + 4 * m_qCM.getE()
                                    * (m_qCM.getE() + 4 * m_qCM.getZ()) * m_t)
                    * (m_powerOfQ[2]
                            + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                                    * sqrt(m_s) - 4 * m_s * m_t))
                    / (128. * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s)
                            * sqrt(-m_t) * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[41] = (-3 * m_powerOfQ[0] * m_qpPerp
            * (2 * (m_qCM.getE() - m_qCM.getZ()) * m_qpCM.getE() + m_powerOfQ[0]
                    + m_t)
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t))
            / (128. * m_qCM.getZ() * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[42] = -(m_qpPerp
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t)
            * (5 * m_powerOfQ[2] + 4 * pow(m_qCM.getE(), 2) * m_t
                    - m_powerOfQ[0]
                            * (2
                                    * (-6 * pow(m_qCM.getE(), 2)
                                            + 7 * m_qCM.getE() * m_qpCM.getE()
                                            + m_qCM.getZ() * m_qpCM.getE())
                                    + 3 * m_t)))
            / (128. * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[43] = -((m_powerOfQ[2]
            + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0] * sqrt(m_s)
            - 4 * m_s * m_t)
            * (m_powerOfQ[2]
                    + m_powerOfQ[0]
                            * ((m_qCM.getE() - m_qpCM.getE())
                                    * (m_qCM.getE() + m_qCM.getZ()
                                            + 2 * m_qpCM.getE())
                                    + 2 * pow(m_qpPerp, 2) + m_t)
                    + (m_qCM.getE() + m_qCM.getZ())
                            * (2 * m_qCM.getE() * pow(m_qpPerp, 2)
                                    + (m_qCM.getE() + m_qpCM.getE()) * m_t)))
            / (32. * m_qCM.getZ() * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[44] = (m_qpPerp
            * (m_qCM.getE()
                    * (3 * m_qCM.getE() + m_qCM.getZ() - 2 * m_qpCM.getE())
                    * m_powerOfQ[0] + 2 * m_powerOfQ[2]
                    + m_qCM.getE() * (m_qCM.getE() + m_qCM.getZ()) * m_t)
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t))
            / (16. * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[45] =
            -(m_qpPerp * sqrt(m_s)
                    * ((4 * pow(m_qCM.getE(), 2)
                            + 4 * m_qCM.getZ() * m_qpCM.getE()
                            + 3 * m_qCM.getE() * (m_qCM.getZ() + m_qpCM.getE()))
                            * m_t
                            + m_powerOfQ[0]
                                    * (-3
                                            * (m_qCM.getE() - m_qCM.getZ()
                                                    - 2 * m_qpCM.getE())
                                            * (m_qCM.getE() - m_qpCM.getE())
                                            + 7 * m_t)))
                    / (4. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]
                            * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[46] =
            (sqrt(m_s)
                    * (3 * pow(m_Q, 6)
                            + m_powerOfQ[2]
                                    * (3 * pow(m_qCM.getE(), 2)
                                            + 7 * m_qCM.getE() * m_qCM.getZ()
                                            + 35 * m_qCM.getE() * m_qpCM.getE()
                                            - 3 * m_qCM.getZ() * m_qpCM.getE()
                                            - 22 * pow(m_qpCM.getE(), 2)
                                            + 12 * pow(m_qpPerp, 2) + 3 * m_t)
                            + 8 * m_qCM.getE()
                                    * (4 * m_qCM.getE() + m_qCM.getZ())
                                    * (m_qCM.getE() + m_qpCM.getE())
                                    * (m_qCM.getE() * pow(m_qpPerp, 2)
                                            + m_qpCM.getE() * m_t)
                            + m_powerOfQ[0]
                                    * (8 * (4 * m_qCM.getE() + m_qCM.getZ())
                                            * m_qpCM.getE()
                                            * (pow(m_qCM.getE(), 2)
                                                    - pow(m_qpCM.getE(), 2))
                                            + 2
                                                    * (22 * pow(m_qCM.getE(), 2)
                                                            - 7 * m_qCM.getZ()
                                                                    * m_qpCM.getE()
                                                            + m_qCM.getE()
                                                                    * (15
                                                                            * m_qCM.getZ()
                                                                            + 16
                                                                                    * m_qpCM.getE()))
                                                    * pow(m_qpPerp, 2)\

                                            + (3 * pow(m_qCM.getE(), 2)
                                                    + 7 * m_qCM.getE()
                                                            * (m_qCM.getZ()
                                                                    + 5
                                                                            * m_qpCM.getE())
                                                    + m_qpCM.getE()
                                                            * (11 * m_qCM.getZ()
                                                                    + 16
                                                                            * m_qpCM.getE()))
                                                    * m_t)))
                    / (16. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[2]
                            * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[47] = (-3 * m_powerOfQ[0] * sqrt(m_s)
            * (m_powerOfQ[2]
                    + 2
                            * (2 * pow(m_qCM.getE(), 2)
                                    + m_qCM.getE() * m_qCM.getZ()
                                    - m_qCM.getZ() * m_qpCM.getE())
                            * pow(m_qpPerp, 2)
                    + (m_qCM.getE() + m_qCM.getZ())
                            * (m_qCM.getE() + m_qpCM.getE()) * m_t
                    + m_powerOfQ[0]
                            * ((m_qCM.getE() - m_qpCM.getE())
                                    * (m_qCM.getE() + m_qCM.getZ()
                                            + 2 * m_qpCM.getE())
                                    + 4 * pow(m_qpPerp, 2) + m_t)))
            / (16. * sqrt(2.) * m_qCM.getZ()
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[48] = -(sqrt(m_s)
            * (pow(m_Q, 6)
                    + m_powerOfQ[2]
                            * (pow(m_qCM.getE(), 2)
                                    - 3 * m_qCM.getE() * m_qCM.getZ()
                                    + m_qCM.getE() * m_qpCM.getE()
                                    - m_qCM.getZ() * m_qpCM.getE()
                                    - 2 * pow(m_qpCM.getE(), 2)
                                    + 4 * pow(m_qpPerp, 2) + m_t)
                    - 8 * m_qCM.getE() * m_qCM.getZ()
                            * (m_qCM.getE() + m_qpCM.getE())
                            * (m_qCM.getE() * pow(m_qpPerp, 2)
                                    + m_qpCM.getE() * m_t)
                    + m_powerOfQ[0]
                            * (8 * m_qCM.getZ() * m_qpCM.getE()
                                    * (-pow(m_qCM.getE(), 2)
                                            + pow(m_qpCM.getE(), 2))
                                    + (4 * pow(m_qCM.getE(), 2)
                                            - 22 * m_qCM.getE() * m_qCM.getZ()
                                            + 6 * m_qCM.getZ() * m_qpCM.getE())
                                            * pow(m_qpPerp, 2)
                                    + (pow(m_qCM.getE(), 2)
                                            - 7 * m_qCM.getZ() * m_qpCM.getE()
                                            + m_qCM.getE()
                                                    * (-3 * m_qCM.getZ()
                                                            + m_qpCM.getE()))
                                            * m_t)))
            / (16. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[2]
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[49] = ((m_qCM.getE() - m_qpCM.getE()) * m_qpPerp * sqrt(m_s)
            * ((m_qCM.getE() + m_qCM.getZ() - 2 * m_qpCM.getE()) * m_powerOfQ[0]
                    + (m_qCM.getE() + m_qCM.getZ()) * m_t))
            / (4. * sqrt(2.) * m_qCM.getZ()
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[50] = -(sqrt(m_s)
            * (pow(m_Q, 6)
                    + m_powerOfQ[2]
                            * (pow(m_qCM.getE(), 2)
                                    + 3 * m_qCM.getE() * m_qpCM.getE()
                                    + m_qCM.getZ() * m_qpCM.getE()
                                    - 3 * pow(m_qpCM.getE(), 2)
                                    + 4 * pow(m_qpPerp, 2) + m_t)
                    + 2 * m_qCM.getE() * (m_qCM.getE() + m_qCM.getZ())
                            * (m_qCM.getE() + m_qpCM.getE())
                            * (m_qCM.getE() * pow(m_qpPerp, 2)
                                    + m_qpCM.getE() * m_t)
                    + m_powerOfQ[0]
                            * (2 * (m_qCM.getE() + m_qCM.getZ()) * m_qpCM.getE()
                                    * (pow(m_qCM.getE(), 2)
                                            - pow(m_qpCM.getE(), 2))
                                    + 2 * m_qCM.getE()
                                            * (3 * m_qCM.getE()
                                                    + 2 * m_qCM.getZ()
                                                    + m_qpCM.getE())
                                            * pow(m_qpPerp, 2)
                                    + (pow(m_qCM.getE(), 2)
                                            + 3 * m_qCM.getE() * m_qpCM.getE()
                                            + m_qpCM.getE()
                                                    * (m_qCM.getZ()
                                                            + m_qpCM.getE()))
                                            * m_t)))
            / (2. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[2]
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[51] = (m_qpPerp
            * ((m_qCM.getE() + m_qCM.getZ() - 2 * m_qpCM.getE()) * m_powerOfQ[0]
                    + m_qCM.getE() * m_t)) / (8. * sqrt(2.) * m_qCM.getZ());
    m_Ur[52] = ((3 * m_qCM.getE() + 7 * m_qCM.getZ() + 10 * m_qpCM.getE())
            * m_powerOfQ[2]
            + 8 * m_qCM.getE() * (4 * m_qCM.getE() + m_qCM.getZ())
                    * (m_qCM.getE() * pow(m_qpPerp, 2) + m_qpCM.getE() * m_t)
            + m_powerOfQ[0]
                    * (8 * (4 * m_qCM.getE() + m_qCM.getZ())
                            * (m_qCM.getE() - m_qpCM.getE()) * m_qpCM.getE()
                            + (32 * m_qCM.getE() + 6 * m_qCM.getZ())
                                    * pow(m_qpPerp, 2)
                            + (3 * m_qCM.getE() + 7 * m_qCM.getZ()
                                    + 16 * m_qpCM.getE()) * m_t))
            / (32. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[2]);
    m_Ur[53] = (3 * m_powerOfQ[0]
            * ((m_qCM.getE() + m_qCM.getZ() - 2 * m_qpCM.getE()) * m_powerOfQ[0]
                    + 2 * m_qCM.getZ() * pow(m_qpPerp, 2)
                    + (m_qCM.getE() + m_qCM.getZ()) * m_t))
            / (32. * sqrt(2.) * m_qCM.getZ());
    m_Ur[54] = ((-m_qCM.getE() + 3 * m_qCM.getZ() + 2 * m_qpCM.getE())
            * m_powerOfQ[2]
            - m_powerOfQ[0]
                    * (2 * m_qCM.getZ()
                            * (4 * m_qpCM.getE()
                                    * (-m_qCM.getE() + m_qpCM.getE())
                                    + pow(m_qpPerp, 2))
                            + (m_qCM.getE() - 3 * m_qCM.getZ()) * m_t)
            + 8 * m_qCM.getE() * m_qCM.getZ()
                    * (m_qCM.getE() * pow(m_qpPerp, 2) + m_qpCM.getE() * m_t))
            / (32. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[2]);
    m_Ur[55] = (m_qpPerp
            * ((m_qCM.getE() - m_qCM.getZ() - 2 * m_qpCM.getE()) * m_powerOfQ[0]
                    + (m_qCM.getE() + 3 * m_qCM.getZ()) * m_t))
            / (8. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]);
    m_Ur[56] = -(m_qpPerp
            * ((m_qCM.getE() + m_qCM.getZ() - 2 * m_qpCM.getE()) * m_powerOfQ[0]
                    + (m_qCM.getE() + m_qCM.getZ()) * m_t))
            / (8. * sqrt(2.) * m_qCM.getZ());
    m_Ur[57] = (m_qpPerp
            * (3 * (m_qCM.getE() - m_qCM.getZ() - 2 * m_qpCM.getE())
                    * m_powerOfQ[0] + (3 * m_qCM.getE() + m_qCM.getZ()) * m_t))
            / (8. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]);
    m_Ur[58] = -((m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[2]
            + 2 * m_qCM.getE() * (m_qCM.getE() + m_qCM.getZ())
                    * (m_qCM.getE() * pow(m_qpPerp, 2) + m_qpCM.getE() * m_t)
            + m_powerOfQ[0]
                    * (2 * (m_qCM.getE() + m_qCM.getZ())
                            * (m_qCM.getE() - m_qpCM.getE()\
) * m_qpCM.getE()
                            + 2 * (m_qCM.getE() - m_qCM.getZ())
                                    * pow(m_qpPerp, 2)
                            + (m_qCM.getE() + m_qpCM.getE()) * m_t))
            / (4. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[2]);
    m_Ur[59] = -(m_qpPerp
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t)
            * (11 * m_powerOfQ[2]
                    + 4 * m_qCM.getE() * (m_qCM.getE() - 4 * m_qCM.getZ()) * m_t
                    + m_powerOfQ[0]
                            * (2
                                    * (6 * pow(m_qCM.getE(), 2)
                                            + 3 * m_qCM.getZ() * m_qpCM.getE()
                                            - m_qCM.getE()
                                                    * (8 * m_qCM.getZ()
                                                            + m_qpCM.getE()))
                                    + 3 * m_t)))
            / (128. * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[60] = -(m_qpPerp
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t)
            * (7 * m_powerOfQ[2] + 4 * pow(m_qCM.getE(), 2) * m_t
                    - m_powerOfQ[0]
                            * (2
                                    * (-6 * pow(m_qCM.getE(), 2)
                                            + 5 * m_qCM.getE() * m_qpCM.getE()
                                            + m_qCM.getZ() * m_qpCM.getE())
                                    + m_t)))
            / (128. * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[61] = -((m_powerOfQ[2]
            + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0] * sqrt(m_s)
            - 4 * m_s * m_t)
            * (m_powerOfQ[2]
                    + 2 * m_qCM.getE() * (3 * m_qCM.getE() - m_qCM.getZ())
                            * pow(m_qpPerp, 2)
                    + (m_qCM.getE() + m_qCM.getZ())
                            * (m_qCM.getE() + m_qpCM.getE()) * m_t
                    + m_powerOfQ[0]
                            * ((m_qCM.getE() - m_qpCM.getE())
                                    * (m_qCM.getE() + m_qCM.getZ()
                                            + 2 * m_qpCM.getE())
                                    + 6 * pow(m_qpPerp, 2) + m_t)))
            / (32. * m_qCM.getZ() * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[62] = ((m_powerOfQ[2]
            + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0] * sqrt(m_s)
            - 4 * m_s * m_t)
            * (m_powerOfQ[2]
                    - 2 * m_qCM.getE() * (m_qCM.getE() - 3 * m_qCM.getZ())
                            * pow(m_qpPerp, 2)
                    + (m_qCM.getE() + m_qCM.getZ())
                            * (m_qCM.getE() + m_qpCM.getE()) * m_t
                    + m_powerOfQ[0]
                            * ((m_qCM.getE() - m_qpCM.getE())
                                    * (m_qCM.getE() + m_qCM.getZ()
                                            + 2 * m_qpCM.getE())
                                    - 2 * pow(m_qpPerp, 2) + m_t)))
            / (32. * m_qCM.getZ() * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[63] = (m_qpPerp
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t)
            * (m_powerOfQ[2]
                    + m_qCM.getE() * (m_qCM.getE() - m_qCM.getZ()) * m_t
                    - m_powerOfQ[0]
                            * (-3 * pow(m_qCM.getE(), 2)
                                    - 2 * m_qCM.getZ() * m_qpCM.getE()
                                    + m_qCM.getE()
                                            * (m_qCM.getZ() + 4 * m_qpCM.getE())
                                    + m_t)))
            / (16. * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[64] =
            -(sqrt(m_s)
                    * (13 * pow(m_Q, 6)
                            + m_powerOfQ[2]
                                    * (13 * pow(m_qCM.getE(), 2)
                                            + 3 * m_qCM.getE() * m_qCM.getZ()
                                            + 45 * m_qCM.getE() * m_qpCM.getE()
                                            - 7 * m_qCM.getZ() * m_qpCM.getE()
                                            - 42 * pow(m_qpCM.getE(), 2)
                                            + 52 * pow(m_qpPerp, 2) + 13 * m_t)
                            + 8 * m_qCM.getE()
                                    * (4 * m_qCM.getE() - m_qCM.getZ())
                                    * (m_qCM.getE() + m_qpCM.getE())
                                    * (m_qCM.getE() * pow(m_qpPerp, 2)
                                            + m_qpCM.getE() * m_t)
                            + m_powerOfQ[0]
                                    * (8 * (4 * m_qCM.getE() - m_qCM.getZ())
                                            * m_qpCM.getE()
                                            * (pow(m_qCM.getE(), 2)
                                                    - pow(m_qpCM.getE(), 2))
                                            + 2
                                                    * (42 * pow(m_qCM.getE(), 2)
                                                            - 3 * m_qCM.getZ()
                                                                    * m_qpCM.getE()
                                                            + m_qCM.getE()
                                                                    * (-5
                                                                            * m_qCM.getZ()
                                                                            + 16
                                                                                    * m_qpCM.getE()))
                                                    * pow(m_qpPerp, 2)
                                            + (13 * pow(m_qCM.getE(), 2)
                                                    + 3 * m_qCM.getE()
                                                            * (m_qCM.getZ()
                                                                    + 15
                                                                            * m_qpCM.getE())
                                                    + m_qpCM.getE()
                                                            * (-m_qCM.getZ()
                                                                    + 16
                                                                            * m_qpCM.getE()))
                                                    * m_t)))
                    / (16. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]
                            * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[65] = -(sqrt(m_s)
            * (pow(m_Q, 6)
                    + m_powerOfQ[2]
                            * (pow(m_qCM.getE(), 2)
                                    - m_qCM.getE() * m_qCM.getZ()
                                    + m_qCM.getE() * m_qpCM.getE()
                                    - 3 * m_qCM.getZ() * m_qpCM.getE()
                                    - 2 * pow(m_qpCM.getE(), 2)
                                    + 4 * pow(m_qpPerp, 2) + m_t)
                    - 8 * m_qCM.getE() * m_qCM.getZ()
                            * (m_qCM.getE() + m_qpCM.getE())
                            * (m_qCM.getE() * pow(m_qpPerp, 2)
                                    + m_qpCM.getE() * m_t)
                    + m_powerOfQ[0]
                            * (8 * m_qCM.getZ() * m_qpCM.getE()
                                    * (-pow(m_qCM.getE(), 2)
                                            + pow(m_qpCM.getE(), 2))
                                    + 2
                                            * (2 * pow(m_qCM.getE(), 2)
                                                    - 9 * m_qCM.getE()
                                                            * m_qCM.getZ()
                                                    + m_qCM.getZ()
                                                            * m_qpCM.getE())
                                            * pow(m_qpPerp, 2)
                                    + (pow(m_qCM.getE(), 2)
                                            - 5 * m_qCM.getZ() * m_qpCM.getE()
                                            + m_qCM.getE()
                                                    * (-m_qCM.getZ()
                                                            + m_qpCM.getE()))
                                            * m_t)))
            / (16. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[66] =
            (m_qpPerp * sqrt(m_s)
                    * ((3 * pow(m_qCM.getE(), 2)
                            - 3 * m_qCM.getZ() * m_qpCM.getE()
                            + m_qCM.getE() * (-m_qCM.getZ() + m_qpCM.getE()))
                            * m_t
                            + m_powerOfQ[0]
                                    * (-((m_qCM.getE() + m_qCM.getZ()
                                            - 2 * m_qpCM.getE())
                                            * (m_qCM.getE() - m_qpCM.getE()))
                                            + 4 * m_t)))
                    / (4. * sqrt(2.) * m_qCM.getZ()
                            * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[67] =
            (m_qpPerp * sqrt(m_s)
                    * ((pow(m_qCM.getE(), 2)
                            - 3 * m_qCM.getE() * (m_qCM.getZ() - m_qpCM.getE())
                            - m_qCM.getZ() * m_qpCM.getE()) * m_t
                            + m_powerOfQ[0]
                                    * (-3
                                            * (m_qCM.getE() + m_qCM.getZ()
                                                    - 2 * m_qpCM.getE())
                                            * (m_qCM.getE() - m_qpCM.getE())
                                            + 4 * m_t)))
                    / (4. * sqrt(2.) * m_qCM.getZ()
                            * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[68] =
            (sqrt(m_s)
                    * (-((m_qCM.getE() * (m_qCM.getZ() - 2 * m_qpCM.getE())
                            + pow(m_qpCM.getE(), 2)) * m_powerOfQ[2])
                            + 2 * m_qCM.getE() * (m_qCM.getE() - m_qCM.getZ())
                                    * (m_qCM.getE() + m_qpCM.getE())
                                    * (m_qCM.getE() * pow(m_qpPerp, 2)
                                            + m_qpCM.getE() * m_t)
                            + m_powerOfQ[0]
                                    * (2 * (m_qCM.getE() - m_qCM.getZ())
                                            * m_qpCM.getE()
                                            * (pow(m_qCM.getE(), 2)
                                                    - pow(m_qpCM.getE(), 2))
                                            + 2
                                                    * (pow(m_qCM.getE(), 2)
                                                            + m_qCM.getZ()
                                                                    * m_qpCM.getE()
                                                            + m_qCM.getE()
                                                                    * (-3
                                                                            * m_qCM.getZ()
                                                                            + m_qpCM.getE()))
                                                    * pow(m_qpPerp, 2)
                                            + (-(m_qCM.getE() * m_qCM.getZ())
                                                    + 2 * m_qCM.getE()
                                                            * m_qpCM.getE()
                                                    - 2 * m_qCM.getZ()
                                                            * m_qpCM.getE()
                                                    + pow(m_qpCM.getE(), 2))
                                                    * m_t)))
                    / (2. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]
                            * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[69] = (m_qpPerp
            * (3 * (m_qCM.getE() + m_qCM.getZ() - 2 * m_qpCM.getE())
                    * m_powerOfQ[0]
                    + (3 * m_qCM.getE() - 4 * m_qCM.getZ()) * m_t))
            / (8. * sqrt(2.) * m_qCM.getZ());
    m_Ur[70] = ((13 * m_qCM.getE() + 3 * m_qCM.getZ() - 10 * m_qpCM.getE())
            * m_powerOfQ[2]
            + 8 * m_qCM.getE() * (4 * m_qCM.getE() - m_qCM.getZ())
                    * (m_qCM.getE() * pow(m_qpPerp, 2) + m_qpCM.getE() * m_t)
            + m_powerOfQ[0]
                    * (8 * (4 * m_qCM.getE() - m_qCM.getZ())
                            * (m_qCM.getE() - m_qpCM.getE()) * m_qpCM.getE()
                            + 2 * (16 * m_qCM.getE() + 7 * m_qCM.getZ())
                                    * pow(m_qpPerp, 2)
                            + (13 * m_qCM.getE() + 3 * m_qCM.getZ()
                                    + 16 * m_qpCM.getE()) * m_t))
            / (32. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]);
    m_Ur[71] = ((m_qCM.getE() - m_qCM.getZ() - 2 * m_qpCM.getE())
            * m_powerOfQ[2]
            + m_powerOfQ[0]
                    * (2 * m_qCM.getZ()
                            * (4 * m_qpCM.getE()
                                    * (-m_qCM.getE() + m_qpCM.getE())
                                    + 3 * pow(m_qpPerp, 2))
                            + (m_qCM.getE() - m_qCM.getZ()) * m_t)
            - 8 * m_qCM.getE() * m_qCM.getZ()
                    * (m_qCM.getE() * pow(m_qpPerp, 2) + m_qpCM.getE() * m_t))
            / (32. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]);
    m_Ur[72] = ((m_qCM.getZ() - m_qpCM.getE()) * m_powerOfQ[2]
            + m_powerOfQ[0]
                    * (-2 * (m_qCM.getE() - m_qCM.getZ())
                            * (m_qCM.getE() - m_qpCM.getE()\
) * m_qpCM.getE()
                            - 2 * m_qCM.getE() * pow(m_qpPerp, 2)
                            + (m_qCM.getZ() - m_qpCM.getE()) * m_t)
            - 2 * m_qCM.getE() * (m_qCM.getE() - m_qCM.getZ())
                    * (m_qCM.getE() * pow(m_qpPerp, 2) + m_qpCM.getE() * m_t))
            / (4. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]);
    m_Ur[73] = (m_qpCM.getE() * m_qpPerp
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t))
            / (16. * sqrt(2.) * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[74] = ((m_powerOfQ[2]
            + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0] * sqrt(m_s)
            - 4 * m_s * m_t)
            * (3 * m_powerOfQ[2]
                    + m_qCM.getE()
                            * (8 * m_qCM.getE() * pow(m_qpPerp, 2)
                                    + 3 * (m_qCM.getE() + m_qpCM.getE()) * m_t)
                    + m_powerOfQ[0]
                            * (8 * pow(m_qpPerp, 2)
                                    + 3
                                            * (pow(m_qCM.getE(), 2)
                                                    + m_qCM.getE()
                                                            * m_qpCM.getE()
                                                    - 2 * pow(m_qpCM.getE(), 2)
                                                    + m_t))))
            / (16. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s)
                    * sqrt(-m_t) * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[75] = -((m_powerOfQ[2]
            + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0] * sqrt(m_s)
            - 4 * m_s * m_t)
            * (m_powerOfQ[2]
                    + m_qCM.getE() * (m_qCM.getE() + m_qpCM.getE()) * m_t
                    + m_powerOfQ[0]
                            * (pow(m_qCM.getE(), 2)
                                    + m_qCM.getE() * m_qpCM.getE()
                                    - 2 * pow(m_qpCM.getE(), 2) + m_t)))
            / (16. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s)
                    * sqrt(-m_t) * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[76] = -(m_qpPerp
            * ((m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                    + m_qCM.getE() * m_t)
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t))
            / (8. * sqrt(2.) * m_powerOfQ[0] * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[77] = (m_qCM.getZ() * pow(m_qpPerp, 2)
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t))
            / (4. * sqrt(2.) * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[78] = -(sqrt(m_s)
            * (m_powerOfQ[2]
                    + m_powerOfQ[0]
                            * (pow(m_qCM.getE(), 2)
                                    + m_qCM.getE() * m_qpCM.getE()
                                    - 2 * pow(m_qpCM.getE(), 2)
                                    + 4 * pow(m_qpPerp, 2) + m_t)
                    + m_qCM.getE()
                            * (4 * m_qCM.getE() * pow(m_qpPerp, 2)
                                    + (m_qCM.getE() + m_qpCM.getE()) * m_t)))
            / (8. * m_qCM.getZ() * m_powerOfQ[0]
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[79] = -(m_qpPerp * sqrt(m_s)
            * ((m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                    + (m_qCM.getE() - 2 * m_qpCM.getE()) * m_t))
            / (2. * m_powerOfQ[0] * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[80] = (m_qpPerp * sqrt(m_s)
            * ((m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                    + m_qCM.getE() * m_t))
            / (2. * m_powerOfQ[0] * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[81] = (sqrt(m_s)
            * (pow(m_Q, 6)
                    + m_powerOfQ[2]
                            * (pow(m_qCM.getE(), 2)
                                    + 5 * m_qCM.getE() * m_qpCM.getE()
                                    - 4 * pow(m_qpCM.getE(), 2)
                                    + 4 * pow(m_qpPerp, 2) + m_t)
                    + 4 * pow(m_qCM.getE(), 2) * (m_qCM.getE() + m_qpCM.getE())
                            * (m_qCM.getE() * pow(m_qpPerp, 2)
                                    + m_qpCM.getE() * m_t)
                    + m_powerOfQ[0]
                            * (4 * pow(m_qCM.getE(), 3) * m_qpCM.getE()
                                    - 4 * m_qCM.getE() * pow(m_qpCM.getE(), 3)
                                    + 4 * m_qCM.getE()
                                            * (2 * m_qCM.getE() + m_qpCM.getE())
                                            * pow(m_qpPerp, 2)
                                    + pow(m_qCM.getE(), 2) * m_t
                                    + 5 * m_qCM.getE() * m_qpCM.getE() * m_t
                                    + 2 * pow(m_qpCM.getE(), 2) * m_t)))
            / (4. * m_qCM.getZ() * m_powerOfQ[0]
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[82] = (m_qpPerp * sqrt(m_s)
            * ((m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                    + (m_qCM.getE() + m_qpCM.getE()) * m_t))
            / (2. * m_powerOfQ[0] * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[83] = -(m_powerOfQ[2]
            + m_powerOfQ[0]
                    * (4 * (m_qCM.getE() - m_qpCM.getE()) * m_qpCM.getE()
                            - 2 * pow(m_qpPerp, 2) + m_t)
            + 4 * m_qCM.getE()
                    * (m_qCM.getE() * pow(m_qpPerp, 2) + m_qpCM.getE() * m_t))
            / (16. * m_powerOfQ[0]);
    m_Ur[84] = (m_powerOfQ[0] + 2 * pow(m_qpPerp, 2) + m_t)
            / (16. * m_powerOfQ[0]);
    m_Ur[85] = (m_qpPerp
            * ((m_qCM.getE() - 2 * m_qpCM.getE()) * m_powerOfQ[0]
                    + m_qCM.getE() * m_t))
            / (4. * m_qCM.getZ() * m_powerOfQ[0]);
    m_Ur[86] =
            -(m_qpPerp
                    * (m_powerOfQ[2]
                            + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                                    * sqrt(m_s) - 4 * m_s * m_t)
                    * (11 * m_powerOfQ[2]
                            + 4 * m_qCM.getE()
                                    * (m_qCM.getE() + 4 * m_qCM.getZ()) * m_t
                            + m_powerOfQ[0]
                                    * (12 * pow(m_qCM.getE(), 2)
                                            + 16 * m_qCM.getE() * m_qCM.getZ()
                                            - 2 * m_qCM.getE() * m_qpCM.getE()
                                            - 6 * m_qCM.getZ() * m_qpCM.getE()
                                            + 3 * m_t)))
                    / (128. * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s)
                            * sqrt(-m_t) * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[87] = -(m_qpPerp
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t)
            * (7 * m_powerOfQ[2] + 4 * pow(m_qCM.getE(), 2) * m_t
                    - m_powerOfQ[0]
                            * (-2
                                    * (6 * pow(m_qCM.getE(), 2)
                                            - 5 * m_qCM.getE() * m_qpCM.getE()
                                            + m_qCM.getZ() * m_qpCM.getE())
                                    + m_t)))
            / (128. * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[88] = ((m_powerOfQ[2]
            + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0] * sqrt(m_s)
            - 4 * m_s * m_t)
            * (m_powerOfQ[2]
                    + 2 * m_qCM.getE() * (3 * m_qCM.getE() + m_qCM.getZ())
                            * pow(m_qpPerp, 2)
                    + (m_qCM.getE() - m_qCM.getZ())
                            * (m_qCM.getE() + m_qpCM.getE()) * m_t
                    + m_powerOfQ[0]
                            * ((m_qCM.getE() - m_qpCM.getE())
                                    * (m_qCM.getE() - m_qCM.getZ()
                                            + 2 * m_qpCM.getE())
                                    + 6 * pow(m_qpPerp, 2) + m_t)))
            / (32. * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[89] = -((m_powerOfQ[2]
            + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0] * sqrt(m_s)
            - 4 * m_s * m_t)
            * (m_powerOfQ[2]
                    - 2 * m_qCM.getE() * (m_qCM.getE() + 3 * m_qCM.getZ())
                            * pow(m_qpPerp, 2)
                    + (m_qCM.getE() - m_qCM.getZ())
                            * (m_qCM.getE() + m_qpCM.getE()) * m_t
                    + m_powerOfQ[0]
                            * ((m_qCM.getE() - m_qpCM.getE())
                                    * (m_qCM.getE() - m_qCM.getZ()
                                            + 2 * m_qpCM.getE())
                                    - 2 * pow(m_qpPerp, 2) + m_t)))
            / (32. * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[90] = (m_qpPerp
            * (m_powerOfQ[2]
                    + 2 * (m_qCM.getE() - m_qpCM.getE()) * m_powerOfQ[0]
                            * sqrt(m_s) - 4 * m_s * m_t)
            * (m_powerOfQ[2]
                    + m_qCM.getE() * (m_qCM.getE() + m_qCM.getZ()) * m_t
                    - m_powerOfQ[0]
                            * (-3 * pow(m_qCM.getE(), 2)
                                    - m_qCM.getE() * m_qCM.getZ()
                                    + 4 * m_qCM.getE() * m_qpCM.getE()
                                    + 2 * m_qCM.getZ() * m_qpCM.getE() + m_t)))
            / (16. * m_qCM.getZ() * m_powerOfQ[0] * sqrt(m_s) * sqrt(-m_t)
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[91] =
            (sqrt(m_s)
                    * (13 * pow(m_Q, 6)
                            + m_powerOfQ[2]
                                    * (13 * pow(m_qCM.getE(), 2)
                                            - 3 * m_qCM.getE() * m_qCM.getZ()
                                            + 45 * m_qCM.getE() * m_qpCM.getE()
                                            + 7 * m_qCM.getZ() * m_qpCM.getE()
                                            - 42 * pow(m_qpCM.getE(), 2)
                                            + 52 * pow(m_qpPerp, 2) + 13 * m_t)
                            + 8 * m_qCM.getE()
                                    * (4 * m_qCM.getE() + m_qCM.getZ())
                                    * (m_qCM.getE() + m_qpCM.getE())
                                    * (m_qCM.getE() * pow(m_qpPerp, 2)
                                            + m_qpCM.getE() * m_t)
                            + m_powerOfQ[0]
                                    * (8 * (4 * m_qCM.getE() + m_qCM.getZ())
                                            * m_qpCM.getE()
                                            * (pow(m_qCM.getE(), 2)
                                                    - pow(m_qpCM.getE(), 2))
                                            + 2
                                                    * (42 * pow(m_qCM.getE(), 2)
                                                            + 3 * m_qCM.getZ()
                                                                    * m_qpCM.getE()
                                                            + m_qCM.getE()
                                                                    * (5
                                                                            * m_qCM.getZ()
                                                                            + 16
                                                                                    * m_qpCM.getE()))
                                                    * pow(m_qpPerp, 2)\

                                            + (13 * pow(m_qCM.getE(), 2)
                                                    - 3 * m_qCM.getE()
                                                            * (m_qCM.getZ()
                                                                    - 15
                                                                            * m_qpCM.getE())
                                                    + m_qpCM.getE()
                                                            * (m_qCM.getZ()
                                                                    + 16
                                                                            * m_qpCM.getE()))
                                                    * m_t)))
                    / (16. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[2]
                            * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[92] = (sqrt(m_s)
            * (pow(m_Q, 6)
                    + m_powerOfQ[2]
                            * (pow(m_qCM.getE(), 2)
                                    + m_qCM.getE() * m_qCM.getZ()
                                    + m_qCM.getE() * m_qpCM.getE()
                                    + 3 * m_qCM.getZ() * m_qpCM.getE()
                                    - 2 * pow(m_qpCM.getE(), 2)
                                    + 4 * pow(m_qpPerp, 2) + m_t)
                    + 8 * m_qCM.getE() * m_qCM.getZ()
                            * (m_qCM.getE() + m_qpCM.getE())
                            * (m_qCM.getE() * pow(m_qpPerp, 2)
                                    + m_qpCM.getE() * m_t)
                    + m_powerOfQ[0]
                            * (8 * m_qCM.getZ() * m_qpCM.getE()
                                    * (pow(m_qCM.getE(), 2)
                                            - pow(m_qpCM.getE(), 2))
                                    + 2
                                            * (2 * pow(m_qCM.getE(), 2)
                                                    + 9 * m_qCM.getE()
                                                            * m_qCM.getZ()
                                                    - m_qCM.getZ()
                                                            * m_qpCM.getE())
                                            * pow(m_qpPerp, 2)
                                    + (pow(m_qCM.getE(), 2)
                                            + 5 * m_qCM.getZ() * m_qpCM.getE()
                                            + m_qCM.getE()
                                                    * (m_qCM.getZ()
                                                            + m_qpCM.getE()))
                                            * m_t)))
            / (16. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[2]
                    * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[93] =
            (m_qpPerp * sqrt(m_s)
                    * ((3 * pow(m_qCM.getE(), 2)
                            + 3 * m_qCM.getZ() * m_qpCM.getE()
                            + m_qCM.getE() * (m_qCM.getZ() + m_qpCM.getE()))
                            * m_t
                            + m_powerOfQ[0]
                                    * (-((m_qCM.getE() - m_qCM.getZ()
                                            - 2 * m_qpCM.getE())
                                            * (m_qCM.getE() - m_qpCM.getE()))
                                            + 4 * m_t)))
                    / (4. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]
                            * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[94] =
            (m_qpPerp * sqrt(m_s)
                    * ((pow(m_qCM.getE(), 2) + m_qCM.getZ() * m_qpCM.getE()
                            + 3 * m_qCM.getE() * (m_qCM.getZ() + m_qpCM.getE()))
                            * m_t
                            + m_powerOfQ[0]
                                    * (-3
                                            * (m_qCM.getE() - m_qCM.getZ()
                                                    - 2 * m_qpCM.getE())
                                            * (m_qCM.getE() - m_qpCM.getE())
                                            + 4 * m_t)))
                    / (4. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]
                            * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[95] =
            -(sqrt(m_s)
                    * ((-pow(m_qpCM.getE(), 2)
                            + m_qCM.getE() * (m_qCM.getZ() + 2 * m_qpCM.getE()))
                            * m_powerOfQ[2]
                            + 2 * m_qCM.getE() * (m_qCM.getE() + m_qCM.getZ())
                                    * (m_qCM.getE() + m_qpCM.getE())
                                    * (m_qCM.getE() * pow(m_qpPerp, 2)
                                            + m_qpCM.getE() * m_t)
                            + m_powerOfQ[0]
                                    * (2 * (m_qCM.getE() + m_qCM.getZ())
                                            * m_qpCM.getE()
                                            * (pow(m_qCM.getE(), 2)
                                                    - pow(m_qpCM.getE(), 2))
                                            + 2
                                                    * (pow(m_qCM.getE(), 2)
                                                            - m_qCM.getZ()
                                                                    * m_qpCM.getE()
                                                            + m_qCM.getE()
                                                                    * (3
                                                                            * m_qCM.getZ()
                                                                            + m_qpCM.getE()))
                                                    * pow(m_qpPerp, 2)
                                            + (m_qpCM.getE()
                                                    * (2 * m_qCM.getZ()
                                                            + m_qpCM.getE())
                                                    + m_qCM.getE()
                                                            * (m_qCM.getZ()
                                                                    + 2
                                                                            * m_qpCM.getE()))
                                                    * m_t)))
                    / (2. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[2]
                            * sqrt(m_powerOfQ[2] - 4 * m_s * m_t));
    m_Ur[96] = (m_qpPerp
            * ((-3 * m_qCM.getE() + 3 * m_qCM.getZ() + 6 * m_qpCM.getE())
                    * m_powerOfQ[0]
                    - (3 * m_qCM.getE() + 4 * m_qCM.getZ()) * m_t))
            / (8. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[0]);
    m_Ur[97] = ((13 * m_qCM.getE() - 3 * m_qCM.getZ() - 10 * m_qpCM.getE())
            * m_powerOfQ[2]
            + 8 * m_qCM.getE() * (4 * m_qCM.getE() + m_qCM.getZ())
                    * (m_qCM.getE() * pow(m_qpPerp, 2) + m_qpCM.getE() * m_t)
            + m_powerOfQ[0]
                    * (8 * (4 * m_qCM.getE() + m_qCM.getZ())
                            * (m_qCM.getE() - m_qpCM.getE()) * m_qpCM.getE()
                            + 2 * (16 * m_qCM.getE() - 7 * m_qCM.getZ())
                                    * pow(m_qpPerp, 2)
                            + (13 * m_qCM.getE() - 3 * m_qCM.getZ()
                                    + 16 * m_qpCM.getE()) * m_t))
            / (32. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[2]);
    m_Ur[98] = ((m_qCM.getE() + m_qCM.getZ() - 2 * m_qpCM.getE())
            * m_powerOfQ[2]
            + m_powerOfQ[0]
                    * (-2 * m_qCM.getZ()
                            * (4 * m_qpCM.getE()
                                    * (-m_qCM.getE() + m_qpCM.getE())
                                    + 3 * pow(m_qpPerp, 2))
                            + (m_qCM.getE() + m_qCM.getZ()) * m_t)
            + 8 * m_qCM.getE() * m_qCM.getZ()
                    * (m_qCM.getE() * pow(m_qpPerp, 2) + m_qpCM.getE() * m_t))
            / (32. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[2]);
    m_Ur[99] = -((m_qCM.getZ() + m_qpCM.getE()) * m_powerOfQ[2]
            + 2 * m_qCM.getE() * (m_qCM.getE() + m_qCM.getZ())
                    * (m_qCM.getE() * pow(m_qpPerp, 2) + m_qpCM.getE() * m_t)
            + m_powerOfQ[0]
                    * (2 * (m_qCM.getE() + m_qCM.getZ())
                            * (m_qCM.getE() - m_qpCM.getE()\
) * m_qpCM.getE()
                            + 2 * m_qCM.getE() * pow(m_qpPerp, 2)
                            + (m_qCM.getZ() + m_qpCM.getE()) * m_t))
            / (4. * sqrt(2.) * m_qCM.getZ() * m_powerOfQ[2]);

    /*-------------------------------- VCS cross sections ----------------------------------*/

    SigmaVCSPol0[0] = (3 * pow(IMvcs[0][0], 2) - 2 * pow(IMvcs[0][1], 2)
            + 3 * pow(IMvcs[0][2], 2) + 3 * pow(IMvcs[1][0], 2)
            - 2 * pow(IMvcs[1][1], 2) + 3 * pow(IMvcs[1][2], 2)
            + 3 * pow(IMvcs[2][0], 2) - 2 * pow(IMvcs[2][1], 2)
            + 3 * pow(IMvcs[2][2], 2) + 3 * pow(IMvcs[3][0], 2)
            - 2 * pow(IMvcs[3][1], 2) + 3 * pow(IMvcs[3][2], 2)
            + 3 * pow(RMvcs[0][0], 2) - 2 * pow(RMvcs[0][1], 2)
            + 3 * pow(RMvcs[0][2], 2) + 3 * pow(RMvcs[1][0], 2)
            - 2 * pow(RMvcs[1][1], 2) + 3 * pow(RMvcs[1][2], 2)
            + 3 * pow(RMvcs[2][0], 2) - 2 * pow(RMvcs[2][1], 2)
            + 3 * pow(RMvcs[2][2], 2) + 3 * pow(RMvcs[3][0], 2)
            - 2 * pow(RMvcs[3][1], 2) + 3 * pow(RMvcs[3][2], 2)) / 4.;
    SigmaVCSPol0[1] = (pow(IMvcs[0][0], 2) + 2 * pow(IMvcs[0][1], 2)
            + pow(IMvcs[0][2], 2) + pow(IMvcs[1][0], 2)
            + 2 * pow(IMvcs[1][1], 2) + pow(IMvcs[1][2], 2)
            + pow(IMvcs[2][0], 2) + 2 * pow(IMvcs[2][1], 2)
            + pow(IMvcs[2][2], 2) + pow(IMvcs[3][0], 2)
            + 2 * pow(IMvcs[3][1], 2) + pow(IMvcs[3][2], 2)
            + pow(RMvcs[0][0], 2) + 2 * pow(RMvcs[0][1], 2)
            + pow(RMvcs[0][2], 2) + pow(RMvcs[1][0], 2)
            + 2 * pow(RMvcs[1][1], 2) + pow(RMvcs[1][2], 2)
            + pow(RMvcs[2][0], 2) + 2 * pow(RMvcs[2][1], 2)
            + pow(RMvcs[2][2], 2) + pow(RMvcs[3][0], 2)
            + 2 * pow(RMvcs[3][1], 2) + pow(RMvcs[3][2], 2)) / 4.;
    SigmaVCSPol0[2] = (IMvcs[0][1] * (IMvcs[0][0] - IMvcs[0][2])
            + IMvcs[1][1] * (IMvcs[1][0] - IMvcs[1][2])
            + IMvcs[2][1] * (IMvcs[2][0] - IMvcs[2][2])
            + IMvcs[3][1] * (IMvcs[3][0] - IMvcs[3][2])
            + RMvcs[0][1] * (RMvcs[0][0] - RMvcs[0][2])
            + RMvcs[1][1] * (RMvcs[1][0] - RMvcs[1][2])
            + RMvcs[2][1] * (RMvcs[2][0] - RMvcs[2][2])
            + RMvcs[3][1] * (RMvcs[3][0] - RMvcs[3][2])) / sqrt(2.);
    SigmaVCSPol0[3] = (IMvcs[0][0] * IMvcs[0][2] + IMvcs[1][0] * IMvcs[1][2]
            + IMvcs[2][0] * IMvcs[2][2] + IMvcs[3][0] * IMvcs[3][2]
            + RMvcs[0][0] * RMvcs[0][2] + RMvcs[1][0] * RMvcs[1][2]
            + RMvcs[2][0] * RMvcs[2][2] + RMvcs[3][0] * RMvcs[3][2]) / 2.;
    SigmaVCSPol0[4] = sqrt(2.)
            * ((IMvcs[0][0] - IMvcs[0][2]) * RMvcs[0][1]
                    + IMvcs[0][1] * (-RMvcs[0][0] + RMvcs[0][2])
                    + (IMvcs[1][0] - IMvcs[1][2]) * RMvcs[1][1]
                    + IMvcs[1][1] * (-RMvcs[1][0] + RMvcs[1][2])
                    + (IMvcs[2][0] - IMvcs[2][2]) * RMvcs[2][1]
                    + IMvcs[2][1] * (-RMvcs[2][0] + RMvcs[2][2])
                    + (IMvcs[3][0] - IMvcs[3][2]) * RMvcs[3][1]
                    + IMvcs[3][1] * (-RMvcs[3][0] + RMvcs[3][2]));
    SigmaVCSPolX[0] = 2
            * (IMvcs[0][0] * IMvcs[1][0] - IMvcs[0][2] * IMvcs[1][2]
                    - IMvcs[2][0] * IMvcs[3][0] + IMvcs[2][2] * IMvcs[3][2]
                    + RMvcs[0][0] * RMvcs[1][0] - RMvcs[0][2] * RMvcs[1][2]
                    - RMvcs[2][0] * RMvcs[3][0] + RMvcs[2][2] * RMvcs[3][2]);
    SigmaVCSPolX[1] = sqrt(2.)
            * ((IMvcs[0][0] + IMvcs[0][2]) * IMvcs[1][1]
                    + IMvcs[0][1] * (IMvcs[1][0] + IMvcs[1][2])
                    - (IMvcs[2][0] + IMvcs[2][2]) * IMvcs[3][1]
                    - IMvcs[2][1] * (IMvcs[3][0] + IMvcs[3][2])
                    + (RMvcs[0][0] + RMvcs[0][2]) * RMvcs[1][1]
                    + RMvcs[0][1] * (RMvcs[1][0] + RMvcs[1][2])
                    - (RMvcs[2][0] + RMvcs[2][2]) * RMvcs[3][1]
                    - RMvcs[2][1] * (RMvcs[3][0] + RMvcs[3][2]));
    SigmaVCSPolX[2] = ((IMvcs[1][0] + IMvcs[1][2]) * RMvcs[0][1]
            - IMvcs[1][1] * (RMvcs[0][0] + RMvcs[0][2])
            + (IMvcs[0][0] + IMvcs[0][2]) * RMvcs[1][1]
            - IMvcs[0][1] * (RMvcs[1][0] + RMvcs[1][2])
            - (IMvcs[3][0] + IMvcs[3][2]) * RMvcs[2][1]
            + IMvcs[3][1] * (RMvcs[2][0] + RMvcs[2][2])
            - (IMvcs[2][0] + IMvcs[2][2]) * RMvcs[3][1]
            + IMvcs[2][1] * (RMvcs[3][0] + RMvcs[3][2])) / sqrt(2.);
    SigmaVCSPolX[3] = (-(IMvcs[1][2] * RMvcs[0][0]) + IMvcs[1][0] * RMvcs[0][2]
            - IMvcs[0][2] * RMvcs[1][0] + IMvcs[0][0] * RMvcs[1][2]
            + IMvcs[3][2] * RMvcs[2][0] - IMvcs[3][0] * RMvcs[2][2]
            + IMvcs[2][2] * RMvcs[3][0] - IMvcs[2][0] * RMvcs[3][2]) / 2.;
    SigmaVCSPolY[0] = sqrt(2.)
            * ((IMvcs[0][0] - IMvcs[0][2]) * IMvcs[2][1]
                    + IMvcs[0][1] * (-IMvcs[2][0] + IMvcs[2][2])
                    + (IMvcs[1][0] - IMvcs[1][2]) * IMvcs[3][1]
                    + IMvcs[1][1] * (-IMvcs[3][0] + IMvcs[3][2])
                    + (RMvcs[0][0] - RMvcs[0][2]) * RMvcs[2][1]
                    + RMvcs[0][1] * (-RMvcs[2][0] + RMvcs[2][2])
                    + (RMvcs[1][0] - RMvcs[1][2]) * RMvcs[3][1]
                    + RMvcs[1][1] * (-RMvcs[3][0] + RMvcs[3][2]));
    SigmaVCSPolY[1] = (3 * IMvcs[2][0] * RMvcs[0][0]
            - 2 * IMvcs[2][1] * RMvcs[0][1] + 3 * IMvcs[2][2] * RMvcs[0][2]
            + 3 * IMvcs[3][0] * RMvcs[1][0] - 2 * IMvcs[3][1] * RMvcs[1][1]
            + 3 * IMvcs[3][2] * RMvcs[1][2] - 3 * IMvcs[0][0] * RMvcs[2][0]
            + 2 * IMvcs[0][1] * RMvcs[2][1] - 3 * IMvcs[0][2] * RMvcs[2][2]
            - 3 * IMvcs[1][0] * RMvcs[3][0] + 2 * IMvcs[1][1] * RMvcs[3][1]
            - 3 * IMvcs[1][2] * RMvcs[3][2]) / 2.;
    SigmaVCSPolY[2] = (IMvcs[2][0] * RMvcs[0][0] + 2 * IMvcs[2][1] * RMvcs[0][1]
            + IMvcs[2][2] * RMvcs[0][2] + IMvcs[3][0] * RMvcs[1][0]
            + 2 * IMvcs[3][1] * RMvcs[1][1] + IMvcs[3][2] * RMvcs[1][2]
            - IMvcs[0][0] * RMvcs[2][0] - 2 * IMvcs[0][1] * RMvcs[2][1]
            - IMvcs[0][2] * RMvcs[2][2] - IMvcs[1][0] * RMvcs[3][0]
            - 2 * IMvcs[1][1] * RMvcs[3][1] - IMvcs[1][2] * RMvcs[3][2]) / 2.;
    SigmaVCSPolY[3] = ((IMvcs[2][0] - IMvcs[2][2]) * RMvcs[0][1]
            + IMvcs[2][1] * (RMvcs[0][0] - RMvcs[0][2])
            + (IMvcs[3][0] - IMvcs[3][2]) * RMvcs[1][1]
            + IMvcs[3][1] * (RMvcs[1][0] - RMvcs[1][2])
            + (-IMvcs[0][0] + IMvcs[0][2]) * RMvcs[2][1]
            + IMvcs[0][1] * (-RMvcs[2][0] + RMvcs[2][2])
            + (-IMvcs[1][0] + IMvcs[1][2]) * RMvcs[3][1]
            + IMvcs[1][1] * (-RMvcs[3][0] + RMvcs[3][2])) / sqrt(2.);
    SigmaVCSPolY[4] = (IMvcs[2][2] * RMvcs[0][0] + IMvcs[2][0] * RMvcs[0][2]
            + IMvcs[3][2] * RMvcs[1][0] + IMvcs[3][0] * RMvcs[1][2]
            - IMvcs[0][2] * RMvcs[2][0] - IMvcs[0][0] * RMvcs[2][2]
            - IMvcs[1][2] * RMvcs[3][0] - IMvcs[1][0] * RMvcs[3][2]) / 2.;
    SigmaVCSPolZ[0] = 2
            * (IMvcs[1][0] * IMvcs[2][0] - IMvcs[1][2] * IMvcs[2][2]
                    + IMvcs[0][0] * IMvcs[3][0] - IMvcs[0][2] * IMvcs[3][2]
                    + RMvcs[1][0] * RMvcs[2][0] - RMvcs[1][2] * RMvcs[2][2]
                    + RMvcs[0][0] * RMvcs[3][0] - RMvcs[0][2] * RMvcs[3][2]);
    SigmaVCSPolZ[1] = sqrt(2.)
            * ((IMvcs[1][0] + IMvcs[1][2]) * IMvcs[2][1]
                    + IMvcs[1][1] * (IMvcs[2][0] + IMvcs[2][2])
                    + (IMvcs[0][0] + IMvcs[0][2]) * IMvcs[3][1]
                    + IMvcs[0][1] * (IMvcs[3][0] + IMvcs[3][2])
                    + (RMvcs[1][0] + RMvcs[1][2]) * RMvcs[2][1]
                    + RMvcs[1][1] * (RMvcs[2][0] + RMvcs[2][2])
                    + (RMvcs[0][0] + RMvcs[0][2]) * RMvcs[3][1]
                    + RMvcs[0][1] * (RMvcs[3][0] + RMvcs[3][2]));
    SigmaVCSPolZ[2] = ((IMvcs[3][0] + IMvcs[3][2]) * RMvcs[0][1]
            - IMvcs[3][1] * (RMvcs[0][0] + RMvcs[0][2])
            + (IMvcs[2][0] + IMvcs[2][2]) * RMvcs[1][1]
            - IMvcs[2][1] * (RMvcs[1][0] + RMvcs[1][2])
            + (IMvcs[1][0] + IMvcs[1][2]) * RMvcs[2][1]
            - IMvcs[1][1] * (RMvcs[2][0] + RMvcs[2][2])
            + (IMvcs[0][0] + IMvcs[0][2]) * RMvcs[3][1]
            - IMvcs[0][1] * (RMvcs[3][0] + RMvcs[3][2])) / sqrt(2.);
    SigmaVCSPolZ[3] = (-(IMvcs[3][2] * RMvcs[0][0]) + IMvcs[3][0] * RMvcs[0][2]
            - IMvcs[2][2] * RMvcs[1][0] + IMvcs[2][0] * RMvcs[1][2]
            - IMvcs[1][2] * RMvcs[2][0] + IMvcs[1][0] * RMvcs[2][2]
            - IMvcs[0][2] * RMvcs[3][0] + IMvcs[0][0] * RMvcs[3][2]) / 2.;

    /*--------------------------- Interference cross sections ------------------------------*/

    if (InitExactBHCrossSections == kFALSE) {
        GV2008Model::MakeExactBHCrossSections();

        // Flag

        InitExactBHCrossSections = kTRUE;
    } // end if InitExactBHCrossSections

    SigmaIPol0[0] = Jem[0][1]
            * (-2 * RMvcs[0][0] * m_Ur[0] * m_powerOfQ[0]
                    + 2 * RMvcs[0][1] * m_Ur[24] * m_powerOfQ[1]
                    - 2 * RMvcs[0][2] * m_Ur[39] * m_powerOfQ[2])
            + Jem[2][1]
                    * (-2 * RMvcs[2][0] * m_Ur[0] * m_powerOfQ[0]
                            + 2 * RMvcs[2][1] * m_Ur[24] * m_powerOfQ[1]
                            - 2 * RMvcs[2][2] * m_Ur[39] * m_powerOfQ[2])
            + Jem[0][2]
                    * (-2 * RMvcs[0][0] * m_Ur[8] * m_powerOfQ[0]
                            + 2 * RMvcs[0][1] * m_Ur[29] * m_powerOfQ[1]
                            - 2 * RMvcs[0][2] * m_Ur[45] * m_powerOfQ[2])
            + Jem[2][2]
                    * (-2 * RMvcs[2][0] * m_Ur[8] * m_powerOfQ[0]
                            + 2 * RMvcs[2][1] * m_Ur[29] * m_powerOfQ[1]
                            - 2 * RMvcs[2][2] * m_Ur[45] * m_powerOfQ[2])
            + Jem[1][2]
                    * (-2 * RMvcs[1][0] * m_Ur[69] * m_powerOfQ[0]
                            + 2 * RMvcs[1][1] * m_Ur[83] * m_powerOfQ[1]
                            - 2 * RMvcs[1][2] * m_Ur[96] * m_powerOfQ[2])
            + Jem[3][2]
                    * (-2 * RMvcs[3][0] * m_Ur[69] * m_powerOfQ[0]
                            + 2 * RMvcs[3][1] * m_Ur[83] * m_powerOfQ[1]
                            - 2 * RMvcs[3][2] * m_Ur[96] * m_powerOfQ[2]);
    SigmaIPol0[1] = Jem[0][1]
            * (-2 * RMvcs[0][0] * m_Ur[2] * m_powerOfQ[0]
                    - 2 * RMvcs[0][1] * m_Ur[24] * m_powerOfQ[1]
                    - 2 * RMvcs[0][2] * m_Ur[1] * m_powerOfQ[2])
            + Jem[2][1]
                    * (-2 * RMvcs[2][0] * m_Ur[2] * m_powerOfQ[0]
                            - 2 * RMvcs[2][1] * m_Ur[24] * m_powerOfQ[1]
                            - 2 * RMvcs[2][2] * m_Ur[1] * m_powerOfQ[2])
            + Jem[1][2]
                    * (-2 * RMvcs[1][0] * m_Ur[51] * m_powerOfQ[0]
                            - 2 * RMvcs[1][1] * m_Ur[83] * m_powerOfQ[1]
                            + 2 * RMvcs[1][2] * m_Ur[16] * m_powerOfQ[2])
            + Jem[3][2]
                    * (-2 * RMvcs[3][0] * m_Ur[51] * m_powerOfQ[0]
                            - 2 * RMvcs[3][1] * m_Ur[83] * m_powerOfQ[1]
                            + 2 * RMvcs[3][2] * m_Ur[16] * m_powerOfQ[2])
            + Jem[0][2]
                    * (-2 * RMvcs[0][0] * m_Ur[10] * m_powerOfQ[0]
                            - 2 * RMvcs[0][1] * m_Ur[29] * m_powerOfQ[1]
                            - 2 * RMvcs[0][2] * m_Ur[9] * m_powerOfQ[2])
            + Jem[2][2]
                    * (-2 * RMvcs[2][0] * m_Ur[10] * m_powerOfQ[0]
                            - 2 * RMvcs[2][1] * m_Ur[29] * m_powerOfQ[1]
                            - 2 * RMvcs[2][2] * m_Ur[9] * m_powerOfQ[2]);
    SigmaIPol0[2] = Jem[0][1]
            * (-2 * RMvcs[0][0] * m_Ur[3] * m_powerOfQ[1]
                    - 2 * RMvcs[0][2] * m_Ur[40] * m_powerOfQ[1]
                    + 2 * RMvcs[0][1] * m_Ur[26] * m_powerOfQ[2])
            + Jem[2][1]
                    * (-2 * RMvcs[2][0] * m_Ur[3] * m_powerOfQ[1]
                            - 2 * RMvcs[2][2] * m_Ur[40] * m_powerOfQ[1]
                            + 2 * RMvcs[2][1] * m_Ur[26] * m_powerOfQ[2])
            + Jem[0][2]
                    * (-2 * RMvcs[0][0] * m_Ur[11] * m_powerOfQ[1]
                            + 2 * RMvcs[0][1] * m_Ur[31] * m_powerOfQ[2]
                            - 2 * RMvcs[0][2] * m_Ur[46] * m_powerOfQ[3])
            + Jem[2][2]
                    * (-2 * RMvcs[2][0] * m_Ur[11] * m_powerOfQ[1]
                            + 2 * RMvcs[2][1] * m_Ur[31] * m_powerOfQ[2]
                            - 2 * RMvcs[2][2] * m_Ur[46] * m_powerOfQ[3])
            + Jem[1][2]
                    * (-2 * RMvcs[1][0] * m_Ur[70] * m_powerOfQ[1]
                            + 2 * RMvcs[1][1] * m_Ur[85] * m_powerOfQ[2]
                            - 2 * RMvcs[1][2] * m_Ur[97] * m_powerOfQ[3])
            + Jem[3][2]
                    * (-2 * RMvcs[3][0] * m_Ur[70] * m_powerOfQ[1]
                            + 2 * RMvcs[3][1] * m_Ur[85] * m_powerOfQ[2]
                            - 2 * RMvcs[3][2] * m_Ur[97] * m_powerOfQ[3]);
    SigmaIPol0[3] = Jem[0][1]
            * (-2 * RMvcs[0][2] * m_Ur[42] * m_powerOfQ[1]
                    - 2 * RMvcs[0][0] * m_Ur[5] * m_powerOfQ[1]
                    + 2 * RMvcs[0][1] * m_Ur[27] * m_powerOfQ[2])
            + Jem[2][1]
                    * (-2 * RMvcs[2][2] * m_Ur[42] * m_powerOfQ[1]
                            - 2 * RMvcs[2][0] * m_Ur[5] * m_powerOfQ[1]
                            + 2 * RMvcs[2][1] * m_Ur[27] * m_powerOfQ[2])
            + Jem[0][2]
                    * (-2 * RMvcs[0][0] * m_Ur[13] * m_powerOfQ[1]
                            + 2 * RMvcs[0][1] * m_Ur[32] * m_powerOfQ[2]
                            - 2 * RMvcs[0][2] * m_Ur[48] * m_powerOfQ[3])
            + Jem[2][2]
                    * (-2 * RMvcs[2][0] * m_Ur[13] * m_powerOfQ[1]
                            + 2 * RMvcs[2][1] * m_Ur[32] * m_powerOfQ[2]
                            - 2 * RMvcs[2][2] * m_Ur[48] * m_powerOfQ[3])
            + Jem[1][2]
                    * (-2 * RMvcs[1][0] * m_Ur[71] * m_powerOfQ[1]
                            - 2 * RMvcs[1][1] * m_Ur[85] * m_powerOfQ[2]
                            - 2 * RMvcs[1][2] * m_Ur[98] * m_powerOfQ[3])
            + Jem[3][2]
                    * (-2 * RMvcs[3][0] * m_Ur[71] * m_powerOfQ[1]
                            - 2 * RMvcs[3][1] * m_Ur[85] * m_powerOfQ[2]
                            - 2 * RMvcs[3][2] * m_Ur[98] * m_powerOfQ[3]);
    SigmaIPol0[4] = Jem[0][1]
            * (-2 * RMvcs[0][2] * m_Ur[2] * m_powerOfQ[0]
                    + 2 * RMvcs[0][1] * m_Ur[25] * m_powerOfQ[1]
                    - 2 * RMvcs[0][0] * m_Ur[1] * m_powerOfQ[2])
            + Jem[2][1]
                    * (-2 * RMvcs[2][2] * m_Ur[2] * m_powerOfQ[0]
                            + 2 * RMvcs[2][1] * m_Ur[25] * m_powerOfQ[1]
                            - 2 * RMvcs[2][0] * m_Ur[1] * m_powerOfQ[2])
            + Jem[0][2]
                    * (-2 * RMvcs[0][2] * m_Ur[10] * m_powerOfQ[0]
                            - 2 * RMvcs[0][0] * m_Ur[9] * m_powerOfQ[2]
                            + 2 * RMvcs[0][1] * m_Ur[30] * m_powerOfQ[3])
            + Jem[2][2]
                    * (-2 * RMvcs[2][2] * m_Ur[10] * m_powerOfQ[0]
                            - 2 * RMvcs[2][0] * m_Ur[9] * m_powerOfQ[2]
                            + 2 * RMvcs[2][1] * m_Ur[30] * m_powerOfQ[3])
            + Jem[1][2]
                    * (-2 * RMvcs[1][2] * m_Ur[51] * m_powerOfQ[0]
                            + 2 * RMvcs[1][0] * m_Ur[16] * m_powerOfQ[2]
                            + 2 * RMvcs[1][1] * m_Ur[84] * m_powerOfQ[3])
            + Jem[3][2]
                    * (-2 * RMvcs[3][2] * m_Ur[51] * m_powerOfQ[0]
                            + 2 * RMvcs[3][0] * m_Ur[16] * m_powerOfQ[2]
                            + 2 * RMvcs[3][1] * m_Ur[84] * m_powerOfQ[3]);
    SigmaIPol0[5] = Jem[0][1]
            * (-2 * RMvcs[0][2] * m_Ur[41] * m_Q
                    - 2 * RMvcs[0][0] * m_Ur[4] * m_powerOfQ[1])
            + Jem[2][1]
                    * (-2 * RMvcs[2][2] * m_Ur[41] * m_Q
                            - 2 * RMvcs[2][0] * m_Ur[4] * m_powerOfQ[1])
            + Jem[0][2]
                    * (-2 * RMvcs[0][2] * m_Ur[47] * m_Q
                            - 2 * RMvcs[0][0] * m_Ur[12] * m_powerOfQ[3])
            + Jem[2][2]
                    * (-2 * RMvcs[2][2] * m_Ur[47] * m_Q
                            - 2 * RMvcs[2][0] * m_Ur[12] * m_powerOfQ[3])
            + Jem[1][2]
                    * (-2 * RMvcs[1][2] * m_Ur[53] * m_Q
                            + 2 * RMvcs[1][0] * m_Ur[18] * m_powerOfQ[3])
            + Jem[3][2]
                    * (-2 * RMvcs[3][2] * m_Ur[53] * m_Q
                            + 2 * RMvcs[3][0] * m_Ur[18] * m_powerOfQ[3]);
    SigmaIPol0[6] = Jem[0][1]
            * (-2 * IMvcs[0][1] * m_Ur[28] * m_powerOfQ[0]
                    + 2 * IMvcs[0][2] * m_Ur[44] * m_powerOfQ[1]
                    + 2 * IMvcs[0][0] * m_Ur[7] * m_powerOfQ[1])
            + Jem[2][1]
                    * (-2 * IMvcs[2][1] * m_Ur[28] * m_powerOfQ[0]
                            + 2 * IMvcs[2][2] * m_Ur[44] * m_powerOfQ[1]
                            + 2 * IMvcs[2][0] * m_Ur[7] * m_powerOfQ[1])
            + Jem[0][2]
                    * (2 * IMvcs[0][0] * m_Ur[15] * m_powerOfQ[1]
                            - 2 * IMvcs[0][1] * m_Ur[33] * m_powerOfQ[2]
                            + 2 * IMvcs[0][2] * m_Ur[50] * m_powerOfQ[3])
            + Jem[2][2]
                    * (2 * IMvcs[2][0] * m_Ur[15] * m_powerOfQ[1]
                            - 2 * IMvcs[2][1] * m_Ur[33] * m_powerOfQ[2]
                            + 2 * IMvcs[2][2] * m_Ur[50] * m_powerOfQ[3])
            + Jem[1][2]
                    * (2 * IMvcs[1][0] * m_Ur[72] * m_powerOfQ[1]
                            + 2 * IMvcs[1][1] * m_Ur[85] * m_powerOfQ[2]
                            + 2 * IMvcs[1][2] * m_Ur[99] * m_powerOfQ[3])
            + Jem[3][2]
                    * (2 * IMvcs[3][0] * m_Ur[72] * m_powerOfQ[1]
                            + 2 * IMvcs[3][1] * m_Ur[85] * m_powerOfQ[2]
                            + 2 * IMvcs[3][2] * m_Ur[99] * m_powerOfQ[3]);
    SigmaIPol0[7] = Jem[0][1]
            * (2 * IMvcs[0][2] * m_Ur[43] * m_powerOfQ[0]
                    - 4 * IMvcs[0][1] * m_Ur[25] * m_powerOfQ[1]
                    + 2 * IMvcs[0][0] * m_Ur[6] * m_powerOfQ[2])
            + Jem[2][1]
                    * (2 * IMvcs[2][2] * m_Ur[43] * m_powerOfQ[0]
                            - 4 * IMvcs[2][1] * m_Ur[25] * m_powerOfQ[1]
                            + 2 * IMvcs[2][0] * m_Ur[6] * m_powerOfQ[2])
            + Jem[0][2]
                    * (2 * IMvcs[0][2] * m_Ur[49] * m_powerOfQ[0]
                            + 2 * IMvcs[0][0] * m_Ur[14] * m_powerOfQ[2]
                            - 4 * IMvcs[0][1] * m_Ur[30] * m_powerOfQ[3])
            + Jem[2][2]
                    * (2 * IMvcs[2][2] * m_Ur[49] * m_powerOfQ[0]
                            + 2 * IMvcs[2][0] * m_Ur[14] * m_powerOfQ[2]
                            - 4 * IMvcs[2][1] * m_Ur[30] * m_powerOfQ[3])
            + Jem[1][2]
                    * (2 * IMvcs[1][2] * m_Ur[56] * m_powerOfQ[0]
                            - 2 * IMvcs[1][0] * m_Ur[21] * m_powerOfQ[2]
                            - 4 * IMvcs[1][1] * m_Ur[84] * m_powerOfQ[3])
            + Jem[3][2]
                    * (2 * IMvcs[3][2] * m_Ur[56] * m_powerOfQ[0]
                            - 2 * IMvcs[3][0] * m_Ur[21] * m_powerOfQ[2]
                            - 4 * IMvcs[3][1] * m_Ur[84] * m_powerOfQ[3]);
    SigmaIPolX[0] = Jem[0][1]
            * (2 * IMvcs[1][0] * m_Ur[59] * m_powerOfQ[1]
                    + 2 * IMvcs[1][2] * m_Ur[86] * m_powerOfQ[1]
                    - 2 * IMvcs[1][1] * m_Ur[74] * m_powerOfQ[2])
            + Jem[2][1]
                    * (-2 * IMvcs[3][0] * m_Ur[59] * m_powerOfQ[1]
                            - 2 * IMvcs[3][2] * m_Ur[86] * m_powerOfQ[1]
                            + 2 * IMvcs[3][1] * m_Ur[74] * m_powerOfQ[2])
            + Jem[1][2]
                    * (2 * IMvcs[0][0] * m_Ur[17] * m_powerOfQ[1]
                            - 2 * IMvcs[0][1] * m_Ur[35] * m_powerOfQ[2]
                            + 2 * IMvcs[0][2] * m_Ur[52] * m_powerOfQ[3])
            + Jem[3][2]
                    * (-2 * IMvcs[2][0] * m_Ur[17] * m_powerOfQ[1]
                            + 2 * IMvcs[2][1] * m_Ur[35] * m_powerOfQ[2]
                            - 2 * IMvcs[2][2] * m_Ur[52] * m_powerOfQ[3])
            + Jem[0][2]
                    * (2 * IMvcs[1][0] * m_Ur[64] * m_powerOfQ[1]
                            - 2 * IMvcs[1][1] * m_Ur[79] * m_powerOfQ[2]
                            + 2 * IMvcs[1][2] * m_Ur[91] * m_powerOfQ[3])
            + Jem[2][2]
                    * (-2 * IMvcs[3][0] * m_Ur[64] * m_powerOfQ[1]
                            + 2 * IMvcs[3][1] * m_Ur[79] * m_powerOfQ[2]
                            - 2 * IMvcs[3][2] * m_Ur[91] * m_powerOfQ[3]);
    SigmaIPolX[1] = Jem[0][1]
            * (2 * IMvcs[1][0] * m_Ur[60] * m_powerOfQ[1]
                    + 2 * IMvcs[1][2] * m_Ur[87] * m_powerOfQ[1]
                    - 2 * IMvcs[1][1] * m_Ur[75] * m_powerOfQ[2])
            + Jem[2][1]
                    * (-2 * IMvcs[3][0] * m_Ur[60] * m_powerOfQ[1]
                            - 2 * IMvcs[3][2] * m_Ur[87] * m_powerOfQ[1]
                            + 2 * IMvcs[3][1] * m_Ur[75] * m_powerOfQ[2])
            + Jem[1][2]
                    * (2 * IMvcs[0][0] * m_Ur[19] * m_powerOfQ[1]
                            - 2 * IMvcs[0][1] * m_Ur[36] * m_powerOfQ[2]
                            + 2 * IMvcs[0][2] * m_Ur[54] * m_powerOfQ[3])
            + Jem[3][2]
                    * (-2 * IMvcs[2][0] * m_Ur[19] * m_powerOfQ[1]
                            + 2 * IMvcs[2][1] * m_Ur[36] * m_powerOfQ[2]
                            - 2 * IMvcs[2][2] * m_Ur[54] * m_powerOfQ[3])
            + Jem[0][2]
                    * (2 * IMvcs[1][0] * m_Ur[65] * m_powerOfQ[1]
                            - 2 * IMvcs[1][1] * m_Ur[80] * m_powerOfQ[2]
                            + 2 * IMvcs[1][2] * m_Ur[92] * m_powerOfQ[3])
            + Jem[2][2]
                    * (-2 * IMvcs[3][0] * m_Ur[65] * m_powerOfQ[1]
                            + 2 * IMvcs[3][1] * m_Ur[80] * m_powerOfQ[2]
                            - 2 * IMvcs[3][2] * m_Ur[92] * m_powerOfQ[3]);
    SigmaIPolX[2] = Jem[0][1]
            * (2 * IMvcs[1][2] * m_Ur[2] * m_powerOfQ[0]
                    - 2 * IMvcs[1][1] * m_Ur[73] * m_powerOfQ[1]
                    - 2 * IMvcs[1][0] * m_Ur[1] * m_powerOfQ[2])
            + Jem[2][1]
                    * (-2 * IMvcs[3][2] * m_Ur[2] * m_powerOfQ[0]
                            + 2 * IMvcs[3][1] * m_Ur[73] * m_powerOfQ[1]
                            + 2 * IMvcs[3][0] * m_Ur[1] * m_powerOfQ[2])
            + Jem[1][2]
                    * (2 * IMvcs[0][2] * m_Ur[51] * m_powerOfQ[0]
                            + 2 * IMvcs[0][0] * m_Ur[16] * m_powerOfQ[2]
                            - 2 * IMvcs[0][1] * m_Ur[34] * m_powerOfQ[3])
            + Jem[3][2]
                    * (-2 * IMvcs[2][2] * m_Ur[51] * m_powerOfQ[0]
                            - 2 * IMvcs[2][0] * m_Ur[16] * m_powerOfQ[2]
                            + 2 * IMvcs[2][1] * m_Ur[34] * m_powerOfQ[3])
            + Jem[0][2]
                    * (2 * IMvcs[1][2] * m_Ur[10] * m_powerOfQ[0]
                            - 2 * IMvcs[1][0] * m_Ur[9] * m_powerOfQ[2]
                            - 2 * IMvcs[1][1] * m_Ur[78] * m_powerOfQ[3])
            + Jem[2][2]
                    * (-2 * IMvcs[3][2] * m_Ur[10] * m_powerOfQ[0]
                            + 2 * IMvcs[3][0] * m_Ur[9] * m_powerOfQ[2]
                            + 2 * IMvcs[3][1] * m_Ur[78] * m_powerOfQ[3]);
    SigmaIPolX[3] = Jem[0][1]
            * (2 * IMvcs[1][2] * m_Ur[41] * m_Q
                    - 2 * IMvcs[1][0] * m_Ur[4] * m_powerOfQ[1])
            + Jem[2][1]
                    * (-2 * IMvcs[3][2] * m_Ur[41] * m_Q
                            + 2 * IMvcs[3][0] * m_Ur[4] * m_powerOfQ[1])
            + Jem[0][2]
                    * (2 * IMvcs[1][2] * m_Ur[47] * m_Q
                            - 2 * IMvcs[1][0] * m_Ur[12] * m_powerOfQ[3])
            + Jem[2][2]
                    * (-2 * IMvcs[3][2] * m_Ur[47] * m_Q
                            + 2 * IMvcs[3][0] * m_Ur[12] * m_powerOfQ[3])
            + Jem[1][2]
                    * (2 * IMvcs[0][2] * m_Ur[53] * m_Q
                            + 2 * IMvcs[0][0] * m_Ur[18] * m_powerOfQ[3])
            + Jem[3][2]
                    * (-2 * IMvcs[2][2] * m_Ur[53] * m_Q
                            - 2 * IMvcs[2][0] * m_Ur[18] * m_powerOfQ[3]);
    SigmaIPolX[4] = Jem[1][2]
            * (-2 * RMvcs[0][0] * m_Ur[20] * m_powerOfQ[0]
                    + 2 * RMvcs[0][1] * m_Ur[37] * m_powerOfQ[1]
                    - 2 * RMvcs[0][2] * m_Ur[55] * m_powerOfQ[2])
            + Jem[3][2]
                    * (2 * RMvcs[2][0] * m_Ur[20] * m_powerOfQ[0]
                            - 2 * RMvcs[2][1] * m_Ur[37] * m_powerOfQ[1]
                            + 2 * RMvcs[2][2] * m_Ur[55] * m_powerOfQ[2])
            + Jem[0][1]
                    * (-2 * RMvcs[1][0] * m_Ur[61] * m_powerOfQ[0]
                            + 2 * RMvcs[1][1] * m_Ur[76] * m_powerOfQ[1]
                            - 2 * RMvcs[1][2] * m_Ur[88] * m_powerOfQ[2])
            + Jem[2][1]
                    * (2 * RMvcs[3][0] * m_Ur[61] * m_powerOfQ[0]
                            - 2 * RMvcs[3][1] * m_Ur[76] * m_powerOfQ[1]
                            + 2 * RMvcs[3][2] * m_Ur[88] * m_powerOfQ[2])
            + Jem[0][2]
                    * (-2 * RMvcs[1][0] * m_Ur[66] * m_powerOfQ[0]
                            + 2 * RMvcs[1][1] * m_Ur[81] * m_powerOfQ[1]
                            - 2 * RMvcs[1][2] * m_Ur[93] * m_powerOfQ[2])
            + Jem[2][2]
                    * (2 * RMvcs[3][0] * m_Ur[66] * m_powerOfQ[0]
                            - 2 * RMvcs[3][1] * m_Ur[81] * m_powerOfQ[1]
                            + 2 * RMvcs[3][2] * m_Ur[93] * m_powerOfQ[2]);
    SigmaIPolX[5] = Jem[1][2]
            * (-2 * RMvcs[0][0] * m_Ur[22] * m_powerOfQ[0]
                    - 2 * RMvcs[0][1] * m_Ur[37] * m_powerOfQ[1]
                    - 2 * RMvcs[0][2] * m_Ur[57] * m_powerOfQ[2])
            + Jem[3][2]
                    * (2 * RMvcs[2][0] * m_Ur[22] * m_powerOfQ[0]
                            + 2 * RMvcs[2][1] * m_Ur[37] * m_powerOfQ[1]
                            + 2 * RMvcs[2][2] * m_Ur[57] * m_powerOfQ[2])
            + Jem[0][1]
                    * (-2 * RMvcs[1][0] * m_Ur[62] * m_powerOfQ[0]
                            - 2 * RMvcs[1][1] * m_Ur[76] * m_powerOfQ[1]
                            - 2 * RMvcs[1][2] * m_Ur[89] * m_powerOfQ[2])
            + Jem[2][1]
                    * (2 * RMvcs[3][0] * m_Ur[62] * m_powerOfQ[0]
                            + 2 * RMvcs[3][1] * m_Ur[76] * m_powerOfQ[1]
                            + 2 * RMvcs[3][2] * m_Ur[89] * m_powerOfQ[2])
            + Jem[0][2]
                    * (-2 * RMvcs[1][0] * m_Ur[67] * m_powerOfQ[0]
                            - 2 * RMvcs[1][1] * m_Ur[81] * m_powerOfQ[1]
                            - 2 * RMvcs[1][2] * m_Ur[94] * m_powerOfQ[2])
            + Jem[2][2]
                    * (2 * RMvcs[3][0] * m_Ur[67] * m_powerOfQ[0]
                            + 2 * RMvcs[3][1] * m_Ur[81] * m_powerOfQ[1]
                            + 2 * RMvcs[3][2] * m_Ur[94] * m_powerOfQ[2]);
    SigmaIPolX[6] = Jem[0][1]
            * (2 * RMvcs[1][1] * m_Ur[77] * m_powerOfQ[0]
                    - 2 * RMvcs[1][0] * m_Ur[63] * m_powerOfQ[1]
                    - 2 * RMvcs[1][2] * m_Ur[90] * m_powerOfQ[1])
            + Jem[2][1]
                    * (-2 * RMvcs[3][1] * m_Ur[77] * m_powerOfQ[0]
                            + 2 * RMvcs[3][0] * m_Ur[63] * m_powerOfQ[1]
                            + 2 * RMvcs[3][2] * m_Ur[90] * m_powerOfQ[1])
            + Jem[1][2]
                    * (-2 * RMvcs[0][0] * m_Ur[23] * m_powerOfQ[1]
                            + 2 * RMvcs[0][1] * m_Ur[38] * m_powerOfQ[2]
                            - 2 * RMvcs[0][2] * m_Ur[58] * m_powerOfQ[3])
            + Jem[3][2]
                    * (2 * RMvcs[2][0] * m_Ur[23] * m_powerOfQ[1]
                            - 2 * RMvcs[2][1] * m_Ur[38] * m_powerOfQ[2]
                            + 2 * RMvcs[2][2] * m_Ur[58] * m_powerOfQ[3])
            + Jem[0][2]
                    * (-2 * RMvcs[1][0] * m_Ur[68] * m_powerOfQ[1]
                            + 2 * RMvcs[1][1] * m_Ur[82] * m_powerOfQ[2]
                            - 2 * RMvcs[1][2] * m_Ur[95] * m_powerOfQ[3])
            + Jem[2][2]
                    * (2 * RMvcs[3][0] * m_Ur[68] * m_powerOfQ[1]
                            - 2 * RMvcs[3][1] * m_Ur[82] * m_powerOfQ[2]
                            + 2 * RMvcs[3][2] * m_Ur[95] * m_powerOfQ[3]);
    SigmaIPolX[7] = Jem[0][1]
            * (-2 * RMvcs[1][2] * m_Ur[43] * m_powerOfQ[0]
                    + 4 * RMvcs[1][1] * m_Ur[73] * m_powerOfQ[1]
                    + 2 * RMvcs[1][0] * m_Ur[6] * m_powerOfQ[2])
            + Jem[2][1]
                    * (2 * RMvcs[3][2] * m_Ur[43] * m_powerOfQ[0]
                            - 4 * RMvcs[3][1] * m_Ur[73] * m_powerOfQ[1]
                            - 2 * RMvcs[3][0] * m_Ur[6] * m_powerOfQ[2])
            + Jem[1][2]
                    * (-2 * RMvcs[0][2] * m_Ur[56] * m_powerOfQ[0]
                            - 2 * RMvcs[0][0] * m_Ur[21] * m_powerOfQ[2]
                            + 4 * RMvcs[0][1] * m_Ur[34] * m_powerOfQ[3])
            + Jem[3][2]
                    * (2 * RMvcs[2][2] * m_Ur[56] * m_powerOfQ[0]
                            + 2 * RMvcs[2][0] * m_Ur[21] * m_powerOfQ[2]
                            - 4 * RMvcs[2][1] * m_Ur[34] * m_powerOfQ[3])
            + Jem[0][2]
                    * (-2 * RMvcs[1][2] * m_Ur[49] * m_powerOfQ[0]
                            + 2 * RMvcs[1][0] * m_Ur[14] * m_powerOfQ[2]
                            + 4 * RMvcs[1][1] * m_Ur[78] * m_powerOfQ[3])
            + Jem[2][2]
                    * (2 * RMvcs[3][2] * m_Ur[49] * m_powerOfQ[0]
                            - 2 * RMvcs[3][0] * m_Ur[14] * m_powerOfQ[2]
                            - 4 * RMvcs[3][1] * m_Ur[78] * m_powerOfQ[3]);
    SigmaIPolY[0] = Jem[2][1]
            * (2 * IMvcs[0][0] * m_Ur[0] * m_powerOfQ[0]
                    - 2 * IMvcs[0][1] * m_Ur[24] * m_powerOfQ[1]
                    + 2 * IMvcs[0][2] * m_Ur[39] * m_powerOfQ[2])
            + Jem[0][1]
                    * (-2 * IMvcs[2][0] * m_Ur[0] * m_powerOfQ[0]
                            + 2 * IMvcs[2][1] * m_Ur[24] * m_powerOfQ[1]
                            - 2 * IMvcs[2][2] * m_Ur[39] * m_powerOfQ[2])
            + Jem[2][2]
                    * (2 * IMvcs[0][0] * m_Ur[8] * m_powerOfQ[0]
                            - 2 * IMvcs[0][1] * m_Ur[29] * m_powerOfQ[1]
                            + 2 * IMvcs[0][2] * m_Ur[45] * m_powerOfQ[2])
            + Jem[0][2]
                    * (-2 * IMvcs[2][0] * m_Ur[8] * m_powerOfQ[0]
                            + 2 * IMvcs[2][1] * m_Ur[29] * m_powerOfQ[1]
                            - 2 * IMvcs[2][2] * m_Ur[45] * m_powerOfQ[2])
            + Jem[3][2]
                    * (2 * IMvcs[1][0] * m_Ur[69] * m_powerOfQ[0]
                            - 2 * IMvcs[1][1] * m_Ur[83] * m_powerOfQ[1]
                            + 2 * IMvcs[1][2] * m_Ur[96] * m_powerOfQ[2])
            + Jem[1][2]
                    * (-2 * IMvcs[3][0] * m_Ur[69] * m_powerOfQ[0]
                            + 2 * IMvcs[3][1] * m_Ur[83] * m_powerOfQ[1]
                            - 2 * IMvcs[3][2] * m_Ur[96] * m_powerOfQ[2]);
    SigmaIPolY[1] = Jem[2][1]
            * (2 * IMvcs[0][0] * m_Ur[2] * m_powerOfQ[0]
                    + 2 * IMvcs[0][1] * m_Ur[24] * m_powerOfQ[1]
                    + 2 * IMvcs[0][2] * m_Ur[1] * m_powerOfQ[2])
            + Jem[0][1]
                    * (-2 * IMvcs[2][0] * m_Ur[2] * m_powerOfQ[0]
                            - 2 * IMvcs[2][1] * m_Ur[24] * m_powerOfQ[1]
                            - 2 * IMvcs[2][2] * m_Ur[1] * m_powerOfQ[2])
            + Jem[3][2]
                    * (2 * IMvcs[1][0] * m_Ur[51] * m_powerOfQ[0]
                            + 2 * IMvcs[1][1] * m_Ur[83] * m_powerOfQ[1]
                            - 2 * IMvcs[1][2] * m_Ur[16] * m_powerOfQ[2])
            + Jem[1][2]
                    * (-2 * IMvcs[3][0] * m_Ur[51] * m_powerOfQ[0]
                            - 2 * IMvcs[3][1] * m_Ur[83] * m_powerOfQ[1]
                            + 2 * IMvcs[3][2] * m_Ur[16] * m_powerOfQ[2])
            + Jem[2][2]
                    * (2 * IMvcs[0][0] * m_Ur[10] * m_powerOfQ[0]
                            + 2 * IMvcs[0][1] * m_Ur[29] * m_powerOfQ[1]
                            + 2 * IMvcs[0][2] * m_Ur[9] * m_powerOfQ[2])
            + Jem[0][2]
                    * (-2 * IMvcs[2][0] * m_Ur[10] * m_powerOfQ[0]
                            - 2 * IMvcs[2][1] * m_Ur[29] * m_powerOfQ[1]
                            - 2 * IMvcs[2][2] * m_Ur[9] * m_powerOfQ[2]);
    SigmaIPolY[2] = Jem[2][1]
            * (2 * IMvcs[0][0] * m_Ur[3] * m_powerOfQ[1]
                    + 2 * IMvcs[0][2] * m_Ur[40] * m_powerOfQ[1]
                    - 2 * IMvcs[0][1] * m_Ur[26] * m_powerOfQ[2])
            + Jem[0][1]
                    * (-2 * IMvcs[2][0] * m_Ur[3] * m_powerOfQ[1]
                            - 2 * IMvcs[2][2] * m_Ur[40] * m_powerOfQ[1]
                            + 2 * IMvcs[2][1] * m_Ur[26] * m_powerOfQ[2])
            + Jem[2][2]
                    * (2 * IMvcs[0][0] * m_Ur[11] * m_powerOfQ[1]
                            - 2 * IMvcs[0][1] * m_Ur[31] * m_powerOfQ[2]
                            + 2 * IMvcs[0][2] * m_Ur[46] * m_powerOfQ[3])
            + Jem[0][2]
                    * (-2 * IMvcs[2][0] * m_Ur[11] * m_powerOfQ[1]
                            + 2 * IMvcs[2][1] * m_Ur[31] * m_powerOfQ[2]
                            - 2 * IMvcs[2][2] * m_Ur[46] * m_powerOfQ[3])
            + Jem[3][2]
                    * (2 * IMvcs[1][0] * m_Ur[70] * m_powerOfQ[1]
                            - 2 * IMvcs[1][1] * m_Ur[85] * m_powerOfQ[2]
                            + 2 * IMvcs[1][2] * m_Ur[97] * m_powerOfQ[3])
            + Jem[1][2]
                    * (-2 * IMvcs[3][0] * m_Ur[70] * m_powerOfQ[1]
                            + 2 * IMvcs[3][1] * m_Ur[85] * m_powerOfQ[2]
                            - 2 * IMvcs[3][2] * m_Ur[97] * m_powerOfQ[3]);
    SigmaIPolY[3] = Jem[2][1]
            * (2 * IMvcs[0][2] * m_Ur[42] * m_powerOfQ[1]
                    + 2 * IMvcs[0][0] * m_Ur[5] * m_powerOfQ[1]
                    - 2 * IMvcs[0][1] * m_Ur[27] * m_powerOfQ[2])
            + Jem[0][1]
                    * (-2 * IMvcs[2][2] * m_Ur[42] * m_powerOfQ[1]
                            - 2 * IMvcs[2][0] * m_Ur[5] * m_powerOfQ[1]
                            + 2 * IMvcs[2][1] * m_Ur[27] * m_powerOfQ[2])
            + Jem[2][2]
                    * (2 * IMvcs[0][0] * m_Ur[13] * m_powerOfQ[1]
                            - 2 * IMvcs[0][1] * m_Ur[32] * m_powerOfQ[2]
                            + 2 * IMvcs[0][2] * m_Ur[48] * m_powerOfQ[3])
            + Jem[0][2]
                    * (-2 * IMvcs[2][0] * m_Ur[13] * m_powerOfQ[1]
                            + 2 * IMvcs[2][1] * m_Ur[32] * m_powerOfQ[2]
                            - 2 * IMvcs[2][2] * m_Ur[48] * m_powerOfQ[3])
            + Jem[3][2]
                    * (2 * IMvcs[1][0] * m_Ur[71] * m_powerOfQ[1]
                            + 2 * IMvcs[1][1] * m_Ur[85] * m_powerOfQ[2]
                            + 2 * IMvcs[1][2] * m_Ur[98] * m_powerOfQ[3])
            + Jem[1][2]
                    * (-2 * IMvcs[3][0] * m_Ur[71] * m_powerOfQ[1]
                            - 2 * IMvcs[3][1] * m_Ur[85] * m_powerOfQ[2]
                            - 2 * IMvcs[3][2] * m_Ur[98] * m_powerOfQ[3]);
    SigmaIPolY[4] = Jem[2][1]
            * (2 * IMvcs[0][2] * m_Ur[2] * m_powerOfQ[0]
                    - 2 * IMvcs[0][1] * m_Ur[25] * m_powerOfQ[1]
                    + 2 * IMvcs[0][0] * m_Ur[1] * m_powerOfQ[2])
            + Jem[0][1]
                    * (-2 * IMvcs[2][2] * m_Ur[2] * m_powerOfQ[0]
                            + 2 * IMvcs[2][1] * m_Ur[25] * m_powerOfQ[1]
                            - 2 * IMvcs[2][0] * m_Ur[1] * m_powerOfQ[2])
            + Jem[2][2]
                    * (2 * IMvcs[0][2] * m_Ur[10] * m_powerOfQ[0]
                            + 2 * IMvcs[0][0] * m_Ur[9] * m_powerOfQ[2]
                            - 2 * IMvcs[0][1] * m_Ur[30] * m_powerOfQ[3])
            + Jem[0][2]
                    * (-2 * IMvcs[2][2] * m_Ur[10] * m_powerOfQ[0]
                            - 2 * IMvcs[2][0] * m_Ur[9] * m_powerOfQ[2]
                            + 2 * IMvcs[2][1] * m_Ur[30] * m_powerOfQ[3])
            + Jem[3][2]
                    * (2 * IMvcs[1][2] * m_Ur[51] * m_powerOfQ[0]
                            - 2 * IMvcs[1][0] * m_Ur[16] * m_powerOfQ[2]
                            - 2 * IMvcs[1][1] * m_Ur[84] * m_powerOfQ[3])
            + Jem[1][2]
                    * (-2 * IMvcs[3][2] * m_Ur[51] * m_powerOfQ[0]
                            + 2 * IMvcs[3][0] * m_Ur[16] * m_powerOfQ[2]
                            + 2 * IMvcs[3][1] * m_Ur[84] * m_powerOfQ[3]);
    SigmaIPolY[5] = Jem[2][1]
            * (2 * IMvcs[0][2] * m_Ur[41] * m_Q
                    + 2 * IMvcs[0][0] * m_Ur[4] * m_powerOfQ[1])
            + Jem[0][1]
                    * (-2 * IMvcs[2][2] * m_Ur[41] * m_Q
                            - 2 * IMvcs[2][0] * m_Ur[4] * m_powerOfQ[1])
            + Jem[2][2]
                    * (2 * IMvcs[0][2] * m_Ur[47] * m_Q
                            + 2 * IMvcs[0][0] * m_Ur[12] * m_powerOfQ[3])
            + Jem[0][2]
                    * (-2 * IMvcs[2][2] * m_Ur[47] * m_Q
                            - 2 * IMvcs[2][0] * m_Ur[12] * m_powerOfQ[3])
            + Jem[3][2]
                    * (2 * IMvcs[1][2] * m_Ur[53] * m_Q
                            - 2 * IMvcs[1][0] * m_Ur[18] * m_powerOfQ[3])
            + Jem[1][2]
                    * (-2 * IMvcs[3][2] * m_Ur[53] * m_Q
                            + 2 * IMvcs[3][0] * m_Ur[18] * m_powerOfQ[3]);
    SigmaIPolY[6] = Jem[2][1]
            * (-2 * RMvcs[0][1] * m_Ur[28] * m_powerOfQ[0]
                    + 2 * RMvcs[0][2] * m_Ur[44] * m_powerOfQ[1]
                    + 2 * RMvcs[0][0] * m_Ur[7] * m_powerOfQ[1])
            + Jem[0][1]
                    * (2 * RMvcs[2][1] * m_Ur[28] * m_powerOfQ[0]
                            - 2 * RMvcs[2][2] * m_Ur[44] * m_powerOfQ[1]
                            - 2 * RMvcs[2][0] * m_Ur[7] * m_powerOfQ[1])
            + Jem[2][2]
                    * (2 * RMvcs[0][0] * m_Ur[15] * m_powerOfQ[1]
                            - 2 * RMvcs[0][1] * m_Ur[33] * m_powerOfQ[2]
                            + 2 * RMvcs[0][2] * m_Ur[50] * m_powerOfQ[3])
            + Jem[0][2]
                    * (-2 * RMvcs[2][0] * m_Ur[15] * m_powerOfQ[1]
                            + 2 * RMvcs[2][1] * m_Ur[33] * m_powerOfQ[2]
                            - 2 * RMvcs[2][2] * m_Ur[50] * m_powerOfQ[3])
            + Jem[3][2]
                    * (2 * RMvcs[1][0] * m_Ur[72] * m_powerOfQ[1]
                            + 2 * RMvcs[1][1] * m_Ur[85] * m_powerOfQ[2]
                            + 2 * RMvcs[1][2] * m_Ur[99] * m_powerOfQ[3])
            + Jem[1][2]
                    * (-2 * RMvcs[3][0] * m_Ur[72] * m_powerOfQ[1]
                            - 2 * RMvcs[3][1] * m_Ur[85] * m_powerOfQ[2]
                            - 2 * RMvcs[3][2] * m_Ur[99] * m_powerOfQ[3]);
    SigmaIPolY[7] = Jem[2][1]
            * (2 * RMvcs[0][2] * m_Ur[43] * m_powerOfQ[0]
                    - 4 * RMvcs[0][1] * m_Ur[25] * m_powerOfQ[1]
                    + 2 * RMvcs[0][0] * m_Ur[6] * m_powerOfQ[2])
            + Jem[0][1]
                    * (-2 * RMvcs[2][2] * m_Ur[43] * m_powerOfQ[0]
                            + 4 * RMvcs[2][1] * m_Ur[25] * m_powerOfQ[1]
                            - 2 * RMvcs[2][0] * m_Ur[6] * m_powerOfQ[2])
            + Jem[2][2]
                    * (2 * RMvcs[0][2] * m_Ur[49] * m_powerOfQ[0]
                            + 2 * RMvcs[0][0] * m_Ur[14] * m_powerOfQ[2]
                            - 4 * RMvcs[0][1] * m_Ur[30] * m_powerOfQ[3])
            + Jem[0][2]
                    * (-2 * RMvcs[2][2] * m_Ur[49] * m_powerOfQ[0]
                            - 2 * RMvcs[2][0] * m_Ur[14] * m_powerOfQ[2]
                            + 4 * RMvcs[2][1] * m_Ur[30] * m_powerOfQ[3])
            + Jem[3][2]
                    * (2 * RMvcs[1][2] * m_Ur[56] * m_powerOfQ[0]
                            - 2 * RMvcs[1][0] * m_Ur[21] * m_powerOfQ[2]
                            - 4 * RMvcs[1][1] * m_Ur[84] * m_powerOfQ[3])
            + Jem[1][2]
                    * (-2 * RMvcs[3][2] * m_Ur[56] * m_powerOfQ[0]
                            + 2 * RMvcs[3][0] * m_Ur[21] * m_powerOfQ[2]
                            + 4 * RMvcs[3][1] * m_Ur[84] * m_powerOfQ[3]);
    SigmaIPolZ[0] = Jem[2][1]
            * (2 * IMvcs[1][0] * m_Ur[59] * m_powerOfQ[1]
                    + 2 * IMvcs[1][2] * m_Ur[86] * m_powerOfQ[1]
                    - 2 * IMvcs[1][1] * m_Ur[74] * m_powerOfQ[2])
            + Jem[0][1]
                    * (2 * IMvcs[3][0] * m_Ur[59] * m_powerOfQ[1]
                            + 2 * IMvcs[3][2] * m_Ur[86] * m_powerOfQ[1]
                            - 2 * IMvcs[3][1] * m_Ur[74] * m_powerOfQ[2])
            + Jem[3][2]
                    * (2 * IMvcs[0][0] * m_Ur[17] * m_powerOfQ[1]
                            - 2 * IMvcs[0][1] * m_Ur[35] * m_powerOfQ[2]
                            + 2 * IMvcs[0][2] * m_Ur[52] * m_powerOfQ[3])
            + Jem[1][2]
                    * (2 * IMvcs[2][0] * m_Ur[17] * m_powerOfQ[1]
                            - 2 * IMvcs[2][1] * m_Ur[35] * m_powerOfQ[2]
                            + 2 * IMvcs[2][2] * m_Ur[52] * m_powerOfQ[3])
            + Jem[2][2]
                    * (2 * IMvcs[1][0] * m_Ur[64] * m_powerOfQ[1]
                            - 2 * IMvcs[1][1] * m_Ur[79] * m_powerOfQ[2]
                            + 2 * IMvcs[1][2] * m_Ur[91] * m_powerOfQ[3])
            + Jem[0][2]
                    * (2 * IMvcs[3][0] * m_Ur[64] * m_powerOfQ[1]
                            - 2 * IMvcs[3][1] * m_Ur[79] * m_powerOfQ[2]
                            + 2 * IMvcs[3][2] * m_Ur[91] * m_powerOfQ[3]);
    SigmaIPolZ[1] = Jem[2][1]
            * (2 * IMvcs[1][0] * m_Ur[60] * m_powerOfQ[1]
                    + 2 * IMvcs[1][2] * m_Ur[87] * m_powerOfQ[1]
                    - 2 * IMvcs[1][1] * m_Ur[75] * m_powerOfQ[2])
            + Jem[0][1]
                    * (2 * IMvcs[3][0] * m_Ur[60] * m_powerOfQ[1]
                            + 2 * IMvcs[3][2] * m_Ur[87] * m_powerOfQ[1]
                            - 2 * IMvcs[3][1] * m_Ur[75] * m_powerOfQ[2])
            + Jem[3][2]
                    * (2 * IMvcs[0][0] * m_Ur[19] * m_powerOfQ[1]
                            - 2 * IMvcs[0][1] * m_Ur[36] * m_powerOfQ[2]
                            + 2 * IMvcs[0][2] * m_Ur[54] * m_powerOfQ[3])
            + Jem[1][2]
                    * (2 * IMvcs[2][0] * m_Ur[19] * m_powerOfQ[1]
                            - 2 * IMvcs[2][1] * m_Ur[36] * m_powerOfQ[2]
                            + 2 * IMvcs[2][2] * m_Ur[54] * m_powerOfQ[3])
            + Jem[2][2]
                    * (2 * IMvcs[1][0] * m_Ur[65] * m_powerOfQ[1]
                            - 2 * IMvcs[1][1] * m_Ur[80] * m_powerOfQ[2]
                            + 2 * IMvcs[1][2] * m_Ur[92] * m_powerOfQ[3])
            + Jem[0][2]
                    * (2 * IMvcs[3][0] * m_Ur[65] * m_powerOfQ[1]
                            - 2 * IMvcs[3][1] * m_Ur[80] * m_powerOfQ[2]
                            + 2 * IMvcs[3][2] * m_Ur[92] * m_powerOfQ[3]);
    SigmaIPolZ[2] = Jem[2][1]
            * (2 * IMvcs[1][2] * m_Ur[2] * m_powerOfQ[0]
                    - 2 * IMvcs[1][1] * m_Ur[73] * m_powerOfQ[1]
                    - 2 * IMvcs[1][0] * m_Ur[1] * m_powerOfQ[2])
            + Jem[0][1]
                    * (2 * IMvcs[3][2] * m_Ur[2] * m_powerOfQ[0]
                            - 2 * IMvcs[3][1] * m_Ur[73] * m_powerOfQ[1]
                            - 2 * IMvcs[3][0] * m_Ur[1] * m_powerOfQ[2])
            + Jem[3][2]
                    * (2 * IMvcs[0][2] * m_Ur[51] * m_powerOfQ[0]
                            + 2 * IMvcs[0][0] * m_Ur[16] * m_powerOfQ[2]
                            - 2 * IMvcs[0][1] * m_Ur[34] * m_powerOfQ[3])
            + Jem[1][2]
                    * (2 * IMvcs[2][2] * m_Ur[51] * m_powerOfQ[0]
                            + 2 * IMvcs[2][0] * m_Ur[16] * m_powerOfQ[2]
                            - 2 * IMvcs[2][1] * m_Ur[34] * m_powerOfQ[3])
            + Jem[2][2]
                    * (2 * IMvcs[1][2] * m_Ur[10] * m_powerOfQ[0]
                            - 2 * IMvcs[1][0] * m_Ur[9] * m_powerOfQ[2]
                            - 2 * IMvcs[1][1] * m_Ur[78] * m_powerOfQ[3])
            + Jem[0][2]
                    * (2 * IMvcs[3][2] * m_Ur[10] * m_powerOfQ[0]
                            - 2 * IMvcs[3][0] * m_Ur[9] * m_powerOfQ[2]
                            - 2 * IMvcs[3][1] * m_Ur[78] * m_powerOfQ[3]);
    SigmaIPolZ[3] = Jem[2][1]
            * (2 * IMvcs[1][2] * m_Ur[41] * m_Q
                    - 2 * IMvcs[1][0] * m_Ur[4] * m_powerOfQ[1])
            + Jem[0][1]
                    * (2 * IMvcs[3][2] * m_Ur[41] * m_Q
                            - 2 * IMvcs[3][0] * m_Ur[4] * m_powerOfQ[1])
            + Jem[2][2]
                    * (2 * IMvcs[1][2] * m_Ur[47] * m_Q
                            - 2 * IMvcs[1][0] * m_Ur[12] * m_powerOfQ[3])
            + Jem[0][2]
                    * (2 * IMvcs[3][2] * m_Ur[47] * m_Q
                            - 2 * IMvcs[3][0] * m_Ur[12] * m_powerOfQ[3])
            + Jem[3][2]
                    * (2 * IMvcs[0][2] * m_Ur[53] * m_Q
                            + 2 * IMvcs[0][0] * m_Ur[18] * m_powerOfQ[3])
            + Jem[1][2]
                    * (2 * IMvcs[2][2] * m_Ur[53] * m_Q
                            + 2 * IMvcs[2][0] * m_Ur[18] * m_powerOfQ[3]);
    SigmaIPolZ[4] = Jem[3][2]
            * (-2 * RMvcs[0][0] * m_Ur[20] * m_powerOfQ[0]
                    + 2 * RMvcs[0][1] * m_Ur[37] * m_powerOfQ[1]
                    - 2 * RMvcs[0][2] * m_Ur[55] * m_powerOfQ[2])
            + Jem[1][2]
                    * (-2 * RMvcs[2][0] * m_Ur[20] * m_powerOfQ[0]
                            + 2 * RMvcs[2][1] * m_Ur[37] * m_powerOfQ[1]
                            - 2 * RMvcs[2][2] * m_Ur[55] * m_powerOfQ[2])
            + Jem[2][1]
                    * (-2 * RMvcs[1][0] * m_Ur[61] * m_powerOfQ[0]
                            + 2 * RMvcs[1][1] * m_Ur[76] * m_powerOfQ[1]
                            - 2 * RMvcs[1][2] * m_Ur[88] * m_powerOfQ[2])
            + Jem[0][1]
                    * (-2 * RMvcs[3][0] * m_Ur[61] * m_powerOfQ[0]
                            + 2 * RMvcs[3][1] * m_Ur[76] * m_powerOfQ[1]
                            - 2 * RMvcs[3][2] * m_Ur[88] * m_powerOfQ[2])
            + Jem[2][2]
                    * (-2 * RMvcs[1][0] * m_Ur[66] * m_powerOfQ[0]
                            + 2 * RMvcs[1][1] * m_Ur[81] * m_powerOfQ[1]
                            - 2 * RMvcs[1][2] * m_Ur[93] * m_powerOfQ[2])
            + Jem[0][2]
                    * (-2 * RMvcs[3][0] * m_Ur[66] * m_powerOfQ[0]
                            + 2 * RMvcs[3][1] * m_Ur[81] * m_powerOfQ[1]
                            - 2 * RMvcs[3][2] * m_Ur[93] * m_powerOfQ[2]);
    SigmaIPolZ[5] = Jem[3][2]
            * (-2 * RMvcs[0][0] * m_Ur[22] * m_powerOfQ[0]
                    - 2 * RMvcs[0][1] * m_Ur[37] * m_powerOfQ[1]
                    - 2 * RMvcs[0][2] * m_Ur[57] * m_powerOfQ[2])
            + Jem[1][2]
                    * (-2 * RMvcs[2][0] * m_Ur[22] * m_powerOfQ[0]
                            - 2 * RMvcs[2][1] * m_Ur[37] * m_powerOfQ[1]
                            - 2 * RMvcs[2][2] * m_Ur[57] * m_powerOfQ[2])
            + Jem[2][1]
                    * (-2 * RMvcs[1][0] * m_Ur[62] * m_powerOfQ[0]
                            - 2 * RMvcs[1][1] * m_Ur[76] * m_powerOfQ[1]
                            - 2 * RMvcs[1][2] * m_Ur[89] * m_powerOfQ[2])
            + Jem[0][1]
                    * (-2 * RMvcs[3][0] * m_Ur[62] * m_powerOfQ[0]
                            - 2 * RMvcs[3][1] * m_Ur[76] * m_powerOfQ[1]
                            - 2 * RMvcs[3][2] * m_Ur[89] * m_powerOfQ[2])
            + Jem[2][2]
                    * (-2 * RMvcs[1][0] * m_Ur[67] * m_powerOfQ[0]
                            - 2 * RMvcs[1][1] * m_Ur[81] * m_powerOfQ[1]
                            - 2 * RMvcs[1][2] * m_Ur[94] * m_powerOfQ[2])
            + Jem[0][2]
                    * (-2 * RMvcs[3][0] * m_Ur[67] * m_powerOfQ[0]
                            - 2 * RMvcs[3][1] * m_Ur[81] * m_powerOfQ[1]
                            - 2 * RMvcs[3][2] * m_Ur[94] * m_powerOfQ[2]);
    SigmaIPolZ[6] = Jem[2][1]
            * (2 * RMvcs[1][1] * m_Ur[77] * m_powerOfQ[0]
                    - 2 * RMvcs[1][0] * m_Ur[63] * m_powerOfQ[1]
                    - 2 * RMvcs[1][2] * m_Ur[90] * m_powerOfQ[1])
            + Jem[0][1]
                    * (2 * RMvcs[3][1] * m_Ur[77] * m_powerOfQ[0]
                            - 2 * RMvcs[3][0] * m_Ur[63] * m_powerOfQ[1]
                            - 2 * RMvcs[3][2] * m_Ur[90] * m_powerOfQ[1])
            + Jem[3][2]
                    * (-2 * RMvcs[0][0] * m_Ur[23] * m_powerOfQ[1]
                            + 2 * RMvcs[0][1] * m_Ur[38] * m_powerOfQ[2]
                            - 2 * RMvcs[0][2] * m_Ur[58] * m_powerOfQ[3])
            + Jem[1][2]
                    * (-2 * RMvcs[2][0] * m_Ur[23] * m_powerOfQ[1]
                            + 2 * RMvcs[2][1] * m_Ur[38] * m_powerOfQ[2]
                            - 2 * RMvcs[2][2] * m_Ur[58] * m_powerOfQ[3])
            + Jem[2][2]
                    * (-2 * RMvcs[1][0] * m_Ur[68] * m_powerOfQ[1]
                            + 2 * RMvcs[1][1] * m_Ur[82] * m_powerOfQ[2]
                            - 2 * RMvcs[1][2] * m_Ur[95] * m_powerOfQ[3])
            + Jem[0][2]
                    * (-2 * RMvcs[3][0] * m_Ur[68] * m_powerOfQ[1]
                            + 2 * RMvcs[3][1] * m_Ur[82] * m_powerOfQ[2]
                            - 2 * RMvcs[3][2] * m_Ur[95] * m_powerOfQ[3]);
    SigmaIPolZ[7] = Jem[2][1]
            * (-2 * RMvcs[1][2] * m_Ur[43] * m_powerOfQ[0]
                    + 4 * RMvcs[1][1] * m_Ur[73] * m_powerOfQ[1]
                    + 2 * RMvcs[1][0] * m_Ur[6] * m_powerOfQ[2])
            + Jem[0][1]
                    * (-2 * RMvcs[3][2] * m_Ur[43] * m_powerOfQ[0]
                            + 4 * RMvcs[3][1] * m_Ur[73] * m_powerOfQ[1]
                            + 2 * RMvcs[3][0] * m_Ur[6] * m_powerOfQ[2])
            + Jem[3][2]
                    * (-2 * RMvcs[0][2] * m_Ur[56] * m_powerOfQ[0]
                            - 2 * RMvcs[0][0] * m_Ur[21] * m_powerOfQ[2]
                            + 4 * RMvcs[0][1] * m_Ur[34] * m_powerOfQ[3])
            + Jem[1][2]
                    * (-2 * RMvcs[2][2] * m_Ur[56] * m_powerOfQ[0]
                            - 2 * RMvcs[2][0] * m_Ur[21] * m_powerOfQ[2]
                            + 4 * RMvcs[2][1] * m_Ur[34] * m_powerOfQ[3])
            + Jem[2][2]
                    * (-2 * RMvcs[1][2] * m_Ur[49] * m_powerOfQ[0]
                            + 2 * RMvcs[1][0] * m_Ur[14] * m_powerOfQ[2]
                            + 4 * RMvcs[1][1] * m_Ur[78] * m_powerOfQ[3])
            + Jem[0][2]
                    * (-2 * RMvcs[3][2] * m_Ur[49] * m_powerOfQ[0]
                            + 2 * RMvcs[3][0] * m_Ur[14] * m_powerOfQ[2]
                            + 4 * RMvcs[3][1] * m_Ur[78] * m_powerOfQ[3]);

//	} // end if InitExactVCSAndInterfCrossSections

} // end MakeExactVCSAndInterfCrossSections

/*---------------------------- Function DdirectDcrossed(phi) ---------------------------*
 | Computes the denominator of the Bethe Heitler cross section.                         |
 | Only depends on kinematics.                                                          |
 *--------------------------------------------------------------------------------------*/

double GV2008Model::DdirectDcrossed(double phi) {
    double DDDC;

    DDDC = pow(m_powerOfQ[0] + m_t, 2) / 4.
            - pow(
                    (m_powerOfQ[0] * (m_powerOfQ[0] + m_t * (-1 + 2 * m_xB))
                            * TMath::CosH(m_Omega))
                            / (2.
                                    * sqrt(
                                            m_powerOfQ[2]
                                                    + 4 * m_powerOfProtonMass[0]
                                                            * m_powerOfQ[0]
                                                            * pow(m_xB, 2)))
                            - m_Q * m_qpPerp * TMath::Cos(phi)
                                    * TMath::SinH(m_Omega), 2);

    return DDDC;
}

double GV2008Model::CrossSectionBH(double beamHelicity, double beamCharge,
        Vector3D targetPolarization) {
    return SqrAmplBH(beamHelicity, beamCharge, targetPolarization)
            * m_phaseSpace;
}

double GV2008Model::CrossSectionVCS(double beamHelicity, double beamCharge,
        Vector3D targetPolarization) {
    return SqrAmplVCS(beamHelicity, beamCharge, targetPolarization)
            * m_phaseSpace;
}

double GV2008Model::CrossSectionInterf(double beamHelicity, double beamCharge,
        Vector3D targetPolarization) {
    return SqrAmplInterf(beamHelicity, beamCharge, targetPolarization)
            * m_phaseSpace;
}
