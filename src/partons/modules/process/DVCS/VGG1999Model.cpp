#include "../../../../../include/partons/modules/process/DVCS/VGG1999Model.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/linear_algebra/vector/Vector3D.h>
#include <cmath>
#include <cstdio>

#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"

const unsigned int VGG1999Model::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new VGG1999Model("VGG1999Model"));

VGG1999Model::VGG1999Model(const std::string &className) :
        DVCSModule(className) {

    m_nu = 0.;
    m_y = 0.;
    m_s = 0.;
    m_xi = 0.;

    k_in_mom = 0.;
    k_out_mom = 0.;
    q_in_mom = 0.;
    q_out_mom = 0.;
    k_in_en = 0.;
    k_out_en = 0.;
    q_in_en = 0.;
    q_out_en = 0.;
    p_in_en = 0.;
    p_out_en = 0.;

    k_in.Clear();
    k_out.Clear();
    q_in.Clear();
    q_out.Clear();
    p_in.Clear();
    p_out.Clear();

    k4_in.Clear();
    k4_out.Clear();
    q4_in.Clear();
    q4_out.Clear();
    p4_in.Clear();
    p4_out.Clear();

    an_th_q_out = 0.;
    an_phi_q_out = 0.;

    m_target_polarization = 0.;
    m_target_polarizationL = 0.;
    m_target_polarizationT = 0.;
    m_target_polarizationT_x = 0.;
    m_target_polarizationT_y = 0.;
    m_proton_spinor_orientation = UNDEFINED;
}

VGG1999Model::VGG1999Model(const VGG1999Model& other) :
        DVCSModule(other) {

    m_nu = other.m_nu;
    m_y = other.m_y;
    m_s = other.m_s;
    m_xi = other.m_xi;

    k_in_mom = other.k_in_mom;
    k_out_mom = other.k_out_mom;
    q_in_mom = other.q_in_mom;
    q_out_mom = other.q_out_mom;
    k_in_en = other.k_in_en;
    k_out_en = other.k_out_en;
    q_in_en = other.q_in_en;
    q_out_en = other.q_out_en;
    p_in_en = other.p_in_en;
    p_out_en = other.p_out_en;

    k_in = other.k_in;
    k_out = other.k_out;
    q_in = other.q_in;
    q_out = other.q_out;
    p_in = other.p_in;
    p_out = other.p_out;

    k4_in = other.k4_in;
    k4_out = other.k4_out;
    q4_in = other.q4_in;
    q4_out = other.q4_out;
    p4_in = other.p4_in;
    p4_out = other.p4_out;

    an_th_q_out = other.an_th_q_out;
    an_phi_q_out = other.an_phi_q_out;

    m_target_polarization = other.m_target_polarization;
    m_target_polarizationL = other.m_target_polarizationL;
    m_target_polarizationT = other.m_target_polarizationT;
    m_target_polarizationT_x = other.m_target_polarizationT_x;
    m_target_polarizationT_y = other.m_target_polarizationT_y;
    m_proton_spinor_orientation = other.m_proton_spinor_orientation;
}

VGG1999Model* VGG1999Model::clone() const {
    return new VGG1999Model(*this);
}

VGG1999Model::~VGG1999Model() {
}

void VGG1999Model::initModule() {
    DVCSModule::initModule();
}

