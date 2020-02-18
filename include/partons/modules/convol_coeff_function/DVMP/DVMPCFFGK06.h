#ifndef DVMP_CFF_GK06_H
#define DVMP_CFF_GK06_H

/**
 * @file DVMPCFFGK06.h
 * @author Kemal Tezgin
 * @date October 21, 2019
 * @version 1.0
 */

#include <stddef.h>
#include <complex>
#include <string>

#include "../../../beans/QuarkFlavor.h"
#include "DVMPConvolCoeffFunctionModule.h"

namespace PARTONS {

/**
 * @class DVMPCFFGK06
 *
 * This module calculates DVMP Compton Form Factors ...
 *
 * Available CFF types: H, E, Ht, Et, ...
 */
class DVMPCFFGK06: public DVMPConvolCoeffFunctionModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    DVMPCFFGK06(const std::string &className);

    virtual DVMPCFFGK06* clone() const;

    /**
     * Default destructor.
     */
    virtual ~DVMPCFFGK06();

    virtual void resolveObjectDependencies();

    friend double DVMPCFFGK06IntegrationFunctionQuark(double* x, size_t n,
            void* voidParams);
    friend double DVMPCFFGK06IntegrationFunctionGluon(double* x, size_t n,
            void* voidParams);

protected:

    /**
     * Copy constructor.
     * @param other
     */
    DVMPCFFGK06(const DVMPCFFGK06 &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual std::complex<double> computeCFF();

    /**
     * Quark unintegrated amplitude.
     * @param x Nucleon's momentum fraction.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     * @param quarkType Quark type.
     */
    double quarkUnintegratedAmplitude(double x, double tau, double b,
            QuarkFlavor::Type quarkType) const;

    /**
     * Gluon unintegrated amplitude.
     * @param x Nucleon's momentum fraction.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */
    double gluonUnintegratedAmplitude(double x, double tau, double b) const;

private:

    const double m_cNf; ///< Number of active flavors.
    const double m_cLambdaQCD; ///< Lambda QCD

    /**
     * Running coupling.
     */
    double alphaS() const;

    /**
     * Sudakov factor.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */
    double sudakovFactor(double tau, double b) const;

    /**
     * Sudakov factor function s.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */
    double sudakovFactorFunctionS(double tau, double b) const;

    /**
     * Meson wave function.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */
    double mesonWF(double tau, double b) const;

    /**
     * Meson wave function (Gaussian parameterization).
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     * @param f Decay constant (in GeV).
     * @param a Transverse size parameter (in GeV-1)
     */
    double mesonWFGaussian(double tau, double b, double f, double a) const;

    /**
     * Quark propagator.
     * @param x Nucleon's momentum fraction.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */
    double quarkPropagator(double x, double tau, double b) const;

    /**
     * Gluon propagator.
     * @param x Nucleon's momentum fraction.
     * @param tau Meson's momentum fraction.
     * @param b Impact-space parameter.
     */
    double gluonPropagator(double x, double tau, double b) const;

    /**
     * Quark unintegrated amplitude.
     * @param quarkType Quark type.
     */
    double quarkIntegratedAmplitude(QuarkFlavor::Type quarkType) const;

    /**
     * Gluon unintegrated amplitude.
     */
    double gluonIntegratedAmplitude() const;
};

} /* namespace PARTONS */

#endif /* DVMP_CFF_GK06_H */
