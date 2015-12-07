/**
 * @file DVCSCFFHeavyQuarkModel.h
 * @author jakub
 * @date 11 February 2015
 * @version 1.0
 *
 * @class DVCSCFFHeavyQuarkModel
 *
 * @brief
 */

#ifndef DVCS_CFF_HEAVY_QUARK_MODEL_H
#define DVCS_CFF_HEAVY_QUARK_MODEL_H

#include <complex>
#include <string>

#include "DVCSCFFModel.h"

class DVCSCFFHeavyQuarkModel: public DVCSCFFModel {
public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    DVCSCFFHeavyQuarkModel(const std::string &className);

    virtual DVCSCFFHeavyQuarkModel* clone() const;

    virtual ~DVCSCFFHeavyQuarkModel();

protected:
    /**
     * Copy constructor
     * @param other
     */

    DVCSCFFHeavyQuarkModel(const DVCSCFFHeavyQuarkModel &other);

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
    double ConvolReKernelGluonMassiveV(double* x, double* params);
    double ConvolImKernelGluonMassiveV(double* x, double* params);
    double ConvolReKernelGluonMassiveA(double* x, double* params);
    double ConvolImKernelGluonMassiveA(double* x, double* params);

};

#endif /* DVCS_CFF_HEAVY_QUARK_MODEL_H */
