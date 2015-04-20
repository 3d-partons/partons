#ifndef DVCS_CFF_HEAVY_QUARK_MODEL_H
#define DVCS_CFF_HEAVY_QUARK_MODEL_H

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

#include <complex>
#include <string>

#include "DVCSCFFModel.h"

class DVCSCFFHeavyQuarkModel: public DVCSCFFModel {
public:

    static const std::string moduleID; ///< Unique ID to automatically register the module in the factory.

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

    virtual std::complex<double> KernelGluonNLOA(double x); ///<Modified for heavy quarks
    virtual std::complex<double> KernelGluonNLOV(double x); ///<Modified for heavy quarks

    virtual std::complex<double> computeUnpolarized();
    virtual std::complex<double> computePolarized();

    void computeSubtractionFunctionsV();

private:
    std::complex<double> m_betas;
    std::complex<double> m_rs;
    std::complex<double> m_betaq;
    std::complex<double> m_rq;
    std::complex<double> m_TF;
    double m_eps;
    std::complex<double> beta(std::complex<double> s, double mq);
    std::complex<double> r(std::complex<double> s, double mq);

};

#endif /* DVCS_CFF_HEAVY_QUARK_MODEL_H */
