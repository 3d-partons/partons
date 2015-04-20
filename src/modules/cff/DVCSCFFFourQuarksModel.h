#ifndef DVCS_CFF_FOUR_QUARKS_MODEL_H
#define DVCS_CFF_FOUR_QUARKS_MODEL_H

/**
 * @file DVCSCFFHeavyQuarkModel.h
 * @author Jakub
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

class DVCSCFFFourQuarksModel: public DVCSCFFModel {
public:
    static const std::string moduleID; ///< Unique ID to automatically register the module in the factory.

    DVCSCFFFourQuarksModel(const std::string &className);

    virtual DVCSCFFFourQuarksModel* clone() const;

protected:
    /**
     * Copy constructor
     * @param other
     */
    DVCSCFFFourQuarksModel(const DVCSCFFFourQuarksModel &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual std::complex<double> KernelGluonNLOV(double x); ///< T^{g, NLO, V}, appendix A, eq. (A2)
private:

    double m_Constant;

};

#endif /* DVCS_CFF_FOUR_QUARKS_MODEL_H */
