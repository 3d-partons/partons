#ifndef TCS_CFF_STANDARD_H
#define TCS_CFF_STANDARD_H

/**
 * @file TCSCFFStandard.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @author Hervé MOUTARDE (SPhN / CEA Saclay)
 * @author Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date September 08, 2014
 * @version 1.0
 */

#include <stddef.h>
#include <complex>
#include <map>
#include <string>
#include <vector>

#include "../../../beans/automation/BaseObjectData.h"
#include "TCSConvolCoeffFunctionModule.h"

namespace NumA {
class FunctionType1D;
} /* namespace NumA */
namespace PARTONS {
class PartonDistribution;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class TCSCFFStandard
 *
 * TODO: Add description.
 *
 * Available CFF types: H, E, Ht, Et.
 */
class TCSCFFStandard: public TCSConvolCoeffFunctionModule {
public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    TCSCFFStandard(const std::string &className);

    virtual TCSCFFStandard* clone() const;

    /**
     * Default destructor.
     */
    virtual ~TCSCFFStandard();

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
    TCSCFFStandard(const TCSCFFStandard &other);

    double m_logQ2PrimOverMu2;

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual std::complex<double> KernelGluonNLOA(double x); ///< T^{g, NLO, A}, appendix A, eq. (A2)
    virtual std::complex<double> KernelGluonNLOV(double x); ///< T^{g, NLO, V}, appendix A, eq. (A2)

    virtual std::complex<double> KernelQuarkNLOV(double x); ///< T^{q, NLO, V}, appendix A, eq. (A2)
    virtual std::complex<double> KernelQuarkNLOA(double x); ///< T^{q, NLO, A}, appendix A, eq. (A2)

    virtual void computeSubtractionFunctionsV();
    virtual void computeSubtractionFunctionsA();

    double m_realPartSubtractQuark;
    double m_imaginaryPartSubtractQuark;
    double m_realPartSubtractGluon;
    double m_imaginaryPartSubtractGluon;
    double m_CF;                     ///< ( Nc^2 - 1 ) / ( 2 Nc ) (colour)

    double m_Zeta;
    double m_QPrim;
    double m_alphaSOver2Pi;

    virtual std::complex<double> computeUnpolarized();
    virtual std::complex<double> computePolarized();

protected:

    double m_quarkDiagonal;
    double m_gluonDiagonal;

    virtual void computeDiagonalGPD();
    virtual double computeSquareChargeAveragedGPD(
            const PartonDistribution &partonDistribution);

private:

    size_t m_nf; ///< Number of active flavors.
    RunningAlphaStrongModule *m_pRunningAlphaStrongModule; ///< Related alphaS module.

    virtual std::complex<double> KernelQuarkV(double x); ///< T^{q, V/A}, appendix A, eq. (A1)
    virtual std::complex<double> KernelGluonV(double x); ///< T^{g, V/A}, appendix A, eq. (A1)

    virtual double ConvolReKernelQuark1V(double x, std::vector<double> params); ///< eq. (8), real part of amplitude, \int_0^zeta
    virtual double ConvolReKernelQuark2V(double x, std::vector<double> params); ///< eq. (8), real part of amplitude, \int_zeta^1
    virtual double ConvolImKernelQuark1V(double x, std::vector<double> params); ///< eq. (8), imaginary part of amplitude, \int_0^zeta
    virtual double ConvolImKernelQuark2V(double x, std::vector<double> params); ///< eq. (8), imaginary part of amplitude, \int_zeta^1
    virtual double ConvolReKernelGluon1V(double x, std::vector<double> params); ///< eq. (9), real part of amplitude, \int_0^zeta
    virtual double ConvolReKernelGluon2V(double x, std::vector<double> params); ///< eq. (9), real part of amplitude, \int_zeta^1
    virtual double ConvolImKernelGluon1V(double x, std::vector<double> params); ///< eq. (9), imaginary part of amplitude, \int_0^zeta
    virtual double ConvolImKernelGluon2V(double x, std::vector<double> params); ///< eq. (9), imaginary part of amplitude, \int_zeta^1

    virtual std::complex<double> KernelQuarkA(double x); ///< T^{q, V/A}, appendix A, eq. (A1)
    virtual std::complex<double> KernelGluonA(double x); ///< T^{g, V/A}, appendix A, eq. (A1)

    virtual double ConvolReKernelQuark1A(double x, std::vector<double> params); ///< eq. (8), real part of amplitude, \int_0^zeta
    virtual double ConvolReKernelQuark2A(double x, std::vector<double> params); ///< eq. (8), real part of amplitude, \int_zeta^1
    virtual double ConvolImKernelQuark1A(double x, std::vector<double> params); ///< eq. (8), imaginary part of amplitude, \int_0^zeta
    virtual double ConvolImKernelQuark2A(double x, std::vector<double> params); ///< eq. (8), imaginary part of amplitude, \int_zeta^1
    virtual double ConvolReKernelGluon1A(double x, std::vector<double> params); ///< eq. (9), real part of amplitude, \int_0^zeta
    virtual double ConvolReKernelGluon2A(double x, std::vector<double> params); ///< eq. (9), real part of amplitude, \int_zeta^1
    virtual double ConvolImKernelGluon1A(double x, std::vector<double> params); ///< eq. (9), imaginary part of amplitude, \int_0^zeta
    virtual double ConvolImKernelGluon2A(double x, std::vector<double> params); ///< eq. (9), imaginary part of amplitude, \int_zeta^1

    std::complex<double> computeIntegralsV();
    std::complex<double> computeIntegralsA();

    NumA::FunctionType1D* m_pConvolReKernelQuark1V;
    NumA::FunctionType1D* m_pConvolReKernelQuark2V;
    NumA::FunctionType1D* m_pConvolImKernelQuark1V;
    NumA::FunctionType1D* m_pConvolImKernelQuark2V;
    NumA::FunctionType1D* m_pConvolReKernelGluon1V;
    NumA::FunctionType1D* m_pConvolReKernelGluon2V;
    NumA::FunctionType1D* m_pConvolImKernelGluon1V;
    NumA::FunctionType1D* m_pConvolImKernelGluon2V;
    NumA::FunctionType1D* m_pConvolReKernelQuark1A;
    NumA::FunctionType1D* m_pConvolReKernelQuark2A;
    NumA::FunctionType1D* m_pConvolImKernelQuark1A;
    NumA::FunctionType1D* m_pConvolImKernelQuark2A;
    NumA::FunctionType1D* m_pConvolReKernelGluon1A;
    NumA::FunctionType1D* m_pConvolReKernelGluon2A;
    NumA::FunctionType1D* m_pConvolImKernelGluon1A;
    NumA::FunctionType1D* m_pConvolImKernelGluon2A;

    void initFunctorsForIntegrations(); ///< Initialize functors.
};

} /* namespace PARTONS */

#endif /* TCS_CFF_STANDARD_H */