void VGG1999Model::initModule(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {
    DVCSModule::initModule(beamHelicity, beamCharge, targetPolarization);
}

void VGG1999Model::isModuleWellConfigured() {
    DVCSModule::isModuleWellConfigured();
}

double VGG1999Model::CrossSectionBH(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {

    return CrossSectionMechanism(beamHelicity, beamCharge, targetPolarization,
            1);
}

double VGG1999Model::CrossSectionVCS(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {

    return CrossSectionMechanism(beamHelicity, beamCharge, targetPolarization,
            2);
}

double VGG1999Model::CrossSectionInterf(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {

    return CrossSectionMechanism(beamHelicity, beamCharge, targetPolarization,
            3);
}

double VGG1999Model::CrossSectionMechanism(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization, int mechanism) {

    //fill internal variables
    fillInternalVariables(targetPolarization);

    //return
    double result = 0.;

    //unpolarized target
    if (m_target_polarization == 0.) {

        m_proton_spinor_orientation = z;

        double resultZp = 0.;
        double resultZm = 0.;

        resultZp = v_compton_doublepol_cross_inv(beamCharge, mechanism,
                beamHelicity, 0.5);
        resultZm = v_compton_doublepol_cross_inv(beamCharge, mechanism,
                beamHelicity, -0.5);

        result += 0.5 * resultZp + 0.5 * resultZm;
    } else {

        //polarization x (transverse)
        if (m_target_polarizationT_x != 0.) {

            m_proton_spinor_orientation = VGG1999Model::x;

            double resultXp = 0.;
            double resultXm = 0.;

            if (m_target_polarizationT_x != -1.)
                resultXp = v_compton_doublepol_cross_inv(beamCharge, mechanism,
                        beamHelicity, 0.5);
            if (m_target_polarizationT_x != 1.)
                resultXm = v_compton_doublepol_cross_inv(beamCharge, mechanism,
                        beamHelicity, -0.5);

            result += 0.5 * (1. + m_target_polarizationT_x) * resultXp
                    + 0.5 * (1. - m_target_polarizationT_x) * resultXm;
        }

        //polarization y (transverse)
        if (m_target_polarizationT_y != 0.) {

            m_proton_spinor_orientation = VGG1999Model::y;

            double resultYp = 0.;
            double resultYm = 0.;

            if (m_target_polarizationT_y != -1.)
                resultYp = v_compton_doublepol_cross_inv(beamCharge, mechanism,
                        beamHelicity, 0.5);
            if (m_target_polarizationT_y != 1.)
                resultYm = v_compton_doublepol_cross_inv(beamCharge, mechanism,
                        beamHelicity, -0.5);

            result += 0.5 * (1. + m_target_polarizationT_y) * resultYp
                    + 0.5 * (1. - m_target_polarizationT_y) * resultYm;
        }

        //polarization z (longidudinal)
        if (m_target_polarizationL != 0.) {

            m_proton_spinor_orientation = VGG1999Model::z;

            double resultZp = 0.;
            double resultZm = 0.;

            if (m_target_polarizationL != -1.)
                resultZp = v_compton_doublepol_cross_inv(beamCharge, mechanism,
                        beamHelicity, 0.5);
            if (m_target_polarizationL != 1.)
                resultZm = v_compton_doublepol_cross_inv(beamCharge, mechanism,
                        beamHelicity, -0.5);

            result += 0.5 * (1. + m_target_polarizationL) * resultZp
                    + 0.5 * (1. - m_target_polarizationL) * resultZm;
        }
    }

    //return
    return result;
}

void VGG1999Model::fillInternalVariables(NumA::Vector3D targetPolarization) {

    //VARIABLES

    //nu
    m_nu = m_Q2 / (2. * PROTON_MASS * m_xB);

    //y
    m_y = m_nu / m_E;

    //s
    m_s = pow(PROTON_MASS, 2) - m_Q2 + 2. * PROTON_MASS * m_nu;

    //xi
    m_xi = m_xB / (2. - m_xB);

    //MOMENTA

    //momentum of incoming lepton
    k_in_mom = m_E;

    //momentum of outgoing lepton
    k_out_mom = (1. - m_y) * m_E;

    //momentum of incoming photon
    q_in_mom = sqrt(m_Q2 + pow(m_nu, 2.));

    //momentum of outgoing photon
    q_out_mom = (m_s + m_t + m_Q2 - pow(PROTON_MASS, 2.)) / (2. * PROTON_MASS);

    //ENERGIES

    //energy of incoming lepton
    k_in_en = m_E;

    //energy of outgoing lepton
    k_out_en = (1. - m_y) * m_E;

    //energy of incoming photon
    q_in_en = m_nu;

    //energy of incoming proton
    p_in_en = PROTON_MASS;

    //energy of outgoing photon
    q_out_en = q_out_mom;

    //ANGLES
    //polar angle of outgoing photon
    an_th_q_out = acos(
            (m_t + m_Q2 + 2. * q_in_en * q_out_en)
                    / (2. * q_in_mom * q_out_mom));

    //azimuthal angle of outgoing photon
    an_phi_q_out = m_phi;

    //MOMENTA VECTORS

    //momentum vector of incoming lepton
    double k_in_x_lab = k_in_mom * k_out_mom / q_in_mom
            * sin(2. * asin(sqrt(m_Q2 / (4. * k_in_mom * k_out_mom))));
    double k_in_z_lab = sqrt(pow(k_in_mom, 2.) - pow(k_in_x_lab, 2.));

    k_in.SetElement(0, std::complex<double>(k_in_x_lab, 0.));
    k_in.SetElement(1, std::complex<double>(0., 0.));
    k_in.SetElement(2, std::complex<double>(k_in_z_lab, 0.));

    //momentum vector of outgoing lepton
    double k_out_x_lab = k_in_x_lab;
    double k_out_z_lab = k_in_z_lab - q_in_mom;

    k_out.SetElement(0, std::complex<double>(k_out_x_lab, 0.));
    k_out.SetElement(1, std::complex<double>(0., 0.));
    k_out.SetElement(2, std::complex<double>(k_out_z_lab, 0.));

    //momentum vector of incoming photon
    q_in = k_in - k_out;

    //momentum vector of outgoing photon
    q_out = Vpolar(q_out_mom, an_th_q_out, an_phi_q_out);

    //momentum vector of incoming proton
    p_in.SetElement(0, std::complex<double>(0., 0.));
    p_in.SetElement(1, std::complex<double>(0., 0.));
    p_in.SetElement(2, std::complex<double>(0., 0.));

    //momentum vector of outgoing proton
    p_out = q_in - q_out;

    //energy of outgoing proton
    p_out_en = sqrt(pow(p_out.Mag(), 2) + pow(PROTON_MASS, 2));

    //LERENTZ VECTORS

    //four vector of incoming lepton
    k4_in = V4ini(std::complex<double>(k_in_en, 0.0), k_in);

    //four vector of outgoing lepton
    k4_out = V4ini(std::complex<double>(k_out_en, 0.0), k_out);

    //four vector of incoming photon
    q4_in = k4_in - k4_out;

    //four vector of outgoing photon
    q4_out = V4ini(std::complex<double>(q_out_en, 0.0), q_out);

    //four vector of incoming proton
    p4_in = V4ini(std::complex<double>(p_in_en, 0.0), p_in);

    //four vector of outgoing proton
    p4_out = V4ini(std::complex<double>(p_out_en, 0.0), p_out);

    //TARGET POLARISATION

    //target polarization
    m_target_polarization = sqrt(
            pow(targetPolarization.getX(), 2)
                    + pow(targetPolarization.getY(), 2)
                    + pow(targetPolarization.getZ(), 2));

    //target polarisation longitudinal component
    m_target_polarizationL = targetPolarization.getZ();

    //target polarisation transverse component
    m_target_polarizationT = sqrt(
            pow(targetPolarization.getX(), 2)
                    + pow(targetPolarization.getY(), 2));

    //target polarisation transverse x component
    m_target_polarizationT_x = targetPolarization.getX();

    //target polarisation transverse y component
    m_target_polarizationT_y = targetPolarization.getY();
}

NumA::VectorComplex4D VGG1999Model::V4ini(std::complex<double> a,
        NumA::VectorComplex3D b) {

    //return
    NumA::VectorComplex4D r;

    //set element 0
    r.SetElement(0, a);

    //set elements 1-3
    for (int i = 0; i < 3; i++)
        r.SetElement(i + 1, b.GetElement(i));

    //return
    return r;
}

NumA::VectorComplex4D VGG1999Model::spinor_pos(NumA::VectorComplex3D p,
        double mass, double sproj) {

    //return
    NumA::VectorComplex4D a;

    //energy
    double energy = sqrt(pow(p.Mag(), 2.) + pow(mass, 2.));

    if (m_proton_spinor_orientation == x) {

        //spin projection +
        if (sproj == 0.5) {
            a.SetElement(0, std::complex<double>(1. / sqrt(2.), 0.));
            a.SetElement(1, std::complex<double>(1. / sqrt(2.), 0.));
            a.SetElement(2,
                    (1. / (energy + mass) * 1. / sqrt(2.)
                            * ((p.GetElement(0)
                                    + (p.GetElement(1)
                                            * std::complex<double>(0., -1.)))
                                    + p.GetElement(2))));
            a.SetElement(3,
                    (1. / (energy + mass) * 1. / sqrt(2.)
                            * ((p.GetElement(0)
                                    + (p.GetElement(1)
                                            * std::complex<double>(0., 1.)))
                                    - p.GetElement(2))));
        }

        //spin projection -
        if (sproj == -0.5) {
            a.SetElement(0, std::complex<double>(1. / sqrt(2.), 0.));
            a.SetElement(1, std::complex<double>(-1. / sqrt(2.), 0.));
            a.SetElement(2,
                    (1. / (energy + mass) * 1. / sqrt(2.)
                            * (((-1. * p.GetElement(0))
                                    + (p.GetElement(1)
                                            * std::complex<double>(0., 1.)))
                                    + p.GetElement(2))));
            a.SetElement(3,
                    (1. / (energy + mass) * 1. / sqrt(2.)
                            * ((p.GetElement(0)
                                    + (p.GetElement(1)
                                            * std::complex<double>(0., 1.)))
                                    + p.GetElement(2))));
        }
    }

    if (m_proton_spinor_orientation == y) {

        //spin projection +
        if (sproj == 0.5) {
            a.SetElement(0, std::complex<double>(1. / sqrt(2.), 0.));
            a.SetElement(1, std::complex<double>(0., 1. / sqrt(2.)));
            a.SetElement(2,
                    (1. / (energy + mass) * 1. / sqrt(2.)
                            * ((p.GetElement(1)
                                    + (p.GetElement(0)
                                            * std::complex<double>(0., 1.)))
                                    + p.GetElement(2))));
            a.SetElement(3,
                    (1. / (energy + mass) * 1. / sqrt(2.)
                            * ((p.GetElement(0)
                                    + (p.GetElement(1)
                                            * std::complex<double>(0., 1.)))
                                    + (p.GetElement(2)
                                            * std::complex<double>(0., -1.)))));
        }

        //spin projection +
        if (sproj == -0.5) {
            a.SetElement(0, std::complex<double>(1. / sqrt(2.), 0.));
            a.SetElement(1, std::complex<double>(0., -1. / sqrt(2.)));
            a.SetElement(2,
                    (1. / (energy + mass) * 1. / sqrt(2.)
                            * (((-1. * p.GetElement(1))
                                    + (p.GetElement(0)
                                            * std::complex<double>(0., -1.)))
                                    + p.GetElement(2))));
            a.SetElement(3,
                    (1. / (energy + mass) * 1. / sqrt(2.)
                            * ((p.GetElement(0)
                                    + (p.GetElement(1)
                                            * std::complex<double>(0., 1.)))
                                    + (p.GetElement(2)
                                            * std::complex<double>(0., 1.)))));
        }
    }

    if (m_proton_spinor_orientation == z) {

        //spin projection +
        if (sproj == 0.5) {
            a.SetElement(0, std::complex<double>(1., 0.));
            a.SetElement(1, std::complex<double>(0., 0.));
            a.SetElement(2, (1. / (energy + mass)) * p.Dot(pauli(0.5, 0.5)));
            a.SetElement(3, (1. / (energy + mass)) * p.Dot(pauli(-0.5, 0.5)));
        }

        //spin projection -
        if (sproj == -0.5) {
            a.SetElement(0, std::complex<double>(0., 0.));
            a.SetElement(1, std::complex<double>(1., 0.));
            a.SetElement(2, (1. / (energy + mass)) * p.Dot(pauli(0.5, -0.5)));
            a.SetElement(3, (1. / (energy + mass)) * p.Dot(pauli(-0.5, -0.5)));
        }
    }

    //return
    return a;
}

NumA::MatrixComplex4D VGG1999Model::dirac_gamma(int mu) {

    //return
    NumA::MatrixComplex4D gamma;

    //time like
    if (mu == 0) {
        gamma.SetElement(0, 0, std::complex<double>(1., 0.));
        gamma.SetElement(0, 1, std::complex<double>(0., 0.));
        gamma.SetElement(0, 2, std::complex<double>(0., 0.));
        gamma.SetElement(0, 3, std::complex<double>(0., 0.));
        gamma.SetElement(1, 0, std::complex<double>(0., 0.));
        gamma.SetElement(1, 1, std::complex<double>(1., 0.));
        gamma.SetElement(1, 2, std::complex<double>(0., 0.));
        gamma.SetElement(1, 3, std::complex<double>(0., 0.));
        gamma.SetElement(2, 0, std::complex<double>(0., 0.));
        gamma.SetElement(2, 1, std::complex<double>(0., 0.));
        gamma.SetElement(2, 2, std::complex<double>(-1., 0.));
        gamma.SetElement(2, 3, std::complex<double>(0., 0.));
        gamma.SetElement(3, 0, std::complex<double>(0., 0.));
        gamma.SetElement(3, 1, std::complex<double>(0., 0.));
        gamma.SetElement(3, 2, std::complex<double>(0., 0.));
        gamma.SetElement(3, 3, std::complex<double>(-1., 0.));
    }

    //space like 1
    if (mu == 1) {
        gamma.SetElement(0, 0, std::complex<double>(0., 0.));
        gamma.SetElement(0, 1, std::complex<double>(0., 0.));
        gamma.SetElement(0, 2, std::complex<double>(0., 0.));
        gamma.SetElement(0, 3, std::complex<double>(1., 0.));
        gamma.SetElement(1, 0, std::complex<double>(0., 0.));
        gamma.SetElement(1, 1, std::complex<double>(0., 0.));
        gamma.SetElement(1, 2, std::complex<double>(1., 0.));
        gamma.SetElement(1, 3, std::complex<double>(0., 0.));
        gamma.SetElement(2, 0, std::complex<double>(0., 0.));
        gamma.SetElement(2, 1, std::complex<double>(-1., 0.));
        gamma.SetElement(2, 2, std::complex<double>(0., 0.));
        gamma.SetElement(2, 3, std::complex<double>(0., 0.));
        gamma.SetElement(3, 0, std::complex<double>(-1., 0.));
        gamma.SetElement(3, 1, std::complex<double>(0., 0.));
        gamma.SetElement(3, 2, std::complex<double>(0., 0.));
        gamma.SetElement(3, 3, std::complex<double>(0., 0.));
    }

    //space like 2
    if (mu == 2) {
        gamma.SetElement(0, 0, std::complex<double>(0., 0.));
        gamma.SetElement(0, 1, std::complex<double>(0., 0.));
        gamma.SetElement(0, 2, std::complex<double>(0., 0.));
        gamma.SetElement(0, 3, std::complex<double>(0., -1.));
        gamma.SetElement(1, 0, std::complex<double>(0., 0.));
        gamma.SetElement(1, 1, std::complex<double>(0., 0.));
        gamma.SetElement(1, 2, std::complex<double>(0., 1.));
        gamma.SetElement(1, 3, std::complex<double>(0., 0.));
        gamma.SetElement(2, 0, std::complex<double>(0., 0.));
        gamma.SetElement(2, 1, std::complex<double>(0., 1.));
        gamma.SetElement(2, 2, std::complex<double>(0., 0.));
        gamma.SetElement(2, 3, std::complex<double>(0., 0.));
        gamma.SetElement(3, 0, std::complex<double>(0., -1.));
        gamma.SetElement(3, 1, std::complex<double>(0., 0.));
        gamma.SetElement(3, 2, std::complex<double>(0., 0.));
        gamma.SetElement(3, 3, std::complex<double>(0., 0.));
    }

    //space like 3
    if (mu == 3) {
        gamma.SetElement(0, 0, std::complex<double>(0., 0.));
        gamma.SetElement(0, 1, std::complex<double>(0., 0.));
        gamma.SetElement(0, 2, std::complex<double>(1., 0.));
        gamma.SetElement(0, 3, std::complex<double>(0., 0.));
        gamma.SetElement(1, 0, std::complex<double>(0., 0.));
        gamma.SetElement(1, 1, std::complex<double>(0., 0.));
        gamma.SetElement(1, 2, std::complex<double>(0., 0.));
        gamma.SetElement(1, 3, std::complex<double>(-1., 0.));
        gamma.SetElement(2, 0, std::complex<double>(-1., 0.));
        gamma.SetElement(2, 1, std::complex<double>(0., 0.));
        gamma.SetElement(2, 2, std::complex<double>(0., 0.));
        gamma.SetElement(2, 3, std::complex<double>(0., 0.));
        gamma.SetElement(3, 0, std::complex<double>(0., 0.));
        gamma.SetElement(3, 1, std::complex<double>(1., 0.));
        gamma.SetElement(3, 2, std::complex<double>(0., 0.));
        gamma.SetElement(3, 3, std::complex<double>(0., 0.));
    }

    //return
    return gamma;
}

NumA::MatrixComplex4D VGG1999Model::dirac_gamma5() {

    //return
    NumA::MatrixComplex4D gamma5;

    //set elements
    gamma5.SetElement(0, 0, std::complex<double>(0., 0.));
    gamma5.SetElement(0, 1, std::complex<double>(0., 0.));
    gamma5.SetElement(0, 2, std::complex<double>(1., 0.));
    gamma5.SetElement(0, 3, std::complex<double>(0., 0.));
    gamma5.SetElement(1, 0, std::complex<double>(0., 0.));
    gamma5.SetElement(1, 1, std::complex<double>(0., 0.));
    gamma5.SetElement(1, 2, std::complex<double>(0., 0.));
    gamma5.SetElement(1, 3, std::complex<double>(1., 0.));
    gamma5.SetElement(2, 0, std::complex<double>(1., 0.));
    gamma5.SetElement(2, 1, std::complex<double>(0., 0.));
    gamma5.SetElement(2, 2, std::complex<double>(0., 0.));
    gamma5.SetElement(2, 3, std::complex<double>(0., 0.));
    gamma5.SetElement(3, 0, std::complex<double>(0., 0.));
    gamma5.SetElement(3, 1, std::complex<double>(1., 0.));
    gamma5.SetElement(3, 2, std::complex<double>(0., 0.));
    gamma5.SetElement(3, 3, std::complex<double>(0., 0.));

    //return
    return gamma5;
}

double VGG1999Model::metric(int mu, int nu) {

    //return
    double g;

    //timelike convention (1, -1, -1, -1)
    if (mu == nu) {
        if (mu == 0)
            g = 1.;
        else
            g = -1.;
    } else
        g = 0.;

    //return
    return g;
}

NumA::VectorComplex3D VGG1999Model::pauli(double row, double col) {

    //return
    NumA::VectorComplex3D t;

    //row == col?
    double delta = fabs(row + col);

    //set elements
    t.SetElement(0, std::complex<double>(1. - delta, 0.));
    t.SetElement(1,
            std::complex<double>(0.,
                    ((col < 0.) ? (-1.) : (1.)) * (1. - delta)));
    t.SetElement(2,
            std::complex<double>(((col < 0.) ? (-1.) : (1.)) * delta, 0.));

    //return
    return t;
}

NumA::VectorComplex4D VGG1999Model::spinor_adj(NumA::VectorComplex4D a) {

    //return
    NumA::VectorComplex4D b;

    //set elements
    b.SetElement(0, std::conj(a.GetElement(0)));
    b.SetElement(1, std::conj(a.GetElement(1)));
    b.SetElement(2, -1. * std::conj(a.GetElement(2)));
    b.SetElement(3, -1. * std::conj(a.GetElement(3)));

    //return
    return b;
}

std::complex<double> VGG1999Model::spinleft_mat_spinright(
        NumA::VectorComplex4D l, NumA::MatrixComplex4D m,
        NumA::VectorComplex4D r) {

    //return
    std::complex<double> sum = std::complex<double>(0., 0.);

    //adjoint of spinor left
    NumA::VectorComplex4D l_adj = spinor_adj(l);

    //calculate elements
    for (int i = 0; i <= 3; i++) {
        for (int j = 0; j <= 3; j++) {
            sum = sum
                    + l_adj.GetElement(i)
                            * (m.GetElement(i, j) * r.GetElement(j));
        }
    }

    //return
    return sum;
}

NumA::MatrixComplex4D VGG1999Model::fvec_slash(NumA::VectorComplex4D a) {

    //return
    NumA::MatrixComplex4D b;

    //calculate elements
    for (int mu1 = 0; mu1 <= 3; mu1++) {
        for (int mu2 = 0; mu2 <= 3; mu2++) {
            b = b + dirac_gamma(mu1) * (metric(mu1, mu2) * a.GetElement(mu2));
        }
    }

    //return
    return b;
}

std::complex<double> VGG1999Model::V4mul(NumA::VectorComplex4D a,
        NumA::VectorComplex4D b) {

    //return
    std::complex<double> c(0., 0.);

    //calculate elements
    for (int mu1 = 0; mu1 <= 3; mu1++) {
        for (int mu2 = 0; mu2 <= 3; mu2++) {
            c = c + metric(mu1, mu2) * (a.GetElement(mu1) * b.GetElement(mu2));
        }
    }

    //return
    return c;
}

std::complex<double> VGG1999Model::Cinv(std::complex<double> z) {

    //return
    std::complex<double> c;

    //calculate real and imaginary parts
    c.real(z.real() / (z.real() * z.real() + z.imag() * z.imag()));
    c.imag(-z.imag() / (z.real() * z.real() + z.imag() * z.imag()));

    //return
    return c;
}

NumA::VectorComplex4D VGG1999Model::spinor_hel(momentum3 p, double hel) {

    //return
    spinor a;

    //polar angle
    double th = polar_angle(p);

    //azimuthal angle
    double phi = azimut_angle(p);

    //helicity +
    if (hel == +1.) {
        a.SetElement(0, std::complex<double>(cos(th / 2.), 0.));
        a.SetElement(1,
                std::complex<double>(sin(th / 2.) * cos(phi),
                        sin(th / 2.) * sin(phi)));
        a.SetElement(2, std::complex<double>(cos(th / 2.), 0.));
        a.SetElement(3,
                std::complex<double>(sin(th / 2.) * cos(phi),
                        sin(th / 2.) * sin(phi)));
    }

    //helicity -
    if (hel == -1.) {
        a.SetElement(0,
                std::complex<double>(-sin(th / 2.) * cos(phi),
                        sin(th / 2.) * sin(phi)));
        a.SetElement(1, std::complex<double>(cos(th / 2.), 0.0));
        a.SetElement(2,
                std::complex<double>(sin(th / 2.) * cos(phi),
                        -sin(th / 2.) * sin(phi)));
        a.SetElement(3, std::complex<double>(-cos(th / 2.), 0.0));
    }

    //return
    return a;
}

double VGG1999Model::polar_angle(NumA::VectorComplex3D t) {
    return acos((t.GetElement(2).real()) / t.Mag());
}

double VGG1999Model::azimut_angle(NumA::VectorComplex3D t) {

    if (t.GetElement(0).real() == 0.) {

        if (t.GetElement(1).real() >= 0.)
            return .5 * PI;
        else
            return 1.5 * PI;

    } else {

        if (t.GetElement(0).real() > 0.) {

            if (t.GetElement(1).real() >= 0.)
                return atan(t.GetElement(1).real() / t.GetElement(0).real());
            else
                return (2 * PI
                        - atan(
                                fabs(
                                        t.GetElement(1).real()
                                                / t.GetElement(0).real())));
        } else {
            if (t.GetElement(1).real() >= 0.)
                return (PI
                        - atan(
                                fabs(
                                        t.GetElement(1).real()
                                                / t.GetElement(0).real())));
            else
                return (PI
                        + atan(
                                fabs(
                                        t.GetElement(1).real()
                                                / t.GetElement(0).real())));
        }
    }
}

NumA::MatrixComplex4D VGG1999Model::dirac_sigma(int mu, int nu) {

    //return
    NumA::MatrixComplex4D a;

    //calculate
    a = (((dirac_gamma(mu).Mult(dirac_gamma(nu)))
            - (dirac_gamma(nu).Mult(dirac_gamma(mu))))
            * std::complex<double>(0.0, 0.5));

    //return
    return a;
}

NumA::MatrixComplex4D VGG1999Model::unit_spinor_mat() {

    //return
    NumA::MatrixComplex4D a;

    //set elements
    a.SetElement(0, 0, std::complex<double>(1., 0.));
    a.SetElement(1, 1, std::complex<double>(1., 0.));
    a.SetElement(2, 2, std::complex<double>(1., 0.));
    a.SetElement(3, 3, std::complex<double>(1., 0.));

    //return
    return a;
}

NumA::VectorComplex3D VGG1999Model::Vpolar(double magn, double th, double phi) {

    //return
    NumA::VectorComplex3D t;

    //set elements
    t.SetElement(0, std::complex<double>(magn * sin(th) * cos(phi), 0.0));
    t.SetElement(1, std::complex<double>(magn * sin(th) * sin(phi), 0.0));
    t.SetElement(2, std::complex<double>(magn * cos(th), 0.0));

    //return
    return t;
}

double VGG1999Model::antisymm_epsilon(int ka, int la, int mu, int nu) {

    //return
    double g;

    if ((ka == la) || (ka == mu) || (ka == nu) || (la == mu) || (la == nu)
            || (mu == nu)) {
        g = 0.;
    } else {
        switch (ka) {
        case 0: {
            switch (la) {
            case 1: {
                if ((mu == 2) && (nu == 3))
                    g = +1;
                if ((mu == 3) && (nu == 2))
                    g = -1;

                break;
            }
            case 2: {
                if ((mu == 1) && (nu == 3))
                    g = -1;
                if ((mu == 3) && (nu == 1))
                    g = +1;

                break;
            }
            case 3: {
                if ((mu == 1) && (nu == 2))
                    g = +1;
                if ((mu == 2) && (nu == 1))
                    g = -1;

                break;
            }
            }
            break;
        }
        case 1: {
            switch (la) {
            case 0: {
                if ((mu == 2) && (nu == 3))
                    g = -1;
                if ((mu == 3) && (nu == 2))
                    g = +1;

                break;
            }
            case 2: {
                if ((mu == 0) && (nu == 3))
                    g = +1;
                if ((mu == 3) && (nu == 0))
                    g = -1;

                break;
            }
            case 3: {
                if ((mu == 0) && (nu == 2))
                    g = -1;
                if ((mu == 2) && (nu == 0))
                    g = +1;

                break;
            }
            }
            break;
        }
        case 2: {
            switch (la) {
            case 0: {
                if ((mu == 1) && (nu == 3))
                    g = +1;
                if ((mu == 3) && (nu == 1))
                    g = -1;

                break;
            }
            case 1: {
                if ((mu == 0) && (nu == 3))
                    g = -1;
                if ((mu == 3) && (nu == 0))
                    g = +1;

                break;
            }
            case 3: {
                if ((mu == 0) && (nu == 1))
                    g = +1;
                if ((mu == 1) && (nu == 0))
                    g = -1;

                break;
            }
            }

            break;
        }
        case 3: {
            switch (la) {
            case 0: {
                if ((mu == 1) && (nu == 2))
                    g = -1;
                if ((mu == 2) && (nu == 1))
                    g = +1;

                break;
            }
            case 1: {
                if ((mu == 0) && (nu == 2))
                    g = +1;
                if ((mu == 2) && (nu == 0))
                    g = -1;

                break;
            }
            case 2: {
                if ((mu == 0) && (nu == 1))
                    g = -1;
                if ((mu == 1) && (nu == 0))
                    g = +1;

                break;
            }
            }

            break;
        }
        }
    }

    //return
    return g;
}

NumA::MatrixComplex4D VGG1999Model::gamma_NN_vertex(int mu,
        NumA::VectorComplex4D q) {

    //photon virtuality (this is not virtual photon emitted by lepton but that emitted by proton)
    double Q_sqr = -V4mul(q, q).real();

    //magnetic moment of proton
    double P_MAGN = 2.79;

    //magnetic form factor
    double G_M = P_MAGN * 1.
            / (1. + 0.116 * sqrt(Q_sqr) + 2.874 * pow(sqrt(Q_sqr), 2.)
                    + 0.241 * pow(sqrt(Q_sqr), 3.)
                    + 1.006 * pow(sqrt(Q_sqr), 4.)
                    + 0.345 * pow(sqrt(Q_sqr), 5.));

    //electric form factor
    double G_E = G_M / P_MAGN * (1. - 0.13 * (Q_sqr - 0.04));

    //Dirac form factor
    double ff_F1 = (Q_sqr / pow(2. * PROTON_MASS, 2.) * G_M + G_E)
            / (1. + Q_sqr / pow(2. * PROTON_MASS, 2.));

    //Pauli form factor
    double ff_F2 = (G_M - G_E) / (1. + Q_sqr / pow(2. * PROTON_MASS, 2.));

    //Dirac part
    NumA::MatrixComplex4D dirac = dirac_gamma(mu)
            * std::complex<double>(ff_F1, 0.0);

    //Pauli part
    NumA::MatrixComplex4D pauli, pauli_incr;
    pauli.Clear();

    for (int nu1 = 0; nu1 <= 3; nu1++) {
        for (int nu2 = 0; nu2 <= 3; nu2++) {
            if (metric(nu1, nu2) == 0.0)
                pauli_incr.Clear();
            else {
                pauli_incr = dirac_sigma(mu, nu1)
                        * (q.GetElement(nu2)
                                * std::complex<double>(0.0,
                                        metric(nu1, nu2) * ff_F2
                                                / (2. * PROTON_MASS)));
            }
            pauli = pauli + pauli_incr;

        }
    }

    //return
    return dirac + pauli;
}

std::complex<double> VGG1999Model::AmplitudeBH(int mu, double el_hel,
        double sp_in, double sp_out) {

    //return
    std::complex<double> result;

    //four momentum squared
    double four_mom_transf_sqr = pow(p_out_en - p_in_en, 2.)
            - pow((p_out - p_in).Mag(), 2.);

    //virtual photon emitted by proton
    momentum4 q4_proton = p4_out - p4_in;

    //spinor of incoming lepton
    spinor spinor_e_in = spinor_hel(k_in, el_hel);

    //spinor of outgoing lepton
    spinor spinor_e_out = spinor_hel(k_out, el_hel);

    //spinor of incoming proton
    spinor spinor_p_in = spinor_pos(p_in, PROTON_MASS, sp_in);

    //spinor of outgoing proton
    spinor spinor_p_out = spinor_pos(p_out, PROTON_MASS, sp_out);

    //calculate result
    spinorMatrix ee_2gamma;

    std::complex<double> proton_part, electron_part, e_times_p_incr;
    std::complex<double> e_times_p = std::complex<double>(0., 0.);

    for (int nu = 0; nu <= 3; nu++) {

        ee_2gamma = dirac_gamma(nu).Mult(
                (((fvec_slash(k4_in) - fvec_slash(q4_out))
                        + (unit_spinor_mat()
                                * std::complex<double>(ELECTRON_MASS, 0.)))
                        * Cinv(((-2. * V4mul(k4_in, q4_out))))).Mult(
                        dirac_gamma(mu)))

                +

                dirac_gamma(mu).Mult(
                        (((fvec_slash(k4_out) + fvec_slash(q4_out))
                                + (unit_spinor_mat()
                                        * std::complex<double>(ELECTRON_MASS,
                                                0.)))
                                * Cinv(((2. * V4mul(k4_out, q4_out))))).Mult(
                                dirac_gamma(nu)));

        electron_part = spinleft_mat_spinright(spinor_e_out, ee_2gamma,
                spinor_e_in);
        proton_part = spinleft_mat_spinright(spinor_p_out,
                gamma_NN_vertex(nu, q4_proton), spinor_p_in);
        e_times_p_incr = metric(nu, nu) * (electron_part * proton_part);
        e_times_p = e_times_p + e_times_p_incr;
    }

    //electric charge with pow(e, 2) / (4 * PI) = 1 / 137
    double ELEC = 0.302862;

    //calculate phasespace and include in result
    result =
            (-sqrt(k_in_mom * k_out_mom) * sqrt(p_in_en + PROTON_MASS)
                    * sqrt(p_out_en + PROTON_MASS) * pow(ELEC, 3.)
                    / four_mom_transf_sqr) * e_times_p;

    //return
    return result;

}

std::complex<double> VGG1999Model::v_compton_t_matrix_hel(double leptcharge,
        int mechanism, int mu, double el_hel, double sp_in, double sp_out) {

    switch (mechanism) {

    case 1: {
        return AmplitudeBH(mu, el_hel, sp_in, sp_out);
        break;
    }

    case 2: {
        return AmplitudeDVCS(mu, el_hel, sp_in, sp_out);
        break;
    }

    default: {
        return std::complex<double>(0., 0.);
        break;
    }
    }
}

double VGG1999Model::v_compton_doublepol_sqrampl(double leptcharge,
        int mechanism, double el_hel, double sp_in) {

    //return
    double tmat = 0.;

    //four vector of outgoing photon
    momentum4 q4_out = V4ini(
            std::complex<double>(sqrt(pow(q_out.Mag(), 2.)), 0.), q_out);

    //polarization vector
    momentum3 pol_ph_x;
    pol_ph_x.SetElement(0,
            std::complex<double>(cos(an_th_q_out) * cos(an_phi_q_out), 0.));
    pol_ph_x.SetElement(1,
            std::complex<double>(cos(an_th_q_out) * sin(an_phi_q_out), 0.));
    pol_ph_x.SetElement(2, std::complex<double>(-sin(an_th_q_out), 0.));

    momentum4 pol4_ph_x = V4ini(std::complex<double>(0., 0.), pol_ph_x);

    momentum3 pol_ph_y;
    pol_ph_y.SetElement(0, std::complex<double>(-sin(an_phi_q_out), 0.));
    pol_ph_y.SetElement(1, std::complex<double>(cos(an_phi_q_out), 0.));
    pol_ph_y.SetElement(2, std::complex<double>(0., 0.));

    momentum4 pol4_ph_y = V4ini(std::complex<double>(0., 0.), pol_ph_y);

    //loop over outgoing proton helicities
    double t_incr;
    std::complex<double> t_matrix_mu[3];

    for (double sp_out = -.5; sp_out <= .5; sp_out += 1.) {

        std::complex<double> t_matrix_x[3];
        std::complex<double> t_matrix_y[3];

        for (int mu = 0; mu <= 3; mu++) {

            switch (mechanism) {

            case 1: {
                t_matrix_mu[0] = v_compton_t_matrix_hel(leptcharge, 1, mu,
                        el_hel, sp_in, sp_out);
                t_matrix_mu[1] = std::complex<double>(0., 0.);
                t_matrix_mu[2] = std::complex<double>(0., 0.);
                break;
            }

            case 2: {
                t_matrix_mu[0] = std::complex<double>(0., 0.);
                t_matrix_mu[1] = v_compton_t_matrix_hel(leptcharge, 2, mu,
                        el_hel, sp_in, sp_out);
                t_matrix_mu[2] = std::complex<double>(0., 0.);
                break;
            }

            case 3: {
                t_matrix_mu[0] = v_compton_t_matrix_hel(leptcharge, 1, mu,
                        el_hel, sp_in, sp_out);
                t_matrix_mu[1] = v_compton_t_matrix_hel(leptcharge, 2, mu,
                        el_hel, sp_in, sp_out);
                t_matrix_mu[2] = t_matrix_mu[0] + (leptcharge * t_matrix_mu[1]);
                break;
            }

            default: {
                t_matrix_mu[0] = std::complex<double>(0., 0.);
                t_matrix_mu[1] = std::complex<double>(0., 0.);
                t_matrix_mu[2] = std::complex<double>(0., 0.);
                break;
            }
            }

            for (int p = 0; p < 3; p++) {

                t_matrix_x[p] =
                        (t_matrix_x[p]
                                + (metric(mu, mu)
                                        * (t_matrix_mu[p]
                                                * std::conj(
                                                        pol4_ph_x.GetElement(
                                                                mu)))));
                t_matrix_y[p] =
                        (t_matrix_y[p]
                                + (metric(mu, mu)
                                        * (t_matrix_mu[p]
                                                * std::conj(
                                                        pol4_ph_y.GetElement(
                                                                mu)))));
            }
        }

        switch (mechanism) {

        case 1: {
            t_incr = (((t_matrix_x[0] * std::conj(t_matrix_x[0]))
                    + (t_matrix_y[0] * std::conj(t_matrix_y[0])))).real();
            break;
        }

        case 2: {
            t_incr = (((t_matrix_x[1] * std::conj(t_matrix_x[1]))
                    + (t_matrix_y[1] * std::conj(t_matrix_y[1])))).real();
            break;
        }

        case 3: {
            t_incr = (((t_matrix_x[2] * std::conj(t_matrix_x[2]))
                    + (t_matrix_y[2] * std::conj(t_matrix_y[2])))).real();

            t_incr -= (((t_matrix_x[0] * std::conj(t_matrix_x[0]))
                    + (t_matrix_y[0] * std::conj(t_matrix_y[0])))).real();

            t_incr -= (((t_matrix_x[1] * std::conj(t_matrix_x[1]))
                    + (t_matrix_y[1] * std::conj(t_matrix_y[1])))).real();
            break;
        }

        default: {
            t_incr = 0.;
            break;
        }
        }

        tmat += t_incr;

    }

    //return
    return tmat;
}

double VGG1999Model::v_compton_doublepol_cross_inv(double leptcharge,
        int mechanism, double el_hel, double sp_in) {

    //return
    return (1. / pow(2. * PI, 5.)) / 32. * m_xB * pow(m_y, 2.) / pow(m_Q2, 2.)
            / sqrt(1. + pow(2. * PROTON_MASS * m_xB, 2.) / m_Q2)
            * v_compton_doublepol_sqrampl(leptcharge, mechanism, el_hel, sp_in);

}

std::complex<double> VGG1999Model::J_DVCS(int mu, int nu, double sp_in,
        double sp_out) {

    //return
    std::complex<double> result;

    //get CFFs
    std::complex<double> cffH = getConvolCoeffFunctionValue(GPDType::H);
    std::complex<double> cffE = getConvolCoeffFunctionValue(GPDType::E);
    std::complex<double> cffHt = getConvolCoeffFunctionValue(GPDType::Ht);
    std::complex<double> cffEt = getConvolCoeffFunctionValue(GPDType::Et);

    //convert to convention used in original the code
    cffH *= -1;
    cffE *= -1;
    cffHt *= -1;
    cffEt *= -1;

    //"avarage" 4momentum of proton
    momentum4 p4_av = ((p4_in + p4_out) * 0.5);

    //transfer of 4momentum in proton vertex
    momentum4 delta = p4_out - p4_in;

    //square of Delta
    double delta_sqr = V4mul(delta, delta).real();

    //?
    double Mb_sqr = pow(PROTON_MASS, 2.) - 0.25 * delta_sqr;

    //?
    momentum4 p4_sud = (((p4_av * (m_Q2 / (4. * m_xi)))
            + (q4_in * -0.5 * Mb_sqr))
            * (1. / (m_Q2 / (4. * m_xi) + m_xi * Mb_sqr)));
    //?
    momentum4 n4_sud = (((p4_av * 2. * m_xi) + q4_in)
            * (1. / (m_Q2 / (4. * m_xi) + m_xi * Mb_sqr)));

    //?
    std::complex<double> vecstruct_gmunuperp = (std::complex<double>(
            metric(mu, nu), 0.)
            - ((p4_sud.GetElement(mu) * n4_sud.GetElement(nu))
                    + (p4_sud.GetElement(nu) * n4_sud.GetElement(mu))));

    //? no gauge invariance
    std::complex<double> vecstruct_gaugecorr = std::complex<double>(0., 0.);

    //? no gauge invariance
    std::complex<double> eps_perp_gaugecorr = std::complex<double>(0., 0.);

    //spinor of incomming proton
    spinor spinor_p_in = spinor_pos(p_in, PROTON_MASS, sp_in);

    //spinor ofoutgoing proton
    spinor spinor_p_out = spinor_pos(p_out, PROTON_MASS, sp_out);

    //matrix element
    std::complex<double> me_gammaplus = spinleft_mat_spinright(spinor_p_out,
            fvec_slash(n4_sud), spinor_p_in);

    //tensor matrix
    spinorMatrix temp_tensor;
    temp_tensor.Clear();

    for (int kappa = 0; kappa <= 3; kappa++) {
        for (int lambda = 0; lambda <= 3; lambda++) {

            temp_tensor = (temp_tensor
                    + (dirac_sigma(kappa, lambda)
                            * (std::complex<double>(0.,
                                    metric(kappa, kappa)
                                            * metric(lambda, lambda)
                                            / (2. * PROTON_MASS))
                                    * (n4_sud.GetElement(kappa)
                                            * delta.GetElement(lambda)))));

        }

    }

    //?
    std::complex<double> eps_perp = std::complex<double>(0., 0.);

    for (int alpha = 0; alpha <= 3; alpha++) {
        for (int beta = 0; beta <= 3; beta++) {
            eps_perp = (eps_perp
                    + (metric(mu, mu) * metric(nu, nu)
                            * antisymm_epsilon(mu, nu, alpha, beta)
                            * (n4_sud.GetElement(alpha)
                                    * p4_sud.GetElement(beta))));
        }
    }

    //matrix element
    std::complex<double> me_gammaplus5 = spinleft_mat_spinright(spinor_p_out,
            (fvec_slash(n4_sud).Mult(dirac_gamma5())), spinor_p_in);

    //matrix element
    std::complex<double> me_pseudoscalar = spinleft_mat_spinright(spinor_p_out,
            dirac_gamma5(), spinor_p_in);

    //structs
    std::complex<double> dvcs_twist2vec_struct = (0.5
            * ((vecstruct_gmunuperp + vecstruct_gaugecorr) * me_gammaplus));

    std::complex<double> dvcs_twist2tensor_struct = (0.5
            * ((vecstruct_gmunuperp + vecstruct_gaugecorr)
                    * spinleft_mat_spinright(spinor_p_out, temp_tensor,
                            spinor_p_in)));

    std::complex<double> dvcs_twist2axvec_struct = (std::complex<double>(0.,
            -0.5) * ((eps_perp + eps_perp_gaugecorr) * me_gammaplus5));

    std::complex<double> dvcs_twist2ps_struct = (std::complex<double>(0., -0.5)
            * (((eps_perp + eps_perp_gaugecorr)) * me_pseudoscalar))
            * V4mul(delta, n4_sud) / (2. * PROTON_MASS);

    //return
    result = dvcs_twist2vec_struct * cffH + dvcs_twist2tensor_struct * cffE
            + dvcs_twist2axvec_struct * cffHt + dvcs_twist2ps_struct * cffEt;

    return result;
}

std::complex<double> VGG1999Model::AmplitudeDVCS(int mu, double el_hel,
        double sp_in, double sp_out) {

    //return
    std::complex<double> result;

    //spinor of incoming lepton
    spinor spinor_e_in = spinor_hel(k_in, el_hel);

    //spinor of outgoing lepton
    spinor spinor_e_out = spinor_hel(k_out, el_hel);

    //calculate result
    std::complex<double> nucleon_part, electron_part, e_times_p, e_times_p_incr;

    e_times_p = std::complex<double>(0., 0.);

    for (int nu = 0; nu <= 3; nu++) {

        electron_part = spinleft_mat_spinright(spinor_e_out, dirac_gamma(nu),
                spinor_e_in);

        nucleon_part = J_DVCS(mu, nu, sp_in, sp_out);

        e_times_p_incr = (metric(nu, nu) * (electron_part * nucleon_part));
        e_times_p = (e_times_p + e_times_p_incr);
    }

    //electric charge with pow(e, 2) / (4 * PI) = 1 / 137
    double ELEC = 0.302862;

    //calculate phasespace and include in result
    result = ((-sqrt(k_in_mom * k_out_mom) * sqrt(p_in_en + PROTON_MASS)
            * sqrt(p_out_en + PROTON_MASS) * pow(ELEC, 3.) / m_Q2) * e_times_p);

    //return
    return result;
}
