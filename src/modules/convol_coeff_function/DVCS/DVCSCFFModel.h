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

class DVCSCFFModel: public DVCSConvolCoeffFunctionModule {
public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    DVCSCFFModel(const std::string &className);

    virtual DVCSCFFModel* clone() const;

    virtual ~DVCSCFFModel();

    virtual void init();

    // ##### GETTERS & SETTERS #####

protected:
    /**
     * Copy constructor
     * @param other
     */
    DVCSCFFModel(const DVCSCFFModel &other);

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

private:

    double m_quarkDiagonal;
    double m_gluonDiagonal;

    virtual void computeDiagonalGPD();

    virtual std::complex<double> KernelQuarkV(double x); ///< T^{q, V/A}, appendix A, eq. (A1)
    virtual std::complex<double> KernelGluonV(double x); ///< T^{g, V/A}, appendix A, eq. (A1)

    virtual double ConvolReKernelQuark1V(double* x, double* params); ///< eq. (8), real part of amplitude, \int_0^zeta
    virtual double ConvolReKernelQuark2V(double* x, double* params); ///< eq. (8), real part of amplitude, \int_zeta^1
    virtual double ConvolImKernelQuarkV(double* x, double* params); ///< eq. (8), imaginary part of amplitude
    virtual double ConvolReKernelGluon1V(double* x, double* params); ///< eq. (9), real part of amplitude, \int_0^zeta
    virtual double ConvolReKernelGluon2V(double* x, double* params); ///< eq. (9), real part of amplitude, \int_zeta^1
    virtual double ConvolImKernelGluonV(double* x, double* params); ///< eq. (9), imaginary part of amplitude

    virtual std::complex<double> KernelQuarkA(double x); ///< T^{q, V/A}, appendix A, eq. (A1)
    virtual std::complex<double> KernelGluonA(double x); ///< T^{g, V/A}, appendix A, eq. (A1)

    virtual double ConvolReKernelQuark1A(double* x, double* params); ///< eq. (8), real part of amplitude, \int_0^zeta
    virtual double ConvolReKernelQuark2A(double* x, double* params); ///< eq. (8), real part of amplitude, \int_zeta^1
    virtual double ConvolImKernelQuarkA(double* x, double* params); ///< eq. (8), imaginary part of amplitude
    virtual double ConvolReKernelGluon1A(double* x, double* params); ///< eq. (9), real part of amplitude, \int_0^zeta
    virtual double ConvolReKernelGluon2A(double* x, double* params); ///< eq. (9), real part of amplitude, \int_zeta^1
    virtual double ConvolImKernelGluonA(double* x, double* params); ///< eq. (9), imaginary part of amplitude

    std::complex<double> computeIntegralsV();
    std::complex<double> computeIntegralsA();

    virtual std::complex<double> computeUnpolarized();
    virtual std::complex<double> computePolarized();

    double computeSquareChargeAveragedGPD(
            const PartonDistribution &partonDistribution);

};

#endif /* DVCS_CFF_MODULE_H */
