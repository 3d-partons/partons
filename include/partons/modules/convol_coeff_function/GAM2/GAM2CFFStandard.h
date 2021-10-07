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
 * TODO: Add description.
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

    double gslIntegrationWrapper(NumA::FunctionType1D* functor,
            const std::vector<double>& range, std::vector<double>& params) ;

    RunningAlphaStrongModule *m_pRunningAlphaStrongModule; ///< Related alphaS module.

    NumA::FunctionType1D* m_pConvol_NLO_V_Re;
    NumA::FunctionType1D* m_pConvol_NLO_V_Im;

    double m_quark_diagonal_V;
    double m_quark_diagonal_A;

    double computeCubedChargeAveragedGPD(
            const PartonDistribution &partonDistribution);

    void computeDiagonalGPD_V();
    void computeDiagonalGPD_A();

    void initFunctorsForIntegrations(); ///< Initialize functors.

   // double exampleIntegration(double x, std::vector<double> params); ///< Exemplary integration.

    // Trace \mathcal{A}, see Eq. 25
    double A(double, std::vector<double>, std::vector<double>, std::vector<double>);

    // LO amplitude, see Eq. 24
    std::complex<double> M0(double, double, double,
            std::vector<double>, std::vector<double>, std::vector<double>);

    // Sum of finite parts of amplitudes 2.L/R and 3.L/R, see Eqs. 30-33
    std::complex<double> M23LR(double, double, double,
            std::vector<double>, std::vector<double>, std::vector<double>);

    // Eq. 49
    std::complex <double> M3M(double, double, double,
                              std::vector<double>, std::vector<double>, std::vector<double>);

    // Trace structures in amplitudes 4.L and 5.L - Eqs. 50 and 51
    // Tr_4/5L_Fnab stands by the function F(n, a, b, ...)
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

    // Functions F(n, a, b, ...)

    std::complex<double> F100(std::complex<double> a, std::complex<double> b, std::complex<double> c);
    std::complex<double> F110(std::complex<double> a, std::complex<double> b, std::complex<double> c);
    std::complex<double> F210(std::complex<double> a, std::complex<double> b, std::complex<double> c);
    std::complex<double> F211(std::complex<double> a, std::complex<double> b, std::complex<double> c);
    std::complex<double> F220(std::complex<double> a, std::complex<double> b, std::complex<double> c);
    std::complex<double> F221(std::complex<double> a, std::complex<double> b, std::complex<double> c);
    std::complex<double> G(std::complex<double> a, std::complex<double> b, std::complex<double> c);

    std::complex<double> F100x(double x, double xi, double beta0, double beta1);

    std::complex<double> F110x(double x, double xi, double beta0, double beta1);

    std::complex<double> F210x(double x, double xi, double beta0, double beta1);

    std::complex<double> F201x(double x, double xi, double beta0, double beta1);

    std::complex<double> F211x(double x, double xi, double beta0, double beta1);

    // Amplitudes 4.L/R and 5.L/R, see Eqs. 50 and 51

    std::complex<double> M4L(double s, double x, double xi,
                      std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    std::complex<double> M5L(double s, double x, double xi,
                      std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    std::complex<double> M4R(double s, double x, double xi,
                      std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);
    std::complex<double> M5R(double s, double x, double xi,
                      std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);

    // Eq. 52
    std::complex<double> M_scale(double s, double x, double xi,
            std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);

    // The collinear term
    std::complex<double> Ccoll(double s, double x, double xi,
            std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);

    // Vector NLO amplitude - a single permutation of photons
    std::complex<double> NLO_V_permutation(double s, double x, double xi, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek);

    // The full NLO vector amplitude
    std::complex<double> Convol_NLO_V(double x, std::vector<double> params);

    double Convol_NLO_V_Re(double x, std::vector<double> params);

    double Convol_NLO_V_Im(double x, std::vector<double> params);


};

} /* namespace PARTONS */

#endif /* GAM2_CFF_STANDARD_H */
