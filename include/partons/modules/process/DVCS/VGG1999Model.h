#ifndef VGG_1999_MODEL_H
#define VGG_1999_MODEL_H

#include <NumA/linear_algebra/matrix_complex/MatrixComplex4D.h>
#include <NumA/linear_algebra/vector_complex/VectorComplex3D.h>
#include <NumA/linear_algebra/vector_complex/VectorComplex4D.h>
#include <complex>
#include <string>

#include "../DVCSModule.h"

/**
 * VGG process model for DVCS
 *
 * For the reference see:
 * - Prog. Part. Nucl. Phys. 47, 401 (2001)
 * - Phys. Rev. Lett. 80 5064 (1998).
 * - Phys. Rev. D 60, 094017 (1999).
 * - Phys. Rev. D 72, 054013 (2005).
 *
 * Module based on the original code received from M. Guidal as a private communication
 */
class VGG1999Model: public DVCSModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /** Constructor
     @param className Name of this class
     */
    VGG1999Model(const std::string &className);

    virtual ~VGG1999Model();
    virtual VGG1999Model* clone() const;
    virtual double CrossSectionBH(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);
    virtual double CrossSectionVCS(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);
    virtual double CrossSectionInterf(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);

protected:

    /** Copy constructor
     @param other Object to be copied
     */
    VGG1999Model(const VGG1999Model& other);

    virtual void initModule();
    virtual void initModule(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);
    virtual void isModuleWellConfigured();

