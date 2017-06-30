#ifndef DVCSCFFHEAVYQUARK_H
#define DVCSCFFHEAVYQUARK_H

/**
 * @file DVCSCFFHeavyQuark.h
 * @author jakub
 * @date 11 February 2015
 * @version 1.0
 */

#include <complex>
#include <string>
#include <vector>

#include "DVCSCFFStandard.h"

/**
 * @class DVCSCFFHeavyQuark
 *
 * TODO: Add description.
 *
 * Available CFF types: H, E, Ht, Et.
 */
class DVCSCFFHeavyQuark: public DVCSCFFStandard {
public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     *
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    DVCSCFFHeavyQuark(const std::string &className);

    virtual DVCSCFFHeavyQuark* clone() const;

    /**
     * Default destructor.
     */
    virtual ~DVCSCFFHeavyQuark();

    virtual void resolveObjectDependencies();

protected:
    /**
     * Copy constructor
     * @param other
     */
    DVCSCFFHeavyQuark(const DVCSCFFHeavyQuark &other);

//    virtual void initModule();
//    virtual void isModuleWellConfigured();

    virtual std::complex<double> computeUnpolarized();
    virtual std::complex<double> computePolarized();

private:
    std::complex<double> m_betas;
    std::complex<double> m_rs;
    std::complex<double> m_betaq;
    std::complex<double> m_rq;
    std::complex<double> m_TF;

    std::complex<double> beta(double s, double mq);
    std::complex<double> r(double s, double mq);

    std::complex<double> MassiveKernelGluonNLOA(double x);
    std::complex<double> MassiveKernelGluonNLOV(double x);
    std::complex<double> computeIntegralsMassiveV();
    std::complex<double> computeIntegralsMassiveA();
    double ConvolReKernelGluonMassiveV(double x, std::vector<double> params);
    double ConvolImKernelGluonMassiveV(double x, std::vector<double> params);
    double ConvolReKernelGluonMassiveA(double x, std::vector<double> params);
    double ConvolImKernelGluonMassiveA(double x, std::vector<double> params);

    NumA::FunctionType1D* m_pConvolReKernelGluonMassiveV;
    NumA::FunctionType1D* m_pConvolImKernelGluonMassiveV;
    NumA::FunctionType1D* m_pConvolReKernelGluonMassiveA;
    NumA::FunctionType1D* m_pConvolImKernelGluonMassiveA;

    void initFunctorsForIntegrations(); ///< Initialize functors.
};

#endif /* DVCSCFFHEAVYQUARK_H */
