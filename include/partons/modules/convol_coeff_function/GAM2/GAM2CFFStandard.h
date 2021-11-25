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

struct GAM2CFFStandardIntegrationParameters {

    NumA::FunctionType1D* m_pIntegrator;
    std::vector<double> m_parameters;
};

double GAM2CFFStandardIntegrationFunction(double x, void* p);

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

    double gslIntegrationWrapper(NumA::FunctionType1D* functor, NumA::FunctionType1D* functorSym, NumA::FunctionType1D* functorSymConst,
            const std::vector<double>& range, std::vector<double>& params) ;

    RunningAlphaStrongModule *m_pRunningAlphaStrongModule; ///< Related alphaS module.

    NumA::FunctionType1D* m_pConvol_NLO_V_Re;
    NumA::FunctionType1D* m_pConvol_NLO_V_Im;
    NumA::FunctionType1D* m_pConvol_NLO_V_Re_Sym;
    NumA::FunctionType1D* m_pConvol_NLO_V_Im_Sym;
    NumA::FunctionType1D* m_pConvol_NLO_V_Re_Sym_Const;
    NumA::FunctionType1D* m_pConvol_NLO_V_Im_Sym_Const;

    double m_quark_diagonal_V;
    double m_quark_diagonal_A;

    double computeCubedChargeAveragedGPD(
            const PartonDistribution &partonDistribution);

    void computeDiagonalGPD_V();
    void computeDiagonalGPD_A();

    void initFunctorsForIntegrations(); ///< Initialize functors.

   // double exampleIntegration(double x, std::vector<double> params); ///< Exemplary integration.

    // Trace \mathcal{A}, NLO paper, Eq. (21)
    double A(double, std::vector<double>, std::vector<double>, std::vector<double>);

    // LO amplitude for single photons permutation, NLO paper, Eq. (20)
    std::complex<double> M0(double, double, double,
            std::vector<double>, std::vector<double>, std::vector<double>);

    // Sum of finite parts of amplitudes 2.L/R and 3.L/R, NLO paper, Eqs. (26)-(29)
    // NOTE: the factorization scale dependent term is included in the collinear part!
    std::complex<double> M23LR(double, double, double,
            std::vector<double>, std::vector<double>, std::vector<double>);

    // OG's MSc thesis, Eq. (B.8)
    std::complex <double> M3M(double, double, double,
                              std::vector<double>, std::vector<double>, std::vector<double>);

    // Trace structures in amplitudes 4.L and 5.L,
    // NLO paper, Eqs. (43)-(44), see also the Mathematica notebook Traces
    // Tr_4/5L_Fnab is the trace multiplying the function F_nab (see NLO paper)

    double Tr_4L_F210(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    double Tr_4L_F201(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    double Tr_4L_F211(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    double Tr_4L_F220(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    double Tr_4L_F221(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    double Tr_4L_F100(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    double Tr_4L_F110(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    double Tr_4L_G(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    double Tr_5L_F201(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    double Tr_5L_F210(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    double Tr_5L_F211(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    double Tr_5L_F220(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    double Tr_5L_F221(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    double Tr_5L_F100(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    double Tr_5L_F110(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    double Tr_5L_G(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);

    // Functions F_nab

    std::complex<double> F100(std::complex<double> a, std::complex<double> b, std::complex<double> c);
    std::complex<double> F110(std::complex<double> a, std::complex<double> b, std::complex<double> c);
    std::complex<double> F210(std::complex<double> a, std::complex<double> b, std::complex<double> c);
    std::complex<double> F211(std::complex<double> a, std::complex<double> b, std::complex<double> c);
    std::complex<double> F220(std::complex<double> a, std::complex<double> b, std::complex<double> c);
    std::complex<double> F221(std::complex<double> a, std::complex<double> b, std::complex<double> c);
    std::complex<double> G(std::complex<double> a, std::complex<double> b, std::complex<double> c);
    std::complex<double> GNew(double x, double xi, const std::vector<double>& beta);
    std::complex<double> F201New(double x, double xi, const std::vector<double>& beta);
    std::complex<double> F210New(double x, double xi, const std::vector<double>& beta);
    std::complex<double> F110New(double x, double xi, const std::vector<double>& beta);
    std::complex<double> F100New(double x, double xi, const std::vector<double>& beta);



    // Amplitudes 4.L/R and 5.L/R, NLO paper, Eqs. (43)-(44)
    // For .R amplitudes, see the comment below Eq. (44) in the NLO paper
    // ATTENTION! Terms from the last lines of (43)-(44) are included in M_scale

    std::complex<double> M4L(double s, double x, double xi,
                      std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    std::complex<double> M5L(double s, double x, double xi,
                      std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    std::complex<double> M4R(double s, double x, double xi,
                      std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    std::complex<double> M5R(double s, double x, double xi,
                      std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);

    // Artefact of using a different definition of the hard scale in OG's MSc thesis,
    // see the comment after Eq. (44) in NLO paper
    std::complex<double> M_scale(double s, double x, double xi,
            std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);

    // The collinear term.
    // For now, it implements formulas (3.105)-(3.106) from OG's MSc thesis
    // In particular, it is the contribution from the single permutation of photons!
    // Do not mistake it with Eq. (40) in NLO paper.
    // TODO re-write it so that it implements Eq. (40) from NLO paper
    std::complex<double> Ccoll(double s, double x, double xi,
            std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);

    // Vector NLO amplitude - a single permutation of photons
    // See the description at the beginning of IV.A in NLO paper
    std::complex<double> NLO_V_permutation(double s, double x, double xi, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);

    // The full NLO vector amplitude
    std::complex<double> Convol_NLO_V(double x, std::vector<double> params);

    double Convol_NLO_V_Re(double x, std::vector<double> params);
    double Convol_NLO_V_Im(double x, std::vector<double> params);
    double Convol_NLO_V_Re_Sym(double x, std::vector<double> params);
    double Convol_NLO_V_Im_Sym(double x, std::vector<double> params);
    double Convol_NLO_V_Re_Sym_Const(double x, std::vector<double> params);
    double Convol_NLO_V_Im_Sym_Const(double x, std::vector<double> params);


};

} /* namespace PARTONS */

#endif /* GAM2_CFF_STANDARD_H */
