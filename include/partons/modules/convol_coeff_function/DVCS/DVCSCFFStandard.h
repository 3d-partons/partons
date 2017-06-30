#ifndef DVCSCFFSTANDARD_H
#define DVCSCFFSTANDARD_H

/**
 * @file DVCSCFFStandard.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @author Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date September 08, 2014
 * @version 1.0
 */

#include <complex>
#include <string>
#include <vector>

#include "DVCSConvolCoeffFunctionModule.h"

namespace NumA {
class FunctionType1D;
} /* namespace NumA */

class PartonDistribution;

/**
 * @class DVCSCFFStandard
 *
 * TODO: Add description.
 *
 * Available CFF types: H, E, Ht, Et.
 */
class DVCSCFFStandard: public DVCSConvolCoeffFunctionModule {
public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    DVCSCFFStandard(const std::string &className);

    virtual DVCSCFFStandard* clone() const;

    /**
     * Default destructor.
     */
    virtual ~DVCSCFFStandard();

    virtual void resolveObjectDependencies();

    // ##### GETTERS & SETTERS #####

protected:
    /**
     * Copy constructor.
     * @param other
     */
    DVCSCFFStandard(const DVCSCFFStandard &other);

    double m_logQ2OverMu2;

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
    double m_Q;
    double m_alphaSOver2Pi;

    virtual std::complex<double> computeUnpolarized();
    virtual std::complex<double> computePolarized();

private:

    double m_quarkDiagonal;
    double m_gluonDiagonal;

    virtual void computeDiagonalGPD();

    virtual std::complex<double> KernelQuarkV(double x); ///< T^{q, V/A}, appendix A, eq. (A1)
    virtual std::complex<double> KernelGluonV(double x); ///< T^{g, V/A}, appendix A, eq. (A1)

    virtual double ConvolReKernelQuark1V(double x, std::vector<double> params); ///< eq. (8), real part of amplitude, \int_0^zeta
    virtual double ConvolReKernelQuark2V(double x, std::vector<double> params); ///< eq. (8), real part of amplitude, \int_zeta^1
    virtual double ConvolImKernelQuarkV(double x, std::vector<double> params); ///< eq. (8), imaginary part of amplitude
    virtual double ConvolReKernelGluon1V(double x, std::vector<double> params); ///< eq. (9), real part of amplitude, \int_0^zeta
    virtual double ConvolReKernelGluon2V(double x, std::vector<double> params); ///< eq. (9), real part of amplitude, \int_zeta^1
    virtual double ConvolImKernelGluonV(double x, std::vector<double> params); ///< eq. (9), imaginary part of amplitude

    virtual std::complex<double> KernelQuarkA(double x); ///< T^{q, V/A}, appendix A, eq. (A1)
    virtual std::complex<double> KernelGluonA(double x); ///< T^{g, V/A}, appendix A, eq. (A1)

    virtual double ConvolReKernelQuark1A(double x, std::vector<double> params); ///< eq. (8), real part of amplitude, \int_0^zeta
    virtual double ConvolReKernelQuark2A(double x, std::vector<double> params); ///< eq. (8), real part of amplitude, \int_zeta^1
    virtual double ConvolImKernelQuarkA(double x, std::vector<double> params); ///< eq. (8), imaginary part of amplitude
    virtual double ConvolReKernelGluon1A(double x, std::vector<double> params); ///< eq. (9), real part of amplitude, \int_0^zeta
    virtual double ConvolReKernelGluon2A(double x, std::vector<double> params); ///< eq. (9), real part of amplitude, \int_zeta^1
    virtual double ConvolImKernelGluonA(double x, std::vector<double> params); ///< eq. (9), imaginary part of amplitude

    std::complex<double> computeIntegralsV();
    std::complex<double> computeIntegralsA();

    double computeSquareChargeAveragedGPD(
            const PartonDistribution &partonDistribution);

    NumA::FunctionType1D* m_pConvolReKernelQuark1V;
    NumA::FunctionType1D* m_pConvolReKernelQuark2V;
    NumA::FunctionType1D* m_pConvolImKernelQuarkV;
    NumA::FunctionType1D* m_pConvolReKernelGluon1V;
    NumA::FunctionType1D* m_pConvolReKernelGluon2V;
    NumA::FunctionType1D* m_pConvolImKernelGluonV;
    NumA::FunctionType1D* m_pConvolReKernelQuark1A;
    NumA::FunctionType1D* m_pConvolReKernelQuark2A;
    NumA::FunctionType1D* m_pConvolImKernelQuarkA;
    NumA::FunctionType1D* m_pConvolReKernelGluon1A;
    NumA::FunctionType1D* m_pConvolReKernelGluon2A;
    NumA::FunctionType1D* m_pConvolImKernelGluonA;

    void initFunctorsForIntegrations(); ///< Initialize functors.
};

#endif /* DVCSCFFSTANDARD_H */
