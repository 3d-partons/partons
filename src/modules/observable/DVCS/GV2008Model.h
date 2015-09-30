#ifndef GV_2008_MODEL
#define GV_2008_MODEL

/**
 * @file GV2008Model.h
 * @author Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date unknow
 * @version 1.0
 *
 * @class GV2008Model
 *
 * @brief 25-09-2015 (Bryan BERTHOU) : refactoring
 */

#include <string>
#include <vector>

#include "../../../utils/vector/Vector4D.h"
#include "../DVCSModule.h"

class GV2008Model: public DVCSModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    GV2008Model(const std::string &className);
    virtual ~GV2008Model();

    virtual GV2008Model* clone() const;

    // Cross sections
    virtual double CrossSectionBH(double beamHelicity, double beamCharge,
            Vector3D targetPolarization);
    // Bethe Heitler cross section

    virtual double CrossSectionVCS(double beamHelicity, double beamCharge,
            Vector3D targetPolarization);
    // Virtual Compton Scattering cross section

    virtual double CrossSectionInterf(double beamHelicity, double beamCharge,
            Vector3D targetPolarization);
    // Interference cross section

protected:
    /**
     * Copy constructor
     *
     * Use by the factory
     *
     * @param other
     */
    GV2008Model(const GV2008Model& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    // Store each power of Q
    // [0] = Q2
    // [1] = Q3
    // [2] = Q4
    // [3] = Q5
    std::vector<double> m_powerOfQ;

    // Store each power of M
    // [0] = M2
    // [1] = M4
    // [2] = M6
    // [3] = M8
    std::vector<double> m_powerOfProtonMass;

    // Variable used in the computation of Omega;
    double m_y;
    double m_xB2;

    double m_xBMin, m_xBMax;

    /*---------------------------------- Tests variables -----------------------------------*/

    // Printouts
    bool Validation;
    bool NoPrint;

    /*---------------------------------------- Kinematics ----------------------------------*/

    // Frame dependent scalars
    //double m_E; // Beam energy
    double m_thetag; // Angle between real and virtual photons
    double m_qpPerp; // Component (here x-axis) of the real photon 3-momentum orthogonal
                     // to the virtual photon trajectory (here z-axis) in the
                     // hadronic plane (here xz-plane)
    double m_Ur[100]; // Coefficients of the expansion of the interference cross section
                      // wrt (combinations of) helicity amplitudes
    double m_Omega; // (Fonction of) the linear polarization rate

    // Invariant scalars
    double m_s; // Mandelstam variable (square of the total incoming 4-momentum)
    double m_Q;  // Photon virtual mass i.e. square root of Q2

    // 4-vectors defined in the CM frame :
    Vector4D m_qCM; // Virtual photon (propagates along z-axis)
    Vector4D m_pCM; // Incoming proton (propagates along z-axis)
    Vector4D m_qpCM; // Real photon (defines hadronic plane xz)
    Vector4D m_ppCM; // Outgoing proton

    /*------------------------ (Combinations of) helicity amplitudes -----------------------*/

    double Jem[4][3]; // Helicity amplitudes of the interference process assuming the
                      // real photon has helicity +1.
    double RMvcs[4][3]; //  Real part of the helicity amplitudes of the VCS process
                        //assuming the real photon has helicity +1.
    double IMvcs[4][3]; //  Imaginary part of the helicity amplitudes of the VCS process
                        // assuming the real photon has helicity +1.

    /*------------------ Expansion of cross sections for harmonic analysis -----------------*/

                        // Bethe Heitler process
    double SigmaBHPol0[4]; // coefficients for the unpolarized cross section
    double SigmaBHPolX[2]; // coefficients for the x-polarized cross section
    double SigmaBHPolY; // coefficient for the y-polarized cross section
    double SigmaBHPolZ[2]; // coefficients for the z-polarized cross section

    // Virtual Compton Scattering process

    double SigmaVCSPol0[5]; // coefficients for the unpolarized cross section
    double SigmaVCSPolX[4]; // coefficients for the x-polarized cross section
    double SigmaVCSPolY[5]; // coefficients for the y-polarized cross section
    double SigmaVCSPolZ[4]; // coefficients for the z-polarized cross section

    // Interference

    double SigmaIPol0[8]; // coefficients for the unpolarized cross section
    double SigmaIPolX[8]; // coefficients for the x-polarized cross section
    double SigmaIPolY[8]; // coefficients for the x-polarized cross section
    double SigmaIPolZ[8]; // coefficients for the x-polarized cross section

    /*-------------------------------------- Methods ---------------------------------------*/

    // Initialisations
    void SetBeamEnergy(double EBeam);
    // Sets beam energy

    void MakeExactBHCrossSections();
    // Fills the Jem and SigmaBHPol. arrays

    void MakeExactVCSAndInterfCrossSections();
    // Fills the Ur, SigmaVCSPol. and SigmaIPol. arrays

    double DdirectDcrossed(double phi);
    // Denominator of the Bethe Heitler cross section

    double SqrAmplBH(double beamHelicity, double beamCharge,
            Vector3D targetPolarization);

    double SqrAmplVCSAndInterf(double beamHelicity, double beamCharge,
            Vector3D targetPolarization);
    double SqrAmplVCS(double beamHelicity, double beamCharge,
            Vector3D targetPolarization);
    double SqrAmplInterf(double beamHelicity, double beamCharge,
            Vector3D targetPolarization);

    void MakeVCSHelicityAmplitudes();

};

#endif /* GV_2008_MODEL */

