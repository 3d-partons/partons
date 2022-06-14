#ifndef DDVCS_PROCESS_DMSW22_H
#define DDVCS_PROCESS_DMSW22_H

#include <complex>
#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "DDVCSProcessModule.h"

namespace PARTONS {

/**
 * @class DDVCSProcessDMSW22
 *
 * @brief TODO
 *
 * TODO
 */
class DDVCSProcessDMSW22: public DDVCSProcessModule {
public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    DDVCSProcessDMSW22(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DDVCSProcessDMSW22();

    virtual DDVCSProcessDMSW22* clone() const;

protected:

    /**
     * Copy constructor.
     */
    DDVCSProcessDMSW22(const DDVCSProcessDMSW22& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual PhysicalType<double> CrossSectionBH();
    virtual PhysicalType<double> CrossSectionVCS();
    virtual PhysicalType<double> CrossSectionInterf();

private:

    //Amplitudes:
    std::complex<double> ampliBH1(int s2, int s1, int sl, double lminus[4],
            double lplus[4], int s, double kPrime[4], double k[4], double Mll2,
            double t);
    std::complex<double> ampliBH1crossed(int s2, int s1, int sl,
            double lminus[4], double lplus[4], int s, double kPrime[4],
            double k[4], double Mll2, double t);
    std::complex<double> ampliBH2(int s2, int s1, int sl, double lminus[4],
            double lplus[4], int s, double kPrime[4], double k[4], double Qcal2,
            double t);
    std::complex<double> ampliBH2crossed(int s2, int s1, int sl,
            double lminus[4], double lplus[4], int s, double kPrime[4],
            double k[4], double Qcal2, double t);
    std::complex<double> ampliVCS(int s2, int s1, int sl, double lminus[4],
            double lplus[4], int s, double kPrime[4], double k[4], double Qcal2,
            double Mll2);

    //Cross-sections:
    double crossSectionBH(int polariz, double xB, double Qcal2, double Mll2,
            double t, double thetal);
    double crossSectionVCS(int polariz, double xB, double Qcal2, double Mll2,
            double t, double thetal);
    double crossSectionInterf(int polariz, double xB, double Qcal2,
            double Mll2, double t, double thetal);

    //Auxiliary functions:
    std::complex<double> sKS(double r1[4], double r2[4]) const;
    std::complex<double> tKS(double r1[4], double r2[4]) const;
    std::complex<double> fFunction(int lambda, double k0[4], double k1[4],
            int lambdaPrime, double k2[4], double k3[4]) const;
    std::complex<double> gFunction(int lambda, double L[4], double R[4],
            double k[4]) const;
    std::complex<double> Yfunction(int s2, int s1) const;
    std::complex<double> Zfunction(int s2, int s1) const;
    std::complex<double> J2function(int s2, int s1) const;
    double MinkProd(double p[4], double q[4]) const;
    std::complex<double> Gfunction(int s, double kPrime[4], double V[4][4],
            double R[4][4], double k[4]) const;
    std::complex<double> jFunction(int mu, double p1[4], double p2[4],
            int helic) const;
    std::complex<double> J15plus(int s2, int s1) const;
    std::complex<double> J25plus(int s2, int s1) const;
    double LCperp(int mu, int nu); //Levi-Civita tensor for mu and nu perpendicular components

    void computeInternalVariables(double Mnucleon, double Ebeam, double t,
            double xB, double Qcal2, double Mll2, double phi, double phil,
            double thetal);

    //Everything needed to compute the light-like vectors r1, r2, rPrime1 & rPrime2 for proton momenta p1 = r1+r2 (incoming) and p2 = rPrime1+rPrime2 (outgoing)
    double m_DMSW_r1[4];
    double m_DMSW_r2[4];
    double m_DMSW_p1[4];
    double m_DMSW_rPrime1[4];
    double m_DMSW_rPrime2[4];
    double m_DMSW_p2[4];
    double m_DMSW_p2I[4]; //p2 in TRF-I frame, eqs 12 and 13 in BM2003
    double m_DMSW_modp2I; //modulus of 3D component of p2 in TRF-I frame, eq 12 in BM2003
    double m_DMSW_epsilon2; //squared of the epsilon variable define in paragraph above eq 8 in BM2003
    double m_DMSW_Q2; //Q2 is the squared of the incoming and outgoing photon momenta's average

    //Other momenta
    double m_DMSW_y; //y = p1*q1/(p1*k): paragraph below eq 6 in BM2003
    double m_DMSW_k[4]; //incoming electron's momentum in TRF-II, eq 20 in BM2003
    double m_DMSW_Delta[4]; // p2 - p1
    double m_DMSW_q2[4]; // outgoing-photon 4-vector in TRF-II, above eq 19 in BM2003
    double m_DMSW_q1[4]; // incoming-photon 4-vector in TRF-II, eq 19 in BM2003
    double m_DMSW_kPrime[4]; //outgoin-electron momentum in TRF-II frame
    double m_DMSW_lminus[4]; //4-momentum of muon, eq 23 in BM2003
    double m_DMSW_lplus[4]; //4-momentum of anti-muon

    //EM form factors for proton
    double m_DMSW_F1;
    double m_DMSW_F2;

    //Nucleon mass and electron charge in absolute value
    double m_DMSW_Mnucleon;
    double m_DMSW_charge_e;

    //eta and xi variables
//    double m_DMSW_eta;
    double m_DMSW_xi;

    //CFFs
    std::complex<double> m_cffH;
    std::complex<double> m_cffE;
    std::complex<double> m_cffHt;
    std::complex<double> m_cffEt;
    std::complex<double> m_cffHL;
    std::complex<double> m_cffEL;
};

} /* namespace PARTONS */

#endif /* DDVCS_PROCESS_DMSW22_H */

