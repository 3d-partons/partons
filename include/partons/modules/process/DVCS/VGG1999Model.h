#ifndef VGG_1999_MODEL_H
#define VGG_1999_MODEL_H

#include <NumA/linear_algebra/matrix_complex/MatrixComplex4D.h>
#include <NumA/linear_algebra/vector_complex/VectorComplex3D.h>
#include <NumA/linear_algebra/vector_complex/VectorComplex4D.h>
#include <complex>
#include <string>

#include "../DVCSModule.h"

class VGG1999Model: public DVCSModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    VGG1999Model(const std::string &className); ///< Constructor
    virtual ~VGG1999Model();    ///< Destructor

    virtual VGG1999Model* clone() const;    ///< Clone

    virtual double CrossSectionBH(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization); ///< Bethe Heitler cross section

    virtual double CrossSectionVCS(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization); ///< Virtual Compton Scattering cross section

    virtual double CrossSectionInterf(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization); ///< Interference cross section

protected:

    VGG1999Model(const VGG1999Model& other); ///< Copy constructor used by the Factory
    virtual void initModule();  ///< Init module
    virtual void initModule(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);  ///< Init module
    virtual void isModuleWellConfigured();  ///< Check if it is well configured

private:

    //TYPES

    typedef NumA::VectorComplex3D momentum3;
    typedef NumA::VectorComplex4D momentum4;
    typedef NumA::VectorComplex4D spinor;
    typedef NumA::MatrixComplex4D spinorMatrix;

    enum SpinorOrientation {
        UNDEFINED = 0, x = 1, y = 2, z = 3
    };

    //INTERNAL VARIABLES

    double m_target_polarization;   ///< Target polarization
    double m_target_polarizationL; ///< Target polarization longitudinal component
    double m_target_polarizationT; ///< Target polarization transverse component
    double m_target_polarizationT_x; ///< Target polarization transverse x component
    double m_target_polarizationT_y; ///< Target polarization transverse y component
    SpinorOrientation m_proton_spinor_orientation; ///< Orientation of target spinor

    double m_nu;    ///< s
    double m_y;     ///< y
    double m_s;     ///< s
    double m_xi;    ///< xi

    double k_in_mom;    ///< momentum of incomming lepton
    double k_out_mom;   ///< momentum of outgoing lepton
    double q_in_mom;    ///< momentum of incoming photon
    double q_out_mom;   ///< momentum of outgoing photon
    double k_in_en;     ///< energy of incomming lepton
    double k_out_en;    ///< energy of outgoing lepton
    double q_in_en;     ///< energy of incoming photon
    double q_out_en;    ///< energy of outgoing photon
    double p_in_en;     ///< energy of incomming proton
    double p_out_en;    ///< energy of outgoing proton

    momentum3 k_in;                 ///< momentum vector of incoming lepton
    momentum3 k_out;                ///< momentum vector of outgoing lepton
    momentum3 q_in;                 ///< momentum vector of incoming photon
    momentum3 q_out;                ///< momentum vector of outgoing photon
    momentum3 p_in;                 ///< momentum vector of incoming proton
    momentum3 p_out;                ///< momentum vector of outgoing proton

    momentum4 k4_in;                ///< four vector of incomming lepton
    momentum4 k4_out;               ///< four vector of outgoing lepton
    momentum4 q4_in;                ///< four vector of incomming photon
    momentum4 q4_out;               ///< four vector of outgoing photon
    momentum4 p4_in;                ///< four vector of incomming proton
    momentum4 p4_out;               ///< four vector of outgoing proton

    double an_th_q_out;             ///< polar angle of outgoing photon
    double an_phi_q_out;            ///< azimuthal angle of outgoing photon

    void fillInternalVariables(NumA::Vector3D targetPolarization); ///< Fill internal variables

    //ALGEBRA

    NumA::VectorComplex4D V4ini(std::complex<double> a,
            NumA::VectorComplex3D b); ///< Initialize 4vector from number and 3vector
    NumA::VectorComplex4D spinor_pos(NumA::VectorComplex3D p, double mass,
            double sproj); ///< The positive-energy Dirac spinor
    NumA::MatrixComplex4D dirac_gamma(int mu); ///< Contravariant Dirac matrices as 4 by 4 matrices in spinor space  (convention Bjorken and Drell 1964)
    NumA::MatrixComplex4D dirac_gamma5(); ///< gamma5 matrix
    double metric(int mu, int nu); ///< Metric signature
    NumA::VectorComplex3D pauli(double row, double col); ///< Elements of three Pauli matrices for given index element (up/left: ++, u/r:+-, d/l: -+, d/r:--)
    NumA::VectorComplex4D spinor_adj(NumA::VectorComplex4D a); ///< adjoint Dirac spinor
    std::complex<double> spinleft_mat_spinright(NumA::VectorComplex4D l,
            NumA::MatrixComplex4D m, NumA::VectorComplex4D r); ///< Spinor|gamma|spinor product
    NumA::MatrixComplex4D fvec_slash(NumA::VectorComplex4D a); ///< Contraction of a four-vector a with the dirac gamma matrices
    std::complex<double> V4mul(NumA::VectorComplex4D a,
            NumA::VectorComplex4D b); ///< Contraction of two four-vectors
    std::complex<double> Cinv(std::complex<double> z); ///< Inverse of a complex number
    NumA::VectorComplex4D spinor_hel(momentum3 p, double hel); ///< Helicity spinor for a particle with momentum, mass 0 and helicity +/-
    double polar_angle(NumA::VectorComplex3D t); ///< Polar angle
    double azimut_angle(NumA::VectorComplex3D t); ///< Azimuthal angle
    NumA::MatrixComplex4D dirac_sigma(int mu, int nu); ///< Contravariant tensor sigma in spinor space (convention Bjorken and Drell 1964)
    NumA::MatrixComplex4D unit_spinor_mat(); ///< Unit spinor matrix
    NumA::VectorComplex3D Vpolar(double magn, double th, double phi); ///< Vector set via magnitude, polar and azimuthal angles
    double antisymm_epsilon(int ka, int la, int mu, int nu); ///< Epsilon

    //PHYSICS

    double CrossSectionMechanism(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization, int mechanism); ///< Calculate cross section with given polarisations
    double v_compton_doublepol_cross_inv(double leptcharge, int mechanism,
            double el_hel, double sp_in); ///< Calculate cross section with given helicities
    NumA::MatrixComplex4D gamma_NN_vertex(int mu, NumA::VectorComplex4D q); ///< Gamma_NN electromagnetic vertex
    std::complex<double> AmplitudeBH(int mu, double el_hel, double sp_in,
            double sp_out); ///< Amplitude for BH process
    std::complex<double> AmplitudeDVCS(int mu, double el_hel, double sp_in,
            double sp_out); ///< Amplitude for DVCS process
    std::complex<double> J_DVCS(int mu, int nu, double sp_in, double sp_out); ///< squared amplitude for the reaction
    std::complex<double> v_compton_t_matrix_hel(double leptcharge,
            int mechanism, int mu, double el_hel, double sp_in, double sp_out); ///< Matrix element for the reaction
    double v_compton_doublepol_sqrampl(double leptcharge, int mechanism,
            double el_hel, double sp_in); ///< squared amplitude for the reaction

    //OTHERS
    double nbToGeVm2conversion(double nb);

};

#endif /* VGG_1999_MODEL_H */
