#ifndef DVCS_CFF_MODULE_H
#define DVCS_CFF_MODULE_H

/**
 * @date 08 September 2014
 * Last update : 22 September 2014
 */

#include <complex>
#include <string>

#include "../QCDOrderType.h"
#include "CFFModule.h"

//TODO In Progress

class DVCSCFFModule: public CFFModule {
public:
    static const std::string moduleID; ///< Unique ID to automatically register the module in the factory.

    DVCSCFFModule();

    virtual DVCSCFFModule* clone() const;

    virtual ~DVCSCFFModule();

    virtual std::complex<double> compute(const double &xB, const double &t,
            const double Q2, const double &MuF, const double &MuR,
            const QCDOrderType &qcdOrderType);

    // ##### GETTERS & SETTERS #####

private:

    double m_xi;
    double m_Zeta;
    double m_logQ2OverMu2;
    double m_Q;
    unsigned int m_nbOfActiveFlavour;
    double m_alphaSOver2Pi;
    double m_quarkDiagonal;
    double m_gluonDiagonal;

    double m_realPartSubtractQuark;
    double m_imaginaryPartSubtractQuark;
    double m_realPartSubtractGluon;
    double m_imaginaryPartSubtractGluon;

    const double m_CF;                     ///< ( Nc^2 - 1 ) / ( 2 Nc ) (colour)

    bool isModuleConfigured();
    void updateVariables(const double & xB, const double & Q2,
            const double & MuF, const double & MuR);

    void computeDiagonalGPD(const double & t, const double & MuF,
            const double & MuR);
    void computeSubtractionFunctions(const QCDOrderType &qcdOrderType);
    void computeIntegrals(const QCDOrderType &qcdOrderType);
};

#endif /* DVCS_CFF_MODULE_H */
