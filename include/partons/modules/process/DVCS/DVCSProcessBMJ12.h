#ifndef DVCS_PROCESS_BMJ12_H
#define DVCS_PROCESS_BMJ12_H

/**
 * @file DVCSProcessBMJ12.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date 07 October 2015
 * @version 1.0
 */

#include <complex>
#include <string>
#include <vector>

#include "../../../beans/gpd/GPDType.h"
#include "DVCSProcessModule.h"

namespace PARTONS {

/**
 * @class DVCSProcessBMJ12
 *
 * Module for the DVCS process using the Belitsky-Müller set of formulas.
 *
 * Code based on the published papers:
 * - arxiv:hep-ph/0112108 @cite Belitsky2001ns for the BH amplitude ;
 * - arxiv:1212.6674 @cite Belitsky2012ch for the DVCS amplitude and interference.
 */
class DVCSProcessBMJ12: public DVCSProcessModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    DVCSProcessBMJ12(const std::string &className);
    /**
     * Default destructor.
     */
    virtual ~DVCSProcessBMJ12();

    virtual DVCSProcessBMJ12* clone() const;

protected:
    /**
     * Copy constructor.
     *
     * Used by the factory.
     *
     * @param other
     */
    DVCSProcessBMJ12(const DVCSProcessBMJ12& other);

    virtual void initModule();
    virtual void initModule(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);
    virtual void isModuleWellConfigured();

    // Cross sections
    virtual double CrossSectionBH(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);

    virtual double CrossSectionVCS(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);

    virtual double CrossSectionInterf(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);

private:

    // The vectors here are actually static arrays, DO NOT PUSH BACK OR POP BACK.
    // Use them as arrays! Size fixed in the constructor.

    double m_phi1BMK; ///< Angle small phi of BMK (between electron and outgoing proton).
    double m_phi2BMK; ///< Angle small phi of BMK (between target polarization and outgoing proton).
    double m_PhiBMK; ///< Angle capital Phi of BMK (between electron and target polarization).
    double m_theta; ///< Polarization angle of target.
    double m_Lambda; ///< Longitudinal polarization of target.
    double m_lambda; ///< Lepton helicity.

    double m_xB2; ///< Square of xB.
    std::vector<double> m_Q; ///< Square root of virtuality Q2.
                             ///< m_Q[0] = Q, m_Q[1] = Q^2, etc...
    std::vector<double> m_xBtQ2;
    std::vector<double> m_M; ///< Proton mass.
                             ///< m_M[0] = M, m_M[1] = M^2, etc...
    std::vector<double> m_yBMJ; ///< Lepton energy fraction.
                                ///< m_y[0] = y, m_y[1] = y^2, etc...
    std::vector<double> m_epsilonBMJ; ///<
                                      ///< m_epsilon[0] = epsilon, m_epsilon[1] = epsilon^2, etc...
    std::vector<double> m_epsroot; ///< sqrt(1+epsilon^2)
    std::vector<double> m_K, m_Kt; ///< Kinematical factors K and K tilde.
                                   ///< m_K[0] = K, m_K[1] = K^2, etc...
    std::vector<double> m_Delta2; ///< Mandelstam variable t.
                                  ///< m_Delta2[0] = t, m_Delta2[1] = Delta^4 = t^2, etc...

    //@{
    double m_P1, m_P2; ///< Lepton propagators.
    //@}

    void defineAngles(const NumA::Vector3D &targetPolarization); ///< Defines the BMK angles. Conversion to the BMK convention.

    double m_F1, m_F2; ///< Dirac and Pauli form factors.
    void computeFormFactors(); ///< Compute F1 and F2 form factors.

    /* Bethe Heitler coeffs (BMK2002) */
    /** Fourier coeffs of the BH squared amplitude.
     *  1st index: [0]=unp, [1]=LP, [2]=TP. \n
     *  2nd index: [0]=c0, [1]=c1, [2]=c2.
     */
    std::vector<std::vector<double> > m_cBH;

    double m_s1BHTP; ///< BH Fourier coeff s1 TP.

    void computeFourierCoeffsBH(); ///< Computes c_BH and s_BH.

    /* VCS coeffs (BMJ2012) */
    /** Fourier coeffs (cosinus) of the VCS squared amplitude.
     *  1st index: [0]=unp, [1]=LP, [2]=TP. \n
     *  2nd index: [0]=c0, [1]=c1, [2]=c2.
     */
    std::vector<std::vector<double> > m_cVCS;
    /** Fourier coeffs (sinus) of the VCS squared amplitude.
     *  1st index: [0]=unp, [1]=LP, [2]=TP. \n
     *  2nd index: [0]=0., [1]=s1, [2]=s2.
     */
    std::vector<std::vector<double> > m_sVCS;

