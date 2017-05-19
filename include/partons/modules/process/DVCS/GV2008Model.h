#ifndef GV_2008_MODEL
#define GV_2008_MODEL

/**
 * @file GV2008Model.h
 * @author Hervé MOUTARDE (SPhN / CEA Saclay)
 * @version 1.0
 *
 * @date 25-09-2015 (Bryan BERTHOU) : refactoring
 */

#include <NumA/linear_algebra/vector/Vector4D.h>
#include <string>
#include <vector>

#include "../DVCSModule.h"

/**
 * @class GV2008Model
 *
 * Module for the DVCS process using the unpublished Guichon-Vanderhagen set of formulas.
 */
class GV2008Model: public DVCSModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    GV2008Model(const std::string &className);
    /**
     * Default destructor.
     */
    virtual ~GV2008Model();

    virtual GV2008Model* clone() const;

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
    virtual void initModule(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);
    virtual void isModuleWellConfigured();

    // Cross sections
    virtual double CrossSectionBH(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);
    // Bethe Heitler cross section

    virtual double CrossSectionVCS(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);
    // Virtual Compton Scattering cross section

    virtual double CrossSectionInterf(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);
    // Interference cross section

private:

    double m_phiGV; ///< Angle between hadronic and leptonic planes (opposite sign of the phi angle of Trento).

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
    double m_yGV;
    double m_xB2; ///< Square of @f$ x_B @f$.

    double m_xBMin, m_xBMax; // Should be removed. xBMin already in DVCSModule and xBMax is wrong.

    /*---------------------------------- Tests variables -----------------------------------*/

    // Printouts
    bool Validation;
    bool NoPrint;

    /*---------------------------------------- Kinematics ----------------------------------*/

    // Frame dependent scalars
    //double m_E; // Beam energy
    double m_thetag; ///< Angle between real and virtual photons.
    double m_qpPerp; ///< Component (here x-axis) of the real photon 3-momentum orthogonal
                     ///< to the virtual photon trajectory (here z-axis) in the
                     ///< hadronic plane (here xz-plane).
    double m_Ur[100]; ///< Coefficients of the expansion of the interference cross section
                      ///< wrt (combinations of) helicity amplitudes.
    double m_Omega; ///< (Function of) the linear polarization rate.

    // Invariant scalars
    double m_s; ///< Mandelstam variable (square of the total incoming 4-momentum).
    double m_Q;  ///< Photon virtual mass i.e.\ square root of Q2.

    // 4-vectors defined in the CM frame :
    NumA::Vector4D m_qCM; ///< Virtual photon (propagates along z-axis). 4-vector defined in the CM frame.
    NumA::Vector4D m_pCM; ///< Incoming proton (propagates along z-axis). 4-vector defined in the CM frame.
    NumA::Vector4D m_qpCM; ///< Real photon (defines hadronic plane xz). 4-vector defined in the CM frame.
    NumA::Vector4D m_ppCM; ///< Outgoing proton. 4-vector defined in the CM frame.

    /*------------------------ (Combinations of) helicity amplitudes -----------------------*/

    double Jem[4][3]; ///< Helicity amplitudes of the interference process assuming the
                      ///< real photon has helicity +1.
    double RMvcs[4][3]; ///<  Real part of the helicity amplitudes of the VCS process
                        ///< assuming the real photon has helicity +1.
    double IMvcs[4][3]; ///<  Imaginary part of the helicity amplitudes of the VCS process
                        ///< assuming the real photon has helicity +1.

    /*------------------ Expansion of cross sections for harmonic analysis -----------------*/

                        // Bethe Heitler process
    double SigmaBHPol0[4]; ///< Coefficients for the unpolarized cross section of the Bethe-Heitler process.
    double SigmaBHPolX[2]; ///< Coefficients for the x-polarized cross section of the Bethe-Heitler process.
    double SigmaBHPolY; ///< Coefficient for the y-polarized cross section of the Bethe-Heitler process.
    double SigmaBHPolZ[2]; ///< Coefficients for the z-polarized cross section of the Bethe-Heitler process.

    // Virtual Compton Scattering process

    double SigmaVCSPol0[5]; ///< Coefficients for the unpolarized cross section of the Virtual Compton Scattering process.
    double SigmaVCSPolX[4]; ///< Coefficients for the x-polarized cross section of the Virtual Compton Scattering process.
    double SigmaVCSPolY[5]; ///< Coefficients for the y-polarized cross section of the Virtual Compton Scattering process.
    double SigmaVCSPolZ[4]; ///< Coefficients for the z-polarized cross section of the Virtual Compton Scattering process.

    // Interference

    double SigmaIPol0[8]; ///< Coefficients for the unpolarized cross section (interference term).
    double SigmaIPolX[8]; ///< Coefficients for the x-polarized cross section (interference term).
    double SigmaIPolY[8]; ///< Coefficients for the x-polarized cross section (interference term).
    double SigmaIPolZ[8]; ///< Coefficients for the x-polarized cross section (interference term).

    /*-------------------------------------- Methods ---------------------------------------*/

    // Initialisations
    void MakeExactBHCrossSections(); ///< Fills the Jem and SigmaBHPol arrays.

    void MakeExactVCSAndInterfCrossSections(); ///< Fills the Ur, SigmaVCSPol and SigmaIPol arrays.

    double DdirectDcrossed(double phi); ///< Denominator of the Bethe Heitler cross section.

    double SqrAmplBH(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization); ///< Returns the squared amplitude of Bethe Heitler process.

    double SqrAmplVCSAndInterf(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization); ///< Sum of the squared amplitude of VCS process and interference term.
    double SqrAmplVCS(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization); ///< Returns the squared amplitude of VCS process.
    double SqrAmplInterf(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization); ///< Returns the interference term of the squared amplitude.

    void MakeVCSHelicityAmplitudes();

};

#endif /* GV_2008_MODEL */

