#ifndef GAM2_CFF_STANDARD_H
#define GAM2_CFF_STANDARD_H

/**
 * @file GAM2CFFStandard.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @author Hervé MOUTARDE (SPhN / CEA Saclay)
 * @author Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date September 08, 2014
 * @version 1.0
 */

#include <complex>
#include <map>
#include <string>
#include <vector>

#include "../../../beans/automation/BaseObjectData.h"
#include "GAM2ConvolCoeffFunctionModule.h"

#include "../../../../../include/algorithm/LiSK/lisk.hpp"

namespace NumA {
class FunctionType1D;
} /* namespace NumA */
namespace PARTONS {
class PartonDistribution;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class GAM2CFFStandard
 *
 * Class used to compute CFF for photoproduction of diphoton
 * Used formulas are presented in
 * https://arxiv.org/pdf/2108.03426.pdf
 * (called the NLO paper),
 * in some cases we refer also to:
 * https://arxiv.org/pdf/2108.03426.pdf
 * (OG's MSc thesis)
 *
 * Available CFF types: H, E, Ht, Et.
 */
class GAM2CFFStandard: public GAM2ConvolCoeffFunctionModule {
public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    GAM2CFFStandard(const std::string &className);

    virtual GAM2CFFStandard* clone() const;

    /**
     * Default destructor.
     */
    virtual ~GAM2CFFStandard();

    virtual void resolveObjectDependencies();

    void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    // ##### GETTERS & SETTERS #####

    /**
     * Get alphaS module.
     */
    RunningAlphaStrongModule* getRunningAlphaStrongModule() const;

    /**
     * Set alphaS module.
     */
    void setRunningAlphaStrongModule(
            RunningAlphaStrongModule* pRunningAlphaStrongModule);

    // Set iEpsilon.
    void setIEpsilon(double iEps);

    // Get iEpsilon.
    double getIEpsilon() const;

    // Only evaluate Re (0), Im (1) or both (2) - set.
    void setReIm(int reim);

    // Only evaluate Re (0), Im (1) or both (2) - get.
    int getReIm() const;

    // Set if phi-dependemt.
    void setPhiDep(bool phiDep);

    // Get if phi-dependemt.
    bool getPhiDep() const;

    double Convol_NLO_V_x(double x, const std::vector<double>& params);
    double Convol_NLO_V_x_Sym(double x, const std::vector<double>& params);
    double Convol_NLO_V_x_Sym_Const(double x,
            const std::vector<double>& params);

    double Convol_NLO_V_xz(double x, double z,
            const std::vector<double>& params);

protected:

    /**
     * Copy constructor.
     * @param other
     */
    GAM2CFFStandard(const GAM2CFFStandard &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual std::complex<double> computeUnpolarized();
    virtual std::complex<double> computePolarized();

    double m_CF;                     ///< ( Nc^2 - 1 ) / ( 2 Nc ) (colour)
    double m_alphaSOver2Pi;

private:

    LiSK::LiSK<std::complex<double> > m_lisk;

    double gslIntegrationWrapper(NumA::FunctionType1D* functor,
            NumA::FunctionType1D* functorSym,
            NumA::FunctionType1D* functorSymConst,
            const std::vector<double>& range,
            const std::vector<double>& params);

    RunningAlphaStrongModule *m_pRunningAlphaStrongModule; ///< Related alphaS module.

    NumA::FunctionType1D* m_pConvol_NLO_V_x;
    NumA::FunctionType1D* m_pConvol_NLO_V_x_Sym;
    NumA::FunctionType1D* m_pConvol_NLO_V_x_Sym_Const;

    double m_quark_diagonal_V;
    double m_quark_diagonal_A;

    double computeCubedChargeAveragedGPD(
            const PartonDistribution &partonDistribution);

    void computeDiagonalGPD_V();
    void computeDiagonalGPD_A();

    void initFunctorsForIntegrations();

    // Trace \mathcal{A}, NLO paper, Eq. (21)
    double A(double s, const std::vector<double>& beta,
            const std::vector<double>& ee, const std::vector<double>& ek) const;

    // LO amplitude for single photons permutation, NLO paper, Eq. (20)
    std::complex<double> M0(double s, double x, double xi,
            const std::vector<double>& beta, const std::vector<double>& ee,
            const std::vector<double>& ek);

    // Sum of finite parts of amplitudes 2.L/R and 3.L/R, NLO paper, Eqs. (26)-(29)
    // NOTE: the factorization scale dependent term is included in the collinear part!
    std::complex<double> M23LR(double s, double x, double xi,
            const std::vector<double>& beta, const std::vector<double>& ee,
            const std::vector<double>& ek);

    // M3M amplitude, NLO paper, Eq. (30)
    std::complex<double> M3M(double s, double x, double xi,
            const std::vector<double>& beta, const std::vector<double>& ee,
            const std::vector<double>& ek);

    // Trace structures in amplitudes 4.L and 5.L,
    // NLO paper, Eqs. (43)-(44), see also the Mathematica notebook Traces
    // Tr_4/5L_Fnab is the trace multiplying the function F_nab (see NLO paper)
    double Tr_4L_F210(double xi, double s, const std::vector<double>& beta,
            const std::vector<double>& ee, const std::vector<double>& ek) const;
    double Tr_4L_F201(double xi, double s, const std::vector<double>& beta,
            const std::vector<double>& ee, const std::vector<double>& ek) const;
    double Tr_4L_F211(double xi, double s, const std::vector<double>& beta,
            const std::vector<double>& ee, const std::vector<double>& ek) const;
    double Tr_4L_F220(double xi, double s, const std::vector<double>& beta,
            const std::vector<double>& ee, const std::vector<double>& ek) const;
    double Tr_4L_F221(double xi, double s, const std::vector<double>& beta,
            const std::vector<double>& ee, const std::vector<double>& ek) const;
    double Tr_4L_F100(double xi, double s, const std::vector<double>& beta,
            const std::vector<double>& ee, const std::vector<double>& ek) const;
    double Tr_4L_F110(double xi, double s, const std::vector<double>& beta,
            const std::vector<double>& ee, const std::vector<double>& ek) const;
    double Tr_4L_G(double xi, double s, const std::vector<double>& beta,
            const std::vector<double>& ee, const std::vector<double>& ek) const;
    double Tr_5L_F201(double xi, double s, const std::vector<double>& beta,
            const std::vector<double>& ee, const std::vector<double>& ek) const;
    double Tr_5L_F210(double xi, double s, const std::vector<double>& beta,
            const std::vector<double>& ee, const std::vector<double>& ek) const;
    double Tr_5L_F211(double xi, double s, const std::vector<double>& beta,
            const std::vector<double>& ee, const std::vector<double>& ek) const;
    double Tr_5L_F220(double xi, double s, const std::vector<double>& beta,
            const std::vector<double>& ee, const std::vector<double>& ek) const;
    double Tr_5L_F221(double xi, double s, const std::vector<double>& beta,
            const std::vector<double>& ee, const std::vector<double>& ek) const;
    double Tr_5L_F100(double xi, double s, const std::vector<double>& beta,
            const std::vector<double>& ee, const std::vector<double>& ek) const;
    double Tr_5L_F110(double xi, double s, const std::vector<double>& beta,
            const std::vector<double>& ee, const std::vector<double>& ek) const;
    double Tr_5L_G(double xi, double s, const std::vector<double>& beta,
            const std::vector<double>& ee, const std::vector<double>& ek) const;

    // Amplitudes 4.L/R and 5.L/R, NLO paper, Eqs. (43)-(44)
    // For .R amplitudes, see the comment below Eq. (44) in the NLO paper
    // ATTENTION! Terms from the last lines of (43)-(44) are included in M_scale

    std::complex<double> M4L(double s, double x, double xi,
            const std::vector<double>& beta, const std::vector<double>& ee,
            const std::vector<double>& ek);
    std::complex<double> M5L(double s, double x, double xi,
            const std::vector<double>& beta, const std::vector<double>& ee,
            const std::vector<double>& ek);
    std::complex<double> M4R(double s, double x, double xi,
            const std::vector<double>& beta, const std::vector<double>& ee,
            const std::vector<double>& ek);
    std::complex<double> M5R(double s, double x, double xi,
            const std::vector<double>& beta, const std::vector<double>& ee,
            const std::vector<double>& ek);

    std::complex<double> M4L(double s, double x, double xi,
            const std::vector<double>& beta, const std::vector<double>& ee,
            const std::vector<double>& ek, double z);
    std::complex<double> M5L(double s, double x, double xi,
            const std::vector<double>& beta, const std::vector<double>& ee,
            const std::vector<double>& ek, double z);
    std::complex<double> M4R(double s, double x, double xi,
            const std::vector<double>& beta, const std::vector<double>& ee,
            const std::vector<double>& ek, double z);
    std::complex<double> M5R(double s, double x, double xi,
            const std::vector<double>& beta, const std::vector<double>& ee,
            const std::vector<double>& ek, double z);

    // Artefact of using a different definition of the hard scale in OG's MSc thesis,
    // see the comment after Eq. (44) in NLO paper
    std::complex<double> M_scale(double s, double x, double xi,
            std::vector<double> beta, std::vector<double> ee,
            std::vector<double> ek);

    // The collinear term.
    // For now, it implements formulas (3.105)-(3.106) from OG's MSc thesis
    // In particular, it is the contribution from the single permutation of photons!
    // Do not mistake it with Eq. (40) in NLO paper.
    // TODO re-write it so that it implements Eq. (40) from NLO paper
    std::complex<double> Ccoll(double s, double x, double xi,
            std::vector<double> beta, std::vector<double> ee,
            std::vector<double> ek);

    // Vector NLO amplitude - a single permutation of photons
    // See the description at the beginning of IV.A in NLO paper
    double NLO_V_permutation(double x, const std::vector<double>& params);

    // Vector NLO amplitude - a single permutation of photons
    // See the description at the beginning of IV.A in NLO paper
    double NLO_V_permutation(double x, double z,
            const std::vector<double>& params);

    // The full NLO vector amplitude
    double NLO_V(double x, const std::vector<double>& params);

    // The full NLO vector amplitude
    // z-dependent part
    double NLO_V(double x, double z, const std::vector<double>& params);

    // Functions F_nab
    std::complex<double> F100(double x, double xi,
            const std::vector<double>& beta, double s);
    std::complex<double> F110(double x, double xi,
            const std::vector<double>& beta, double s);
    std::complex<double> F201(double x, double xi,
            const std::vector<double>& beta, double s);
    std::complex<double> F210(double x, double xi,
            const std::vector<double>& beta, double s);
    std::complex<double> F211(double x, double xi,
            const std::vector<double>& beta, double s);
    std::complex<double> F220(double x, double xi,
            const std::vector<double>& beta, double s);
    std::complex<double> F221(double x, double xi,
            const std::vector<double>& beta, double s);

    std::complex<double> G(double x, double xi, const std::vector<double>& beta,
            double z);

    // Sign
    double sgn(double x) const;

    // iEps
    std::complex<double> m_iepsilon;

    // only evaluate Re (0), Im (1) or both (2) - get.
    int m_reim;

    //make phi-dependent.
    bool m_phiDep;

};

struct GAM2CFFStandardIntegrationParameters {

    GAM2CFFStandard* m_GAM2CFFStandardGAM2CFFStandard;
    std::vector<double> const* m_parameters;
};

double GAM2CFFStandardIntegrationFunction(double* x, size_t dim, void* p);

} /* namespace PARTONS */

#endif /* GAM2_CFF_STANDARD_H */
