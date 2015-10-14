/*
 * BMJ2012Model.h
 *
 *  Created on: Oct 7, 2015
 *      Author: debian
 */

#ifndef BMJ2012MODEL_H_
#define BMJ2012MODEL_H_

#include <string>
#include <vector>

#include "../DVCSModule.h"

/**
 * @file BMJ2012Model.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date 07-10-2015
 * @version 1.0
 *
 * @class BMJ2012Model
 */

class BMJ2012Model: public DVCSModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    BMJ2012Model(const std::string &className);
    virtual ~BMJ2012Model();

    virtual BMJ2012Model* clone() const;

    // Cross sections
    virtual double CrossSectionBH(double beamHelicity, double beamCharge,
            Vector3D targetPolarization); ///< Bethe Heitler cross section

    virtual double CrossSectionVCS(double beamHelicity, double beamCharge,
            Vector3D targetPolarization); ///< Virtual Compton Scattering cross section

    virtual double CrossSectionInterf(double beamHelicity, double beamCharge,
            Vector3D targetPolarization); ///< Interference cross section

protected:
    /**
     * Copy constructor
     *
     * Use by the factory
     *
     * @param other
     */
    BMJ2012Model(const BMJ2012Model& other);

    virtual void initModule();
    virtual void initModule(double beamHelicity, double beamCharge,
            Vector3D targetPolarization);
    virtual void isModuleWellConfigured();

private:

    // The vectors here are actually static arrays, DO NOT PUSH BACK OR POP BACK.
    // Use them as arrays! Size fixed in the constructor.

    double m_phi1BMK; ///< Angle small phi of BMK (between electron and outgoing proton)
    double m_phi2BMK; ///< Angle small phi of BMK (between target polarization and outgoing proton)
    double m_PhiBMK; ///< Angle capital Phi of BMK (between electron and target polarization)
    double m_theta; ///< Polarization angle of target
    double m_Lambda; ///< Longitudinal polarization of target
    double m_lambda; ///< Lepton helicity

    double m_xB2; ///< square of xB
    std::vector<double> m_Q; ///< square root of virtuality Q2
                             ///< m_Q[0] = Q, m_Q[1] = Q^2, etc...
    std::vector<double> m_M; ///< Proton mass
                             ///< m_M[0] = M, m_M[1] = M^2, etc...
    std::vector<double> m_y; ///< Lepton energy fraction
                             ///< m_y[0] = y, m_y[1] = y^2, etc...
    std::vector<double> m_epsilon; ///<
                                   ///< m_epsilon[0] = epsilon, m_epsilon[1] = epsilon^2, etc...
    double m_epsroot; ///< sqrt(1+epsilon^2)
    std::vector<double> m_K; ///< Kinematical factor K
                             ///< m_K[0] = K, m_K[1] = K^2, etc...
    std::vector<double> m_Delta2; ///< Mandelstam variable t
                                  ///< m_Delta2[0] = t, m_Delta2[1] = Delta^4 = t^2, etc...
    double m_Delta2_min; ///< minimum of t

    /// Bethe Heitler coeffs (BMK2002)
    /** Fourier coeffs of the BH squared amplitude
     *  1st index: 0=unp, 1=LP, 2=TP
     *  2nd index: c0, c1, c2
     */
    std::vector<std::vector<double> > m_cBH;

    double m_s1BHTP; ///< BH Fourier coeff s1 TP

    double m_P1, m_P2; ///< Lepton propagators

    void computeFourierCoeffsBH();

    void defineAngles(Vector3D targetPolarization); ///< Define the BMK angles

    double SqrAmplBH(double beamHelicity, double beamCharge,
            Vector3D targetPolarization); ///< Returns the squared amplitude of Bethe Heitler process
    double SqrAmplVCS(double beamHelicity, double beamCharge,
            Vector3D targetPolarization); ///< Returns the squared amplitude of VCS process
    double SqrAmplInterf(double beamHelicity, double beamCharge,
            Vector3D targetPolarization); ///< Returns the interference term of the squared amplitude
};

#endif /* BMJ2012MODEL_H_ */
