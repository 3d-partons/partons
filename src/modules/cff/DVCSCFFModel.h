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

#include "../CFFModule.h"

class GPDOutputData;

class DVCSCFFModel: public CFFModule {
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

    static const std::string moduleID; ///< Unique ID to automatically register the module in the factory.

    DVCSCFFModel(const std::string &className);

    virtual DVCSCFFModel* clone() const;

    virtual ~DVCSCFFModel();

    virtual double functionsToIntegrate(const double * x,
            const double * parameters = 0);

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

    virtual std::complex<double> computeUnpolarized();
    virtual std::complex<double> computePolarized();

    virtual void computeSubtractionFunctionsV();

    double m_realPartSubtractQuark;
    double m_imaginaryPartSubtractQuark;
    double m_realPartSubtractGluon;
    double m_imaginaryPartSubtractGluon;

    double m_Zeta;
    double m_Q;
    double m_alphaSOver2Pi;

private:

    double m_quarkDiagonal;
    double m_gluonDiagonal;

    double m_CF;                     ///< ( Nc^2 - 1 ) / ( 2 Nc ) (colour)

    //ROOT::Math::Integrator m_integrator;

    std::complex<double> KernelQuarkV(double x); ///< T^{q, V/A}, appendix A, eq. (A1)
    std::complex<double> KernelGluonV(double x); ///< T^{g, V/A}, appendix A, eq. (A1)

    std::complex<double> KernelQuarkNLOV(double x); ///< T^{q, NLO, V}, appendix A, eq. (A2)
    double ConvolReKernelQuark1V(const double x); ///< eq. (8), real part of amplitude, \int_0^zeta
    double ConvolReKernelQuark2V(const double x); ///< eq. (8), real part of amplitude, \int_zeta^1
    double ConvolImKernelQuarkV(const double x); ///< eq. (8), imaginary part of amplitude
    double ConvolReKernelGluon1V(const double x); ///< eq. (9), real part of amplitude, \int_0^zeta
    double ConvolReKernelGluon2V(const double x); ///< eq. (9), real part of amplitude, \int_zeta^1
    virtual double ConvolImKernelGluonV(const double x); ///< eq. (9), imaginary part of amplitude

    std::complex<double> KernelQuarkA(double x); ///< T^{q, V/A}, appendix A, eq. (A1)
    std::complex<double> KernelGluonA(double x); ///< T^{g, V/A}, appendix A, eq. (A1)
    std::complex<double> KernelQuarkNLOA(double x); ///< T^{q, NLO, A}, appendix A, eq. (A2)

    double ConvolReKernelQuark1A(const double x); ///< eq. (8), real part of amplitude, \int_0^zeta
    double ConvolReKernelQuark2A(const double x); ///< eq. (8), real part of amplitude, \int_zeta^1
    double ConvolImKernelQuarkA(const double x); ///< eq. (8), imaginary part of amplitude
    double ConvolReKernelGluon1A(const double x); ///< eq. (9), real part of amplitude, \int_0^zeta
    double ConvolReKernelGluon2A(const double x); ///< eq. (9), real part of amplitude, \int_zeta^1
    double ConvolImKernelGluonA(const double x); ///< eq. (9), imaginary part of amplitude

    virtual void computeDiagonalGPD();

    void computeSubtractionFunctionsA();

    std::complex<double> computeIntegralsV();
    std::complex<double> computeIntegralsA();

    double computeSquareChargeAveragedGPD(GPDOutputData &gpdOutputData);

};

#endif /* DVCS_CFF_MODULE_H */