private:

    //TYPES

    typedef NumA::VectorComplex3D momentum3;
    typedef NumA::VectorComplex4D momentum4;
    typedef NumA::VectorComplex4D spinor;
    typedef NumA::MatrixComplex4D spinorMatrix;

    /** Orientation of spinor
     */
    enum SpinorOrientation {
        UNDEFINED = 0, x = 1, y = 2, z = 3
    };

    //INTERNAL VARIABLES
    double m_target_polarization; ///< Target polarization
    double m_target_polarizationL; ///< Longitudinal component of target polarization
    double m_target_polarizationT; ///< Transverse component of target polarization
    double m_target_polarizationT_x; ///< Transverse x component of target polarization
    double m_target_polarizationT_y; ///< Transverse y component of target polarization
    SpinorOrientation m_proton_spinor_orientation; ///< Orientation of target spinor

    double m_nu; ///< Kinematic variable nu
    double m_y; ///< Kinematic variable y
    double m_s; ///< Kinematic variable s
    double m_xi; ///< Kinematic variable xi

    double k_in_mom; ///< Momentum of incoming lepton
    double k_out_mom; ///< Momentum of outgoing lepton
    double q_in_mom; ///< Momentum of incoming photon
    double q_out_mom; ///< Momentum of outgoing photon
    double k_in_en; ///< Energy of incoming lepton
    double k_out_en; ///< Energy of outgoing lepton
    double q_in_en; ///< Energy of incoming photon
    double q_out_en; ///< Energy of outgoing photon
    double p_in_en; ///< Energy of incoming proton
    double p_out_en; ///< Energy of outgoing proton

    momentum3 k_in; ///< Momentum vector of incoming lepton
    momentum3 k_out; ///< Momentum vector of outgoing lepton
    momentum3 q_in; ///< Momentum vector of incoming photon
    momentum3 q_out; ///< Momentum vector of outgoing photon
    momentum3 p_in; ///< Momentum vector of incoming proton
    momentum3 p_out; ///< Momentum vector of outgoing proton

    momentum4 k4_in; ///< Four vector of incomming lepton
    momentum4 k4_out; ///< Four vector of outgoing lepton
    momentum4 q4_in; ///< Four vector of incomming photon
    momentum4 q4_out; ///< Four vector of outgoing photon
    momentum4 p4_in; ///< Four vector of incomming proton
    momentum4 p4_out; ///< Four vector of outgoing proton

    double an_th_q_out; ///< Polar angle of outgoing photon
    double an_phi_q_out; ///< Azimuthal angle of outgoing photon

    /** Used to fill internal variables
     */
    void fillInternalVariables(NumA::Vector3D targetPolarization);

    //ALGEBRA

    /** Initialize four vector from energy and momentum
     * @param a Input energy
     * @param b Input momentum vector
     */
    NumA::VectorComplex4D V4ini(std::complex<double> a,
            NumA::VectorComplex3D b);

    /** Positive-energy Dirac spinor
     * @param p Momentum vector
     * @param mass Mass
     * @param sproj Spin projection
     */
    NumA::VectorComplex4D spinor_pos(NumA::VectorComplex3D p, double mass,
            double sproj);

    /** Contravariant Dirac matrices as 4 by 4 matrices in spinor space (convention Bjorken and Drell 1964)
     * @param mu Dimension index
     */
    NumA::MatrixComplex4D dirac_gamma(int mu);

    /** Gamma5 matrix
     */
    NumA::MatrixComplex4D dirac_gamma5();

    /** Metric signature
     * @param mu, nu Dimension indices
     */
    double metric(int mu, int nu);

    /** Elements of three Pauli matrices for given index element (up/left: ++, u/r:+-, d/l: -+, d/r:--)
     * @param row Row of matrix
     * @param col Column of matrix
     */
    NumA::VectorComplex3D pauli(double row, double col);

    /** Dirac adjoint
     * @param a Input dirac spinor
     */
    NumA::VectorComplex4D spinor_adj(NumA::VectorComplex4D a);

    /** Spinor|gamma|spinor product
     * @param l Spinor left
     * @param m Gamma matrix
     * @param r Spinor right
     */
    std::complex<double> spinleft_mat_spinright(NumA::VectorComplex4D l,
            NumA::MatrixComplex4D m, NumA::VectorComplex4D r);

    /** Contraction of four vector with gamma matrix
     * @param a Input four vector
     */
    NumA::MatrixComplex4D fvec_slash(NumA::VectorComplex4D a);

    /** Contraction of two four vectors
     * @param a, b Input four vectors
     */
    std::complex<double> V4mul(NumA::VectorComplex4D a,
            NumA::VectorComplex4D b);

    /** Complex number inverse
     * @param z Complex number to be inverted
     */
    std::complex<double> Cinv(std::complex<double> z);

    /** Helicity spinor for a particle with momentum, mass 0 and helicity +/-
     * @param p Input momentum
     * @param hel Input helicity
     */
    NumA::VectorComplex4D spinor_hel(momentum3 p, double hel);

    /** Polar angle for given vector
     * @param t Input vector
     */
    double polar_angle(NumA::VectorComplex3D t);

    /** Azimuthal angle for given vector
     * @param t Input vector
     */
    double azimut_angle(NumA::VectorComplex3D t);

    /** Contravariant tensor sigma in spinor space (convention Bjorken and Drell 1964)
     * @param mu, nu Dimension indices
     */
    NumA::MatrixComplex4D dirac_sigma(int mu, int nu);

    /** Unit spinor matrix
     */
    NumA::MatrixComplex4D unit_spinor_mat();

    /** Vector set via magnitude, polar and azimuthal angles
     * @param magn Magnitude
     * @param th Polar angle
     * @param phi Azimuthal angle
     */
    NumA::VectorComplex3D Vpolar(double magn, double th, double phi);

    /** Antisymmetric epsilon
     * @param ka, la, mu, nu Indices
     */
    double antisymm_epsilon(int ka, int la, int mu, int nu);

    //PHYSICS

    /** Calculate cross section
     * @param beamHelicity Beam helicity
     * @param beamCharge Beam charge
     * @param targetPolarization Target polarization
     * @param mechanism Mechanism (0 - BH, 1 - DVCS, 2 - INT)
     */
    double CrossSectionMechanism(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization, int mechanism);

    /** Calculate cross section for specific helicities
     * @param leptcharge Beam charge
     * @param mechanism Mechanism (0 - BH, 1 - DVCS, 2 - INT)
     * @param el_hel Beam helicity
     * @param sp_in Target helicity
     */
    double v_compton_doublepol_cross_inv(double leptcharge, int mechanism,
            double el_hel, double sp_in);

    /** Gamma_NN electromagnetic vertex
     * @param mu Dimension index
     * @param q Four momentum of virtual photon
     */
    NumA::MatrixComplex4D gamma_NN_vertex(int mu, NumA::VectorComplex4D q);

    /**  Amplitude for BH process
     * @param mu Dimension index
     * @param el_hel Beam helicity
     * @param sp_in Target helicity (incoming proton)
     * @param sp_out (outgoing proton)
     */
    std::complex<double> AmplitudeBH(int mu, double el_hel, double sp_in,
            double sp_out);

    /**  Amplitude for DVCS process
     * @param mu Dimension index
     * @param el_hel Beam helicity
     * @param sp_in Target helicity (incoming proton)
     * @param sp_out (outgoing proton)
     */
    std::complex<double> AmplitudeDVCS(int mu, double el_hel, double sp_in,
            double sp_out);

    /** Amplitude for the reaction
     * @param mu, nu Dimension indices
     * @param sp_in Target helicity (incoming proton)
     * @param sp_out (outgoing proton)
     */
    std::complex<double> J_DVCS(int mu, int nu, double sp_in, double sp_out);

    /** Matrix element for the reaction
     * @param leptcharge Beam charge
     * @param mechanism Mechanism (0 - BH, 1 - DVCS, 2 - INT)
     * @param mu Dimension index
     * @param el_hel Beam helicity
     * @param sp_in Target helicity (incoming proton)
     * @param sp_out (outgoing proton)
     */
    std::complex<double> v_compton_t_matrix_hel(double leptcharge,
            int mechanism, int mu, double el_hel, double sp_in, double sp_out);

    /** Squared amplitude for the reaction
     * @param leptcharge Beam charge
     * @param mechanism Mechanism (0 - BH, 1 - DVCS, 2 - INT)
     * @param el_hel Beam helicity
     * @param sp_in Target helicity
     */
    double v_compton_doublepol_sqrampl(double leptcharge, int mechanism,
            double el_hel, double sp_in);
};

#endif /* VGG_1999_MODEL_H */