    void computeFourierCoeffsVCS(); ///< Computes c_VCS and s_VCS.

    /* Interference coeffs (BMJ2012) */
    /** Fourier coeffs (cosinus) of the Interference term.
     *  1st index: [0]=unp, [1]=LP, [2]=TP. \n
     *  2nd index: [0]=c0, [1]=c1, [2]=c2, [3]=c3.
     */
    std::vector<std::vector<double> > m_cI;
    /** Fourier coeffs (sinus) of the Interference term.
     *  1st index: [0]=unp, [1]=LP, [2]=TP. \n
     *  2nd index: [0]=0., [1]=s1, [2]=s2, [3]=s3.
     */
    std::vector<std::vector<double> > m_sI;
    //@{
    /** Angular coeffs @f$ C_ab(n) @f$, @f$ dC_ab(n) @f$, @f$ S_ab(n) @f$, @f$ dS_ab(n) @f$ ..
     * 1st index: [0]=C++, [1]=C-+, [2]=C0+. \n
     * 2nd index: [0]=not, [1]=V, [2]=A. \n
     * 3rd index: n
     */
    std::vector<std::vector<std::vector<double> > > m_C, m_S, m_dC, m_dS;
    //@}

    void computeAngularCoeffsInterf(); ///< Computes C_ab(n), S_ab(n), etc.
    void computeFourierCoeffsInterf(); ///< Computes c_I and s_I.

    /** Coefficients used for computing F+b and F0+.
     * 1st index: [0] = F++, [1] = F+-, [2] = F0+. \n
     * 2nd index: [0]: F coeff, [1]: FT coeff, [2]: FLT coeff.
     */
    std::vector<std::vector<double> > m_cF;

    /** Array that stores the CFFs F+b and F0+.
     * 1st index: [0] = H, [1] = E, [2] = Ht, [3] = Et. \n
     * 2nd index: [0] = F++, [1] = F+-, [2] = F0+.
     */
    std::vector<std::vector<std::complex<double> > > m_CFF;

    /** Method that gives F+b and F0+ already calculated.
     */
    std::complex<double> CFF(GPDType::Type F, int a, int b);

    void computeCFFs(); ///< Computes CFFS F+b and F0+.

    //@{
    /** Method that gives the VCS coefficient bilinear in the CFFs.
     *
     * @param S 0=unp, 1=LP, 2=TP+, 3=TP-
     */
    std::complex<double> C_VCS(unsigned int S, int a1, int b1, int a2, int b2);
    std::complex<double> C_VCS(unsigned int S, int a1, int b1, int a2, int b2,
            int a3, int b3);
    std::complex<double> C_VCS(unsigned int S, int a1, int b1, int a2, int b2,
            int a3, int b3, int a4, int b4);
    //@}

    /** Method that gives the Interference coefficient linear in the CFFs.
     *
     * @param S 0=unp, 1=LP, 2=TP+, 3=TP-.
     * @param VA can be "V" or "A" or "".
     */
    std::complex<double> C_I(unsigned int S, int a, int b,
            const std::string& VA);
    /** Method that gives the "effective" linear combinations of CFFs.
     * Corresponds to Eqs. (68-69) but multiplied by the coefficient @f$ C_{ab}(n) @f$
     * to avoid divisions by zero.
     * @param S 0=unp, 1=LP, 2=TP+, 3=TP-.
     * @param n
     * @param a
     * @param b
     * @return @f$ {\cal C}_{ab,S}(n|Fab) \times C_{ab}(n) @f$
     */
    std::complex<double> C_I(unsigned int S, unsigned int n, int a, int b);
    /** Method that gives the "effective" linear combinations of CFFs.
     * Corresponds to Eqs. (68-69) but multiplied by the coefficient @f$ S_{ab}(n) @f$
     * to avoid divisions by zero.
     * @param S 0=unp, 1=LP, 2=TP+, 3=TP-.
     * @param n
     * @param a
     * @param b
     * @return @f$ {\cal S}_{ab,S}(n|Fab) \times S_{ab}(n) @f$
     */
    std::complex<double> S_I(unsigned int S, unsigned int n, int a, int b);

    double SqrAmplBH(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization); ///< Returns the squared amplitude of Bethe Heitler process.
    double SqrAmplVCS(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization); ///< Returns the squared amplitude of VCS process.
    double SqrAmplInterf(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization); ///< Returns the interference term of the squared amplitude.
};

} /* namespace PARTONS */

#endif /* DVCS_PROCESS_BMJ12_H */
