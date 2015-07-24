#ifndef DVCS_CFF_MODULE_H
#define DVCS_CFF_MODULE_H

/**
 * @file DVCSCFFModel.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @author Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date 08 September 2014
 * @version 1.0
 *
 * @class DVCSCFFModel
 *
 * @brief
 */

#include <complex>
#include <string>

#include "../../../beans/parton_distribution/PartonDistribution.h"
#include "DVCSConvolCoeffFunctionModule.h"

class GPDOutputData;

class DVCSCFFModel: public DVCSConvolCoeffFunctionModule {
public:
    enum FunctionNameToIntegrate {
        CONVOL_RE_KERNEL_QUARK_1V,
        CONVOL_RE_KERNEL_QUARK_2V,
        CONVOL_IM_KERNEL_QUARK_V,
        CONVOL_RE_KERNEL_GLUON_1V,
        CONVOL_RE_KERNEL_GLUON_2V,
        CONVOL_IM_KERNEL_GLUON_V,
        CONVOL_RE_KERNEL_QUARK_1A,
        CONVOL_RE_KERNEL_QUARK_2A,
        CONVOL_IM_KERNEL_QUARK_A,
        CONVOL_RE_KERNEL_GLUON_1A,
        CONVOL_RE_KERNEL_GLUON_2A,
        CONVOL_IM_KERNEL_GLUON_A
    };

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    DVCSCFFModel(const std::string &className);

    virtual DVCSCFFModel* clone() const;

    virtual ~DVCSCFFModel();

    virtual double functionsToIntegrate(double * x, double * parameters = 0);

    virtual void init();

    // ##### GETTERS & SETTERS #####

protected:
    /**
     * Copy constructor
     * @param other
     */
    DVCSCFFModel(const DVCSCFFModel &other);

    double m_logQ2OverMu2;
    unsigned int m_nbOfActiveFlavour;

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

private:

    double m_quarkDiagonal;
    double m_gluonDiagonal;

    //ROOT::Math::Integrator m_integrator;

    virtual void computeDiagonalGPD();

    virtual std::complex<double> KernelQuarkV(double x); ///< T^{q, V/A}, appendix A, eq. (A1)
    virtual std::complex<double> KernelGluonV(double x); ///< T^{g, V/A}, appendix A, eq. (A1)

    virtual double ConvolReKernelQuark1V(const double x); ///< eq. (8), real part of amplitude, \int_0^zeta
    virtual double ConvolReKernelQuark2V(const double x); ///< eq. (8), real part of amplitude, \int_zeta^1
    virtual double ConvolImKernelQuarkV(const double x); ///< eq. (8), imaginary part of amplitude
    virtual double ConvolReKernelGluon1V(const double x); ///< eq. (9), real part of amplitude, \int_0^zeta
    virtual double ConvolReKernelGluon2V(const double x); ///< eq. (9), real part of amplitude, \int_zeta^1
    virtual double ConvolImKernelGluonV(const double x); ///< eq. (9), imaginary part of amplitude

    virtual std::complex<double> KernelQuarkA(double x); ///< T^{q, V/A}, appendix A, eq. (A1)
    virtual std::complex<double> KernelGluonA(double x); ///< T^{g, V/A}, appendix A, eq. (A1)

    virtual double ConvolReKernelQuark1A(const double x); ///< eq. (8), real part of amplitude, \int_0^zeta
    virtual double ConvolReKernelQuark2A(const double x); ///< eq. (8), real part of amplitude, \int_zeta^1
    virtual double ConvolImKernelQuarkA(const double x); ///< eq. (8), imaginary part of amplitude
    virtual double ConvolReKernelGluon1A(const double x); ///< eq. (9), real part of amplitude, \int_0^zeta
    virtual double ConvolReKernelGluon2A(const double x); ///< eq. (9), real part of amplitude, \int_zeta^1
    virtual double ConvolImKernelGluonA(const double x); ///< eq. (9), imaginary part of amplitude

    std::complex<double> computeIntegralsV();
    std::complex<double> computeIntegralsA();

    virtual std::complex<double> computeUnpolarized();
    virtual std::complex<double> computePolarized();

    double computeSquareChargeAveragedGPD(
            const PartonDistribution &partonDistribution);

};

#endif /* DVCS_CFF_MODULE_H */
